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



#include <sovVTKRenderer3D.h>

#include "ShellExplorerGUI.h"

class ShellExplorerApp : public ShellExplorerGUI
{
public:

    typedef sov::VTKRenderer3D RendererType;

    ShellExplorerApp();

    virtual void Run();

    virtual void UpdateShell();

    virtual void UpdateDisplay();

    virtual void ChangeColour();

    virtual void SaveScene();

    virtual void SaveImage();

    virtual void Quit();

    void SaveImageAs( const std::string& filename );

private:

    RendererType::Pointer m_Renderer;
};
