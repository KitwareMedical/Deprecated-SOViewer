/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovInteractor.txx,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovInteractor.h"

namespace sov
{

/** Constructor */
template<unsigned int TSpaceDimension,unsigned int TDOF, typename TScalarType>
Interactor<TSpaceDimension,TDOF,TScalarType>
::Interactor()
{
  m_Scene = 0;
  m_Display = 0;
  m_Parameters.Fill(0);
  m_Width = 1; // avoid dividing by 0
  m_Height = 1; // avoid dividing by 0
  m_RendererList = 0;
  m_Matrix.SetIdentity();
  m_Offset.Fill(0);
  m_AttachedObject = NULL;
}

/** Destructor */
template<unsigned int TSpaceDimension,unsigned int TDOF, typename TScalarType>
Interactor<TSpaceDimension,TDOF,TScalarType>
::~Interactor()
{
}

/** Set the scene spatial object */
template<unsigned int TSpaceDimension,unsigned int TDOF, typename TScalarType>
void
Interactor<TSpaceDimension,TDOF,TScalarType>
::SetScene(void* scene)
{
  m_Scene = scene;
}

/** Attach an object to the interactor. Only one object for the moment */
template<unsigned int TSpaceDimension,unsigned int TDOF, typename TScalarType>
void
Interactor<TSpaceDimension,TDOF,TScalarType>
::AttachWithObject(void * object)
{
  m_AttachedObject = object;
}

/** Update the position of the object. This should be called by the 
 * derived classes */
template<unsigned int TSpaceDimension,unsigned int TDOF, typename TScalarType>
void
Interactor<TSpaceDimension,TDOF,TScalarType>
::UpdateObjectPosition()
{
  if(!m_AttachedObject)
    {
    return;
    }
  typedef itk::SpatialObject<TSpaceDimension> SpatialObjectType;
  typename SpatialObjectType::Pointer object = static_cast<SpatialObjectType*>(m_AttachedObject);
  object->GetObjectToParentTransform()->SetMatrix(m_Matrix);
  object->GetObjectToParentTransform()->SetOffset(m_Offset);
  object->ComputeObjectToWorldTransform();
}


}//end of namespace
