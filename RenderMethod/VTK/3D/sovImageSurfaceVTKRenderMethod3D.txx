/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovImageSurfaceVTKRenderMethod3D.txx,v $
  Language:  C++
  Date:      $Date: 2005-05-12 10:22:53 $
  Version:   $Revision: 1.2 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovImageSurfaceVTKRenderMethod3D.h"
#include <vtkPolyLine.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkCleanPolyData.h>
#include <vtkPolyDataMapper.h>

#include <vtkScalarsToColors.h>
#include <vtkLookupTable.h> 
#include <vtkFloatArray.h>
#include <vtkProperty.h>
#include <vtkContourFilter.h>

#include <itkImage.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkDecimatePro.h>
#include <vtkTriangleFilter.h>


#ifndef vtkFloatingPointType
# define vtkFloatingPointType vtkFloatingPointType
typedef float vtkFloatingPointType;
#endif

namespace sov
{

/** Constructor */
template <class TImage>
ImageSurfaceVTKRenderMethod3D<TImage>
::ImageSurfaceVTKRenderMethod3D()
{
  m_SupportedTypeList->clear();
  AddSupportedType("ImageSpatialObject");
  m_Smoothness = 0.05; // beetween 0.01 and 0.1(smoother)
  m_Decimation = 0.5;
  m_Scale = 1;
  m_Threshold = 0.5;
}

/** Destructor */
template <class TImage>
ImageSurfaceVTKRenderMethod3D<TImage>
::~ImageSurfaceVTKRenderMethod3D()
{
}

/** */
template <class TImage>
void
ImageSurfaceVTKRenderMethod3D<TImage>
::draw( void )
{
}

/** Rebuild the display list using the object attached 
 * to this render method. */
template <class TImage>
void
ImageSurfaceVTKRenderMethod3D<TImage>
::Update()
{
  ObjectListType::iterator currentObject = m_ObjectList.begin();
  ObjectListType::iterator lastObject = m_ObjectList.end();

  for(; currentObject!=lastObject; currentObject++)
    {
    sov::VisualImageProperty * property = static_cast<sov::VisualImageProperty*>((*currentObject)->GetProperty());

    if( (*currentObject)->IsModified()
    ||  (*currentObject)->IsPropertyAskingForUpdate())
      {
      std::cout<<"generating..."<<std::endl;
        
      //RemoveActor(*currentObject); // remove the actor before creating the new one.
      
      ImageConstPointer image = dynamic_cast<ImageSpatialObjectType *>((*currentObject)->GetObject())->GetImage();

      std::cout<<"1"<<std::endl;

      if( image )
        {

        //sov::VisualImageProperty * property = static_cast<sov::VisualImageProperty*>((*currentObject)->GetProperty());

        // I have commented out the following block of code 
        // because it did not compile properly...
        //
        // error description: itkImageToVTKImageFilter.h file was not found 
        //

        /*
        std::cout << "Creating VTK Image ...";
        typedef itk::ImageToVTKImageFilter<ImageType>  ImageToVTKFilterType;
        typename ImageToVTKFilterType::Pointer imageFilter = ImageToVTKFilterType::New();
        imageFilter->SetInput(image);
        imageFilter->Update();
        vtkImageData* vtkImage = imageFilter->GetOutput();
        imageFilter->SetReferenceCount(2);
        std::cout << "Done." << std::endl;
        */

        // This code replace the use of the 
        typename ExportType::Pointer exporter = ExportType::New();
        exporter->SetInput(image);
        exporter->UpdateLargestPossibleRegion();
        exporter->SetReferenceCount(2);
        
        std::cout<<"2"<<std::endl;

        ImportType * importer = ImportType::New();
        importer->SetUpdateInformationCallback(exporter->GetUpdateInformationCallback());
        importer->SetPipelineModifiedCallback(exporter->GetPipelineModifiedCallback());
        importer->SetWholeExtentCallback(exporter->GetWholeExtentCallback());
        importer->SetSpacingCallback(exporter->GetSpacingCallback());
        importer->SetOriginCallback(exporter->GetOriginCallback());
        importer->SetScalarTypeCallback(exporter->GetScalarTypeCallback());
        importer->SetNumberOfComponentsCallback(exporter->GetNumberOfComponentsCallback());
        importer->SetPropagateUpdateExtentCallback(exporter->GetPropagateUpdateExtentCallback());
        importer->SetUpdateDataCallback(exporter->GetUpdateDataCallback());
        importer->SetDataExtentCallback(exporter->GetDataExtentCallback());
        importer->SetBufferPointerCallback(exporter->GetBufferPointerCallback());  
        importer->SetCallbackUserData(exporter->GetCallbackUserData());
        importer->SetDataScalarTypeToUnsignedChar();
        importer->UpdateWholeExtent();

        std::cout<<"3"<<std::endl;

        itk::Size<3> size = image->GetRequestedRegion().GetSize();
        itk::Index<3> index = image->GetRequestedRegion().GetIndex();

        std::list<ImageSurfaceEntity*>::iterator it = this->m_EntityList.begin();
        std::list<ImageSurfaceEntity*>::iterator end = this->m_EntityList.end();

        for(; it!=end; it++)
          {
          if( (*it)->m_Object == (*currentObject)->GetObject() )
            {
            this->m_ActorToBeRemovedList.push_back((*it)->m_Actor);
            (*it)->Clear();
            this->m_EntityList.erase(it);
            break;
            }
          }
        
        std::cout<<"4"<<std::endl;

        typename ImageType::SpacingType spacing = (*currentObject)->GetObject()->GetSpacing();

        ImageSurfaceEntity * entity = new ImageSurfaceEntity();
        entity->m_Object = (*currentObject)->GetObject();
        entity->m_Clipper->SetInput(importer->GetOutput());
        entity->m_Clipper->SetOutputWholeExtent(  index[0],index[0]+size[0],
                                                  index[1],index[1]+size[1],
                                                  index[2],index[2]+size[2]);
        entity->m_Blur->SetRadiusFactors(         property->GetStandardDeviation(),
                                                  property->GetStandardDeviation(),
                                                  property->GetStandardDeviation());
        entity->m_Contour->SetNumberOfContours(1);
        entity->m_Contour->SetValue(0,property->GetThreshold());
        entity->m_Smoother->SetNumberOfIterations(property->GetSmoothingIterations());
        entity->m_Smoother->SetRelaxationFactor(property->GetSmoothingFactor());
        entity->m_Cleaner->SetTolerance(property->GetTolerance());
        entity->m_Cleaner->PointMergingOff();
        entity->m_Decimater->SetTargetReduction(property->GetDecimation());
        entity->m_Decimater->SetPreserveTopology(property->GetPreserveTopology());

        float* propColor = property->GetSurfaceColor();
        vtkFloatingPointType color[3];
        for(unsigned int c=0;c<3;c++)
          {
          color[c] = propColor[c];
          }

        entity->m_Actor->GetProperty()->SetColor(color);
        entity->m_Actor->GetProperty()->SetOpacity((*currentObject)->GetProperty()->GetOpacity());

        std::cout<<"5"<<std::endl;

        this->m_EntityList.push_back(entity);
        m_ActorToBeAddedList.push_back(entity->m_Actor);
        property->SetSurfaceModified(false);
        /**/

        (*currentObject)->UpdateMTime();
        (*currentObject)->UpdatePropertyAskingForUpdateMTime();
        }
      else
        {
        std::cout<<"Image could not be rendered with the ImageSurfaceVTKRenderMethod"<<std::endl;
        }
      }
    
    if((*currentObject)->IsPropertyModified())
      {
      std::cout<<"not regenerating..."<<std::endl;

      std::list<ImageSurfaceEntity*>::iterator it = this->m_EntityList.begin();
      std::list<ImageSurfaceEntity*>::iterator end = this->m_EntityList.end();

      for(; it!=end; it++)
        {
        if( (*it)->m_Object == (*currentObject)->GetObject() )
          {
          (*it)->m_Actor->GetProperty()->SetOpacity((*currentObject)->GetProperty()->GetOpacity());

          float* propColor = property->GetSurfaceColor();
          vtkFloatingPointType color[3];
          for(unsigned int c=0;c<3;c++)
            {
            color[c] = propColor[c];
            }

          (*it)->m_Actor->GetProperty()->SetColor(color);

          switch( property->GetRepresentationType() )
            {
            case SOV_WIREFRAME:
              (*it)->m_Actor->GetProperty()->SetRepresentationToWireframe();
              break;

            case SOV_SURFACE:
              (*it)->m_Actor->GetProperty()->SetRepresentationToSurface();
              break;

            case SOV_POINTS:
              (*it)->m_Actor->GetProperty()->SetRepresentationToPoints();
              break;
            }
          }
        }

      (*currentObject)->UpdatePropertyMTime();
      }

    if((*currentObject)->IsTransformModified())
      {
      ApplyTransform(* currentObject);
      (*currentObject)->UpdateTransformMTime();
      }
  } 
}


/** Print the method */
template <class TImage>
void
ImageSurfaceVTKRenderMethod3D<TImage>
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{
}


} // end of namespace sov
