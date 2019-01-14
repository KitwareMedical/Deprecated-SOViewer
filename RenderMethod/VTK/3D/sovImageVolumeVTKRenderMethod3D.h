/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovImageVolumeVTKRenderMethod3D.h,v $
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
#ifndef _ImageVolumeVTKRenderMethod3D_h
#define _ImageVolumeVTKRenderMethod3D_h

#include "sovVTKRenderMethod3D.h"

#include <itkImageSpatialObject.h>


#include <vtkVolumeRayCastMapper.h>
#include <vtkVolumeRayCastMIPFunction.h>
#include <itkImageRegionIterator.h>
#include <vtkExtractVOI.h>
#include <vtkPlaneSource.h>
#include <vtkTexture.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkOutlineFilter.h>
#include<vtkStructuredPoints.h>

namespace sov
{

template <class TImage>
class ImageVolumeVTKRenderMethod3D
: public VTKRenderMethod3D
{

public:

  typedef ImageVolumeVTKRenderMethod3D      Self;
  typedef VTKRenderMethod3D           Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;

  typedef TImage                      ImageType;
  typedef typename Superclass::ObjectType      ObjectType;
  typedef typename Superclass::ObjectListType  ObjectListType;
  typedef typename ImageType::PixelType        PixelType;

  typedef ImageSpatialObject<3,PixelType> ImageSpatialObjectType;

  itkNewMacro( ImageVolumeVTKRenderMethod3D );
  itkTypeMacro( ImageVolumeVTKRenderMethod3D, GlRenderMethod3D );

  virtual void draw( void );

protected:

  ImageVolumeVTKRenderMethod3D( void );
  ~ImageVolumeVTKRenderMethod3D( void );
  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();

private:
  vtkActor*  mSaggital;
  vtkExtractVOI*  mSaggitalSect;
  double mSaggitalSlice;
  vtkActor* mAxial;
  vtkTexture*  mSaggitalText;
  vtkExtractVOI* mAxialSect;
  double mAxialSlice;
  vtkActor * mCoronal;
  vtkExtractVOI*  mCoronalSect;
  double mCoronalSlice;
  vtkStructuredPoints* mVol;


  vtkPlaneSource*  mSaggitalPlane;
  vtkPolyDataMapper*  mSaggitalMap;

  vtkPlaneSource*  mAxialPlane;
  vtkTexture* mAxialText;
  vtkPolyDataMapper* mAxialMap;

  vtkPlaneSource*  mCoronalPlane;
  vtkTexture*  mCoronalText;
  vtkPolyDataMapper*  mCoronalMap;
};

}

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovImageVolumeVTKRenderMethod3D.txx"
#endif

#endif //_ImageVolumeVTKRenderMethod3D_h
