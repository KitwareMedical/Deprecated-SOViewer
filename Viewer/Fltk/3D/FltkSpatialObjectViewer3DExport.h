/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: FltkSpatialObjectViewer3DExport.h,v $
  Language:  C++
  Date:      $Date: 2005-07-31 18:56:48 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the ObjectViewer copyright.
  See ObjectViewerCopyright.txt or http://www.kitware.com/ObjectViewerCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __FltkSpatialObjectViewer3DExport_h
#define __FltkSpatialObjectViewer3DExport_h

#include "sovConfigure.h"

// add in the Windows variants

#if defined(__CYGWIN__)
#ifndef WIN32
#define WIN32 1
#endif
#ifndef _WIN32
#define _WIN32 1
#endif
#endif

/** Disable some common warnings in MS VC++ */
#if defined(_MSC_VER)

// 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning ( disable : 4244 )

// 'identifier' : truncation from 'type1' to 'type2'
#pragma warning ( disable : 4305 )

// 'conversion' : truncation of constant value
#pragma warning ( disable : 4309 )

// 'identifier' : identifier was truncated to 'number' characters in the
// debug information
#pragma warning ( disable : 4786 )

// 'type' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning ( disable : 4800 )

// 'identifier' : class 'type' needs to have dll-interface to be used by
// clients of class 'type2'
#pragma warning ( disable : 4251 )


// non dll-interface class 'type' used as base for dll-interface class 'type2'
#pragma warning ( disable : 4275 )

// C++ exception specification ignored except to indicate a 
// function is not __declspec(nothrow)
#pragma warning ( disable : 4290 )

// 'type' : inconsistent dll linkage.  dllexport assumed.
#pragma warning ( disable : 4273 )

// typename keyword in default template arguments is not accepted by
// MSVC.  This macro should only be used in such places.
# if !defined(CABLE_CONFIGURATION) && (_MSC_VER < 1310)
#  define SOV_TYPENAME
# else
#  define SOV_TYPENAME typename
# endif
#else
# define SOV_TYPENAME typename
#endif


#define SOV_EXPORT
#if (defined(_WIN32) || defined(WIN32)) && !defined(SOVSTATIC) 
# ifdef FltkSOViewer3D_EXPORTS
#  define FltkSOViewer3D_EXPORT __declspec(dllexport)
# else
#  define FltkSOViewer3D_EXPORT __declspec(dllimport)
# endif  /* SOViewer_EXPORT */
#else
/* unix needs nothing */
#define FltkSOViewer3D_EXPORT 
#endif

#endif
