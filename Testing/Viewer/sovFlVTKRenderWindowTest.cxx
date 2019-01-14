#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "itkTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(sovFlVTKRenderWindowTest);
}

#include <FltkSpatialObjectViewer3D.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <itkTubeSpatialObject.h>
#include <sovVTKRenderer.h>
#include <sovSurfaceTubeVTKRenderMethod3D.h>
#include <sovCenterLineTubeVTKRenderMethod3D.h>
#include <itkSceneSpatialObject.h>

int sovFlVTKRenderWindowTest(int , char*[] )
{ 
  //vtkRenderWindow* ren = vtkRenderWindow::New();

  /** Create the window */
  FltkSpatialObjectViewer3D* myViewer = new FltkSpatialObjectViewer3D; 
  //sov::FlVTKDisplay* myViewer = new sov::FlVTKDisplay; 

  /** Create the spatial object to visualize */
  typedef itk::TubeSpatialObject<3>::Pointer TubePointer;

  /** Create a Tube Net composed of 3 tubes */
  typedef itk::TubeSpatialObjectPoint<3>      TubePointType;
  typedef itk::TubeSpatialObject<3>           TubeType;

  TubeType::PointListType list2;//  = new TubeType::PointListType();
  for( int i=-10; i<10; i++)
    {
    TubePointType p;
    p.SetPosition(-20,i,i);
    p.SetRadius(2);
    list2.push_back(p);
    }

  TubePointer tube2 = TubeType::New();
  tube2->GetProperty()->SetName("Tube 2");
  tube2->SetId(2);
  tube2->GetProperty()->SetRed(0);
  tube2->GetProperty()->SetBlue(1);
  tube2->GetProperty()->SetGreen(0);
  tube2->SetPoints(list2);
  tube2->ComputeBoundingBox();
/*
  myViewer->AddSpatialObjectToScene(tube2);
  
  try
    {
    myViewer->Show();
    }
  catch(...)
    {
    std::cout << "Exception thrown while executing myViewer->Show()" << std::endl;
    return EXIT_FAILURE;
    }

  //myViewer->Update();*/
/*
  std::cout << "Testing AssociateWithRenderMethod: ";
  try
    {
    myViewer->GetRenderer()->AssociateWithRenderMethod(tube2,"CenterLineTubeVTKRenderMethod3D");
    }
  catch(...)
    {
    std::cout << "[FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED]" << std::endl;
*/

 // myViewer->Update();

 // Fl::run();

  delete myViewer;

  std::cout << "Test [DONE]" << std::endl;

  return EXIT_SUCCESS;

}

