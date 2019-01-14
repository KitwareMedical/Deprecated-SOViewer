/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVTKMouseImageInteractor.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovVTKMouseInteractor.h"

#include <vtkActor.h>
#include <vtkCellPicker.h>
#include <vtkPoints.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include "sovObjectViewerWin32Header.h"


#ifndef vtkFloatingPointType
# define vtkFloatingPointType vtkFloatingPointType
typedef float vtkFloatingPointType;
#endif


namespace sov
{

  class SOViewer_EXPORT VTKMouseImageInteractor
  : public VTKMouseInteractor
  {
  public:
    typedef VTKMouseInteractor        Superclass;
    typedef Superclass::RendererType    RendererType;
    typedef vtkCellPicker               PickerType;

    VTKMouseImageInteractor();
    
    ~VTKMouseImageInteractor();

    void GetImageCoordinate( int coord[]) const;
      
    void GetWorldCoordinate( float coord[]) const;

  protected:
    
    PickerType *             m_Picker;

    vtkSphereSource *        m_Sphere;
    vtkPolyDataMapper *      m_SphereMapper;
    vtkActor *               m_SphereActor;

    int   m_ImageCoordinate[3];
    vtkFloatingPointType m_WorldCoordinate[3];

    virtual void RendererListProcess( RendererListType * rendererList );


    virtual void PushProcess(unsigned int x,unsigned y,unsigned int button);

  };//end of class


}//end of namespace
