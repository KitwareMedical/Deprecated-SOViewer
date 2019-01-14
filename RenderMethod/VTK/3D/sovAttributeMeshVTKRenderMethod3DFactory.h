/*=========================================================================

  Program:   SOViewer
  Module:    $RCSfile: sovAttributeMeshVTKRenderMethod3DFactory.h,v $
  Language:  C++
  Date:      $Date: 2005-12-15 19:46:11 $
  Version:   $Revision: 1.2 $
  Author:    Ipek Oguz

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef SURFACE_ATTRIBUTE_MESH_VTK_RENDER_METHOD_3D_FACTORY
#define SURFACE_ATTRIBUTE_MESH_VTK_RENDER_METHOD_3D_FACTORY

#include "sovSurfaceMeshVTKRenderMethod3D.h"

#include <sovViewerFactoryBase.h>
#include <itkIndent.h>

namespace sov 
{
template <class MeshType>
class AttributeMeshVTKRenderMethod3DFactory : public ViewerFactoryBase
{
public:

  /* Typedefs */
  typedef AttributeMeshVTKRenderMethod3DFactory      Self;
  typedef ViewerFactoryBase                      Superclass;
  typedef itk::SmartPointer< Self >              Pointer;
  typedef itk::SmartPointer< const Self >        ConstPointer;
  
  itkFactorylessNewMacro( Self );
  
  /** Run-time type information (and related methods). */
  itkTypeMacro( AttributeMeshVTKRenderMethod3DFactory, ViewerFactoryBase );
  
  /** */
  virtual const char *  GetITKSourceVersion( void ) const; 

  /** */
  virtual const char *  GetDescription( void ) const;

  static void RegisterOneFactory(void)
  {
    itk::SmartPointer<AttributeMeshVTKRenderMethod3DFactory> renderMethodFactory;
    renderMethodFactory = AttributeMeshVTKRenderMethod3DFactory<MeshType>::New();
    ViewerFactoryBase::RegisterFactory(renderMethodFactory);
  }

protected:

  /** Constructor */
  AttributeMeshVTKRenderMethod3DFactory( void );

  /** Destructor */
  ~AttributeMeshVTKRenderMethod3DFactory( void );

  /** Print out the object properties. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

private:

  AttributeMeshVTKRenderMethod3DFactory( const Self & );  //purposely not implemented 
  void operator=( const Self & );         //purposely not implemented

};

}

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovAttributeMeshVTKRenderMethod3DFactory.txx"
#endif


#endif

