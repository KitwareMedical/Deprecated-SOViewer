/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: SOVSpatialObjectTests.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-29 23:21:02 $
  Version:   $Revision: 1.1 $
  Author:    Julien Jomier

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
 REGISTER_TEST(itkSpatialObjectMemoryTest);
 REGISTER_TEST(itkSparsePointsSpatialObjectTest);
}
