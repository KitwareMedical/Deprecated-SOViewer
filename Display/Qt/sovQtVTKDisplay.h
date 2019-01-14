/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovQtVTKDisplay.h,v $
  Language:  C++
  Date:      $Date: 2005-02-07 03:37:45 $
  Version:   $Revision: 1.3 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _QtVTKDisplay_H_
#define _QtVTKDisplay_H_

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include <itkImage.h>
#include <math.h>
#include <qgl.h> //qgl.h should be included before sovRenderer.h!
#include "sovRenderer.h" 

#include "sovNDimensionalRenderer.h" //added

#include "vtkRenderer.h"
#include "vtkLight.h"
#include <qtimer.h>
#include "vtkRenderWindow.h"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkLight.h"
#include "vtkCellPicker.h"
#include "sovMouseInteractor.h"
#include "sovKeyboardInteractor.h"
#include "sovObjectViewerWin32Header.h"

#include "vtkTextActor.h"
#include "vtkTextProperty.h"

#ifndef vtkFloatingPointType
# define vtkFloatingPointType vtkFloatingPointType
typedef float vtkFloatingPointType;
#endif

class SOViewer_EXPORT QtVTKDisplay : public QGLWidget 
{

Q_OBJECT

public:
  
  typedef sov::NDimensionalRenderer          RendererType; //added
  typedef std::list< RendererType::Pointer >   RendererListType;

  QtVTKDisplay( QGLFormat glf, QWidget *parent = 0, const char *name = 0 );
  QtVTKDisplay( QWidget *parent = 0, const char *name = 0 );
  ~QtVTKDisplay();

  vtkRenderer *GetRenderer();
  vtkRenderWindow *GetRenderWindow();

  void SetRendererList( RendererListType * );
  RendererListType * GetRendererList();

  void AddRenderer( RendererType * renderer );
  void Update(void);
  vtkLight* GetCurrentLight() {return currentLight;}

protected:
   virtual void paintGL( );
   virtual void initializeGL( );
   virtual void resizeGL( int w, int h );
  RendererListType *  m_RendererList;
  
private:
   void InitQtVTKDisplay();
   void Init();
   bool initNeeded;
   vtkRenderer*     renderer;
   vtkRenderWindow* renderWindow;
   vtkLight*        light;

public:

   void SetLeftButtonFunction( const char* function );
   void SetMiddleButtonFunction( const char* function );
   void SetRightButtonFunction( const char* function );
   void SetShiftLeftButtonFunction( const char* function );

   void SetResizeFunction( const char * function );

   void SetInteractionActor( vtkActor* actor );
   void SetActorRotation( float x, float y, float z );
   void SetPlane(float *ebene);
   void SetInteractionTime( int time );
   void SetSensitivity( float t );

   int GetInteractionTime( void );
   vtkActor *GetInteractionActor( void );
   float GetSensitivity();
   const char *GetClassName();
   void SetMouseInteractor(sov::MouseInteractor* interactor);
   void SetKeyboardInteractor(sov::KeyboardInteractor* interactor);

signals:
   
 /** This signal is emitted, whenever we have rotated an actor.
  *  xf the angle in degrees around the view-up vector of the current
  *  camera. yf the angle in degrees around the cross of the
  *  viewplane-normal and the view-up vector. */
  void ActorRotated( float xf, float yf );

 /** This signal is emitted, whenever we have rolled an actor
  *  Rolling is rotating in the viewplane of the camera.
  *  angle the angle in degrees around the viewplane-normal of
  *  the current camera. */
  void ActorRolled( float angle );

 /** This signal is emitted, when the interaction is finished.
  *  x the x coordinate of the pointer
  *  y the y coordinate of the pointer */
  void Released( int x, int y );

protected:

  void mousePressEvent( QMouseEvent *event );
  void mouseReleaseEvent( QMouseEvent *event );
  void mouseMoveEvent( QMouseEvent *event );
  void resizeEvent( QResizeEvent *event );

protected slots:

  void ActorRotate();
  void ActorTranslate();
  void ActorZoom();
  void ActorRoll();
  void ActorPlaneRoll();
  void ActorPlaneMove();
  void CameraRotate();
  void CameraTranslate();
  void CameraZoom();
  void CameraRoll();
  void DoNothing();

private:
  
  void DisplayToWorld( vtkFloatingPointType x, vtkFloatingPointType y, vtkFloatingPointType z,
                                              vtkFloatingPointType *worldPoint );

  void WorldToDisplay( vtkFloatingPointType x, vtkFloatingPointType y, vtkFloatingPointType z,
                                              vtkFloatingPointType *displayPoint );

  vtkCellPicker *cellpicker;
  vtkCamera   *currentCamera;
  vtkActor    *interactionActor;
  vtkActor    *collisionActor;
  vtkLight    *currentLight;

  vtkFloatingPointType viewUp[3];
  vtkFloatingPointType vpNormal[3];
  vtkFloatingPointType rotateY[3];
  vtkFloatingPointType renCenter[3];
  vtkFloatingPointType viewFocus[3];
  vtkFloatingPointType camPos[3];
  vtkFloatingPointType deltaElevation;
  vtkFloatingPointType deltaAzimuth;
  vtkFloatingPointType normale[3];
  vtkFloatingPointType ebenekonstante;

  QTimer   *stepTimer;
  bool     mouseEventActive;
  bool     firstCall;
  vtkFloatingPointType    trackballFactor;
  vtkFloatingPointType    cameraRotation[3];
  int      interactionTime;
  int      currentMousePos[2];
  int      lastMousePos[2];
  
  char leftButtonFunction[200];
  char middleButtonFunction[200];
  char rightButtonFunction[200];
  char shiftLeftButtonFunction[200];
  char resizeFunction[200];
  sov::MouseInteractor*  m_MouseInteractor;
  sov::KeyboardInteractor*  m_KeyboardInteractor;
};

#endif // _QtVTKDisplay_H_
