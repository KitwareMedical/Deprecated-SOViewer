/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovTubePhantomRenderMethod.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:58 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __TubePhantomRenderMethod_h
#define __TubePhantomRenderMethod_h

#include "sovPhantomRenderMethod.h"
#include "itkTubeSpatialObject.h"

namespace sov
{

class TubePhantomRenderMethod
:public PhantomRenderMethod
{

public: 

  typedef TubePhantomRenderMethod        Self;
  typedef SmartPointer< Self >                Pointer;
  typedef PhantomRenderMethod                   Superclass;
  
  typedef itk::TubeSpatialObject<3>           TubeType;
  typedef TubeType::Pointer                   TubePointer;
  typedef TubeType::TubePointType             TubePointType;
  typedef TubePointType*                      TubePointPointer;
  typedef TubeType::PointListType             TubePointListType;
  typedef TubePointListType *                 TubePointListPointer;
  typedef TubePointType::PointType            PointType;
  typedef PointType *                         PointPointer;
  typedef Superclass::ObjectListType          ObjectListType;

  itkNewMacro( TubePhantomRenderMethod );

  itkTypeMacro( TubePhantomRenderMethod, PhantomRenderMethod );

  virtual void draw( void );
  virtual void Update();

protected:

  TubePhantomRenderMethod();

  ~TubePhantomRenderMethod();

  void PrintSelf (std::ostream &os, Indent indent) const;

  unsigned int m_NumberOfSides;

};

} // end of namespace sov

#endif //__TubePhantomRenderMethod_h
