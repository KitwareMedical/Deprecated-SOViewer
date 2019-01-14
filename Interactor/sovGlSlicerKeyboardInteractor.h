/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovGlSlicerKeyboardInteractor.h,v $
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
#ifndef __GlSlicerKeyboardInteractor_h
#define __GlSlicerKeyboardInteractor_h

#include <sovKeyboardInteractor.h>
#include <sovDisplay.h>
#include <sovGlRendererSlicer.h>
#include <sovImageGlRenderMethodSlicer.h>
#include "sovObjectViewerWin32Header.h"

namespace sov
{
  
class SOViewer_EXPORT GlSlicerKeyboardInteractor : public KeyboardInteractor
{

public:

  typedef KeyboardInteractor           Superclass;
  typedef GlRendererSlicer             RendererType;
  typedef RenderMethod< 3 >            MethodType;
  typedef MethodType::Pointer          MethodPointer;
  typedef std::list< MethodPointer >   MethodListType;
  typedef Superclass::RendererListType RendererListType;
  typedef itk::Image<unsigned char, 3> ImageType; // Dummy type
  typedef ImageGlRenderMethodSlicer<ImageType>  ImageRenderMethodType;

  GlSlicerKeyboardInteractor();

  ~GlSlicerKeyboardInteractor();

  virtual int Shortcut(int key);

  void SetOrientationCallback(void(*callback)(unsigned int));

  void SetSliceCallback(void(*callback)(unsigned int));

protected:

  void (*m_OrientationCallback)(unsigned int);
  void (*m_SliceCallback)(unsigned int);

  RendererType* m_Renderer; // only one renderer supported for the moment
  ImageRenderMethodType* m_ImageRenderMethod;

};

} //end of namespace sov

#endif //__GlSlicerKeyboardInteractor_h
