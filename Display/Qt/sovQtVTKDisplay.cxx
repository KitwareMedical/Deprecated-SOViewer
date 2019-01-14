/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovQtVTKDisplay.cxx,v $
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
#include "sovQtVTKDisplay.h"

#include "vtkMath.h"
#include "vtkLightCollection.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkMath.h"
#include "sovVTKRenderer.h"
#include "sovVTKRenderer3D.h"

QtVTKDisplay::QtVTKDisplay(QGLFormat glf, QWidget * parent, const char * name )
  : QGLWidget(glf, parent, name) 
{
  this->Init();
}


QtVTKDisplay::QtVTKDisplay( QWidget * parent, const char * name)
  : QGLWidget(parent, name) 
{
  this->Init();
}

/** Set the mouse interactor */
void 
QtVTKDisplay
::SetMouseInteractor(sov::MouseInteractor* interactor)
{
  this->m_MouseInteractor = interactor;
  this->m_MouseInteractor->SetRendererList(this->m_RendererList);
}

/** Set the keyboard interactor */
void 
QtVTKDisplay
::SetKeyboardInteractor(sov::KeyboardInteractor* interactor)
{
  this->m_KeyboardInteractor = interactor;
  this->m_KeyboardInteractor->SetRendererList(this->m_RendererList);
}

void QtVTKDisplay::Init()
{
  this->renderer = NULL;
  this->m_MouseInteractor = NULL;
  this->m_KeyboardInteractor = NULL;
  this->m_RendererList = new RendererListType();
  this->renderWindow = vtkRenderWindow::New();
}

void QtVTKDisplay::InitQtVTKDisplay() 
{
  this->initNeeded = true;
  this->renderWindow->AddRenderer( this->renderer );
  this->light = vtkLight::New();
  this->renderer->AddLight( this->light );
  // just enable one double buffering
  // by default, we use the one of vtk
  #ifdef QT_DOUBLEBUFFER
    // use the double buffering of QT
    this->QtVTKDisplay->SetSwapBuffers(0);
  #else
     // use the double buffering of vtk
    this->setAutoBufferSwap( false );
 #endif


 this->SetLeftButtonFunction( SLOT(CameraRotate()) );
 this->SetMiddleButtonFunction( SLOT(CameraTranslate()) );
 this->SetRightButtonFunction( SLOT(CameraZoom()) );
 this->SetShiftLeftButtonFunction( SLOT(CameraRoll()) );
 this->SetResizeFunction( SLOT(Resize()) );

 this->mouseEventActive = false;
 this->SetInteractionTime( 50 );
 this->SetSensitivity( 10.0 );

 this->stepTimer = new QTimer();
 CHECK_PTR( this->stepTimer );
 this->currentCamera = this->GetRenderer()->GetActiveCamera();
 CHECK_PTR( this->currentCamera );

 // get the light we are using
 vtkLightCollection *lightCollection = this->GetRenderer()->GetLights();
 lightCollection->InitTraversal();
 this->currentLight = lightCollection->GetNextItem();
 CHECK_PTR( this->currentLight );

 this->interactionActor = NULL;
 this->collisionActor = NULL;
}


QtVTKDisplay::~QtVTKDisplay() 
{
  /*if(this->renderer)
    {
    this->renderer->Delete();
    }*/
  this->renderWindow->Delete();
  delete this->stepTimer;
}

/** Set the list of renderers */
void 
QtVTKDisplay
::SetRendererList( RendererListType * list )
{
  m_RendererList = list;
}

/** Add a renderer to the scene */
void
QtVTKDisplay
::AddRenderer( RendererType * renderer )
{
  //renderer->SetReferenceCount(2); // TRICKY !!!

  m_RendererList->push_back(renderer);
  m_RendererList->unique();

  RendererListType::iterator it,end;
  it = m_RendererList->begin();
  this->renderer=static_cast<sov::VTKRenderer<3>*>((*it).GetPointer())->GetVTKRenderer();
  this->InitQtVTKDisplay();
}

/** Redraw all renderers */
void
QtVTKDisplay
::Update(void)
{
  RendererListType::const_iterator it = m_RendererList->begin();
  while(it != m_RendererList->end())
  {
    (*it)->Update();
    (*it)->draw();
    it++;
  }
  this->updateGL();
}

/** Get the list of renderers */
QtVTKDisplay::RendererListType *
QtVTKDisplay
::GetRendererList()
{
  return m_RendererList;
}

vtkRenderer *QtVTKDisplay::GetRenderer() 
{
  return this->renderer;
}


vtkRenderWindow *QtVTKDisplay::GetRenderWindow( ) 
{
  return this->renderWindow;
}


void QtVTKDisplay::initializeGL( ) 
{
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glColor3f(1.0, 0.0, 0.0);
}

/** Resize the OpenGL Window */
void QtVTKDisplay::resizeGL( int w, int h) 
{
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluOrtho2D( 0.0, w, 0.0, h );
  glMatrixMode( GL_MODELVIEW );
  
  //this->status->SetPosition(0,w-5);
  //this->data->SetPosition(0,5);
  
  updateGL();
}


void QtVTKDisplay::paintGL( )
{
  // Get the native window ID and pass it
  // to the VTK render window
  // before we render for the first time...
  if (this->initNeeded) 
  {
    WId nId = winId();
    this->renderWindow->SetWindowId( (void*) nId );
    this->initNeeded = false;
  }
  else 
  {
    this->renderWindow->Render();
  }
}

/** Returns the class name */
const char *QtVTKDisplay::GetClassName() 
{
 return( "QtVTKDisplay" );
}


/** Sets the plane for the actor interaction ActorPlaneMove() and ActorPlaneRoll().
 *  The ebene pointer to a float array with the four values, 
 *  which specifiy a plane (ax+by+cz+d=1). */
void QtVTKDisplay::SetPlane(float *ebene) 
{
  this->normale[0]     = ebene[0];
  this->normale[1]     = ebene[1];
  this->normale[2]     = ebene[2];
  this->ebenekonstante = ebene[3];
}

/** Sets the sensitivity of all interactions */
void QtVTKDisplay::SetSensitivity( float t ) 
{
  this->trackballFactor = t;
}

/** Returns the current sensitivity of all interactions */
float QtVTKDisplay::GetSensitivity() 
{
   return this->trackballFactor;
}


/** Sets the function, which will be used, when the left mouse 
 *  button is pressed
 *  The function can be: ActorRotate(),ActorTranslate(),ActorZoom()
 *                       ActorRoll(), ActorPlaneMove(), ActorPlaneRoll()
 *                       CameraRotate(), CameraTranslate(), CameraZoom()
 *                       CameraRoll(), DoNothing()
 *
 * function is the name of the function, wrapped in a slot, e.g.
 * SLOT(CameraRotate()). If function is NULL,  DoNothing() will be used.*/
void QtVTKDisplay::SetLeftButtonFunction( const char* function )
{
  if( function == NULL )
  {
    strncpy( this->leftButtonFunction, SLOT(DoNothing()), 100 );
  }
  else 
  {
    strncpy( this->leftButtonFunction, function, 100 );
  }
}


/** Sets the function, which will be used, when the middle mouse 
 *  button is pressed */
void QtVTKDisplay::SetMiddleButtonFunction( const char* function )
{
  if( function == NULL ) 
  {
    strncpy( this->middleButtonFunction, SLOT(DoNothing()), 100 );
  }
  else 
  {
    strncpy( this->middleButtonFunction, function, 100 );
  }
}


/** Sets the function, which will be used, when the right mouse button 
 * is pressed */
void QtVTKDisplay::SetRightButtonFunction( const char* function ) 
{
  if( function == NULL )
  {
    strncpy( this->rightButtonFunction, SLOT(DoNothing()), 100 );
  }
  else 
  {
    strncpy( this->rightButtonFunction, function, 100 );
  }
}


/** Sets the function, which will be used, when the left button and 
 *  the shift key are pressed */
void QtVTKDisplay::SetShiftLeftButtonFunction(const char* function ) 
{
 if( function == NULL ) 
 {
   strncpy( this->shiftLeftButtonFunction, SLOT(DoNothing()), 100 );
 }
 else 
 {
   strncpy( this->shiftLeftButtonFunction, function, 100 );
 }
}

void QtVTKDisplay::resizeEvent( QResizeEvent * event )
{
 if(m_MouseInteractor)
    {
    this->m_MouseInteractor->SetDisplaySize(
                        event->size().width(),
                        event->size().height());
   }

  if(m_KeyboardInteractor)
    {
    this->m_KeyboardInteractor->SetDisplaySize(
                      event->size().width(),
                      event->size().height());
    }
}

void QtVTKDisplay::SetResizeFunction( const char* function )
{
  if( function == NULL )
  {
    strncpy( this->resizeFunction, SLOT(DoNothing()), 100 );
  }
  else 
  {
    strncpy( this->resizeFunction, function, 100 );
  }
}

/** Sets the actor, which will be used in actor interactions. */
void QtVTKDisplay::SetInteractionActor( vtkActor* actor ) 
{
  this->interactionActor = actor;
}

/** Returns the actor, which is used in actor interactions.*/
vtkActor *QtVTKDisplay::GetInteractionActor( void ) 
{
   return this->interactionActor;
}

/** Sets the point, in actor coordinate system, around which we rotate the
 *  current actor.
 *  Important: Set first the actor with
 *  SetInteractionActor() before using this function. */
void QtVTKDisplay::SetActorRotation( float x, float y, float z )
{
  ASSERT( this->interactionActor != NULL );
  // set it to the interactionActor
  this->interactionActor->SetOrigin( x, y, z );
}


/** catches the mouse press to react appropriate */
void QtVTKDisplay::mousePressEvent( QMouseEvent *event ) 
{
  if( m_MouseInteractor )
  {
    if( event->button() & LeftButton )
      {
      if( event->state() & AltButton )
        {
        //left mouse click and control button
        this->m_MouseInteractor->Push(event->x(),event->y(),0);
        }
      }
  }

  if( this->mouseEventActive ) 
  {
    return;
  }

  if( event->button() & LeftButton ) 
  {
    if( event->state() & ShiftButton )
    {
      // left mouse click and shift button
      this->mouseEventActive = true;
      QObject::connect( this->stepTimer, SIGNAL(timeout()),
                        this->shiftLeftButtonFunction );
    }
    else
    {
      this->mouseEventActive = true;
      QObject::connect( this->stepTimer, SIGNAL(timeout()),
                        this,this->leftButtonFunction );
         
      QObject::connect( this->stepTimer, SIGNAL(timeout()), this, SLOT(CameraRotate()) );
    }
  }
  else if( event->button() & MidButton ) 
  {
    // middle mouse button
    this->mouseEventActive = true;
    QObject::connect( this->stepTimer, SIGNAL(timeout()),
                      this->middleButtonFunction );
  }
  else if( event->button() & RightButton ) 
  {
    // right mouse button
    this->mouseEventActive = true;
    QObject::connect( this->stepTimer, SIGNAL(timeout()),
                      this, this->rightButtonFunction );
  }

  if( this->mouseEventActive ) 
  {
    this->currentMousePos[0] = event->x();
    this->currentMousePos[1] = event->y();
    this->lastMousePos[0] = event->x();
    this->lastMousePos[1] = event->y();
    this->firstCall = true;
    this->stepTimer->start( this->interactionTime );
  }
}


/** Overriden to catch mouseReleaseEvents and to stop the internal timer. */
void QtVTKDisplay::mouseReleaseEvent( QMouseEvent *event ) 
{
  if( this->mouseEventActive )
  {
    this->stepTimer->stop();
    this->mouseEventActive = false;
    QObject::disconnect( this->stepTimer, 0, this, 0 );
  }

  emit( Released(event->x(), this->height() - event->y()) );
}


/** catches all movements of the mouse */
void QtVTKDisplay::mouseMoveEvent( QMouseEvent *event ) 
{
  this->currentMousePos[0] = event->x();
  this->currentMousePos[1] = event->y();
}


/** Set the time in milliseconds between two interactions. 
 *  If you have huge objects and the interaction is too fast 
 *  for the rendering, set the value bigger.
 *  Note that on some systems, the timer of qt (QTimer) can not be faster
 *  than the internal clock, which is around 16 msec (measured on WinNT 4.0). */
void QtVTKDisplay::SetInteractionTime( int time ) 
{
  this->interactionTime = time;
}

/** Get the time in milliseconds between two interactions */
int QtVTKDisplay::GetInteractionTime( void ) 
{
  return this->interactionTime;
}

/** Rotates an actor around a specific point.
 * The rotation point can be set with SetActorRotation(),
 * the actor must be set with SetInteractionActor() */
void QtVTKDisplay::ActorRotate() 
{
  float xf;
  float yf;

  // do nothing if mouse is still on the same pos
  if( (this->currentMousePos[0] == this->lastMousePos[0]) &&
      (this->currentMousePos[1] == this->lastMousePos[1]))
  {
    return;
  }

  // basic idea: the change in x of the mouse is used to rotate around the
  // view-up vector. the change in y of the mouse is used to rotate around the
  // cross of the viewplane-normal and the view-up vector,

  // first time we do some preprocessing
  if( firstCall ) 
  {
    int *size = this->GetRenderWindow()->GetSize();
    vtkFloatingPointType *vp = this->GetRenderer()->GetViewport();

    this->deltaAzimuth = -20.0/((vp[2] - vp[0])*size[0]);
    this->deltaElevation = -20.0/((vp[3] - vp[1])*size[1]);

    // get the rotation axis: cross of view-up and the normal in the viewplane
    this->currentCamera->GetViewUp( this->viewUp );
    this->currentCamera->GetViewPlaneNormal( this->vpNormal );
    vtkMath::Cross( this->vpNormal, this->viewUp, this->rotateY );

    this->firstCall = false;
  }

  // how much to rotate
  xf = (this->currentMousePos[0] - this->lastMousePos[0])
         * this->trackballFactor * deltaAzimuth * (-1);
  yf = (this->currentMousePos[1] - this->lastMousePos[1])
        * this->trackballFactor * deltaElevation;

  this->interactionActor->RotateWXYZ( xf, this->viewUp[0], this->viewUp[1],
                                          this->viewUp[2] );
  this->interactionActor->RotateWXYZ( yf, this->rotateY[0], this->rotateY[1],
                                          this->rotateY[2] );

  emit ActorRotated( xf, yf );

  // update
  this->updateGL();

  // save mouse pos
  this->lastMousePos[0] = this->currentMousePos[0];
  this->lastMousePos[1] = this->currentMousePos[1];
}


/** Translates the actor on the viewplane
 *  The actor must be set with SetInteractionActor() */
void QtVTKDisplay::ActorTranslate() 
{
  vtkFloatingPointType *center;
  vtkFloatingPointType dispCenter[4];
  vtkFloatingPointType newP[4];
  vtkFloatingPointType oldP[4];

  // do nothing if mouse is still on the same pos
  if( (this->currentMousePos[0] == this->lastMousePos[0]) &&
      (this->currentMousePos[1] == this->lastMousePos[1]) )
  {
    return;
  }

  center = this->interactionActor->GetCenter();
  this->WorldToDisplay( center[0], center[1], center[2], dispCenter );

  this->DisplayToWorld( vtkFloatingPointType(this->currentMousePos[0]),
                        vtkFloatingPointType(this->height() - this->currentMousePos[1]),
                        dispCenter[2],
                        newP );

  this->DisplayToWorld( vtkFloatingPointType(this->lastMousePos[0]),
                        vtkFloatingPointType(this->height() - this->lastMousePos[1]),
                        dispCenter[2],
                        oldP);

  this->interactionActor->AddPosition( newP[0]-oldP[0],
                                        newP[1]-oldP[1],
                                        newP[2]-oldP[2] );
  // update
  this->updateGL();

  // save mouse pos
  this->lastMousePos[0] = this->currentMousePos[0];
  this->lastMousePos[1] = this->currentMousePos[1];
}


/** Zooms the actor on the normal of the viewplane
 *  The actor must be set with SetInteractionActor() */
void QtVTKDisplay::ActorZoom() 
{
  float yf;

  // do nothing if mouse is still on the same pos
  if( (this->currentMousePos[0] == this->lastMousePos[0]) &&
      (this->currentMousePos[1] == this->lastMousePos[1]) )
  {
    return;
  }

  // first time we do some preprocessing
  if( firstCall ) 
  {
    this->currentCamera->GetViewPlaneNormal( this->vpNormal );
    firstCall = false;
  }

  yf = vtkFloatingPointType(this->lastMousePos[1] - this->currentMousePos[1]) /
             vtkFloatingPointType(this->height()) * this->trackballFactor * -10.0;

  this->interactionActor->AddPosition( this->vpNormal[0] * yf,
                                       this->vpNormal[1] * yf,
                                       this->vpNormal[2] * yf );

  // update
  this->updateGL();

  // save mouse pos
  this->lastMousePos[0] = this->currentMousePos[0];
  this->lastMousePos[1] = this->currentMousePos[1];
}


/** Turns an actor on the viewplane of the camera
 *  The actor can be set with SetInteractionActor() */
void QtVTKDisplay::ActorRoll() 
{
  vtkFloatingPointType angle;

  // do nothing if mouse is still on the same pos
  if( (this->currentMousePos[0] == this->lastMousePos[0]) &&
      (this->currentMousePos[1] == this->lastMousePos[1]) ) 
  {
    return;
  }

  // first time we do some preprocessing
  if( firstCall )
  {
    this->currentCamera->GetViewPlaneNormal( this->vpNormal );
    this->renCenter[0] = this->GetRenderer()->GetCenter()[0];
    this->renCenter[1] = this->GetRenderer()->GetCenter()[1];
    this->renCenter[2] = this->GetRenderer()->GetCenter()[2];
    firstCall = false;
  }

  int diffX1 = this->currentMousePos[0] - int(this->renCenter[0]);
  int diffY1 = this->currentMousePos[1] - int(this->renCenter[1]);
  int diffX2 = this->lastMousePos[0] - int(this->renCenter[0]);
  int diffY2 = this->lastMousePos[1] - int(this->renCenter[1]);
  double a1 = atan2( double(diffY1), double(diffX1) );
  double a2 = atan2( double(diffY2), double(diffX2) );
  angle = (a2 - a1) / (2.0 * 3.1415926535) * 360.0 / 10.0 *
          this->trackballFactor;
  this->interactionActor->RotateWXYZ( angle, this->vpNormal[0],
                                             this->vpNormal[1],
                                             this->vpNormal[2] );

  emit ActorRolled( angle );

  // update
  this->updateGL();

  // save mouse pos
  this->lastMousePos[0] = this->currentMousePos[0];
  this->lastMousePos[1] = this->currentMousePos[1];
}


/** Rotates the camera around the point, which was set with
 *  SetCameraRotation().*/
void QtVTKDisplay::CameraRotate() 
{
  vtkFloatingPointType xf;
  vtkFloatingPointType yf;

  // do nothing if mouse is still on the same pos
  if( (this->currentMousePos[0] == this->lastMousePos[0]) &&
      (this->currentMousePos[1] == this->lastMousePos[1]) ) 
  {
    return;
  }

  // first time we do some preprocessing
  if( firstCall ) 
  {
    int *size = this->GetRenderWindow()->GetSize();
    vtkFloatingPointType *vp = this->GetRenderer()->GetViewport();
    this->deltaAzimuth = -20.0/((vp[2] - vp[0])*size[0]);
    this->deltaElevation = -20.0/((vp[3] - vp[1])*size[1]);
    firstCall = false;
  }

  xf = (this->currentMousePos[0] - this->lastMousePos[0]) * this->deltaAzimuth
           * this->trackballFactor;
  yf = (this->lastMousePos[1] - this->currentMousePos[1]) * this->deltaElevation
           * this->trackballFactor;

  this->currentCamera->Azimuth( xf );
  this->currentCamera->Elevation( yf );
  this->currentCamera->OrthogonalizeViewUp();
  this->GetRenderer()->ResetCameraClippingRange();

  // light follows the camera
  this->currentLight->SetPosition( this->currentCamera->GetPosition() );
  this->currentLight->SetFocalPoint( this->currentCamera->GetFocalPoint() );

  // update
  this->updateGL();

  // save mouse pos
  this->lastMousePos[0] = this->currentMousePos[0];
  this->lastMousePos[1] = this->currentMousePos[1];
}


/** Translates the camera along the viewplane. */
void QtVTKDisplay::CameraTranslate() 
{
  vtkFloatingPointType newP[4];
  vtkFloatingPointType viewFocus[4];
  vtkFloatingPointType viewPoint[3];
  double motionVector[3];

  // do nothing if mouse is still on the same pos
  if( (this->currentMousePos[0] == this->lastMousePos[0]) &&
       (this->currentMousePos[1] == this->lastMousePos[1]) ) 
  {
    return;
  }

  this->currentCamera->GetFocalPoint( viewFocus );
  this->WorldToDisplay( viewFocus[0], viewFocus[1], viewFocus[2],
                         viewFocus );
  double focalDepth = viewFocus[2];

  this->DisplayToWorld( vtkFloatingPointType(this->width()/2.0 + (this->currentMousePos[0]
                                - this->lastMousePos[0])),
                        vtkFloatingPointType(this->height()/2.0 - (this->currentMousePos[1]
                                - this->lastMousePos[1])),
                        focalDepth,
                        newP );
 
  this->currentCamera->GetFocalPoint( viewFocus );
  this->currentCamera->GetPosition( viewPoint );

  motionVector[0] = this->trackballFactor / 10.0 * ( viewFocus[0] - newP[0] );
  motionVector[1] = this->trackballFactor / 10.0 * ( viewFocus[1] - newP[1] );
  motionVector[2] = this->trackballFactor / 10.0 * ( viewFocus[2] - newP[2] );

  this->currentCamera->SetFocalPoint( motionVector[0] + viewFocus[0],
                                      motionVector[1] + viewFocus[1],
                                      motionVector[2] + viewFocus[2]);
  this->currentCamera->SetPosition( motionVector[0] + viewPoint[0],
                                    motionVector[1] + viewPoint[1],
                                    motionVector[2] + viewPoint[2]);

  // light follows the camera
  this->currentLight->SetPosition( this->currentCamera->GetPosition() );
  this->currentLight->SetFocalPoint( this->currentCamera->GetFocalPoint() );
  
  // update
  this->updateGL();

  // save mouse pos
  this->lastMousePos[0] = this->currentMousePos[0];
  this->lastMousePos[1] = this->currentMousePos[1];
}


/** Zooms the camera along the normal of the viewplane.*/
void QtVTKDisplay::CameraZoom() 
{
  float yf;

  // do nothing if mouse is still on the same y position
  if( this->currentMousePos[1] == this->lastMousePos[1] ) 
  {
    return;
  }

  yf = float(this->currentMousePos[1]-this->lastMousePos[1]) /
             float(this->height()) * this->trackballFactor;
  // if yf < 0, we need to be between 0..1
  yf = pow( (float)1.1, (float)yf );

  this->currentCamera->Dolly( yf );
  this->GetRenderer()->ResetCameraClippingRange();

  // update
  this->updateGL();

  // save mouse pos
  this->lastMousePos[0] = this->currentMousePos[0];
  this->lastMousePos[1] = this->currentMousePos[1];
}

   
/** Rotates the camera on the viewplane
 *  This function corresponds to a roll-operation
 *  like in airplanes */
void QtVTKDisplay::CameraRoll() 
{
  float angle;

  // do nothing if mouse is still on the same pos
  if( (this->currentMousePos[0] == this->lastMousePos[0]) &&
       (this->currentMousePos[1] == this->lastMousePos[1]) )
  {
    return;
  }

  // first time we do some preprocessing
  if( firstCall ) 
  {
    this->renCenter[0] = this->GetRenderer()->GetCenter()[0];
    this->renCenter[1] = this->GetRenderer()->GetCenter()[1];
    this->renCenter[2] = this->GetRenderer()->GetCenter()[2];
    firstCall = false;
  }

  // calculate the angle in radians and roll the camera
  int diffX1 = this->currentMousePos[0] - int(this->renCenter[0]);
  int diffY1 = this->currentMousePos[1] - int(this->renCenter[1]);
  int diffX2 = this->lastMousePos[0] - int(this->renCenter[0]);
  int diffY2 = this->lastMousePos[1] - int(this->renCenter[1]); 

  double a1 = atan2( double(diffY1), double(diffX1) );
  double a2 = atan2( double(diffY2), double(diffX2) );
  angle = (a2 - a1) / (2.0 * 3.1415926535) * 360.0 / 10.0 *
          this->trackballFactor;
  this->currentCamera->Roll( angle );

  // light follows the camera
  this->currentLight->SetPosition( this->currentCamera->GetPosition() );
  this->currentLight->SetFocalPoint( this->currentCamera->GetFocalPoint() );

  // update
  this->updateGL();

  // save mouse pos
  this->lastMousePos[0] = this->currentMousePos[0];
  this->lastMousePos[1] = this->currentMousePos[1];
}


/** Moves the actor
 *  Moves the actor, which was set with SetInteractionActor()
 *  in the plane, which was set with SetPlane() */
void QtVTKDisplay::ActorPlaneMove() 
{
  vtkFloatingPointType *center;
  vtkFloatingPointType dispCenter[4];
  vtkFloatingPointType newP[4];
  vtkFloatingPointType oldP[4];
  vtkFloatingPointType p0[3],p1[3],t;
  vtkFloatingPointType tkonstante = (this->normale[0])*(this->normale[0]) +
                     (this->normale[1])*(this->normale[1]) +
                     (this->normale[2])*(this->normale[2]) ;

  if( (this->currentMousePos[0] == this->lastMousePos[0]) &&
       (this->currentMousePos[1] == this->lastMousePos[1]) )
  {
    return;
  }

  center = this->interactionActor->GetCenter();
  this->WorldToDisplay( center[0], center[1], center[2], dispCenter );

  this->DisplayToWorld( vtkFloatingPointType(this->currentMousePos[0]),
                        vtkFloatingPointType(this->height() - this->currentMousePos[1]),
                        dispCenter[2],
                        newP );
  this->DisplayToWorld( vtkFloatingPointType(this->lastMousePos[0]),
                        vtkFloatingPointType(this->height() - this->lastMousePos[1]),
                        dispCenter[2],
                        oldP);

  oldP[3] = oldP[0]*(this->normale[0]) +
            oldP[1]*(this->normale[1]) +
            oldP[2]*(this->normale[2]) ;
  t = (this->ebenekonstante - oldP[3])/tkonstante;

  p0[0] = oldP[0] + t*(this->normale[0]);
  p0[1] = oldP[1] + t*(this->normale[1]);
  p0[2] = oldP[2] + t*(this->normale[2]);

  newP[3] = newP[0]*(this->normale[0]) +
            newP[1]*(this->normale[1]) +
            newP[2]*(this->normale[2]);

  t = (this->ebenekonstante - newP[3])/tkonstante;

  p1[0] = newP[0] + t*(this->normale[0]);
  p1[1] = newP[1] + t*(this->normale[1]);
  p1[2] = newP[2] + t*(this->normale[2]);

  this->interactionActor->AddPosition( p1[0]-p0[0],
                                       p1[1]-p0[1],
                                       p1[2]-p0[2] );
  // update
  this->updateGL();

  // save mouse pos
  this->lastMousePos[0] = this->currentMousePos[0];
  this->lastMousePos[1] = this->currentMousePos[1];
}

/** Rolls the actor
 *  Rolls the actor, which was set with SetInteractionActor()
 *  in the plane, which was set with SetPlane(). */
void QtVTKDisplay::ActorPlaneRoll()
{
  vtkFloatingPointType angle;

  // do nothing if mouse is still on the same pos
  if( (this->currentMousePos[0] == this->lastMousePos[0]) &&
      (this->currentMousePos[1] == this->lastMousePos[1]) )
  {
    return;
  }

  // first time we do some preprocessing
  if( firstCall )
  {
    this->renCenter[0] = this->GetRenderer()->GetCenter()[0];
    this->renCenter[1] = this->GetRenderer()->GetCenter()[1];
    this->renCenter[2] = this->GetRenderer()->GetCenter()[2];
    firstCall = false;
  }

  int diffX1 = this->currentMousePos[0] - int(this->renCenter[0]);
  int diffY1 = this->currentMousePos[1] - int(this->renCenter[1]);
  int diffX2 = this->lastMousePos[0] - int(this->renCenter[0]);
  int diffY2 = this->lastMousePos[1] - int(this->renCenter[1]);

  double a1 = atan2( double(diffY1), double(diffX1) );
  double a2 = atan2( double(diffY2), double(diffX2) );
  angle = (a2 - a1) / (2.0 * 3.1415926535) * 360.0 / 10.0 *
          this->trackballFactor;
  this->interactionActor->RotateWXYZ( -angle, this->normale[0],
                                              this->normale[1],
                                              this->normale[2] );

  // update
  this->updateGL();

  // save mouse pos
  this->lastMousePos[0] = this->currentMousePos[0];
  this->lastMousePos[1] = this->currentMousePos[1];
}


/** Does nothing.
 *  This is needed, when a Button should have no interaction meaning. */
void QtVTKDisplay::DoNothing() 
{
}

/** Computes the display point defined with x,y and z 
 *  to world coordinates.*/
void QtVTKDisplay::DisplayToWorld( vtkFloatingPointType x, vtkFloatingPointType y, vtkFloatingPointType z,
                                             vtkFloatingPointType *worldPoint ) 
{
  this->GetRenderer()->SetDisplayPoint( x, y, z );
  this->GetRenderer()->DisplayToWorld();
  this->GetRenderer()->GetWorldPoint( worldPoint );

  // normalize
  if (worldPoint[3]) 
  {
    worldPoint[0] = worldPoint[0] / worldPoint[3];
    worldPoint[1] = worldPoint[1] / worldPoint[3];
    worldPoint[2] = worldPoint[2] / worldPoint[3];
    worldPoint[3] = 1.0;
  }
}


/** Computes the world point defined with \a x, \a y and \a z 
 *  to display coordinates. */
void QtVTKDisplay::WorldToDisplay( vtkFloatingPointType x, vtkFloatingPointType y, vtkFloatingPointType z,
                                             vtkFloatingPointType *displayPoint )
{
  this->GetRenderer()->SetWorldPoint(x, y, z, 1.0 );
  this->GetRenderer()->WorldToDisplay();
  this->GetRenderer()->GetDisplayPoint( displayPoint );
}
