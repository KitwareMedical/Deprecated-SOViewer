/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovRenderMethod.h,v $
  Language:  C++
  Date:      $Date: 2006-02-01 21:24:35 $
  Version:   $Revision: 1.2 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __RenderMethod_h
#define __RenderMethod_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif


#if    defined(_WIN32)
#include <itkWindows.h>
#endif

#include <itkImage.h>
#include <sovNDimensionalRenderMethod.h>
#include "sovVisualObject.h"
#include <itkSpatialObject.h>

namespace sov
{

using namespace itk;

template< unsigned int TDimension >
class RenderMethod
: public NDimensionalRenderMethod
{

public:

  typedef RenderMethod                     Self;
  typedef NDimensionalRenderMethod         Superclass;
  typedef SmartPointer< Self >             Pointer;
  typedef SmartPointer< const Self >       ConstPointer;

  typedef itk::SpatialObject<TDimension>   ObjectType;
  typedef VisualObject<TDimension>         VisualObjectType;
  typedef std::list<VisualObjectType *>    ObjectListType;
  typedef std::list<const char *>          SupportedObjectTypeListType;
  
  itkTypeMacro( RenderMethod, NDimensionalRenderMethod );

  virtual void AddVisualObject(VisualObjectType * object );

  virtual void RemoveVisualObject( VisualObjectType * object );

  void SetObjectList( ObjectListType * );

  const ObjectListType * GetObjectList() const;

  unsigned int GetNumberOfObjects() const;

  const ObjectType * GetObject( const char * ) const;

  const ObjectType * GetConstObject( const char * objectName) const;

  virtual void draw( void ) = 0; // purposely not implemented

  bool IsObjectTypeSupported( const char * ) const;

  bool ContainsObject( const char * ) const;

  unsigned long GetMTime( void ) const;

  const char * GetType() const;

  void SetName( const char * name );

  const char * GetName() const;

  virtual bool IsSupported( const char* object ) const = 0;

  unsigned int GetDimension(void) {return TDimension;}

  itkSetMacro(Width,unsigned int);
  itkGetMacro(Width,unsigned int);
  
  itkSetMacro(Height,unsigned int);
  itkGetMacro(Height,unsigned int);

  virtual void Update() = 0; // purposely not implemented

  ObjectListType  GetVisualObjectList(void) {return m_ObjectList;}

protected:

  ObjectListType                  m_ObjectList;  
  SupportedObjectTypeListType *   m_SupportedTypeList;
  TimeStamp                       m_LastUpdatedTime;
  std::string                     m_Type;
  std::string                     m_Name;

  /** These two variables define the size of the display
   *  because some Render methods need them */
  unsigned int                    m_Width;
  unsigned int                    m_Height;

  RenderMethod( void );
  ~RenderMethod( void );

  void AddSupportedType( const char * );
  void RemoveSupportedType( const char * );
  SupportedObjectTypeListType::iterator FindValue( const char * ) const;

  void PrintSelf(std::ostream &os, Indent indent) const;
  

private:

  RenderMethod( const Self & );
  void operator=( const Self & );

};

} //end of namespace sov

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovRenderMethod.txx"
#endif

#endif //__RenderMethod_h
