#include "sovGlSlicerMouseInteractor.h"

namespace sov
{

  GlSlicerMouseInteractor
  ::GlSlicerMouseInteractor()
  {
    this->m_Renderer = NULL;
    this->m_ImageRenderMethod = NULL;
  }

  GlSlicerMouseInteractor
  ::~GlSlicerMouseInteractor()
  {

  }

  void
  GlSlicerMouseInteractor
  ::Push(unsigned int x,unsigned y,unsigned int itkNotUsed(button))
  {
    std::cout<<"push("<<x<<","<<y<<")"<<std::endl;

    this->m_Renderer->Click(x,y);
    this->m_Renderer->Update();
    
    if(m_Display)
      {
      static_cast<Display*>(m_Display)->Redraw();
      }
    else
      {
      std::cout << "sovGlSlicerKeyboardInteractor: WARNING Display not set!" << std::endl;
      }
  }

  void
  GlSlicerMouseInteractor
  ::Drag(unsigned int itkNotUsed(x),unsigned itkNotUsed(y),unsigned int itkNotUsed(button))
  {
    //std::cout<<"drag"<<std::endl;
  }

  void
  GlSlicerMouseInteractor
  ::Release(unsigned int itkNotUsed(x),unsigned itkNotUsed(y),unsigned int itkNotUsed(button))
  {
    //std::cout<<"release"<<std::endl;
  }

  void
  GlSlicerMouseInteractor
  ::Handle(unsigned int itkNotUsed(x),unsigned itkNotUsed(y))
  {
    //std::cout<<"handle"<<std::endl;
  }

  void
  GlSlicerMouseInteractor
  ::AddRenderer( RendererType * renderer )
  {
    this->m_Renderer = renderer;
  }

}//end of namespace sov
