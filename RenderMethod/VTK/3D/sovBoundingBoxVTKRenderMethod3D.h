/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovBoundingBoxVTKRenderMethod3D.h,v $
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
#ifndef _BoundingBoxVTKRenderMethod3D_h
#define _BoundingBoxVTKRenderMethod3D_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "sovVTKRenderMethod3D.h"
#include <itkImageSpatialObject.h>
#include <sovVTKWin32Header.h>

namespace sov
{

  class  SOVVTKRenderMethod_EXPORT BoundingBoxVTKRenderMethod3D
  : public VTKRenderMethod3D
  {

  public:

    typedef BoundingBoxVTKRenderMethod3D   Self;
    typedef VTKRenderMethod3D              Superclass;
    typedef SmartPointer< Self >           Pointer;
    typedef SmartPointer< const Self >     ConstPointer;

    typedef itk::ImageSpatialObject<3>     BoundingBoxType;
    typedef Superclass::ObjectType         ObjectType;
    typedef Superclass::ObjectListType     ObjectListType;

    itkNewMacro( BoundingBoxVTKRenderMethod3D );

    itkTypeMacro( BoundingBoxVTKRenderMethod3D, VTKRenderMethod3D );

    virtual void draw( void );

  protected:

    BoundingBoxVTKRenderMethod3D( void );

    ~BoundingBoxVTKRenderMethod3D( void );

    void PrintSelf (std::ostream &os, Indent indent) const;

    virtual void Update();
  };

}

#endif //_BoundingBoxVTKRenderMethod3D_h
