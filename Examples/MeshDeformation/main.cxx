/*******************************************************************
 * FILE:     main.cxx
 * PROJECT:  ITKUNC
 * AUTHORS:  CADDLab
 * DATE:     October 2002
 * COMMENTS: None
 *******************************************************************/

#include "MeshDeformationGUIControls.h"
#include "itkImage.h"
#include <time.h>

int main(int , char** )
{ 
  MeshDeformationGUIControls* mGUI = new MeshDeformationGUIControls();
  mGUI->CreateScene();
  mGUI->win->show();
  bool reverse = false;
  float i=0;
  for(float j=0.1;j<1;j+=0.00001)
    {
    
    if(i>=0.95)
      {
      reverse = true;
      }
    if(i<=0.05)
      {
      reverse=false;
      }

    if(reverse)
      {
      i-=0.05;
      }
    else
      {
      i+=0.05;
      }
    mGUI->TestRandom(i);
    }
  Fl::run();

  return EXIT_SUCCESS;
}
