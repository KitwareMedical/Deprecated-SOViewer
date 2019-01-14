/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovNDimensionalRenderer.h,v $
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
#ifndef __NDimensionalRenderer_h
#define __NDimensionalRenderer_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include  <itkObject.h>
#include "sovObjectViewerWin32Header.h"

namespace sov
{

/**
 *
 * 
 */
class SOViewer_EXPORT NDimensionalRenderer
: public itk::Object
{

public:

  typedef NDimensionalRenderer                Self;
  typedef itk::Object                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;

  itkTypeMacro( NDimensionalRenderer, Object );

  itkSetMacro(Width,unsigned int);
  itkGetMacro(Width,unsigned int);
  
  itkSetMacro(Height,unsigned int);
  itkGetMacro(Height,unsigned int);

  virtual void draw( void );
  
  virtual void UpdateDisplaySize(void) =0;
  virtual void Update(void) = 0;

protected:

  NDimensionalRenderer();
  virtual ~NDimensionalRenderer();

  /** These two variables define the size of the display
   *  because some Render methods need them */
  unsigned int   m_Width;
  unsigned int   m_Height;

};

} //end of namespace sov



#endif //__sovNDimensionalRenderer_h
