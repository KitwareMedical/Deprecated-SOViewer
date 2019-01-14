/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovInteractor.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __sovInteractor_h
#define __sovInteractor_h

#include <itkSpatialObject.h>
#include <itkSceneSpatialObject.h>
#include <itkPoint.h>
#include <itkVector.h>
#include <itkMatrix.h>
#include <sovNDimensionalRenderer.h>
#include <sovDisplay.h>

/** This is the base class for SpatialObject interactors 
*/

namespace sov
{
template<unsigned int TSpaceDimension,unsigned int TDOF, typename TScalarType=double>
class Interactor
  {
public:

  typedef itk::Point<TScalarType,TDOF> ParametersType;
  typedef itk::Matrix<TScalarType,TSpaceDimension,TSpaceDimension> MatrixType;
  typedef itk::Vector<TScalarType,TSpaceDimension> OffsetType;
  typedef NDimensionalRenderer        RendererType;
  typedef std::list< RendererType::Pointer > RendererListType;

  Interactor();
  virtual ~Interactor();

  void SetScene(void*);
  ParametersType GetParameters() {return m_Parameters;}
  void AttachWithObject(void *); // update the object automatically
  void UpdateObjectPosition();
  void Start(){}; // does nothing by default
  void Stop(){}; // does nothing by default
  /** To avoid asking for the display size every time, we just let the display set the size on resize */
  void SetDisplaySize( unsigned int width, unsigned int height ) {this->m_Width = width; this->m_Height = height;}
  void SetDisplay(Display * display) {m_Display = display;}
  void SetRendererList(RendererListType* rendererlist) {m_RendererList = rendererlist;}
  
  const MatrixType & GetMatrix() {return m_Matrix;}
  const OffsetType & GetOffset() {return m_Offset;}
  
protected:

  void* m_Scene;
  Display* m_Display;
  ParametersType    m_Parameters;
  unsigned int m_Width;
  unsigned int m_Height;
  RendererListType* m_RendererList;
  MatrixType m_Matrix;
  OffsetType m_Offset;
  void * m_AttachedObject;

  };//end of class
} //end of namespace sov


#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovInteractor.txx"
#endif

#endif //__sovInteractor_h
