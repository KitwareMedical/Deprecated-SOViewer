/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVTKRendererSlicer.h,v $
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
#ifndef __sovVTKRendererSlicer_h
#define __sovVTKRendererSlicer_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "sovVTKRenderer.h"
#include "sovVTKRenderMethodSlicer.h"
#include "vtkRenderWindowInteractor.h"
#include <vtkPicker.h>
#include <vtkInteractorStyle.h>
#include <itkEllipseSpatialObject.h>
#include <vtkPlaneSource.h>
#include <itkEuler3DTransform.h>
#include "sovObjectViewerWin32Header.h"

namespace sov
{

/**
 *
 */

class SOViewer_EXPORT VTKRendererSlicer
: public VTKRenderer<3>
{

public:

  typedef VTKRendererSlicer          Self;
  typedef SmartPointer< Self >   Pointer;
  typedef VTKRenderer<3>         Superclass;
  typedef Superclass::MethodType SuperclassMethodType;
  typedef VTKRenderMethodSlicer      MethodType;
  typedef Superclass::SceneType  SceneType;
  typedef vnl_vector<double>     VnlVectorType;
  typedef itk::Vector<double,3>  VectorType;
  typedef itk::Point<double,3>   PointType;
  typedef itk::SpatialObject<3>  SpatialObjectType;
  typedef Superclass::ObjectType ObjectType; 
  typedef itk::Euler3DTransform<double> TransformType;

  itkNewMacro( Self );
  itkTypeMacro( VTKRendererSlicer, VTKRenderer );

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

  void SetOrientation(unsigned int orientation);
  void SetSliceNumber(unsigned long slice) {m_SliceNumber = slice;}
  void SetDepth(double depth);
  void SetAngles(double alpha, double beta, double gamma);
  void SetOffset(double x, double y, double z);

protected:

  VTKRendererSlicer();
  virtual ~VTKRendererSlicer();

  void PrintSelf( std::ostream& , Indent ) const;

  vtkRenderWindowInteractor *m_Interactor;
  vtkInteractorStyle *m_InteractorStyle;

  bool m_UpdateRendererOnly;
  SpatialObjectType* m_CameraAttachedSpatialObject;
  VectorType m_CameraDirection;
  VectorType m_CameraOffset;
  double m_CameraRoll;
  unsigned int m_Orientation;
  unsigned long m_SliceNumber;
  double m_Depth;
  vtkPlaneSource* m_Plane;
  TransformType::Pointer m_Transform;

  /** Set the optimal zoom to see the slice */
  void AutoZoom(ObjectType* object);

};

} //end of namespace sov

#endif //__sovVTKRendererSlicer_h
