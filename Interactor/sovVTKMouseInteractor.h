/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVTKMouseInteractor.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __VTKMouseInteractor_h
#define __VTKMouseInteractor_h


#include <sovMouseInteractor.h>
#include <sovVTKRenderer3D.h>
#include <sovImageVTKRenderMethod3D.h>

#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include "sovObjectViewerWin32Header.h"

namespace sov
{
  
  class SOViewer_EXPORT VTKMouseInteractor : public MouseInteractor
    {

    public:
      
      typedef MouseInteractor                     Superclass;
      typedef VTKRenderer3D                       RendererType;
      typedef RenderMethod< 3 >                   MethodType;
      typedef MethodType::Pointer                 MethodPointer;
      typedef std::list< MethodPointer >          MethodListType;
      typedef itk::Image<unsigned char, 3>        ImageType; // Dummy type
      typedef ImageVTKRenderMethod3D<ImageType>   ImageRendererMethodType;

      VTKMouseInteractor();

      ~VTKMouseInteractor();

      void Push(unsigned int x,unsigned y,unsigned int button);
      virtual void Drag(unsigned int x,unsigned y,unsigned int button);
      virtual void Release(unsigned int x,unsigned y,unsigned int button);
      virtual void Handle(unsigned int x,unsigned y);

      virtual void AddRenderer(RendererType * renderer);

      virtual void SetDisplaySize( unsigned int width, unsigned int height );

      virtual void SetOrientation( bool invert[] );
      virtual void PushProcess(unsigned int x,unsigned y,unsigned int button){};

    protected:

      bool                     m_InvertedAxis[2];   //required to match widget and VTK 
                                                    //coordinate system orientation
      RendererType *           m_Renderer; // only one renderer supported for the moment

      vtkTextActor *           m_StatusActor;
      vtkTextActor *           m_DataActor;

      virtual void SetData( const char * data );

      virtual void SetStatus( const char * status );
      std::string          m_Status;
      std::string          m_Data;

    };//end of class

}//end of namespace

#endif //__VTKMouseInteractor_h
