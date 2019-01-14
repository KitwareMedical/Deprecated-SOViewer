/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovImageVTKRenderMethodSlicer.h,v $
  Language:  C++
  Date:      $Date: 2005-04-07 15:03:39 $
  Version:   $Revision: 1.2 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ImageVTKRenderMethodSlicer_h
#define _ImageVTKRenderMethodSlicer_h

#include "sovVTKRenderMethodSlicer.h"

#include <itkImageSpatialObject.h>


#include <itkImageRegionIterator.h>
#include <vtkExtractVOI.h>
#include <vtkPlaneSource.h>
#include <vtkTexture.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkOutlineFilter.h>
#include <vtkStructuredPoints.h>
#include <vtkWindowLevelLookupTable.h>
#include <vtkImageReslice.h>
#include <vtkTransform.h>

namespace sov
{


class ImagePropSlicer
{
public:

  ImagePropSlicer()
  {
    LookupTable = vtkWindowLevelLookupTable::New();
    Actor = vtkActor::New();
    Reslicer = vtkImageReslice::New();
    Texture = vtkTexture::New();
    Plane = vtkPlaneSource::New();
    Mapper = vtkPolyDataMapper::New();
    Texture->SetLookupTable(LookupTable);
    Texture->MapColorScalarsThroughLookupTableOff();
    Transform = vtkTransform::New();
    Vol = vtkStructuredPoints::New();
  };

  void Clear()
  {
    Actor->Delete();
    Reslicer->Delete();
    Texture->Delete();
    Plane->Delete();
    Mapper->Delete();
    Vol->Delete();
    Transform->Delete();
  }

  ~ImagePropSlicer()
  {
    Clear();
  };

  unsigned long MTime;
  unsigned int  ID;

  vtkActor*           Actor;
  vtkPlaneSource*     Plane;
  vtkPolyDataMapper*  Mapper;
  vtkTexture*         Texture;
  int                 SliceNumber;
  vtkImageReslice*    Reslicer;
  vtkTransform*       Transform;



  vtkStructuredPoints* Vol;
  vtkWindowLevelLookupTable*  LookupTable;
};



template <class TImage>
class ImageVTKRenderMethodSlicer
: public VTKRenderMethodSlicer
{

public:

  typedef ImageVTKRenderMethodSlicer          Self;
  typedef VTKRenderMethodSlicer               Superclass;
  typedef SmartPointer< Self >            Pointer;
  typedef SmartPointer< const Self >      ConstPointer;

  typedef TImage                           ImageType;
  typedef typename ImageType::ConstPointer ImageConstPointer; 
  typedef Superclass::ObjectType           ObjectType;
  typedef Superclass::ObjectListType       ObjectListType;
  typedef typename ImageType::PixelType    PixelType;

  typedef ImageSpatialObject<3,PixelType> ImageSpatialObjectType;

  itkNewMacro( ImageVTKRenderMethodSlicer );
  itkTypeMacro( ImageVTKRenderMethodSlicer, GlRenderMethodSlicer );

  virtual void Draw( void );

protected:

  ImageVTKRenderMethodSlicer( void );
  ~ImageVTKRenderMethodSlicer( void );
  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();

  std::list<ImagePropSlicer*>     m_ImagePropList;
  ObjectListType::iterator        m_CurrentObject;
  ImageConstPointer               m_ImData;
  std::list<ImagePropSlicer*>::iterator m_PropIt;
};

}

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovImageVTKRenderMethodSlicer.txx"
#endif

#endif //_ImageVTKRenderMethodSlicer_h
