/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVTKRenderer.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:58 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __sovVTKRenderer_h
#define __sovVTKRenderer_h

#include "sovRenderer.h"
#include "vtkRenderer.h"
#include "sovVTKRenderMethodFactory.h"

namespace sov
{

/**
 *
 */
template< unsigned int TDimension >
class VTKRenderer
: public Renderer<TDimension>
{

public:

  typedef VTKRenderer            Self;
  typedef SmartPointer< Self >   Pointer;
  typedef Renderer<TDimension>   Superclass;
  typedef typename Superclass::SceneType  SceneType;
  typedef typename Superclass::MethodPointer MethodPointer;
 
  itkNewMacro( Self );
  itkTypeMacro( VTKRenderer, Renderer );
  vtkRenderer* GetVTKRenderer(void) {return m_Renderer;}

  void SetBackground(float R,float G, float B);
  void draw(void) ;
   
  MethodPointer GetRenderMethodViaFactory(const char * nametype, 
                                    unsigned int, 
                                    const char* type,
                                    const char* methodName,
                                    const char* pixelType=NULL)
                                    {return VTKRenderMethodFactory<TDimension>::CreateRenderMethod( nametype, 
                                            TDimension, type, methodName, pixelType );}

protected:

  VTKRenderer();
  virtual ~VTKRenderer();

  void PrintSelf( std::ostream& , Indent ) const;

  vtkRenderer               *m_Renderer;
};

} //end of namespace sov

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovVTKRenderer.txx"
#endif

#endif //__sovVTKRenderer_h
