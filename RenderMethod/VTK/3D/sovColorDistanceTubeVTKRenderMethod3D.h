/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovColorDistanceTubeVTKRenderMethod3D.h,v $
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
#ifndef __ColorDistanceTubeVTKRenderMethod3D_h
#define __ColorDistanceTubeVTKRenderMethod3D_h

#include "sovVTKRenderMethod3D.h"
#include "itkTubeSpatialObject.h"
#include <vtkLookupTable.h>

namespace sov
{

template <class TImage>
class ColorDistanceTubeVTKRenderMethod3D
:public VTKRenderMethod3D
{

public: 

  typedef ColorDistanceTubeVTKRenderMethod3D  Self;
  typedef SmartPointer< Self >                Pointer;
  typedef VTKRenderMethod3D                   Superclass;
  
  typedef TImage                              ImageType;
  typedef typename ImageType::PixelType       PixelType;
  typedef ImageSpatialObject<3,PixelType>     ImageSpatialObjectType;

  typedef itk::TubeSpatialObject<3>           TubeType;
  typedef TubeType::Pointer                   TubePointer;
  typedef TubeType::ChildrenListType          TubeListType;
  typedef TubeType::TubePointType             TubePointType;
  typedef TubePointType*                      TubePointPointer;
  typedef TubeType::PointListType             TubePointListType;
  typedef TubePointListType *                 TubePointListPointer;
  typedef TubePointType::PointType            PointType;
  typedef PointType *                         PointPointer;

  itkNewMacro( ColorDistanceTubeVTKRenderMethod3D );

  itkTypeMacro( ColorDistanceTubeVTKRenderMethod3D, VTKRenderMethod3D );

  void SetDistanceMap(ImageSpatialObjectType * image);

protected:

  ColorDistanceTubeVTKRenderMethod3D();

  ~ColorDistanceTubeVTKRenderMethod3D();

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();
  
  unsigned int m_NumberOfSides;

  vtkLookupTable* m_LookUpTable;

  void CreateLookUpTable();

  double GetLookUpTableIndex(float r,float g,float b);
  typename ImageSpatialObjectType::Pointer  m_DistanceMap;
  double m_Maximum;
  double m_ColorFactor;
};

} // end of namespace sov

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovColorDistanceTubeVTKRenderMethod3D.txx"
#endif

#endif //__ColorDistanceTubeVTKRenderMethod3D_h
