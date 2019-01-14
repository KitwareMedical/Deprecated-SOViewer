/*=========================================================================

  Program:   SOViewer
  Module:    $RCSfile: AttributeMeshSpatialObject.h,v $
  Language:  C++
  Date:      $Date: 2008-12-08 20:41:21 $
  Version:   $Revision: 1.10 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __AttributeMeshSpatialObject_h
#define __AttributeMeshSpatialObject_h


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <itkMeshSpatialObject.h>
#include <itkTriangleCell.h>
#include <vector>
#include <fstream>
using namespace itk;


template < class TMesh = Mesh<int>, class TAttributeType = float >
class AttributeMeshSpatialObject : public MeshSpatialObject<TMesh>
{
public:
  /** Run-time type information (and related methods). */
  itkTypeMacro( AttributeMeshSpatialObject, MeshSpatialObject );
  typedef AttributeMeshSpatialObject< TMesh, TAttributeType> Self;
  
  typedef MeshSpatialObject< TMesh >                 Superclass;
  typedef SmartPointer< Self >                       Pointer;
  typedef SmartPointer< const Self >                 ConstPointer;

  typedef TMesh                                      MeshType;
  typedef typename MeshType::Pointer                 MeshPointer;
  typedef typename Superclass::TransformType         TransformType;
  typedef typename Superclass::PointType             PointType;
  typedef typename Superclass::BoundingBoxType       BoundingBoxType;

  typedef VectorContainer< unsigned long, PointType> PointContainerType;
  typedef typename PointContainerType::Pointer       PointContainerPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );
  
  typedef std::vector < TAttributeType > TAttributeListType;

  void ReadAttributes ( std::string attrFilename );

  TAttributeListType  GetAttribute (unsigned int vertIndex);
  AttributeMeshSpatialObject();
  virtual ~AttributeMeshSpatialObject();

  void writeToFile(const char *filename);
  void loadFromFile(const char *filename);

  void SetAttributeDimension (int d);
  int GetAttributeDimension ();

private:

  std::vector < TAttributeListType > m_Attributes;
  int                                m_AttributeDimension;
};

#include "AttributeMeshSpatialObject.txx"

#endif
