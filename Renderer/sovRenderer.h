/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovRenderer.h,v $
  Language:  C++
  Date:      $Date: 2006-02-27 20:42:27 $
  Version:   $Revision: 1.3 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __Renderer_h
#define __Renderer_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include <sovRenderMethod.h>

#include "itkSceneSpatialObject.h"
#include "sovVisualObject.h"
#include "sovNDimensionalRenderer.h"

static unsigned int m_NextRendererAvailableId = 1;


namespace sov
{

/**
 *
 * 
 */
template< unsigned int TDimension >
class Renderer
: public NDimensionalRenderer
{

public:

  typedef Renderer                              Self;
  typedef NDimensionalRenderer                  Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef SpatialObject<TDimension>             ObjectType;
  typedef std::list< ObjectType * >             ObjectListType;
  typedef RenderMethod< TDimension >            MethodType;
  typedef typename MethodType::Pointer          MethodPointer;
  typedef std::list< MethodPointer >            MethodListType;
  typedef itk::SceneSpatialObject<TDimension>   SceneType;
  typedef typename SceneType::Pointer           ScenePointer;
  typedef VisualObject<TDimension>              VisualObjectType;
  typedef std::list< VisualObjectType * >       VisualObjectListType;

  itkNewMacro( Renderer );
  itkTypeMacro( Renderer, Superclass );

  virtual void AddObject( ObjectType *, bool addChildren = true );

  virtual void AddObject(ObjectType *, MethodType * );

  void RemoveObject( VisualObjectType * );

  VisualObjectType* GetVisualObject(const char * name) const;
  VisualObjectType* GetVisualObject(ObjectType *,unsigned int i=0) const;

  virtual void AddRenderMethod( MethodType * );

  void RemoveRenderMethod( MethodType * );

  MethodType * GetRenderMethod( const char * ) const;

  static unsigned int GetNextAvailableId()
    {
    return m_NextRendererAvailableId++;
    }

  void Active( bool activated );

  bool Active() const;

  const unsigned int & GetId() const;

  void SetRenderMethodList( const MethodListType * );

  const MethodListType * GetRenderMethodList() const;

  virtual void draw( void );

  virtual void SetScene( SceneType * );

  const SceneType * GetScene() const;

  void PrintSelf( std::ostream & os, Indent indent) const;

  unsigned int GetNumberOfObjects( void ) const;

  const char * GetName( void ) const;

  void SetName( const char * name );

  bool IsSupported( MethodType* method ) const;
    
  /** Update method */
  virtual void Update(void);

  void UpdateDisplaySize(void);

  const char* GetType(){return m_Type.c_str();}

  void AssociateWithRenderMethod(ObjectType* object, const char* methodName);
  void AssociateWithRenderMethod(ObjectType* object, MethodType* method);

  void AddToRenderMethod(ObjectType* object, const char* methodName);
  void AddToRenderMethod(ObjectType* object, MethodType* method);

  itkSetMacro(RemoveUnusedRenderMethods,bool);
  itkGetMacro(RemoveUnusedRenderMethods,bool);
  
  virtual MethodPointer GetRenderMethodViaFactory(const char *, 
                                    unsigned int , 
                                    const char* ,
                                    const char* ,
                                    const char* )
                                    {std::cout << "Not Implemented on purpose!" << std::endl; return NULL;}

protected:

  MethodListType                 m_MethodList;
  VisualObjectListType           m_VisualObjectList;
  ScenePointer                   m_Scene;
  unsigned int                   m_Id;
  bool                           m_Activated;
  std::string                    m_Type;
  char                           m_Name[255];
  unsigned int                   m_NumberOfObjects; // keep track of the number of object in the scene for the update
  bool                           m_RemoveUnusedRenderMethods;

  Renderer();
  virtual ~Renderer();

  MethodType * UpdateMethodList( VisualObjectType* visualObject );
  bool ObjectExists( ObjectType* object ) const;
  unsigned int GetNumberOfVisualObjects( ObjectType* object ) const;
  bool ObjectIsInScene( ObjectType* object ) const;
};

} //end of namespace sov

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovRenderer.txx"
#endif


#endif //__sovRenderer_h
