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
#include "QtSpatialObjectViewer3D.h"
#include <itkSceneSpatialObject.h>
#include <itkEllipseSpatialObject.h>
#include <qapplication.h>
#include <itkArrowSpatialObject.h>
#include <itkBoxSpatialObject.h>
#include <itkSpatialObjectReader.h>
#include <itkImageFileReader.h>
#include <qfiledialog.h>
#include <itkEuler3DTransform.h>
#include <itkUltrasoundProbeSpatialObject.h>

int main(int argc, char** argv)
{ 

  QtSpatialObjectViewer3D* viewer = new QtSpatialObjectViewer3D; // should be first
 /* 
  QString s( QFileDialog::getOpenFileName( QString::null) );

  typedef itk::SpatialObjectReader<3> ReaderType;
*/
/*
  typedef float PixelType;
  typedef itk::Image<float,3> ImageType;
  typedef itk::ImageFileReader<ImageType> ImageReaderType;
  ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName(s.latin1());
  reader->Update();

  typedef itk::ImageSpatialObject<3,PixelType> ImageSOType;
  ImageSOType::Pointer image = ImageSOType::New();
  image->SetImage(reader->GetOutput());


  QString s2( QFileDialog::getOpenFileName( QString::null) );
  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(s2.latin1());
  reader2->Update();*/



  // Create a Rectangle spatial object to represent the magnetic field emitter (this is the origin)
  typedef itk::BoxSpatialObject<3> CubeType;
  CubeType::Pointer emitter = CubeType::New();
  CubeType::SizeType cubeSize;
  cubeSize.Fill(80);
  emitter->SetSize(cubeSize);
  emitter->GetProperty()->SetColor(0.8,0.8,0.6);

  typedef itk::UltrasoundProbeSpatialObject USProbeType;
  USProbeType::Pointer probe = USProbeType::New();
  probe->GetObjectToWorldTransform()->SetOffset(50);

  typedef itk::SceneSpatialObject<3> SceneType;
  SceneType::Pointer scene = SceneType::New();


  /*std::cout << reader->GetOutput()->GetSpacing()[0] << std::endl;
  
  scene->AddSpatialObject(image);

  scene->AddSpatialObject(reader2->GetGroup());
*/
  scene->AddSpatialObject(emitter);
  scene->AddSpatialObject(probe);
  viewer->SetScene(scene);
 
  viewer->Show();

  delete viewer;
  return EXIT_SUCCESS;
}
