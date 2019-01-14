/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: main.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "SpatialObjectManager.h"
#include <itkSceneSpatialObject.h>
#include "itkTubeSpatialObject.h"
#include "itkEllipseSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkGroupSpatialObject.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkSpatialObject.h"
#include "itkAffineTransform.h"
#include "itkSpatialObjectWriter.h"
#include "itkSpatialObjectReader.h"
#include "itkImageRegionIteratorWithIndex.h"

int main(void)
{
  typedef itk::TubeSpatialObject<3>        TubeType;
  typedef TubeType::Pointer                TubePointer;
  typedef itk::GroupSpatialObject<3> TubeNetType;
  typedef TubeNetType::Pointer             TubeNetPointer;
  typedef itk::EllipseSpatialObject<3>     EllipseType;
  typedef EllipseType::Pointer             EllipsePointer;
  typedef itk::ImageSpatialObject<3,unsigned short>  ImageType;

  typedef itk::SpatialObjectWriter<3,unsigned short> WriterType;
  typedef itk::SpatialObjectReader<3,unsigned short> ReaderType;

  typedef itk::TubeSpatialObjectPoint<3>  TubePointType;

   // Create the scene
  typedef itk::SpatialObject<3> NDimObjectType;

  std::cout<<"=================================="<<std::endl;
  std::cout<<"Creating a tube GroupSpatialObject:"<<std::endl<<std::endl;

  TubeType::PointListType list;
  TubeType::PointListType list2;
  TubeType::PointListType list3;

  for( unsigned int i=0; i<10; i++)
  {
    TubePointType p;
    p.SetPosition(i,i,i);
    p.SetRadius(i);
    list.push_back(p);
  }
  

  for( unsigned int i=0; i<5; i++)
  {
    TubePointType p;
    p.SetPosition(i*2,i*2,i*2);
    p.SetRadius(i*2);
    list2.push_back(p);
  }

   for( unsigned int i=0; i<7; i++)
  {
    TubePointType p;
    p.SetPosition(i*3,i*3,i*3);
    p.SetRadius(i*3);
    list3.push_back(p);
  }


  /** Create a Tube Net composed of 3 tubes */
  TubePointer tube1 = TubeType::New();
  tube1->GetProperty()->SetName("Tube 1");
  tube1->SetId(1);
  tube1->SetPoints(list);
  tube1->ComputeBoundingBox();

  TubePointer tube2 = TubeType::New();
  tube2->GetProperty()->SetName("Tube 2");
  tube2->SetId(2);
  tube2->SetPoints(list2);
  tube2->ComputeBoundingBox();

  TubePointer tube3 = TubeType::New();
  tube3->GetProperty()->SetName("Tube 3");
  tube3->SetId(3);
  tube3->SetPoints(list3);
  tube3->ComputeBoundingBox();

 
  TubeNetPointer tubeNet1 = TubeNetType::New();
  tubeNet1->GetProperty()->SetName("tube network 1");
  tubeNet1->AddSpatialObject( tube1 );
  tubeNet1->AddSpatialObject( tube2 );


  TubeNetPointer tubeNet2 = TubeNetType::New();
  tubeNet2->GetProperty()->SetName("tube network 2");
  tubeNet2->SetParentId(1);
  tubeNet2->AddSpatialObject( tube3 );

  EllipsePointer ellipse = EllipseType::New();
  ellipse->SetRadius(9);
  ellipse->GetProperty()->SetName("ellipse 1");


  typedef ImageType::ImageType itkImageType;
  typedef itkImageType::Pointer     ImagePointer;
  typedef itkImageType::SizeType    SizeType;
  typedef itkImageType::RegionType  RegionType;

  ImagePointer itkImage = itkImageType::New();

  SizeType size;

  unsigned int dim = 3;
  double spacing[3];

  for(unsigned int i=0;i<dim;i++)
  {
    size[i] = 10;
    spacing[i] = i;
  }

  RegionType region;
  region.SetSize(size);
  itkImage->SetLargestPossibleRegion(region);
  itkImage->SetBufferedRegion(region);
  itkImage->SetRequestedRegion(region);
  itkImage->SetSpacing(spacing);
  itkImage->Allocate();

  itk::ImageRegionIteratorWithIndex< itkImageType > it(itkImage, region);
  for(unsigned int i = 0; !it.IsAtEnd(); i++, ++it)
  {
    it.Set(i);
  }


  ImageType::Pointer image = ImageType::New();
  image->GetProperty()->SetName("Image 1");
  image->SetImage(itkImage);

  tubeNet2->AddSpatialObject( image );
  tubeNet2->AddSpatialObject( ellipse );

  tubeNet1->AddSpatialObject( tubeNet2 );

  if( tubeNet1->GetNumberOfChildren() != 3 )
  {
    std::cout<<"[FAILED]"<<std::endl;
    return 0;
  }
  else
  {
    std::cout<<"[PASSED]"<<std::endl;
  }
/*
  std::cout<<"=================================="<<std::endl;
  std::cout<<"Testing Writing SceneSpatialObject:"<<std::endl<<std::endl;

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(tubeNet1);
  writer->SetFullFileName("Tube-Ellipse.meta");
  writer->Update();
*/
  SpatialObjectManager::Pointer myManager = SpatialObjectManager::New();
  itk::SceneSpatialObject<3>::Pointer myFirstScene = itk::SceneSpatialObject<3>::New();
  myFirstScene->AddSpatialObject(tubeNet1);
  myManager->SetScene(myFirstScene);
  myManager->Update();
  myManager->Show();

  std::cout<<"[PASSED]"<<std::endl;
/*
  std::cout<<"=================================="<<std::endl;
  std::cout<<"Testing Reading SceneSpatialObject:"<<std::endl<<std::endl;


  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFullFileName("Tube-Ellipse.meta");
  reader->Update();
  
  ReaderType::ScenePointer myScene = reader->GetScene();

  SpatialObjectManager::Pointer myManager2 = SpatialObjectManager::New();
  myManager2->SetScene(myScene);
  myManager2->Update();
  myManager2->Show();
*/
  std::cout << "Done !" << std::endl;
  Fl::run();
  return 1;
}


