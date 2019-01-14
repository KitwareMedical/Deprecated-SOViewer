/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovPhantomRenderMethodFactory.txx,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <itkCreateObjectFunction.h>
#include <itkVersion.h>

#include <cstring>

#include "sovPhantomRenderMethodFactory.h"
#include "sovRenderMethod.h"


// Factories to be registered
#include <sovSpherePhantomRenderMethodFactory.h>
#include <sovBlobPhantomRenderMethodFactory.h>
#include <sovTubePhantomRenderMethodFactory.h>

namespace sov
{


/** Returns a pointer to the method specified as argument.
 *  In that case, it just check if one of the instances returned
 *  by the object factories contains the requested name in its
 *  name */
template < unsigned int TDimension >
typename PhantomRenderMethodFactory<TDimension>::RenderMethodPointer
PhantomRenderMethodFactory<TDimension>::CreateRenderMethod( const char * method, unsigned int itkNotUsed(dimension), 
                                                     const char* type, const char* methodName, const char* pixelType)
  {   
  
// Register the factories
if(TDimension == 3)
  {
   ViewerFactoryBase::RegisterFactory( SpherePhantomRenderMethodFactory::New() );
   ViewerFactoryBase::RegisterFactory( BlobPhantomRenderMethodFactory::New() ); 
   ViewerFactoryBase::RegisterFactory( TubePhantomRenderMethodFactory::New() ); 
  }
else if(TDimension == 2)
  {
  }

  typedef typename RenderMethodType::Pointer  RenderMethodPointer;
  std::list< RenderMethodPointer >            renderMethodList;
  typename std::list< RenderMethodPointer >::iterator  renderMethodIterator;
  std::list< LightObject::Pointer >           objectList;
  std::list< RenderMethodPointer >            methodList;
  objectList = ViewerFactoryBase::CreateAllInstance(method);


  for(std::list<LightObject::Pointer>::iterator i = objectList.begin();
      i != objectList.end(); ++i)
    {
    RenderMethodType* meth = dynamic_cast<RenderMethodType*>(i->GetPointer());
    if(meth)
      {
      if(!methodName)
      {
        methodList.push_back(meth);
      }
      else if(!strcmp((*i)->GetNameOfClass(),methodName))
      {
        methodList.push_back(meth);
      }
      }
    else
      {
      std::cerr << "Error RenderMethod factory did not return a valid RenderMethod type : "
                << (*i)->GetNameOfClass() 
                << std::endl;
      }
    }

  // return the first instance able to render the object
  for(typename std::list<RenderMethodPointer>::iterator k = methodList.begin();
      k != methodList.end(); ++k)
    { 
      if(!strcmp(type,(*k)->GetType()))
      {
      if((!strcmp(method,"ImageSpatialObject")) || (!strcmp(method,"MeshSpatialObject"))) // if the method is for an image we check the pixel type
      {
       if(!strcmp((*k)->GetPixelType(),pixelType))
        {
          return *k;
        }
      }
      else
      {   
        return *k;
      }
      }
    }
  return 0;  
}

template < unsigned int TDimension >
void 
PhantomRenderMethodFactory<TDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  // nothing to print out at that time...
}

} //end of namespace sov
