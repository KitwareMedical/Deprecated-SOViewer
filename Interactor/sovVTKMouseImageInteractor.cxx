/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVTKMouseImageInteractor.cxx,v $
  Language:  C++
  Date:      $Date: 2006-02-24 16:35:10 $
  Version:   $Revision: 1.2 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovVTKMouseImageInteractor.h"
#include <vtkProperty.h>
#include <vtkTexture.h>
#include <stdio.h>

namespace sov
{
  VTKMouseImageInteractor::
  VTKMouseImageInteractor()
    {
    this->m_Picker = PickerType::New();
    this->m_Picker->SetTolerance(0.0001);

    this->m_Sphere = vtkSphereSource::New();
    this->m_Sphere->SetThetaResolution(6);
    this->m_Sphere->SetPhiResolution(6);
    this->m_Sphere->SetRadius(2);

    this->m_SphereMapper = vtkPolyDataMapper::New();
    this->m_SphereMapper->SetInput(this->m_Sphere->GetOutput());

    this->m_SphereActor = vtkActor::New();
    this->m_SphereActor->SetMapper(this->m_SphereMapper);
    this->m_SphereActor->GetProperty()->SetColor(0.0,1.0,0.0);

    this->m_ImageCoordinate[0] = 0;
    this->m_ImageCoordinate[1] = 0;
    this->m_ImageCoordinate[2] = 0;

    this->m_WorldCoordinate[0] = 0;
    this->m_WorldCoordinate[1] = 0;
    this->m_WorldCoordinate[2] = 0;

    this->SetStatus("no point found");
    this->SetData("\nCoord (world): (N/A)\nCoord (image): (N/A)\nValue: (N/A)");
    }
    
  VTKMouseImageInteractor::
  ~VTKMouseImageInteractor()
    {
      this->m_Picker->Delete();
      this->m_Sphere->Delete();
      this->m_SphereMapper->Delete();
      this->m_SphereActor->Delete();
    }

  void
  VTKMouseImageInteractor::
  RendererListProcess( RendererListType * itkNotUsed(rendererList) )
    {
    RendererListType::iterator it = this->m_RendererList->begin();
    RendererListType::iterator end = this->m_RendererList->end();
    
    while(it != m_RendererList->end())
      {    
      RendererType * cast = dynamic_cast<RendererType *>((*it).GetPointer());

      if( cast )
        {
        cast->GetVTKRenderer()->AddActor(this->m_SphereActor);
        }
      it++;
      }
    }

  void 
  VTKMouseImageInteractor::
  PushProcess(unsigned int x,unsigned y,unsigned int itkNotUsed(button))
    {
    if( this->m_Picker->Pick(x,y,0,this->m_Renderer->GetVTKRenderer()) )
      { 
      vtkActor * actor = this->m_Picker->GetActor();

      if( actor )
        {
        vtkTexture * texture = actor->GetTexture();

        if( texture )
          {
          vtkImageData * image = texture->GetInput();

          if( image )
            {
            vtkFloatingPointType paramCoord[3];

            this->m_Picker->GetPickedPositions()->GetPoint(0,this->m_WorldCoordinate);

            if( image->ComputeStructuredCoordinates ( this->m_WorldCoordinate,
                                                      this->m_ImageCoordinate,
                                                      paramCoord) )
              {
              std::string text;
              char* buffer = new char[255];           
              int value = (int)(*(unsigned char*)image->GetScalarPointer(
                                                      this->m_ImageCoordinate[0],
                                                      this->m_ImageCoordinate[1],
                                                      this->m_ImageCoordinate[2]));

              text = "\nCoord (world): (";
              sprintf(buffer,"%f\0",this->m_WorldCoordinate[0]);
              text += buffer;
              text += ",";
              sprintf(buffer,"%f\0",this->m_WorldCoordinate[1]);
              text += buffer;
              text += ",";
              sprintf(buffer,"%f\0",this->m_WorldCoordinate[2]);
              text += buffer;
              text += ")\n";
              text += "Coord (image): (";
              sprintf(buffer,"%d\0",this->m_ImageCoordinate[0]);
              text += buffer;
              text += ",";
              sprintf(buffer,"%d\0",this->m_ImageCoordinate[0]);
              text += buffer;
              text += ",";
              sprintf(buffer,"%d\0",this->m_ImageCoordinate[0]);
              text += buffer;
              text += ")\n";
              text += "Value: (";
              sprintf(buffer,"%d\0",value);
              text += buffer;
              text += ")";

              delete buffer;
              this->SetData(text.c_str());
              this->SetStatus("point found");
                            
              this->m_SphereActor->SetPosition(this->m_WorldCoordinate);
              }
            else
              {
              this->SetStatus("no point found");
              this->SetData("\nCoord (world): (N/A)\nCoord (image): (N/A)\nValue: (N/A)");
              }
            }
          }
        }
      }
    else
      {
      this->SetStatus("point outside image");
      this->SetData("\nCoord (world): (N/A)\nCoord (image): (N/A)\nValue: (N/A)");
      }
    }

  void
  VTKMouseImageInteractor
  ::GetImageCoordinate( int coord[] ) const
    {
    coord[0] = this->m_ImageCoordinate[0];
    coord[1] = this->m_ImageCoordinate[1];
    coord[2] = this->m_ImageCoordinate[2];
    }

  void
  VTKMouseImageInteractor
  ::GetWorldCoordinate( float coord[] ) const
    {
    coord[0] = this->m_WorldCoordinate[0];
    coord[1] = this->m_WorldCoordinate[1];
    coord[2] = this->m_WorldCoordinate[2];
    }

}//end of namespace
