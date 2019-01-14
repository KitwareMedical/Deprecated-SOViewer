/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovPlaneVTKRenderMethod3D.h,v $
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
#ifndef _PlaneVTKRenderMethod3D_h
#define _PlaneVTKRenderMethod3D_h

#include "sovVTKRenderMethod3D.h"
#include <itkPlaneSpatialObject.h>
#include <vtkBMPReader.h>
#include <vtkTexture.h>
#include <sovVTKWin32Header.h>

namespace sov
{

class SOVVTKRenderMethod_EXPORT PlaneVTKRenderMethod3D
: public VTKRenderMethod3D
{

public:

  typedef PlaneVTKRenderMethod3D     Self;
  typedef VTKRenderMethod3D           Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;
  typedef itk::PlaneSpatialObject<3>  PlaneType;
  typedef Superclass::ObjectType        ObjectType;
  typedef Superclass::ObjectListType    ObjectListType;

  itkNewMacro( PlaneVTKRenderMethod3D );
  itkTypeMacro( PlaneVTKRenderMethod3D, VTKRenderMethod3D );

  virtual void draw( void );
  void SetTextureFile(const char* name);

  itkSetMacro(UseTexture,bool);
  virtual void Update();

protected:

  PlaneVTKRenderMethod3D( void );
  ~PlaneVTKRenderMethod3D( void );

  void PrintSelf (std::ostream &os, Indent indent) const;

  bool m_UseTexture;
  const char* m_TextureName;
  vtkTexture* m_Texture;
};

}

#endif //_PlaneVTKRenderMethod3D_h
