/*******************************************************************
 * FILE:     main.cxx
 * PROJECT:  ITKUNC
 * AUTHORS:  CADDLab
 * DATE:     October 2002
 * COMMENTS: None
 *******************************************************************/

/**
 *
 */
#include "FltkSpatialObjectViewerSlicer.h"
#include <itkSpatialObjectReader.h>
#include <itkTubeSpatialObject.h>
#include <itkBlobSpatialObject.h>
#include <itkSceneSpatialObject.h>
#include <itkEllipseSpatialObject.h>
#include <itkSpatialObjectWriter.h>


int main(int , char** )
{ 
  typedef itk::TubeSpatialObject<>    TubeSpatialObjectType;  
  typedef itk::BlobSpatialObject<>    BlobSpatialObjectType;
  typedef itk::SceneSpatialObject<3> SceneType;
  SceneType::Pointer scene = SceneType::New();
  typedef itk::TubeSpatialObject<3>                   TubeType;
  typedef TubeType::TubePointType                     TubePointType;
  typedef TubeType::PointListType                     TubePointListType;
  typedef itk::GroupSpatialObject<3>                  GroupType;


  /** Create and Image */
  typedef short PixelType;
  typedef itk::Image<PixelType, 3> ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  typedef itk::ImageSpatialObject<3,PixelType> ImageSpatialObjectType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName("e:/Workspace/Data/BrainWeb/brainweb1e1a10f20.mha");
  reader->Update();
  ImageType::Pointer m_Image = reader->GetOutput();

   /** Create a Tube Net composed of 3 tubes */
 /* TubeType::PointListType list;

  float j=0;
  for( int i=-10; i<10; i++)
  {
    TubePointType p;
    p.SetPosition(i,i,i);
    p.SetRadius(1+0.1*fabs((float)i));
    p.SetRed(1);
    p.SetGreen(j);
    p.SetBlue(0);
    j+=0.05;
    list.push_back(p);
  }

  TubeType::Pointer m_Tube = TubeType::New();
  m_Tube->GetProperty()->SetName("Tube 1");
  m_Tube->GetProperty()->SetRed(1);
  m_Tube->SetId(1);
  m_Tube->SetPoints(list);*/

 
  // Test the ellipse
/*  typedef itk::EllipseSpatialObject<3> EllipseType;
  EllipseType::Pointer ellipse = EllipseType::New();
  double radius[3];
  radius[0] = 10;
  radius[1] = 10;
  radius[2] = 10;
  ellipse->GetProperty()->SetColor(1.0,0.0,0.0);
  ellipse->SetRadius(radius);
*/
  /*
  typedef itk::SpatialObjectWriter<3> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(ellipse);
  writer->SetFileName("e:/test.txt");
  writer->Update();
  
  
  typedef itk::EllipseSpatialObject<3> EllipseType;
  
  typedef itk::SpatialObjectReader<3> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName("e:/test.txt");
  reader->Update();


  EllipseType::Pointer ellipse2 = static_cast<EllipseType*>((*(reader->GetGroup()->GetChildren(9999)->begin())).GetPointer());

  std::cout << ellipse2->GetSpacing()[0] << std::endl;


  scene->AddSpatialObject(ellipse);
*/

 /* 
  ImageType::SizeType size = {{10,20,30}};
  ImageType::IndexType index = {{0,0,0}};
  ImageType::RegionType region;
  region.SetSize( size );
  region.SetIndex( index );

  ImageType::Pointer m_Image = ImageType::New();
  m_Image->SetRegions( region );
  m_Image->Allocate();
  ImageType::SpacingType spacing;
  spacing.Fill(1.5);
  m_Image->SetSpacing(spacing);
 
  typedef itk::ImageRegionIteratorWithIndex<ImageType> myIteratorType;

  myIteratorType it(m_Image,region);

  it.GoToBegin();
  unsigned int i = 0;
  while( !it.IsAtEnd() ) 
    { 
    it.Set( i++ );
    ++it;
    //it.Set( 0 );
    //++it;
    }
    */
/*
  ImageSpatialObjectType::Pointer m_ImageSpatialObject = ImageSpatialObjectType::New();
  m_ImageSpatialObject->SetImage(m_Image);
  m_ImageSpatialObject->GetProperty()->SetName("myImage");
  scene->AddSpatialObject(m_ImageSpatialObject);
*/

  FltkSpatialObjectViewerSlicer * viewer = new FltkSpatialObjectViewerSlicer;
  //viewer->GetRenderer()->SetScene(scene);
  viewer->SetImageSlicer(m_Image);
  viewer->Update();

//  viewer->GetRenderer()->GetVisualObject(brain)->GetProperty()->SetOpacity(0.3);
  
  viewer->Update();
  viewer->Show();
 
  Fl::run();

  delete viewer;
  return EXIT_SUCCESS;
}
