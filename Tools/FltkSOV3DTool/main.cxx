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
#include "FltkSpatialObjectViewer3D.h"
#include <itkSpatialObjectReader.h>
#include <itkTubeSpatialObject.h>
#include <itkBlobSpatialObject.h>
#include <itkSceneSpatialObject.h>
#include <itkEllipseSpatialObject.h>


int main(int , char** )
{ 

/*  typedef itk::SceneSpatialObject<3> SceneType;
  SceneType::Pointer scene = SceneType::New();
*//*
  typedef itk::SpatialObjectReader<>    SpatialObjectReaderType;
  */
/*
  SpatialObjectReaderType::Pointer  arteries_reader = SpatialObjectReaderType::New();

  arteries_reader->SetFileName( "pilot001_arteries.tre" );
  arteries_reader->Update();

  ObjectListType * sceneChildren = arteries_reader->GetGroup()->GetChildren();
  ObjectListType::const_iterator i = sceneChildren->begin();

  double mra_spacing[3];
  mra_spacing[0] = 0.839;
  mra_spacing[1] = 0.839;
  mra_spacing[2] = 1;

  i++;
  while(i != sceneChildren->end() )
  {
    TubeSpatialObjectType::Pointer arteries = static_cast<TubeSpatialObjectType*>((*i).GetPointer());
    arteries->SetSpacing( mra_spacing );
    scene->AddSpatialObject( arteries );
    i++;
  }
  SpatialObjectReaderType::Pointer  veins_reader = SpatialObjectReaderType::New();
  veins_reader->SetFileName( "vessel_mra_converted.tre" );
  veins_reader->Update();

  ObjectListType * sceneChildren2 = veins_reader->GetGroup()->GetChildren();
  ObjectListType::const_iterator i2 = sceneChildren2->begin();

  double mrv_spacing[3];
  mrv_spacing[0] = 1;
  mrv_spacing[1] = 1;
  mrv_spacing[2] = 1;

  i2++;
  while( i2 != sceneChildren2->end() )
  {
    TubeSpatialObjectType::Pointer veins = static_cast<TubeSpatialObjectType*>((*i2).GetPointer());
    veins->SetSpacing( mrv_spacing );
    scene->AddSpatialObject( veins );
    ++i2;
  }

  SpatialObjectReaderType::Pointer  blob_reader = SpatialObjectReaderType::New();
  blob_reader->SetFileName( "pilot004_motor.fmri.so" );
  blob_reader->Update();

  ObjectListType * blobChildren = blob_reader->GetGroup()->GetChildren();
  ObjectListType::const_iterator  ib = blobChildren->begin();

  while( ib != blobChildren->end() )
  {
    BlobSpatialObjectType::Pointer blob = static_cast<BlobSpatialObjectType*>((*ib).GetPointer() );
    blob->SetSpacing( mra_spacing );
    scene->AddSpatialObject( blob );
    ib++;
  }

  SpatialObjectReaderType::Pointer  blob_reader2 = SpatialObjectReaderType::New();
  blob_reader2->SetFileName( "pilot004_add.fmri.so" );
  blob_reader2->Update();

  ObjectListType * blobChildren2 = blob_reader2->GetGroup()->GetChildren();
  ObjectListType::const_iterator  ib2 = blobChildren2->begin();

  while( ib2 != blobChildren2->end() )
  {
    BlobSpatialObjectType::Pointer blob = static_cast<BlobSpatialObjectType*>((*ib2).GetPointer() );
    blob->SetSpacing( mra_spacing );
    scene->AddSpatialObject( blob );
    ib2++;
  }
*/ 

  /*
  typedef itk::LandmarkSpatialObject<3> SphereType;
  SphereType::Pointer sphere = SphereType::New();
  double radius[3];
  radius[0]=20;
  radius[1]=2;
  radius[2]=3;
  sphere->SetRadius(radius);
  sphere->GetProperty()->SetColor(1.0,1.0,0.0);
*/
/*
  typedef itk::SpatialObjectReader<3>    SpatialObjectReaderType;
  typedef SpatialObjectReaderType::GroupType::ChildrenListType    ObjectListType;

  SpatialObjectReaderType::Pointer  brain_reader = SpatialObjectReaderType::New();

  std::cout << "Reading brain ...";
  brain_reader->SetFileName("h:/brain_convert.cnc");
  brain_reader->Update();
  std::cout << "ok" << std::endl;
  
  ObjectListType* brainlist = brain_reader->GetGroup()->GetChildren();
  BlobSpatialObjectType::Pointer brain = static_cast<BlobSpatialObjectType*>((*(brainlist->begin())).GetPointer());

  delete brainlist;

  typedef itk::SceneSpatialObject<3> SceneType;
  SceneType::Pointer scene = SceneType::New();
  scene->AddSpatialObject(brain);
  //scene->AddSpatialObject(tubes);
  */
  FltkSpatialObjectViewer3D * viewer = new FltkSpatialObjectViewer3D();
 // viewer->SetScene(scene);
 // viewer->Update();

//  viewer->GetRenderer()->GetVisualObject(brain)->GetProperty()->SetOpacity(0.3);
  
  viewer->Update();
  viewer->Show();
 
  Fl::run();

  delete viewer;
  return EXIT_SUCCESS;
}
