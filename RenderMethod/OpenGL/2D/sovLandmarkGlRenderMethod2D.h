/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovLandmarkGlRenderMethod2D.h,v $
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
#ifndef sov_LandmarkGlRenderMethod2D_h
#define sov_LandmarkGlRenderMethod2D_h

#include "sovGlRenderMethod2D.h"
#include "itkLandmarkSpatialObject.h"
#include "sovOpenGLWin32Header.h"

namespace sov
{

typedef enum
{
  SOL_CIRCLE,
  SOL_SQUARE,
  SOL_POINT
} SOL_DISPLAYType;

  
class SOVOpenGLRenderMethod_EXPORT LandmarkGlRenderMethod2D
: public GlRenderMethod2D
{

public:


  typedef LandmarkGlRenderMethod2D        Self;
  typedef GlRenderMethod2D            Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;
  typedef Superclass::ObjectType      ObjectType;
  typedef Superclass::ObjectListType  ObjectListType;

  typedef itk::LandmarkSpatialObject<2>           LandmarkType;
  typedef LandmarkType::Pointer                   LandmarkPointer;
  typedef LandmarkType::LandmarkPointType             LandmarkPointType;
  typedef LandmarkPointType*                      LandmarkPointPointer;
  typedef LandmarkType::PointListType             LandmarkPointListType;
  typedef LandmarkPointListType *                 LandmarkPointListPointer;
  typedef LandmarkPointType::PointType            PointType;
  typedef PointType *                         PointPointer;


  itkNewMacro( LandmarkGlRenderMethod2D );

  itkTypeMacro( LandmarkGlRenderMethod2D, GlRenderMethod2D );

  virtual void draw( void );

  void SetLandmarkSize(float size){m_LandmarkSize = size;}
  void SetDisplayType(SOL_DISPLAYType type) {m_DisplayType = type;}

protected:

  LandmarkGlRenderMethod2D( void );
  ~LandmarkGlRenderMethod2D( void );

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update(); 

  SOL_DISPLAYType m_DisplayType;
  float m_LandmarkSize;
  
};

}

#endif //_LandmarkGlRenderMethod2D_h
