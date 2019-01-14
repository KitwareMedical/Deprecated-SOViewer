/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: ManagerInteractor.cxx,v $
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
#include "FltkSpatialObjectViewerSlicer.h"
#include "ManagerInteractor.h"
 

void ManagerInteractor::Select(SpatialObject<3>* so)
{
  selectedSO = so;
  if(!m_GUI)
    {
    return;
    }

  static_cast<FltkSpatialObjectViewerSlicer*>(m_GUI)->colorR->value(so->GetProperty()->GetColor().GetRed()*255);
  static_cast<FltkSpatialObjectViewerSlicer*>(m_GUI)->colorG->value(so->GetProperty()->GetColor().GetGreen()*255);
  static_cast<FltkSpatialObjectViewerSlicer*>(m_GUI)->colorB->value(so->GetProperty()->GetColor().GetBlue()*255);

  static_cast<FltkSpatialObjectViewerSlicer*>(m_GUI)->colorChooser->rgb(static_cast<FltkSpatialObjectViewerSlicer*>(m_GUI)->colorR->value(),
                                                                    static_cast<FltkSpatialObjectViewerSlicer*>(m_GUI)->colorG->value(),
                                                                    static_cast<FltkSpatialObjectViewerSlicer*>(m_GUI)->colorB->value());

  static_cast<FltkSpatialObjectViewerSlicer*>(m_GUI)->OffsetX->value(so->GetObjectToWorldTransform()->GetOffsetComponent()[0]);
  static_cast<FltkSpatialObjectViewerSlicer*>(m_GUI)->OffsetY->value(so->GetObjectToWorldTransform()->GetOffsetComponent()[1]);
  static_cast<FltkSpatialObjectViewerSlicer*>(m_GUI)->OffsetZ->value(so->GetObjectToWorldTransform()->GetOffsetComponent()[2]);

  static_cast<FltkSpatialObjectViewerSlicer*>(m_GUI)->ScaleX->value(so->GetObjectToWorldTransform()->GetScaleComponent()[0]);
  static_cast<FltkSpatialObjectViewerSlicer*>(m_GUI)->ScaleY->value(so->GetObjectToWorldTransform()->GetScaleComponent()[1]);
  static_cast<FltkSpatialObjectViewerSlicer*>(m_GUI)->ScaleZ->value(so->GetObjectToWorldTransform()->GetScaleComponent()[2]);

  if(static_cast<FltkSpatialObjectViewerSlicer*>(m_GUI)->GetRenderer()->GetVisualObject(so))
    {
  static_cast<FltkSpatialObjectViewerSlicer*>(m_GUI)->Visible->value(
       static_cast<FltkSpatialObjectViewerSlicer*>(m_GUI)->GetRenderer()->GetVisualObject(so)->GetProperty()->GetVisibility());
    }
 };


