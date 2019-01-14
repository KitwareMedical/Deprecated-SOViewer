/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovQtGlDisplay.h,v $
  Language:  C++
  Date:      $Date: 2005-02-07 03:37:44 $
  Version:   $Revision: 1.2 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef QtGlDisplay_h__
#define QtGlDisplay_h__

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkImage.h"
#include "itkColorTable.h"
#include <math.h>
#include <qgl.h>
#include "sovRenderer.h"
#include "sovNDimensionalRenderer.h"
#include "sovMouseInteractor.h"
#include "sovKeyboardInteractor.h"
#include <OpenGLSliceTexture.h>
#include "sovObjectViewerWin32Header.h"

using namespace itk;

class SOViewer_EXPORT QtGlDisplay : public QGLWidget
{
  Q_OBJECT

public:
  
  typedef unsigned char                       ImagePixelType;
  typedef unsigned char                       OverlayPixelType;
  typedef itk::Image<ImagePixelType,3>        ImageType;
  typedef itk::Image<OverlayPixelType,3>      OverlayType;
  typedef ImageType::Pointer                  ImagePointer;
  typedef OverlayType::Pointer                OverlayPointer;
  typedef ImageType::RegionType               RegionType;
  typedef ImageType::SizeType                 SizeType;
  typedef ImageType::IndexType                IndexType;
  typedef sov::NDimensionalRenderer           RendererType;
  typedef std::list< RendererType::Pointer >  RendererListType;
  typedef itk::Image<ImagePixelType,2>        SliceType;

  QtGlDisplay( QWidget *parent = 0, const char *name = 0 );
  QtGlDisplay( QGLFormat glf, QWidget *parent = 0, const char *name = 0 );
  ~QtGlDisplay();

  void SetRendererList( RendererListType  );
  RendererListType  GetRendererList();
 
  void AddRenderer( RendererType * renderer ); 
  virtual void Update();

  void mousePressEvent( QMouseEvent *event ); 
  void mouseMoveEvent( QMouseEvent *event ) ;
  void SetMouseInteractor(sov::MouseInteractor* interactor);
  void SetKeyboardInteractor(sov::KeyboardInteractor* interactor);
  typedef OpenGLSliceTexture<unsigned char> GreyTextureType;

protected:
   
  RendererListType  m_RendererList;
  sov::MouseInteractor*  m_MouseInteractor;
  sov::KeyboardInteractor*  m_KeyboardInteractor;

  void initializeGL();
  void resizeGL( int w, int h);
  void paintGL();
  GreyTextureType* m_GreyTexture;
  SliceType::Pointer m_Image;
};

 
#endif
