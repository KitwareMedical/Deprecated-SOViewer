/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: SOVCommonTests.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-29 23:20:52 $
  Version:   $Revision: 1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "itkTestMain.h" 


void RegisterTests()
{
 REGISTER_TEST(itkMemoryLeakTest);
}
