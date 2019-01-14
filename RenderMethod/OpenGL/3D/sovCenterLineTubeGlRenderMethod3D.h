/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovCenterLineTubeGlRenderMethod3D.h,v $
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
#ifndef _CenterLineTubeGlRenderMethod3D_h
#define _CenterLineTubeGlRenderMethod3D_h

#include "sovGlRenderMethod3D.h"
#include "itkTubeSpatialObject.h"

//#include <FL/gl.h>

namespace sov
{

class CenterLineTubeGlRenderMethod3D
:public GlRenderMethod3D
{

public: 

  typedef CenterLineTubeGlRenderMethod3D      Self;
  typedef SmartPointer< Self >                Pointer;
  typedef GlRenderMethod3D                    Superclass;
  
  typedef itk::TubeSpatialObject<3>           TubeType;
  typedef TubeType::Pointer                   TubePointer;
  typedef TubeType::ChildrenListType          TubeListType;
  typedef TubeType::TubePointType             TubePointType;
  typedef TubePointType*                      TubePointPointer;
  typedef TubeType::PointListType             TubePointListType;
  typedef TubePointListType *                 TubePointListPointer;
  typedef TubePointType::PointType            PointType;
  typedef PointType *                         PointPointer;
  typedef std::list< PointType * >            PointListType;        // list of all the points of one tube
  typedef std::list< PointListType * >        PointListListType;    // list of list of points,
                                                                    // each one defining one tube.
  typedef TubeType::PropertyType::PixelType   PixelType;
  typedef float                               ColorType;
  typedef std::list< const ColorType * >      ColorListType;

  itkNewMacro( CenterLineTubeGlRenderMethod3D );

  itkTypeMacro( CenterLineTubeGlRenderMethod3D, GlRenderMethod3D );

  virtual void draw( void );

protected:
  
  PointListListType *   m_PointListList;
  ColorListType *       m_ColorList;

  CenterLineTubeGlRenderMethod3D();

  ~CenterLineTubeGlRenderMethod3D();

  void PrintSelf (std::ostream &os, Indent indent) const;

  void BuildDisplayList();

  void CheckDisplayList();

  virtual void Update();

private:

  int m_DisplayList;
  int m_Modified;

};

} // end of namespace sov

#endif //_CenterLineTubeGlRenderMethod3D_h
