/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkUltrasoundProbeSpatialObject.h,v $
  Language:  C++
  Date:      $Date: 2005-04-28 14:42:15 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkUltrasoundProbeSpatialObject_h
#define __itkUltrasoundProbeSpatialObject_h

#include <list>

#include "itkSpatialObject.h"
#include "itkBoxSpatialObject.h" 
#include "itkEllipseSpatialObject.h"
#include <sovWin32Header.h>

namespace itk 
{

/** 
* \class UltrasoundProbeSpatialObject
* \brief Representation of a UltrasoundProbe based on the spatial object classes.
*
* A UltrasoundProbeSpatialObject represents a UltrasoundProbe by serving as the parent of
* the elements of the UltrasoundProbe.  Since any itk::SpatialObject can have
* children (see SpatialObject::GetChildren()), this class needs no
* additional methods.
*/

class SOVCommon_EXPORT UltrasoundProbeSpatialObject 
  :public SpatialObject< 3 >
{

public:

  typedef UltrasoundProbeSpatialObject   Self;
  typedef SpatialObject< 3 >             Superclass;
  typedef SmartPointer < Self >          Pointer;
  typedef SmartPointer < const Self >    ConstPointer;
  typedef double                         ScalarType;
  typedef Vector<double,3>               VectorType;
  typedef Point<double,3>                PointType;
  typedef Superclass::TransformType      TransformType;
  typedef TransformType::MatrixType      MatrixType;
  typedef itk::Vector<double,3> OffsetType;
  typedef itk::BoxSpatialObject<3> RectangleType;
  typedef itk::EllipseSpatialObject<3>   SphereType;


  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Method for creation through the object factory. */
  itkTypeMacro( Self, Superclass );

  /** Return the sensor spatial object */
  RectangleType::Pointer GetSensor() {return m_Sensor;}
  SphereType::Pointer GetTip() {return m_Tip;}
  RectangleType::Pointer GetBody() {return m_Body;}

  bool ComputeLocalBoundingBox() const {return false;}
  
  void Construct();
  
  void SetOverlapX(double val) {m_OverlapX = val;}
  double GetOverlapX() {return m_OverlapX;}
  void SetBodyShiftZ(double val) {m_BodyShiftZ = val;}
  double GetBodyShiftZ() {return m_BodyShiftZ;}
  void SetBodySizeY(double val) {m_BodySizeY = val;}
  double GetBodySizeY() {return m_BodySizeY;}
   
protected:

  UltrasoundProbeSpatialObject();
  virtual ~UltrasoundProbeSpatialObject();

  /** Method to print the object.*/
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

private:

  RectangleType::Pointer m_Sensor;
  SphereType::Pointer    m_Tip;
  RectangleType::Pointer m_Body;
  double                 m_OverlapX;
  double                 m_BodyShiftZ;
  double                 m_BodySizeY;
};

} // end namespace itk


#endif // __itkUltrasoundProbeSpatialObject_h
