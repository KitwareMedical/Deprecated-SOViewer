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



#include "ShellExplorerBase.h"

ShellExplorerBase::ShellExplorerBase()
{
    TRACE("ShellExplorerBase::ShellExplorerBase()");

    // Provide some nice defaults
    m_A=0.2; m_B=1; m_C=0.1; m_N=2; m_Ures = 32; m_Vres = 32;

    m_Group = GroupObjectType::New();
    m_MeshObj = MeshObjectType::New();
    m_Scene = SceneType::New();
    m_Writer = SpatialWriterType::New();

    m_Scene->AddSpatialObject( m_Group );

    m_Writer->SetInput( m_Scene );
}

void ShellExplorerBase::MakeMesh()
{
    TRACE("ShellExplorerBase::MakeMesh()");

    static unsigned cellCount = 0;

    m_Mesh = MeshType::New();

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif
    const float theta_step = 2*M_PI/m_Ures;
    const float phi_step   = 2*M_PI/m_Vres;

    // First just generate all the points
    for ( int v=0; v < m_Vres; v++ )
    {
        for ( int u=0; u < m_Ures; u++ )
        {
            unsigned id = UVtoPointId( u, v );
            PointType p;

            float theta = u * theta_step;
            float phi = v * phi_step;

            // Boring default cylinder geometry
            p[0] = cos(theta);
            p[1] = sin(theta);
            p[2] = v;

            m_Mesh->SetPoint( id, p );
        }
    }

    // Now stitch up the points into cells to form quadrilateral mesh
    for ( int v=0; v < m_Vres-1; v++ )
    {
        for ( int u=0; u < m_Ures; u++ )
        {
            // Get the point ids that belong to this quad, in
            // counterclockwise order (for VTK, ensure normals are out)
            unsigned id0 = UVtoPointId( u,   v   );
            unsigned id1 = UVtoPointId( u,   v+1 );
            unsigned id2 = UVtoPointId( u+1, v+1 );
            unsigned id3 = UVtoPointId( u+1, v   );

            // Since SOV doesn't yet support quads, we trianglulate manually,
            // making two tris to form the equivalent quad

            CellAutoPointer tri;

            // Top tri
            tri.TakeOwnership( new TriType );

            tri->SetPointId( 0, id0 );
            tri->SetPointId( 1, id1 );
            tri->SetPointId( 2, id3 );

            m_Mesh->SetCell( cellCount, tri );

            cellCount ++;

            // Bottom tri
            tri.TakeOwnership( new TriType );

            tri->SetPointId( 0, id1 );
            tri->SetPointId( 1, id2 );
            tri->SetPointId( 2, id3 );

            m_Mesh->SetCell( cellCount, tri );

            cellCount ++;
        }
    }

    m_MeshObj->SetMesh( m_Mesh );
    m_Group->AddSpatialObject( m_MeshObj );
}

void ShellExplorerBase::UpdateShell()
{
    TRACE("ShellExplorerBase::UpdateShell()");

    const float theta_step = 2*M_PI/m_Ures;
    const float phi_step   = 2*M_PI/m_Vres;

    PointType center;
    center[0] = 2;
    center[1] = 2;
    center[2] = 0;

    if ( !m_Mesh )
        this->MakeMesh();

    for ( int v=0; v < m_Vres; v++ )
    {
        for ( int u=0; u < m_Ures; u++ )
        {
            unsigned id = UVtoPointId( u, v );
            PointType p;

            float theta = u * theta_step;
            float phi = v * phi_step;

            p[0]=m_A*(1-phi/(2*M_PI))*cos(m_N*phi)*(1+cos(theta))+m_C*cos(m_N*phi);
            p[1]=m_A*(1-phi/(2*M_PI))*sin(m_N*phi)*(1+cos(theta))+m_C*sin(m_N*phi);
            p[2]=m_B*phi/(2*M_PI)+m_A*(1-phi/(2*M_PI))*sin(theta);

            m_Mesh->SetPoint( id, p );
        }
    }

    m_MeshObj->Update();
}

void ShellExplorerBase::SaveSceneAs( const std::string& filename )
{
    TRACE("ShellExplorerBase::SaveSceneAs(...)");

    m_Writer->SetFileName( filename.c_str() );
    m_Writer->Update();
}

void ShellExplorerBase::LoadVolume( const std::string& filename )
{
    TRACE("ShellExplorerBase::LoadVolume(...)");

    ImageReaderType::Pointer reader = ImageReaderType::New();
    reader->SetFileName( filename.c_str() );
    reader->Update();

    //m_Image = reader->GetOutput();
    m_ImageObj = ImageObjectType::New();
    m_ImageObj->SetImage( reader->GetOutput() );
    m_Group->AddSpatialObject( m_ImageObj );
}
