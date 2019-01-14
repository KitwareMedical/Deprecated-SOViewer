/*******************************************************************
 * FILE:     PhantomAppGUIControls.cxx
 * PROJECT:  ITKUNC
 * AUTHORS:  CADDLab
 * DATE:     October 2002
 * COMMENTS: None
 *******************************************************************/
#include "PhantomAppGUIControls.h"
#include "solTubeGlRenderMethodSlicer.h"
#include "solSurfaceTubeGlRenderMethodSlicer.h"
#include <itkEuler3DTransform.h>
/**
 *
 */
PhantomAppGUIControls::PhantomAppGUIControls( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : PhantomAppGUI( parent, name, modal, fl )
{
  m_Timer = new QTimer(this);
  connect( m_Timer, SIGNAL(timeout()),
          this, SLOT(IdleFunction()) );
  m_Timer->start(50);
}

/** Destroys the object and frees any allocated resources */
PhantomAppGUIControls::~PhantomAppGUIControls()
{
  m_Timer->stop();
  delete m_Timer;
}

/** Idle Function */
void
PhantomAppGUIControls
::IdleFunction(void)
{
  gwpTransformMatrix transformMatrix = m_Phantom->getTransformMatrix();

  double position[3];
  itk::Matrix<double,3,3> matrix;
  for(unsigned int i=0;i<3;i++)
  {
    for(unsigned int j=0;j<3;j++)
    { 
      // help the rotation by multiplying the factor
      matrix[i][j]=2*transformMatrix[j][i];
    }
  }

  for(unsigned int i=0;i<3;i++)
  {
    position[i]=transformMatrix[3][i];
  }

  TubeType::TransformType::OffsetType offset;
  offset[0]=position[0];
  offset[1]=position[1];
  offset[2]=position[2];
  m_TubeNetwork->GetLocalToGlobalTransform()->SetOffset(offset);  
  m_TubeNetwork->GetLocalToGlobalTransform()->SetMatrix(matrix);

  bool ButtonIsDown = m_Phantom->getStylusSwitch();
  if(ButtonIsDown)
  {
    m_TubeNetwork->GetProperty()->SetRed(0);
    m_TubeNetwork->GetProperty()->SetBlue(1);
  }
  else
  {  
    m_TubeNetwork->GetProperty()->SetRed(1);
    m_TubeNetwork->GetProperty()->SetBlue(0);
  }

  //m_TubeNetwork->Update();
  Display4->Update();
}

bool
PhantomAppGUIControls
::StartPhantom(void)
{
  // create the root separator and set it as the root of the scene graph
  m_RootSep = new gwpSeparator();
  m_GhostScene.setRoot(m_RootSep);

  // prompt the user to place the PHANToM in the reset position
  std::cout << "Place the PHANToM in its reset position and press <ENTER>." << std::endl;
  std::cin.get();

  // create a PHANToM instance and check to make sure it is valid
  m_Phantom = new gwpPHANToM("Default PHANToM");
  if (!m_Phantom || !m_Phantom->getValidConstruction()) 
  {
    std::cerr << "Unable to initialize PHANToM device." << std::endl;
    return false;
  }
    
  // add the PHANToM object to the scene
  m_RootSep->addChild(m_Phantom);

  // start the haptic simulation
  m_GhostScene.startServoLoop();
  std::cout << "PhantomStarted." << std::endl;
  return true;
}

bool
PhantomAppGUIControls
::StopPhantom(void)
{
  m_Timer->stop();
  m_GhostScene.stopServoLoop();
  std::cout << "Phantom Stoped." << std::endl;
  return true;
}


void PhantomAppGUIControls::DisplayPosition(int x,int y ,int z,float value)
{
}

void PhantomAppGUIControls::CreateScene(void)
{
 
  /** Create a Tube Net composed of 3 tubes */
  TubeType::PointListType list;

  float j=0;
  for( int i=0; i<10; i++)
  {
    TubePointType p;
    p.SetPosition(0,0,i);
    p.SetRadius(0.1+0.1*fabs(i));
    p.SetRed(1);
    p.SetGreen(j);
    p.SetBlue(0);
    j+=0.05;
    //p.SetBlue(i*0.1);
    list.push_back(p);
  }

  m_Tube = TubeType::New();
  m_Tube->SetReferenceCount(2);
  m_Tube->GetProperty()->SetName("Tube 1");
  m_Tube->GetProperty()->SetRed(1);
  m_Tube->SetId(1);
  m_Tube->SetPoints(list);
  m_Tube->ComputeBounds();

  m_TubeNetwork = TubeNetType::New();
  m_TubeNetwork->AddSpatialObject(m_Tube);
  m_TubeNetwork->GetProperty()->SetRed(1);
  
  /** Define the scene */

  m_Scene = SceneType::New();
  m_Scene->AddSpatialObject(m_TubeNetwork);

  ImageType::SizeType size = {{20,20,20}};
  ImageType::IndexType index = {{0,0,0}};
  ImageType::RegionType region;
  region.SetSize( size );
  region.SetIndex( index );

  ImageType::Pointer m_Image = ImageType::New();
  m_Image->SetLargestPossibleRegion( region );
  m_Image->SetBufferedRegion( region) ;
  m_Image->SetRequestedRegion( region );
  m_Image->Allocate();
  double spacing[3];
  spacing[0]=1;
  spacing[1]=1;
  spacing[2]=1;
  m_Image->SetSpacing(spacing);
 
  typedef  itk::ImageRegionIteratorWithIndex<ImageType> myIteratorType;

  myIteratorType it(m_Image,region);

  unsigned int i=0;
  while( !it.IsAtEnd() ) 
  { 
    it.Set( i );
    ++it;
    i++;
  }


  m_ImageSpatialObject = ImageSpatialObjectType::New();
  m_ImageSpatialObject->SetImage(m_Image);
  m_ImageSpatialObject->GetProperty()->SetName("myImage");

  m_Scene->AddSpatialObject(m_ImageSpatialObject);

  /** Create a 3D VTK Renderer */
  typedef sol::VTKRenderer3D RendererType;
  RendererType::Pointer myRenderer3D = RendererType::New();

  /** Create the Render Method and plug it to the renderer */
  typedef sol::SurfaceTubeVTKRenderMethod3D RenderMethodType1;
  RenderMethodType1::Pointer myRenderMethod1 = RenderMethodType1::New();

  // Image Renderer Method 3D
  typedef sol::ImageVTKRenderMethod3D<ImageType> ImageRenderMethodType;
  ImageRenderMethodType::Pointer m_ImageRenderMethod3D = ImageRenderMethodType::New();
  
  // Add the object to the renderer
  myRenderer3D->AddRenderMethod(myRenderMethod1);
  myRenderer3D->AddRenderMethod(m_ImageRenderMethod3D);
  myRenderer3D->SetScene(m_Scene);

  static_cast<sol::VisualImageProperty*>(myRenderer3D->GetVisualObject("myImage")->GetProperty())->SetOpacity(0,0.5);
  static_cast<sol::VisualImageProperty*>(myRenderer3D->GetVisualObject("myImage")->GetProperty())->SetOpacity(1,0.5);
  static_cast<sol::VisualImageProperty*>(myRenderer3D->GetVisualObject("myImage")->GetProperty())->SetOpacity(2,0.5);

  myRenderer3D->Update();
  Display4->AddRenderer(myRenderer3D);
  Display4->Update();
  myRenderer3D->GetVTKRenderer()->ResetCamera();

  // Create a 3D OpenGL Renderer
  myRendererSlicer = RendererSlicerType::New();
  myRendererSlicer->SetName("RendererSlicer1");
  myRendererSlicer->SetOrientation(0);


  // Create a 2D image renderer method
  typedef sol::ImageGlRenderMethodSlicer<ImageType> RenderMethodImageSlicerType;
  RenderMethodImageSlicerType::Pointer myRenderMethodImageSlicer = RenderMethodImageSlicerType::New();

  // Create a 2D surface renderer method
  typedef sol::SurfaceGlRenderMethodSlicer SurfaceRenderMethodSlicerType;
  SurfaceRenderMethodSlicerType::Pointer myRenderMethodSurfaceSlicer = SurfaceRenderMethodSlicerType::New();

  // Create a 2D tube renderer method
  typedef sol::SurfaceTubeGlRenderMethodSlicer TubeRenderMethodSlicerType;
  TubeRenderMethodSlicerType::Pointer myRenderMethodTubeSlicer = TubeRenderMethodSlicerType::New();

  myRendererSlicer->AddRenderMethod(myRenderMethodTubeSlicer);
  myRendererSlicer->AddRenderMethod(myRenderMethodImageSlicer);
  
  myRendererSlicer->SetScene(m_Scene);
  
  //myRendererSlicer->Active(true);
  Display1->AddRenderer(myRendererSlicer);
  
  // Create a second 3D OpenGL Renderer 
  myRendererSlicer2 = RendererSlicerType::New();
  myRendererSlicer2->SetOrientation(1);
  RenderMethodImageSlicerType::Pointer myRenderMethodImageSlicer2 = RenderMethodImageSlicerType::New();
  SurfaceRenderMethodSlicerType::Pointer myRenderMethodSurfaceSlicer2 = SurfaceRenderMethodSlicerType::New();
  TubeRenderMethodSlicerType::Pointer myRenderMethodTubeSlicer2 = TubeRenderMethodSlicerType::New();

  myRendererSlicer2->AddRenderMethod(myRenderMethodTubeSlicer2);
  myRendererSlicer2->AddRenderMethod(myRenderMethodImageSlicer2);
  myRendererSlicer2->SetScene(m_Scene);

  static_cast<sol::VisualImageProperty*>(myRendererSlicer2->GetVisualObject("myImage")->GetProperty())->SetOrientation(1);

  Display2->AddRenderer(myRendererSlicer2);
  

 // Create a second 3D OpenGL Renderer 
  myRendererSlicer3 = RendererSlicerType::New();
  myRendererSlicer3->SetOrientation(2);
  RenderMethodImageSlicerType::Pointer myRenderMethodImageSlicer3 = RenderMethodImageSlicerType::New();
  
  TubeRenderMethodSlicerType::Pointer myRenderMethodTubeSlicer3 = TubeRenderMethodSlicerType::New();

  myRendererSlicer3->AddRenderMethod(myRenderMethodTubeSlicer3);
  myRendererSlicer3->AddRenderMethod(myRenderMethodImageSlicer3);
  myRendererSlicer3->SetScene(m_Scene);

  static_cast<sol::VisualImageProperty*>(myRendererSlicer3->GetVisualObject("myImage")->GetProperty())->SetOrientation(2);


  Display3->AddRenderer(myRendererSlicer3);

  Slider1->setMaxValue(m_ImageSpatialObject->GetImage()->GetLargestPossibleRegion().GetSize()[0]);
  Slider2->setMaxValue(m_ImageSpatialObject->GetImage()->GetLargestPossibleRegion().GetSize()[1]);
  Slider3->setMaxValue(m_ImageSpatialObject->GetImage()->GetLargestPossibleRegion().GetSize()[2]);

  Display1->Update();
  Display2->Update();
  Display3->Update();
  Display4->Update();
}


void PhantomAppGUIControls::ChangeAxialSlice(int value)
{
  m_ImageSpatialObject->SetSlicePosition(0,value);
  myRendererSlicer->SetSliceNumber(value);
  Display1->Update();
  Display4->Update();
}

void PhantomAppGUIControls::ChangeCoronalSlice(int value)
{
  m_ImageSpatialObject->SetSlicePosition(1,value);
  myRendererSlicer2->SetSliceNumber(value);
  Display2->Update();
  Display4->Update();
}

void PhantomAppGUIControls::ChangeSaggitalSlice(int value)
{
  m_ImageSpatialObject->SetSlicePosition(2,value);
  myRendererSlicer3->SetSliceNumber(value);
  Display3->Update();
  Display4->Update();
}
