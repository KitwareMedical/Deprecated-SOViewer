/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovTubeGlRenderMethod2D.h,v $
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
#ifndef sov_TubeGlRenderMethod2D_h
#define sov_TubeGlRenderMethod2D_h

#include "sovGlRenderMethod2D.h"
#include "itkTubeSpatialObject.h"
#include "sovOpenGLWin32Header.h"

namespace sov
{

  
class SOVOpenGLRenderMethod_EXPORT TubeGlRenderMethod2D
: public GlRenderMethod2D
{

public:


  typedef TubeGlRenderMethod2D        Self;
  typedef GlRenderMethod2D            Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;
  typedef Superclass::ObjectType      ObjectType;
  typedef Superclass::ObjectListType  ObjectListType;

  typedef itk::TubeSpatialObject<2>           TubeType;
  typedef TubeType::Pointer                   TubePointer;
  typedef TubeType::TubePointType             TubePointType;
  typedef TubePointType*                      TubePointPointer;
  typedef TubeType::PointListType             TubePointListType;
  typedef TubePointListType *                 TubePointListPointer;
  typedef TubePointType::PointType            PointType;
  typedef PointType *                         PointPointer;


  itkNewMacro( TubeGlRenderMethod2D );

  itkTypeMacro( TubeGlRenderMethod2D, GlRenderMethod3D );

  virtual void draw( void );

  itkSetMacro(ShowRadius,bool);
  itkGetMacro(ShowRadius,bool);

protected:

  TubeGlRenderMethod2D( void );
  ~TubeGlRenderMethod2D( void );

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update(); 
  bool m_ShowRadius;
  
};

}

#endif //_TubeGlRenderMethod2D_h
