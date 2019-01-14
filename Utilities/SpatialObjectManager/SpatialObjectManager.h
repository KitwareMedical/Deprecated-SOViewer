/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: SpatialObjectManager.h,v $
  Language:  C++
  Date:      $Date: 2005-07-31 18:55:16 $
  Version:   $Revision: 1.3 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __SpatialObjectManager_h
#define __SpatialObjectManager_h

#include <itkObjectFactory.h>
#include "SpatialObjectManagerGUI.h"
#include "itkSpatialObject.h"
#include <FL/gl.h>


#include "ellipse.xpm"
#include "tube.xpm"
#include "tube_net.xpm"

#include <itkSceneSpatialObject.h>
#include "itkTubeSpatialObject.h"
#include "itkEllipseSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkGroupSpatialObject.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "SpatialObjectManagerInteractor.h"
#include "SpatialObjectManagerExport.h"

class SpatialObjectManager_EXPORT SpatialObjectManager : public SpatialObjectManagerGUI
{
public:

  /** Standard "Self" typedef. */
  typedef  SpatialObjectManager       Self;

  /** Standard "Superclass" typedef. */
  typedef SpatialObjectManagerGUI  Superclass;

  /** Smart pointer typedef support. */
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  typedef itk::SceneSpatialObject<3>            SceneType;

  /** Run-time type information (and related methods). */
  itkTypeMacro(SpatialObjectManager, SpatialObjectManagerGUI);

  /** Method for creation through the object factory. */
  itkNewMacro(Self); 
  
  /** Show the GUI */
  //void Show(void);

  /** Add a scene to the manager */
  void SetScene(itk::SceneSpatialObject<3>* scene) {m_Scene = scene;}

  /** Update */
  void Update(void);

  /** Select Object */
  void SelectObject(void);

  /** Set the Interactor */
  void SetInteractor(SpatialObjectManagerInteractor* interactor) {m_Interactor = interactor;}


protected:

  /** Default constructor */
   SpatialObjectManager();

  /** Destructor */
  ~SpatialObjectManager();

  //SpatialObjectManagerGUI* m_GUI;
  SceneType::Pointer m_Scene;

  /** Create the tree*/
  void CreateTree(itk::SpatialObject<>* object, int first);

  /** Show properties of an object in the browser window */
  void ShowProperties(itk::SpatialObject<>* object);

 
  SpatialObjectManagerInteractor* m_Interactor;


private:
  Fl_Pixmap  *pellipse, *ptube, *ptubenet;

  int m_CurrentLevel;
};


#endif




