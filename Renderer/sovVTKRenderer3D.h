/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVTKRenderer3D.h,v $
  Language:  C++
  Date:      $Date: 2006-02-25 00:26:12 $
  Version:   $Revision: 1.3 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __sovVTKRenderer3D_h
#define __sovVTKRenderer3D_h


#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "sovVTKRenderer.h"
#include "sovVTKRenderMethod3D.h"
#include "vtkRenderWindowInteractor.h"
#include <vtkPicker.h>
#include <vtkInteractorStyle.h>
#include "sovObjectViewerWin32Header.h"


namespace sov
{
/**
 *
 */

class SOViewer_EXPORT VTKRenderer3D
: public VTKRenderer<3>
{

public:

  typedef VTKRenderer3D          Self;
  typedef SmartPointer< Self >   Pointer;
  typedef VTKRenderer<3>         Superclass;
  typedef Superclass::MethodType SuperclassMethodType;
  typedef VTKRenderMethod3D      MethodType;
  typedef Superclass::SceneType  SceneType;
  typedef vnl_vector<double>     VnlVectorType;
  typedef itk::Vector<double,3>  VectorType;
  typedef itk::Point<double,3>   PointType;
  typedef itk::SpatialObject<3>  SpatialObjectType;
  typedef Superclass::ObjectType ObjectType; 
  itkNewMacro( Self );
  itkTypeMacro( VTKRenderer3D, VTKRenderer );

  vtkRenderWindowInteractor* GetVTKInteractor(void) {return m_Interactor;}

  void AddObject( ObjectType * );
  void AddObject( ObjectType*,bool);
  void AddObject(ObjectType *, SuperclassMethodType * );
  void SetScene( SceneType * );
  void Update(void);

  void SetVTKInteractorStyle(vtkInteractorStyle *style);

  VisualObject<3>* PickObject(float x,float y);
  PointType PickPoint(float x,float y);

  void SaveRenderAsBMP(const char* name);
  void SaveRenderAsJPEG(const char* name);
  void SaveRenderAsPNG(const char* name);
 
  /** This should be set to true if we want to update only 
   *  one renderer method*/
  void SetUpdateRendererOnly(bool value){m_UpdateRendererOnly = value;}

  void CutObject(itk::EllipseSpatialObject<3>* object);

  /** Attach a camera to a specific spatial object */
  void AttachCamera(SpatialObjectType* object);

  /** Set the direction of the camera */
  itkSetMacro(CameraDirection,VectorType);

  /** Set the rolling factor of the camera */
  itkSetMacro(CameraRoll,double);

  /** Set the camera offset in local coordinate of the attached object */
  itkSetMacro(CameraOffset,VectorType);

protected:

  VTKRenderer3D();
  virtual ~VTKRenderer3D();

  void PrintSelf( std::ostream& , Indent ) const;

  vtkRenderWindowInteractor *m_Interactor;
  vtkInteractorStyle *m_InteractorStyle;

  bool m_UpdateRendererOnly;
  SpatialObjectType* m_CameraAttachedSpatialObject;
  VectorType m_CameraDirection;
  VectorType m_CameraOffset;
  double m_CameraRoll;
};

} //end of namespace sov

#endif //__sovVTKRenderer3D_h
