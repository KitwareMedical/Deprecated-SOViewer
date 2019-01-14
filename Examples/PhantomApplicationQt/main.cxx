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
#include "PhantomAppGUIControls.h"


int main(int argc, char** argv)
{ 
   
  QApplication myApp(argc,argv);

  PhantomAppGUIControls m_GUI( 0, 0, TRUE );
  myApp.setMainWidget(&m_GUI);
  m_GUI.setCaption( "Test" );

  m_GUI.CreateScene();
  
  m_GUI.StartPhantom();

  m_GUI.exec();

  // stop the haptic simulation
  m_GUI.StopPhantom();

  return EXIT_SUCCESS;
}
