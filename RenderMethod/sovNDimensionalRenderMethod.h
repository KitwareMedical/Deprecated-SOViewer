/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovNDimensionalRenderMethod.h,v $
  Language:  C++
  Date:      $Date: 2006-02-15 21:35:43 $
  Version:   $Revision: 1.2 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __NDimensionalRenderMethod_h
#define __NDimensionalRenderMethod_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include <itkObject.h>

/** Abstract class to remove the dimensionality of the RenderMethods */

namespace sov
{

using namespace itk;

class NDimensionalRenderMethod
: public itk::Object
{

public:

  typedef NDimensionalRenderMethod         Self;
  typedef itk::Object                      Superclass;
  typedef itk::SmartPointer< Self >        Pointer;
  typedef itk::SmartPointer< const Self >  ConstPointer;

  unsigned int GetDimension() {return m_Dimension;}
  const char * GetType() {return m_Type.c_str();}
  const char* GetPixelType(){return m_PixelType.c_str();}

protected:

  NDimensionalRenderMethod( void )
    {
    m_PixelType = "";
    };
  ~NDimensionalRenderMethod( void ){};  

  std::string m_Type;
  unsigned int m_Dimension;
  std::string                    m_PixelType; // Only used for images at this moment.

};

} //end of namespace sov


#endif //__NDimensionalRenderMethod_h
