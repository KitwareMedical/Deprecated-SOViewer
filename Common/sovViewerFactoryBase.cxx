/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: sovViewerFactoryBase.cxx,v $
  Language:  C++
  Date:      $Date: 2008-04-03 11:51:57 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "sovViewerFactoryBase.h"
#include "itkDynamicLoader.h"
#include "itkDirectory.h"
#include "itkVersion.h"
#include <stdlib.h>
#include <ctype.h>
#include <cstring>
#include <algorithm>
#include <map>

namespace
{
  
class CleanUpViewerFactory
{
public:
  inline void Use() 
  {
  }
  ~CleanUpViewerFactory()
  {
    sov::ViewerFactoryBase::UnRegisterAllFactories();
  }  
};
static CleanUpViewerFactory CleanUpViewerFactoryGlobal;
}




namespace sov
{

/**
 * Add this for the SGI compiler which does not seem
 * to provide a default implementation as it should.
 */
bool operator==(const ViewerFactoryBase::OverrideInformation& rhs, 
                const ViewerFactoryBase::OverrideInformation& lhs)
{
  return (rhs.m_Description == lhs.m_Description
          && rhs.m_OverrideWithName == lhs.m_OverrideWithName);
}

/**
 * Add this for the SGI compiler which does not seem
 * to provide a default implementation as it should.
 */
bool operator<(const ViewerFactoryBase::OverrideInformation& rhs, 
               const ViewerFactoryBase::OverrideInformation& lhs)
{
  return (rhs.m_Description < lhs.m_Description
          && rhs.m_OverrideWithName < lhs.m_OverrideWithName);
}


/** \class StringOverMap
 * \brief Internal implementation class for ObjectFactorBase.
 *
 * Create a sub class to shrink the size of the symbols
 * Also, so a forward reference can be put in ViewerFactoryBase.h
 * and a pointer member can be used.  This avoids other
 * classes including <map> and getting long symbol warnings.
 */
typedef std::multimap<std::string, ViewerFactoryBase::OverrideInformation> 
StringOverMapType;

/** \class OverRideMap
 * \brief Internal implementation class for ObjectFactorBase.
 */
class OverRideMap : public StringOverMapType
{
public:
};

/**
 * Initialize static list of factories.
 */
std::list<ViewerFactoryBase*>* 
ViewerFactoryBase::m_RegisteredFactories = 0;


/**
 * Create an instance of a named itk object using the loaded
 * factories
 */
itk::LightObject::Pointer 
ViewerFactoryBase
::CreateInstance(const char* itkclassname)
{
  if ( !ViewerFactoryBase::m_RegisteredFactories )
    {
    ViewerFactoryBase::Initialize();
    }
  
  for ( std::list<ViewerFactoryBase*>::iterator 
          i = m_RegisteredFactories->begin();
        i != m_RegisteredFactories->end(); ++i )
    {
    itk::LightObject::Pointer newobject = (*i)->CreateObject(itkclassname);
    if(newobject)
      {
      return newobject;
      }
    }
  return 0;
}


std::list<itk::LightObject::Pointer>
ViewerFactoryBase
::CreateAllInstance(const char* itkclassname)
{
  if ( !ViewerFactoryBase::m_RegisteredFactories )
    {
    ViewerFactoryBase::Initialize();
    }
  std::list<itk::LightObject::Pointer> created;
  for ( std::list<ViewerFactoryBase*>::iterator 
          i = m_RegisteredFactories->begin();
        i != m_RegisteredFactories->end(); ++i )
    {
    itk::LightObject::Pointer newobject = (*i)->CreateObject(itkclassname);
    if(newobject)
      {
      created.push_back(newobject);
      }
    }
  return created;
}


/**
 * A one time initialization method.
 */
void 
ViewerFactoryBase
::Initialize()
{
  CleanUpViewerFactoryGlobal.Use();
  /**
   * Don't do anything if we are already initialized
   */
  if ( ViewerFactoryBase::m_RegisteredFactories )
    {
    return;
    }
  
  ViewerFactoryBase::m_RegisteredFactories =
    new std::list<ViewerFactoryBase*>;
  ViewerFactoryBase::RegisterDefaults();
  ViewerFactoryBase::LoadDynamicFactories();
}


/**
 * Register any factories that are always present in ITK like
 * the OpenGL factory, currently this is not done.
 */
void 
ViewerFactoryBase
::RegisterDefaults()
{
}

/**
 * Load all libraries in ITK_AUTOLOAD_PATH
 */
void 
ViewerFactoryBase
::LoadDynamicFactories()
{
  /**
   * follow PATH conventions
   */
#ifdef _WIN32
  char PathSeparator = ';';
#else
  char PathSeparator = ':';
#endif
  
  std::string LoadPath;
  if (getenv("ITK_AUTOLOAD_PATH"))
    {
    LoadPath = getenv("ITK_AUTOLOAD_PATH");
    }
  else
    {
    return;
    }

  if(LoadPath.size() == 0)
    {
    return;
    }
  std::string::size_type EndSeparatorPosition = 0;
  std::string::size_type StartSeparatorPosition = 0;
  while ( StartSeparatorPosition != std::string::npos )
    {
    StartSeparatorPosition = EndSeparatorPosition;
    /**
     * find PathSeparator in LoadPath
     */
    EndSeparatorPosition = LoadPath.find(PathSeparator, 
                                         StartSeparatorPosition);
    if(EndSeparatorPosition == std::string::npos)
      {
      EndSeparatorPosition = LoadPath.size();
      }
    std::string CurrentPath = 
      LoadPath.substr(StartSeparatorPosition, EndSeparatorPosition);
    ViewerFactoryBase::LoadLibrariesInPath(CurrentPath.c_str());
    /**
     * move past separator
     */
    if(EndSeparatorPosition == LoadPath.size())
      {
      StartSeparatorPosition = std::string::npos;
      }
    else
      {
      EndSeparatorPosition++;
      }
    }
}


/**
 * A file scope helper function to concat path and file into
 * a full path
 */
static std::string 
CreateFullPath(const char* path, const char* file)
{
  std::string ret;
#ifdef _WIN32
  const char sep = '\\';
#else
  const char sep = '/';
#endif
  /**
   * make sure the end of path is a separator
   */
  ret = path;
  if ( ret[ret.size()-1] != sep )
    {
    ret += sep;
    }
  ret += file;
  return ret;
}


/**
 * A file scope typedef to make the cast code to the load
 * function cleaner to read.
 */
typedef ViewerFactoryBase* (* ITK_LOAD_FUNCTION)();


/**
 * A file scoped function to determine if a file has
 * the shared library extension in its name, this converts name to lower
 * case before the compare, DynamicLoader always uses
 * lower case for LibExtension values.
 */
inline bool 
NameIsSharedLibrary(const char* name)
{
  std::string sname = name;
  if ( sname.find(itk::DynamicLoader::LibExtension()) != std::string::npos )
    {
    return true;
    }
  return false;
}


/**
 *
 */
void 
ViewerFactoryBase
::LoadLibrariesInPath(const char* path)
{
  itk::Directory::Pointer dir = itk::Directory::New();
  if ( !dir->Load(path) )
    {
    return;
    }
  
  /**
   * Attempt to load each file in the directory as a shared library
   */
  for ( unsigned int i = 0; i < dir->GetNumberOfFiles(); i++ )
    {
    const char* file = dir->GetFile(i);
    /**
     * try to make sure the file has at least the extension
     * for a shared library in it.
     */
    if ( NameIsSharedLibrary(file) )
      {
      std::string fullpath = CreateFullPath(path, file);
      itk::LibHandle lib = itk::DynamicLoader::OpenLibrary(fullpath.c_str());
      if ( lib )
        {
        /**
   * Look for the symbol itkLoad in the library
   */
        ITK_LOAD_FUNCTION loadfunction
          = (ITK_LOAD_FUNCTION)itk::DynamicLoader::GetSymbolAddress(lib, "itkLoad");
        /**
   * if the symbol is found call it to create the factory
         * from the library
   */
        if ( loadfunction )
          {
          ViewerFactoryBase* newfactory = (*loadfunction)();
          /**
     * initialize class members if load worked
     */
          newfactory->m_LibraryHandle = (void*)lib;
          newfactory->m_LibraryPath = fullpath;
          newfactory->m_LibraryDate = 0; // unused for now...
          ViewerFactoryBase::RegisterFactory(newfactory);
          }
        }
      }
    }
}


/**
 * Recheck the ITK_AUTOLOAD_PATH for new libraries
 */
void 
ViewerFactoryBase
::ReHash()
{
  ViewerFactoryBase::UnRegisterAllFactories();
  ViewerFactoryBase::Initialize();
}


/**
 * initialize class members
 */
ViewerFactoryBase::ViewerFactoryBase()
{
  m_LibraryHandle = 0;
  m_LibraryDate = 0;
  m_OverrideMap = new OverRideMap;
}


/**
 * Unload the library and free the path string
 */
ViewerFactoryBase
::~ViewerFactoryBase()
{
  if(m_LibraryHandle)
    {
    itk::DynamicLoader::CloseLibrary((itk::LibHandle)m_LibraryHandle);
    }
  m_OverrideMap->erase(m_OverrideMap->begin(), m_OverrideMap->end());
  delete m_OverrideMap;
}


/**
 * Add a factory to the registered list
 */
void 
ViewerFactoryBase
::RegisterFactory(ViewerFactoryBase* factory)
{
  if ( factory->m_LibraryHandle == 0 )
    {
    const char* nonDynamicName = "Non-Dynamicly loaded factory";
    factory->m_LibraryPath = nonDynamicName;
    }
  if ( strcmp(factory->GetITKSourceVersion(), 
              itk::Version::GetITKSourceVersion()) != 0 )
    {
    itkGenericOutputMacro(<< "Possible incompatible factory load:" 
                          << "\nRunning itk version :\n" << itk::Version::GetITKSourceVersion() 
                          << "\nLoaded Factory version:\n" << factory->GetITKSourceVersion()
                          << "\nLoading factory:\n" << factory->m_LibraryPath << "\n");
    }
  ViewerFactoryBase::Initialize();
  ViewerFactoryBase::m_RegisteredFactories->push_back(factory);
  factory->Register();
}


/**
 *
 */
void 
ViewerFactoryBase
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Factory DLL path: " << m_LibraryPath.c_str() << "\n";
  os << indent << "Factory description: " << this->GetDescription() << std::endl;

  int num = static_cast<int>( m_OverrideMap->size() );
  os << indent << "Factory overides " << num << " classes:" << std::endl;

  indent = indent.GetNextIndent();
  for(OverRideMap::iterator i = m_OverrideMap->begin();
      i != m_OverrideMap->end(); ++i)
    {
    os << indent << "Class : " <<  (*i).first.c_str() << "\n";
    os << indent << "Overriden with: " <<  (*i).second.m_OverrideWithName.c_str()
       << std::endl;
    os << indent << "Enable flag: " << (*i).second.m_EnabledFlag
       << std::endl;
    os << std::endl;
    }
}


/**
 *
 */
void 
ViewerFactoryBase
::UnRegisterFactory(ViewerFactoryBase* factory)
{ 
  for ( std::list<ViewerFactoryBase*>::iterator i = 
          m_RegisteredFactories->begin();
        i != m_RegisteredFactories->end(); ++i )
    {
    if ( factory == *i )
      {
      m_RegisteredFactories->remove(factory);
      factory->UnRegister();
      return;
      }
    }
}
  

/**
 * unregister all factories and delete the RegisteredFactories list
 */
void 
ViewerFactoryBase
::UnRegisterAllFactories()
{
  
  if ( ViewerFactoryBase::m_RegisteredFactories )
    {
    for ( std::list<ViewerFactoryBase*>::iterator i 
            = m_RegisteredFactories->begin();
          i != m_RegisteredFactories->end(); ++i )
      {
      (*i)->UnRegister();
      }
    delete ViewerFactoryBase::m_RegisteredFactories;
    ViewerFactoryBase::m_RegisteredFactories = 0;
    }
}


/**
 *
 */
void 
ViewerFactoryBase
::RegisterOverride(const char* classOverride,
                   const char* subclass,
                   const char* description,
                   bool enableFlag,
                   itk::CreateObjectFunctionBase*
                   createFunction)
{
  ViewerFactoryBase::OverrideInformation info;
  info.m_Description = description;
  info.m_OverrideWithName = subclass;
  info.m_EnabledFlag = enableFlag;
  info.m_CreateObject = createFunction;
  m_OverrideMap->insert(OverRideMap::value_type(classOverride, info));
}


itk::LightObject::Pointer 
ViewerFactoryBase
::CreateObject(const char* itkclassname)
{
  m_OverrideMap->find(itkclassname);
  OverRideMap::iterator pos = m_OverrideMap->find(itkclassname);
  if ( pos != m_OverrideMap->end() )
    {
    return (*pos).second.m_CreateObject->CreateObject();
    }
  return 0;
}


/**
 *
 */
void 
ViewerFactoryBase
::SetEnableFlag(bool flag,
                const char* className,
                const char* subclassName)
{
  OverRideMap::iterator start = m_OverrideMap->lower_bound(className);
  OverRideMap::iterator end = m_OverrideMap->upper_bound(className);
  for ( OverRideMap::iterator i = start; i != end; ++i )
    {
    if ( (*i).second.m_OverrideWithName == subclassName )
      {
      (*i).second.m_EnabledFlag = flag;
      }
    }
}


/**
 *
 */
bool 
ViewerFactoryBase
::GetEnableFlag(const char* className, const char* subclassName)
{
  OverRideMap::iterator start = m_OverrideMap->lower_bound(className);
  OverRideMap::iterator end = m_OverrideMap->upper_bound(className);
  for ( OverRideMap::iterator i = start; i != end; ++i )
    {
    if ( (*i).second.m_OverrideWithName == subclassName )
      {
      return (*i).second.m_EnabledFlag;
      }
    }
  return 0;
}


/**
 *
 */
void 
ViewerFactoryBase
::Disable(const char* className)
{
  OverRideMap::iterator start = m_OverrideMap->lower_bound(className);
  OverRideMap::iterator end = m_OverrideMap->upper_bound(className);
  for ( OverRideMap::iterator i = start; i != end; ++i )
    {
    (*i).second.m_EnabledFlag = 0;
    }
}


/**
 *
 */
std::list<ViewerFactoryBase*> 
ViewerFactoryBase
::GetRegisteredFactories()
{
  return *ViewerFactoryBase::m_RegisteredFactories;
}


/**
 * Return a list of classes that this factory overrides.
 */
std::list<std::string> 
ViewerFactoryBase
::GetClassOverrideNames()
{
  std::list<std::string> ret;
  for ( OverRideMap::iterator i = m_OverrideMap->begin();
        i != m_OverrideMap->end(); ++i )
    {
    ret.push_back((*i).first);
    }
  return ret;
}


/**
 * Return a list of the names of classes that override classes.
 */
std::list<std::string> 
ViewerFactoryBase
::GetClassOverrideWithNames()
{
  std::list<std::string> ret;
  for ( OverRideMap::iterator i = m_OverrideMap->begin();
        i != m_OverrideMap->end(); ++i )
    {
    ret.push_back((*i).second.m_OverrideWithName);
    }
  return ret;
}


/**
 * Retrun a list of descriptions for class overrides
 */
std::list<std::string> 
ViewerFactoryBase
::GetClassOverrideDescriptions()
{ 
  std::list<std::string> ret;
  for ( OverRideMap::iterator i = m_OverrideMap->begin();
        i != m_OverrideMap->end(); ++i )
    {
    ret.push_back((*i).second.m_Description);
    }
  return ret;
}


/**
 * Return a list of enable flags
 */
std::list<bool> 
ViewerFactoryBase
::GetEnableFlags()
{
  std::list<bool> ret;
  for( OverRideMap::iterator i = m_OverrideMap->begin();
       i != m_OverrideMap->end(); ++i)
    {
    ret.push_back((*i).second.m_EnabledFlag);
    }
  return ret;
}

} // end namespace sov
