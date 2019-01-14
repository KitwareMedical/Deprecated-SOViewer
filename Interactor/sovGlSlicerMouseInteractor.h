/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovGlSlicerMouseInteractor.h,v $
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
#ifndef __GlSlicerMouseInteractor_h
#define __GlSlicerMouseInteractor_h

#include <sovMouseInteractor.h>
#include <sovDisplay.h>
#include <sovGlRendererSlicer.h>
#include <sovImageGlRenderMethodSlicer.h>
#include "sovObjectViewerWin32Header.h"

namespace sov
{
  
class SOViewer_EXPORT GlSlicerMouseInteractor : public MouseInteractor
  {

  public:

    typedef GlSlicerMouseInteractor      Self;
    typedef MouseInteractor              Superclass;
    typedef GlRendererSlicer             RendererType;
    typedef RenderMethod< 3 >            MethodType;
    typedef MethodType::Pointer          MethodPointer;
    typedef std::list< MethodPointer >   MethodListType;
    typedef Superclass::RendererListType RendererListType;
    typedef itk::Image<unsigned char, 3> ImageType; // Dummy type
    typedef ImageGlRenderMethodSlicer<ImageType>  ImageRendererMethodType;

    GlSlicerMouseInteractor();

    ~GlSlicerMouseInteractor();

    virtual void Push(unsigned int x,unsigned y,unsigned int button);
    virtual void Drag(unsigned int x,unsigned y,unsigned int button);
    virtual void Release(unsigned int x,unsigned y,unsigned int button);
    virtual void Handle(unsigned int x,unsigned int y);
    
    virtual void AddRenderer(RendererType * renderer);

  protected:
    
    RendererType* m_Renderer; // only one renderer supported for the moment
    ImageRendererMethodType* m_ImageRenderMethod;

  };

} //end of namespace sov

#endif //__GlSlicerMouseInteractor_h
