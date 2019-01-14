//==========================================================================
//
//  Shell Explorer
//
//--------------------------------------------------------------------------
//
//  Written by Gavin Baker <gavinb@cs.mu.oz.au>
//  Copyright (c) 2005
//  Released under the Clarified BSD License - see COPYING for details
//
//==========================================================================



#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkDefaultDynamicMeshTraits.h>
#include <itkMesh.h>
#include <itkSpatialObjectWriter.h>
#include <itkSceneSpatialObject.h>
#include <itkMeshSpatialObject.h>
#include <itkSurfaceSpatialObject.h>
#include <itkImageSpatialObject.h>

#include <string>
#include <iostream>

#ifdef DEBUG
#define TRACE(s) std::cerr<<s<<std::endl
#else
#define TRACE(s)
#endif

class ShellExplorerBase
{
public:
    
    typedef float PixelType;
    enum { Dimension = 3 };

    typedef itk::Image< PixelType, Dimension >                  ImageType;
    typedef itk::ImageFileReader< ImageType >                   ImageReaderType;
    typedef itk::CovariantVector<double, 3>                     VectorType;
    typedef itk::DefaultDynamicMeshTraits< PixelType >          MeshTraitsType;
    typedef itk::Mesh<PixelType, Dimension, MeshTraitsType >    MeshType;
    typedef MeshType::PointType                                 PointType;
    typedef MeshType::CellType                                  CellType;
    typedef CellType::CellAutoPointer                           CellAutoPointer;
    typedef itk::TriangleCell<CellType>                         TriType;

    typedef itk::SpatialObjectWriter<Dimension,
                                     PixelType,
                                     MeshTraitsType>            SpatialWriterType;
    typedef itk::SceneSpatialObject<3>                          SceneType;
    typedef itk::MeshSpatialObject<MeshType>                    MeshObjectType;
    typedef itk::GroupSpatialObject<3>                          GroupObjectType;
    typedef itk::ImageSpatialObject<3, PixelType>               ImageObjectType;

    /**
     *  Creates a new engine, with default parameters (but no mesh)
     */

    ShellExplorerBase();

    /**
     *  Saves the scene as a MetaObject file
     */
    void SaveSceneAs( const std::string& filename );

    /**
     *
     */
    void LoadVolume( const std::string& filename );

    /**
     *  Generates the shell mesh, with a cylindrical topology and default
     *  plain geometry
     */
    void MakeMesh();

    /**
     *  Reshapes the mesh according to the new parameters
     */
    virtual void UpdateShell();

    /**
     *  Takes the (u,v) coordinates for a point on mesh in and returns the
     *  unique point id for that vertex in the mesh.
     */
    inline unsigned UVtoPointId( unsigned u, unsigned v )
    {
        // Treat like Cartesian space: Ures is width, u is x,
        // v is y and wrap around the edge with modulo
        unsigned id = (v % m_Vres) * m_Ures + (u % m_Ures);

        return id;
    }

protected:

    MeshType::Pointer           m_Mesh;
    MeshObjectType::Pointer     m_MeshObj;
    GroupObjectType::Pointer    m_Group;
    SceneType::Pointer          m_Scene;
    SpatialWriterType::Pointer  m_Writer;
    ImageType::Pointer          m_Image;
    ImageObjectType::Pointer    m_ImageObj;

    // Shell parameters

    float m_A;
    float m_B;
    float m_C;
    float m_N;

    // Mesh resolution

    unsigned m_Ures;
    unsigned m_Vres;
};
