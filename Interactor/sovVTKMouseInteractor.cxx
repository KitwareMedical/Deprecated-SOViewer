/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVTKMouseInteractor.cxx,v $
  Language:  C++
  Date:      $Date: 2006-02-24 16:35:10 $
  Version:   $Revision: 1.2 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovVTKMouseInteractor.h"

namespace sov
{
  VTKMouseInteractor
  ::VTKMouseInteractor()
    {
    this->m_Renderer = NULL;
    
    this->m_InvertedAxis[0] = false;
    this->m_InvertedAxis[1] = true;

    this->m_StatusActor = vtkTextActor::New();
    this->m_StatusActor->SetInput(this->m_Status.c_str());
    this->m_StatusActor->SetPosition(5,this->m_Height-5);
    this->m_StatusActor->GetTextProperty()->SetColor(1.0,0.0,0.0);
    this->m_StatusActor->GetTextProperty()->SetFontFamilyToTimes();
    this->m_StatusActor->GetTextProperty()->SetJustificationToLeft();
    this->m_StatusActor->GetTextProperty()->SetVerticalJustificationToTop();
    this->m_StatusActor->GetTextProperty()->ShadowOff();

    this->m_DataActor = vtkTextActor::New();
    this->m_DataActor->SetInput(this->m_Data.c_str());
    this->m_DataActor->SetPosition(5,5);
    this->m_DataActor->GetTextProperty()->SetColor(1.0,0.0,0.0);
    this->m_DataActor->GetTextProperty()->SetFontFamilyToTimes();
    this->m_DataActor->GetTextProperty()->SetJustificationToLeft();
    this->m_DataActor->GetTextProperty()->SetVerticalJustificationToBottom();
    this->m_DataActor->GetTextProperty()->ShadowOff();
    }

  VTKMouseInteractor
  ::~VTKMouseInteractor()
    {
      this->m_StatusActor->Delete();
      this->m_DataActor->Delete();
    }

  void 
  VTKMouseInteractor
  ::AddRenderer(RendererType * renderer)
    {
    m_Renderer = renderer;
    renderer->GetVTKRenderer()->AddActor(this->m_StatusActor);
    renderer->GetVTKRenderer()->AddActor(this->m_DataActor);
    }

  void
  VTKMouseInteractor::
  SetDisplaySize( unsigned int width, unsigned int height )
    {
    Superclass::SetDisplaySize(width,height);
    
    this->m_StatusActor->SetPosition(5,height-5);

    if( this->m_Renderer )
      {
      this->m_Renderer->GetVTKRenderer()->Render();
      }
    }

  void
  VTKMouseInteractor
  ::SetOrientation( bool invert[] )
    {
    this->m_InvertedAxis[0] = invert[0];
    this->m_InvertedAxis[1] = invert[1];
    }

  void 
  VTKMouseInteractor
  ::Push( unsigned int x, unsigned int y, unsigned int button )
    {   
    if(m_Renderer == NULL)
      {
      std::cout << "VTKMouseInteractor::Push() : No renderer Specified" << std::endl;
      return;
      }
      
      if( this->m_InvertedAxis[0] )
        {
        x = this->m_Width-x;
        }

      if( this->m_InvertedAxis[1] )
        {      
        y = this->m_Height-y;
        }

      this->PushProcess(x,y,button);

      this->m_Renderer->GetVTKRenderer()->GetRenderWindow()->Render();

    }

  void 
  VTKMouseInteractor
  ::SetData( const char * data )
    {
    m_Data = data;
    this->m_DataActor->SetInput(this->m_Data.c_str());
    }

  void 
  VTKMouseInteractor
  ::SetStatus( const char * status )
    {
    m_Status = status;
    this->m_StatusActor->SetInput(this->m_Status.c_str());
    }

  void 
  VTKMouseInteractor
  ::Drag( unsigned int x, unsigned int y, unsigned int button )
    {
    std::cout<< "drag" << x << " : " << y << " : " << button << std::endl;
    }

  void 
  VTKMouseInteractor
  ::Release( unsigned int x, unsigned int y, unsigned int button )
    {
    std::cout<< "release" << x << " : " << y << " : " << button << std::endl;
    }

  void 
  VTKMouseInteractor
  ::Handle( unsigned int x, unsigned int y )
    {
    std::cout<< "handle" << x << " : " << y <<std::endl;
    }

  }//end of namespace

