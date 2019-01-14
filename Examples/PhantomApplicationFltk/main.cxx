/*******************************************************************
 * FILE:     main.cxx
 * PROJECT:  ITKUNC
 * AUTHORS:  CADDLab
 * DATE:     October 2002
 * COMMENTS: None
 *******************************************************************/

#include "PhantomAppGUIControls.h"

int main(int , char** )
{ 
  PhantomAppGUIControls* mGUI = new PhantomAppGUIControls();

  mGUI->CreateScene();
  mGUI->win->show();
  mGUI->Loop();
  Fl::run();
  return EXIT_SUCCESS;
}
