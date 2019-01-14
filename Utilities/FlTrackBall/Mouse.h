/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: Mouse.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef Mouse_h
#define Mouse_h

#include <iostream>

class Mouse 
{

public:
  
  inline Mouse () { Reset(); }

  inline Mouse & Reset()
  {
    u=v = uOld=vOld = du=dv = 0;           uw=vh=1;
    x=y = xOld=yOld = dx=dy = 0;  x0=y1=0; xw=yh=1;
    return *this;
  }
  
  inline void SetWindowSize(int uwnew, int vhnew) 
  { 
    uw = (float)uwnew; vh = (float)vhnew; 
  }
  inline void SetOglPosAndSize(float x0left, float y1top, float xwnew, float yhnew)
  { 
    x0 = x0left; y1 = y1top;  xw = xwnew; yh = yhnew; 
  }
  
  inline void TransformScreenToOgl() 
  {
    x = x0 + u*(xw/uw); xOld = x0 + uOld*(xw/uw);
    y = y1 - v*(yh/vh); yOld = y1 - vOld*(yh/vh); 
  }
  
  inline void UpdatePos(int unew, int vnew) 
  {
    uOld = u;  u=unew;  du = u-uOld;  xOld = x;  x = x0 + u * xw/uw;  dx = x-xOld;
    vOld = v;  v=vnew;  dv = v-vOld;  yOld = y;  y = y1 - v * yh/vh;  dy = y-yOld;
    
    xOld = 0;
    yOld = 0;
    x = -1+u * xw/uw;
    y = 1- v * yh/vh;
  }
  inline void UpdateButtons(int buttonsDown) { buttonsPressed = buttonsDown; }

public:
  int   u,v, uOld,vOld, du,dv; float uw,vh;  // screen pixel coordinates, origin at (0,0)
  float x,y, xOld,yOld, dx,dy, x0,y1,xw,yh;  // ogl coordinates
  int buttonsPressed;

}; // Mouse

#endif	// Mouse_h
