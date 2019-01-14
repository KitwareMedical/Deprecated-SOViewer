/*******************************************************************
 * FILE:     main.cxx
 * PROJECT:  ITKUNC
 * AUTHORS:  CADDLab
 * DATE:     October 2002
 * COMMENTS: None
 *******************************************************************/

/**
 *
 */

#include <qapplication.h>
#include "SampleAppGUIControls.h"


int main(int argc, char** argv)
{ 
  QApplication myApp(argc,argv);

  SampleAppGUIControls m_GUI( 0, 0, TRUE );
  myApp.setMainWidget(&m_GUI);
  m_GUI.setCaption( "Test" );

  m_GUI.CreateScene();
  
  m_GUI.show();

  return  myApp.exec();
}
