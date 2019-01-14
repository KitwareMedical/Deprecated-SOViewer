/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovImageVTKRenderMethod3D.h,v $
  Language:  C++
  Date:      $Date: 2006-02-28 22:19:27 $
  Version:   $Revision: 1.4 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ImageVTKRenderMethod3D_h
#define _ImageVTKRenderMethod3D_h

#include "sovVTKRenderMethod3D.h"

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

namespace sov
{


class ImageProp
{
public:

  ImageProp()
  {
    LookupTable = vtkWindowLevelLookupTable::New();
    Saggital = vtkActor::New();
    SaggitalSect = vtkExtractVOI::New();
    SaggitalText = vtkTexture::New();
    SaggitalPlane = vtkPlaneSource::New();
    SaggitalMap = vtkPolyDataMapper::New();
    SaggitalText->SetLookupTable(LookupTable);
    SaggitalText->MapColorScalarsThroughLookupTableOff();

    Coronal = vtkActor::New();
    CoronalSect =  vtkExtractVOI::New();
    CoronalText = vtkTexture::New();
    CoronalPlane = vtkPlaneSource::New();
    CoronalMap = vtkPolyDataMapper::New();
    CoronalText->SetLookupTable(LookupTable);
    CoronalText->MapColorScalarsThroughLookupTableOff();

    Axial = vtkActor::New();
    AxialSect = vtkExtractVOI::New();
    AxialText = vtkTexture::New();
    AxialPlane = vtkPlaneSource::New();
    AxialMap = vtkPolyDataMapper::New();
    AxialText->SetLookupTable(LookupTable);
    AxialText->MapColorScalarsThroughLookupTableOff();
 
    Vol = vtkStructuredPoints::New();
    Object = NULL;
    Image = NULL;
  };

  void Clear()
  {
    LookupTable->Delete();
    //Saggital->Delete();
    SaggitalSect->Delete();
    SaggitalText->Delete();
    SaggitalPlane->Delete();
    SaggitalMap->Delete();

    //Coronal->Delete();
    CoronalSect->Delete();
    CoronalText->Delete();
    CoronalPlane->Delete();
    CoronalMap->Delete();

    //Axial->Delete();
    AxialSect->Delete();
    AxialText->Delete();
    AxialPlane->Delete();
    AxialMap->Delete();

    Vol->Delete();
    Object = NULL;
    Image = NULL;
  }

  ~ImageProp()
  {
    Clear();
  };

  unsigned long MTime;
  unsigned int  ID;

  vtkActor*           Saggital;
  vtkExtractVOI*      SaggitalSect;
  vtkPlaneSource*     SaggitalPlane;
  vtkPolyDataMapper*  SaggitalMap;
  vtkTexture*         SaggitalText;
  int                 SaggitalSlice;
  
  vtkActor*           Axial;
  vtkExtractVOI*      AxialSect;
  vtkPlaneSource*     AxialPlane;
  vtkPolyDataMapper*  AxialMap;
  vtkTexture*         AxialText;
  int                 AxialSlice;

  vtkActor*           Coronal;
  vtkExtractVOI*      CoronalSect;
  vtkPlaneSource*     CoronalPlane;
  vtkPolyDataMapper*  CoronalMap;
  vtkTexture*         CoronalText;
  int                 CoronalSlice;
  void*               Object; // pointer to the spatialobject
  const void*         Image; // pointer to the itkImage

  vtkStructuredPoints* Vol;
  vtkWindowLevelLookupTable*  LookupTable;
};



template <class TImage>
class ImageVTKRenderMethod3D
: public VTKRenderMethod3D
{

public:

  typedef ImageVTKRenderMethod3D          Self;
  typedef VTKRenderMethod3D               Superclass;
  typedef SmartPointer< Self >            Pointer;
  typedef SmartPointer< const Self >      ConstPointer;

  typedef TImage                           ImageType;
  typedef typename ImageType::ConstPointer ImageConstPointer; 
  typedef Superclass::ObjectType           ObjectType;
  typedef Superclass::ObjectListType       ObjectListType;
  typedef typename ImageType::PixelType    PixelType;

  typedef ImageSpatialObject<3,PixelType> ImageSpatialObjectType;

  itkNewMacro( ImageVTKRenderMethod3D );
  itkTypeMacro( ImageVTKRenderMethod3D, GlRenderMethod3D );

  virtual void Draw( void );

protected:

  ImageVTKRenderMethod3D( void );
  ~ImageVTKRenderMethod3D( void );
  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();

  std::list<ImageProp*>           m_ImagePropList;
  ObjectListType::iterator        m_CurrentObject;
  ImageConstPointer               m_ImData;
  std::list<ImageProp*>::iterator m_PropIt;
};

}

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovImageVTKRenderMethod3D.txx"
#endif

#endif //_ImageVTKRenderMethod3D_h
