/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSphereVTKRenderMethodSlicer.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:58 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _SphereVTKRenderMethodSlicer_h
#define _SphereVTKRenderMethodSlicer_h

#include "sovVTKRenderMethodSlicer.h"
#include <itkEllipseSpatialObject.h>
#include <vtkBMPReader.h>
#include <vtkTexture.h>
#include <sovVTKWin32Header.h>

namespace sov
{

class SOVVTKRenderMethod_EXPORT SphereVTKRenderMethodSlicer
: public VTKRenderMethodSlicer
{

public:

  typedef SphereVTKRenderMethodSlicer     Self;
  typedef VTKRenderMethodSlicer           Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;
  typedef itk::EllipseSpatialObject<3>  SphereType;
  typedef Superclass::ObjectType        ObjectType;
  typedef Superclass::ObjectListType    ObjectListType;

  itkNewMacro( SphereVTKRenderMethodSlicer );
  itkTypeMacro( SphereVTKRenderMethodSlicer, VTKRenderMethodSlicer );

  virtual void draw( void );
  void SetTextureFile(const char* name);

  itkSetMacro(UseTexture,bool);

protected:

  SphereVTKRenderMethodSlicer( void );
  ~SphereVTKRenderMethodSlicer( void );

  void PrintSelf (std::ostream &os, Indent indent) const;
  virtual void Update();
  bool m_UseTexture;
  const char* m_TextureName;
  vtkTexture* m_Texture;
};

}

#endif //_SphereVTKRenderMethodSlicer_h
