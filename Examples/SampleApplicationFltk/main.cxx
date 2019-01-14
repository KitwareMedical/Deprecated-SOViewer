/*******************************************************************
 * FILE:     main.cxx
 * PROJECT:  ITKUNC
 * AUTHORS:  CADDLab
 * DATE:     October 2002
 * COMMENTS: None
 *******************************************************************/

#include "SampleAppGUIControls.h"
#include "itkImage.h"

int main(int , char** )
{ 
  SampleAppGUIControls* mGUI = new SampleAppGUIControls();

  mGUI->CreateScene();
  mGUI->win->show();

  Fl::run();
  return EXIT_SUCCESS;
}
