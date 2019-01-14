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

int main( int arc, char* argv[] )
{
    ShellExplorerApp* app = new ShellExplorerApp();

    app->Run();

    delete app;

    return 0;
}
