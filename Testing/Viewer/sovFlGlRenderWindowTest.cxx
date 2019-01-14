/*=========================================================================



  Program:   itkUNC

  Module:    $RCSfile: sovFlGlRenderWindowTest.cxx,v $

  Language:  C++

  Date:      $Date: 2005-05-23 15:27:43 $

  Version:   $Revision: 1.2 $

  Author:    Julien Jomier



  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.

  See itkUNCCopyright.txt for details.



     This software is distributed WITHOUT ANY WARRANTY; without even 

     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 

     PURPOSE.  See the above copyright notices for more information.



=========================================================================*/
#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include <iostream>

#include "itkTestMain.h" 



void RegisterTests()

{

  REGISTER_TEST(sovFlGlRenderWindowTest);

}





#include <FL/Fl.H>

#include <Fl/Fl_Window.H>

#include <itkEllipseSpatialObject.h>

#include <itkBlobSpatialObject.h>

#include <sovGlRenderer2D.h>

#include <FltkSpatialObjectViewer2D.h>



int sovFlGlRenderWindowTest(int, char*[])

{

 

  /** Create the window */

  FltkSpatialObjectViewer2D* myViewer = new FltkSpatialObjectViewer2D; 

  myViewer->GetRenderer()->SetImageSize(200,200);

  /*

  typedef itk::EllipseSpatialObject<2> SphereType;

  SphereType::Pointer sphere = SphereType::New();

  sphere->SetRadius(20);

  itk::Vector<double,2> offset;

  offset.Fill(100);

  sphere->GetProperty()->SetColor(0,0,1);

  sphere->GetObjectToParentTransform()->SetOffset(offset);

  sphere->ComputeObjectToWorldTransform();



  myViewer->AddSpatialObjectToScene(sphere);

*/



  typedef itk::BlobSpatialObject<2> BlobType;

  BlobType::Pointer blob = BlobType::New();

  blob->GetProperty()->SetColor(0,0,1);



  BlobType::BlobPointType pt;

  pt.SetPosition(100,100,100);

  BlobType::PointListType lst;

  lst.push_back(pt);



  blob->SetPoints(lst);





  myViewer->AddSpatialObjectToScene(blob);



  

  try

    {

    myViewer->Show();

    }

  catch(...)

    {

    std::cout << "Exception thrown while executing myViewer->Show()" << std::endl;

    return EXIT_FAILURE;

    }



  myViewer->Update();

  myViewer->Update();





  //Fl::run();



  return EXIT_SUCCESS;

}

