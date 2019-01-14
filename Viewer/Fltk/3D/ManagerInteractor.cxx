/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: ManagerInteractor.cxx,v $
  Language:  C++
  Date:      $Date: 2005-11-03 00:56:19 $
  Version:   $Revision: 1.2 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "FltkSpatialObjectViewer3D.h"
#include "ManagerInteractor.h"
 

void ManagerInteractor::Select(SpatialObject<3>* so)
{
  selectedSO = so;
  if(!m_GUI)
    {
    return;
    }

  static_cast<FltkSpatialObjectViewer3D*>(m_GUI)->colorR->value(so->GetProperty()->GetColor().GetRed());
  static_cast<FltkSpatialObjectViewer3D*>(m_GUI)->colorG->value(so->GetProperty()->GetColor().GetGreen());
  static_cast<FltkSpatialObjectViewer3D*>(m_GUI)->colorB->value(so->GetProperty()->GetColor().GetBlue());

  static_cast<FltkSpatialObjectViewer3D*>(m_GUI)->colorChooser->rgb(static_cast<FltkSpatialObjectViewer3D*>(m_GUI)->colorR->value(),
                                                                    static_cast<FltkSpatialObjectViewer3D*>(m_GUI)->colorG->value(),
                                                                    static_cast<FltkSpatialObjectViewer3D*>(m_GUI)->colorB->value());

  static_cast<FltkSpatialObjectViewer3D*>(m_GUI)->OffsetX->value(so->GetObjectToWorldTransform()->GetOffsetComponent()[0]);
  static_cast<FltkSpatialObjectViewer3D*>(m_GUI)->OffsetY->value(so->GetObjectToWorldTransform()->GetOffsetComponent()[1]);
  static_cast<FltkSpatialObjectViewer3D*>(m_GUI)->OffsetZ->value(so->GetObjectToWorldTransform()->GetOffsetComponent()[2]);

  static_cast<FltkSpatialObjectViewer3D*>(m_GUI)->ScaleX->value(so->GetObjectToWorldTransform()->GetScaleComponent()[0]);
  static_cast<FltkSpatialObjectViewer3D*>(m_GUI)->ScaleY->value(so->GetObjectToWorldTransform()->GetScaleComponent()[1]);
  static_cast<FltkSpatialObjectViewer3D*>(m_GUI)->ScaleZ->value(so->GetObjectToWorldTransform()->GetScaleComponent()[2]);

  if(static_cast<FltkSpatialObjectViewer3D*>(m_GUI)->GetRenderer()->GetVisualObject(so))
    {
  static_cast<FltkSpatialObjectViewer3D*>(m_GUI)->Visible->value(
       static_cast<FltkSpatialObjectViewer3D*>(m_GUI)->GetRenderer()->GetVisualObject(so)->GetProperty()->GetVisibility());
    }
 };


