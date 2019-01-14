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



#include "ShellExplorerApp.h"

#include <FL/Fl_File_Chooser.H>
#include <Fl/Fl_Color_Chooser.H>

ShellExplorerApp::ShellExplorerApp()
{
    TRACE("ShellExplorerApp::ShellExplorerApp()");

    m_Renderer = RendererType::New();

    m_Display->AddRenderer( m_Renderer );

    m_Renderer->SetScene( m_Scene );
    m_Renderer->GetVTKRenderer()->ResetCamera();
}

void ShellExplorerApp::UpdateShell()
{
    TRACE("ShellExplorerApp::UpdateShell()");

    // Get new parameters from GUI

    m_A = this->a_slider->value();
    m_B = this->b_slider->value();
    m_C = this->c_slider->value();
    m_N = this->n_slider->value();

    unsigned ures = (unsigned)this->ures_slider->value();
    unsigned vres = (unsigned)this->vres_slider->value();

    // If the mesh resolution has been changed, we need to regenerate
    if ( ures != m_Ures || vres != m_Vres )
    {
        m_Mesh = NULL;
    }

    m_Ures = ures;
    m_Vres = vres;

    // Now actually update the mesh
    ShellExplorerBase::UpdateShell();
}

void ShellExplorerApp::Run()
{
    TRACE("ShellExplorerApp::Run()");

    main_win->show();

    m_Display->Update();

    Fl::run();
}

void ShellExplorerApp::SaveScene()
{
    TRACE("ShellExplorerApp::SaveScene()");

    const char * filename =
        fl_file_chooser("Save Scene", "*.meta", "shell.meta");

    if( !filename )
    {
        return;
    }

    this->SaveSceneAs( filename );
}

void ShellExplorerApp::SaveImage()
{
    TRACE("ShellExplorerApp::SaveImage()");

    const char * filename =
        fl_file_chooser("Save Image", "*.png", "shell.png");

    if( !filename )
    {
        return;
    }

    this->SaveImageAs( filename );
}

void ShellExplorerApp::SaveImageAs( const std::string& filename )
{
  m_Renderer->SaveRenderAsPNG( filename.c_str() );
}

void ShellExplorerApp::ChangeColour()
{
    TRACE("ShellExplorerApp::ChangeColour()");

    m_MeshObj->GetProperty()->SetColor(
        colour_chooser->r(),
        colour_chooser->g(),
        colour_chooser->b() );
}

void ShellExplorerApp::UpdateDisplay()
{
    TRACE("ShellExplorerApp::UpdateDisplay()");

    m_Renderer->Update();
    m_Renderer->GetVTKRenderer()->ResetCamera();
    m_Display->Update();
}

void ShellExplorerApp::Quit()
{
    TRACE("ShellExplorerApp::Quit()");

    main_win->hide();
}
