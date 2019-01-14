/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovDisplay.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __sovDisplay_h
#define __sovDisplay_h

/** Base class for displays so that interactors can be used with any displays */
#include <iostream>
#include <sovNDimensionalRenderer.h>
#include <list>

namespace sov
  {
  typedef NDimensionalRenderer          RendererType;
  typedef std::list< RendererType::Pointer >   RendererListType;
   
  class Display
    {

    public:

      Display() {};
      virtual ~Display() {};

      virtual void Redraw() = 0; //{std::cout << "Not Implemented" << std::endl;}
      virtual void Update() = 0; //{std::cout << "Not Implemented" << std::endl;}
      
      virtual const RendererListType & GetRendererList() const = 0;

  
    };//end of class

  } //end of namespace sov

#endif //__sovDisplay_h
