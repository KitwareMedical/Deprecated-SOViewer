



#ifndef _sovFlVTKDisplay_h
#define _sovFlVTKDisplay_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4284 )
#pragma warning ( disable : 4018 )
#endif

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <vtkRenderWindowInteractor.h>
#include "sovNDimensionalRenderer.h" 
#include <vtkRenderWindow.h>
#include "sovMouseInteractor.h"
#include "sovKeyboardInteractor.h"
#include <vtkCommand.h>
#include <vtkRenderWindowInteractor.h>
#include <list>
#include "sovObjectViewerWin32Header.h"

namespace sov{

class SOViewer_EXPORT FlVTKDisplay : public Fl_Gl_Window, public vtkRenderWindowInteractor {
 protected:
   // Fl_Gl_Window overrides
   void flush(void);
   void draw( void );
   void resize( int x, int y, int w, int h );
   int  handle( int event );

 public:
   // ctors
   FlVTKDisplay();
   FlVTKDisplay( int x, int y, int w, int h, const char *l="");

  typedef NDimensionalRenderer          RendererType;
  typedef std::list< RendererType * >   RendererListType;

   // vtk ::New()
   static FlVTKDisplay * New();
   // dtor
   ~FlVTKDisplay( void );

   // vtkRenderWindowInteractor overrides
   void Initialize();
   void Enable();
   void Disable();
   void Start();
   void SetRenderWindow(vtkRenderWindow *aren);
   void UpdateSize(int x, int y);
   int CreateTimer(int timertype);
   int DestroyTimer();
   void OnTimer(void);
   void TerminateApp();

   void DisableEvents() {m_EnableEvents = false;}


  void SetRendererList( RendererListType * );
  RendererListType * GetRendererList();

  vtkRenderWindow * GetVTKRenderWindow(void){return this->RenderWindow;}

  void AddRenderer( RendererType * renderer );
  void SetMouseInteractor(MouseInteractor* interactor){m_MouseInteractor = interactor;}
  void SetKeyboardInteractor(KeyboardInteractor* interactor){m_KeyboardInteractor = interactor;}
  void Update(void);

 private:

  RendererListType *  m_RendererList;
  bool                m_ShowAxis;
  //vtkRenderWindow *   m_RenderWindow;
  MouseInteractor*    m_MouseInteractor;
  KeyboardInteractor* m_KeyboardInteractor;
  bool m_EnableEvents;

  };

} // end namespace

#endif
