#include "sovGlSlicerKeyboardInteractor.h"

namespace sov
{
  GlSlicerKeyboardInteractor
  ::GlSlicerKeyboardInteractor()
  {
  m_Renderer = NULL;
  m_ImageRenderMethod = NULL;   
  }

  GlSlicerKeyboardInteractor
  ::~GlSlicerKeyboardInteractor()
  {
  }

  /** Shortcut interaction */
  int 
  GlSlicerKeyboardInteractor
  ::Shortcut(int key)
  {
    m_Renderer = NULL;

    const RendererListType & m_RendererList = m_Display->GetRendererList();
    
    RendererListType::const_iterator it = m_RendererList.begin();
    std::list<ImageRenderMethodType *> methodList;

    while(it != m_RendererList.end())
      {    
      if(!strcmp((*it)->GetNameOfClass(),"class sov::GlRendererSlicer"))
        {
        m_Renderer = static_cast<RendererType*>((*it).GetPointer());
        // Try to get an image render method
        const MethodListType* rmList = m_Renderer->GetRenderMethodList();
        MethodListType::const_iterator ot = rmList->begin();

        while(ot != rmList->end())
          {
          if(!strncmp(typeid(**ot).name(),"class sov::ImageGlRenderMethodSlicer",25))
            {
            //m_ImageRenderMethod = static_cast<ImageRenderMethodType*>((*ot).GetPointer());
            methodList.push_back(static_cast<ImageRenderMethodType*>((*ot).GetPointer()));
            }
          ot++;
          }
          break;
        }
      it++;
      }

    if(!m_Renderer)
      {
      std::cout << "No sovGlRendererSlicer renderer defined!" << std::endl;
      return 0;
      }

    int val = 0;
    std::list<ImageRenderMethodType *>::iterator methodIt = methodList.begin();
    std::list<ImageRenderMethodType *>::iterator methodEnd = methodList.end();

    switch(key)
      {
          case '0':
            m_Renderer->SetOrientation(0);
            if( m_OrientationCallback ) m_OrientationCallback(0);
            val = 1;
            break;

          case '1':
            m_Renderer->SetOrientation(1);
            if( m_OrientationCallback ) m_OrientationCallback(1);
            val = 1;
            break;

          case '2':
            m_Renderer->SetOrientation(2);
            if( m_OrientationCallback ) m_OrientationCallback(2);
            val = 1;
            break;

          case '<':
          case ',':       
            if( m_Renderer->GetSliceNumber() > 0 )
              {
              m_Renderer->SetSliceNumber(m_Renderer->GetSliceNumber()-1);
              if( m_SliceCallback ) m_SliceCallback(m_Renderer->GetSliceNumber());
              val = 1;
              }
            break;

          case '>':
          case '.':
            m_Renderer->SetSliceNumber(m_Renderer->GetSliceNumber()+1);
            if( m_SliceCallback ) m_SliceCallback(m_Renderer->GetSliceNumber());
            val = 1;
            break;

          case 'r':
            while(methodIt != methodEnd)
              {
              (*methodIt)->Reset();
              methodIt++;
              }
            val = 1;
            break;

          case '+':
          case '=':
            while(methodIt != methodEnd)
              {
              (*methodIt)->SetWinZoom((*methodIt)->GetWinZoom()*2.0);
              methodIt++;
              }
            val = 1;
            break;

          case '-':
          case '_':
            while(methodIt != methodEnd)
              {
              (*methodIt)->SetWinZoom((*methodIt)->GetWinZoom()/2.0);
              methodIt++;
              }
            val = 1;
            break;
          case 'x':
            m_Renderer->FlipX(!m_Renderer->FlipX());
            /*
            while(methodIt != methodEnd)
              {
              (*methodIt)->FlipX(!(*methodIt)->GetFlipX(0));
              methodIt++;
              }
            */
            val = 1;
            break;
          case 'y':
            m_Renderer->FlipY(!m_Renderer->FlipY());
            /*
            while(methodIt != methodEnd)
              {
              (*methodIt)->FlipY(!(*methodIt)->GetFlipY(0));
              methodIt++;
              }
            */
            val = 1;
            break;
          case 'z':
            //m_Renderer->FlipZ(!m_Renderer->FlipZ());
            /*
            while(methodIt != methodEnd)
              {
              (*methodIt)->FlipZ(!(*methodIt)->GetFlipZ(0));
              methodIt++;
              }
            */
            val = 1;
            break;
        /* case 'e':
            if(iwModeMax() == IW_FLIP) 
              {
              iwModeMax(IW_MAX);
              this->update();
              }
            else 
              {
              iwModeMax(IW_FLIP);
              this->update();
              }
            return 1;
            break;
          case 'd':
            if(iwModeMin() == IW_FLIP) 
              {
              iwModeMin(IW_MIN);
              this->update();
              }
            else 
              {
              iwModeMin(IW_FLIP);
              this->update();
              }
            return 1;
            break;*/
          case 'l':
            /*
            switch(m_ImageRenderMethod->GetImageMode()) 
              {
              default:
              case sovIMG_VAL:
                m_ImageRenderMethod->SetImageMode(sovIMG_INV);     
                break;
              case sovIMG_INV:
                m_ImageRenderMethod->SetImageMode(sovIMG_LOG);
                break;
              case sovIMG_LOG:
                m_ImageRenderMethod->SetImageMode(sovIMG_DX);
                break;
              case sovIMG_DX:
                m_ImageRenderMethod->SetImageMode(sovIMG_DY);
                break;
              case sovIMG_DY:
                m_ImageRenderMethod->SetImageMode(sovIMG_DZ);
                break;
              case sovIMG_DZ:
                m_ImageRenderMethod->SetImageMode(sovIMG_BLEND);
                break;
              case sovIMG_BLEND:
                m_ImageRenderMethod->SetImageMode(sovIMG_MIP);
                break;
              case sovIMG_MIP:
                m_ImageRenderMethod->SetImageMode(sovIMG_VAL);
                break;
              }
            */
            val = 1;
            break;

          case 'q':
            while(methodIt != methodEnd)
              {
              (*methodIt)->SetMaximumIntensity((*methodIt)->GetMaximumIntensity()
                  -(float)0.02*((*methodIt)->GetDataMax()-(*methodIt)->GetDataMin()));
              methodIt++;
              }
            //if(m_ImageRenderMethod)
              //{
              //m_ImageRenderMethod->SetMaximumIntensity(m_ImageRenderMethod->GetMaximumIntensity()
              //    -(float)0.02*(m_ImageRenderMethod->GetDataMax()-m_ImageRenderMethod->GetDataMin()));
              //}
            val = 1;
            break;

          case 'w':
            while(methodIt != methodEnd)
              {
              (*methodIt)->SetMaximumIntensity((*methodIt)->GetMaximumIntensity()
                  +(float)0.02*((*methodIt)->GetDataMax()-(*methodIt)->GetDataMin()));
              methodIt++;
              }
            //m_ImageRenderMethod->SetMaximumIntensity(m_ImageRenderMethod->GetMaximumIntensity()
            //      +(float)0.02*(m_ImageRenderMethod->GetDataMax()-m_ImageRenderMethod->GetDataMin()));
            val = 1;
            break;

          case 'a':
            while(methodIt != methodEnd)
              {
              (*methodIt)->SetMinimumIntensity((*methodIt)->GetMinimumIntensity()
                  -(float)0.02*((*methodIt)->GetDataMax()-(*methodIt)->GetDataMin()));
              methodIt++;
              }
            //m_ImageRenderMethod->SetMinimumIntensity(m_ImageRenderMethod->GetMinimumIntensity()
            //      -(float)0.02*(m_ImageRenderMethod->GetDataMax()-m_ImageRenderMethod->GetDataMin()));
            val = 1;
            break;

          case 's':
            while(methodIt != methodEnd)
              {
              (*methodIt)->SetMinimumIntensity((*methodIt)->GetMinimumIntensity()
                  +(float)0.02*((*methodIt)->GetDataMax()-(*methodIt)->GetDataMin()));
              methodIt++;
              }
            //m_ImageRenderMethod->SetMinimumIntensity(m_ImageRenderMethod->GetMinimumIntensity()
            //      +(float)0.02*(m_ImageRenderMethod->GetDataMax()-m_ImageRenderMethod->GetDataMin()));
            val = 1;
            break;
    /*     case 'i':
            {
            int newY;
            if(cFlipY[cWinOrientation])
              newY = cWinCenter[cWinOrder[1]]-imgShiftSize;
            else
              newY = cWinCenter[cWinOrder[1]]+imgShiftSize;
            cWinCenter[cWinOrder[1]] = newY;
            winCenter(cWinCenter[0], cWinCenter[1], cWinCenter[2]);
            this->update();
            return 1;
            break;
            }
          case 'm':
            {
            int newY;
            if(cFlipY[cWinOrientation])
              newY = cWinCenter[cWinOrder[1]]+imgShiftSize;
            else
              newY = cWinCenter[cWinOrder[1]]-imgShiftSize;
            cWinCenter[cWinOrder[1]] = newY;
            winCenter(cWinCenter[0], cWinCenter[1], cWinCenter[2]);
            this->update();
            return 1;
            break;
            }
          case 'j':
            {
            int newX;
            if(cFlipX[cWinOrientation])
              newX = cWinCenter[cWinOrder[0]]+imgShiftSize;
            else
              newX = cWinCenter[cWinOrder[0]]-imgShiftSize;
            cWinCenter[cWinOrder[0]] = newX;
            winCenter(cWinCenter[0], cWinCenter[1], cWinCenter[2]);
            this->update();
            return 1;
            break;
            }
          case 'k':
            {
            int newX;
            if(cFlipX[cWinOrientation])
              newX = cWinCenter[cWinOrder[0]]-imgShiftSize;
            else
              newX = cWinCenter[cWinOrder[0]]+imgShiftSize;
            cWinCenter[cWinOrder[0]] = newX;
            winCenter(cWinCenter[0], cWinCenter[1], cWinCenter[2]);
            this->update();
            return 1;
            break;
            }
          case 't':
            Transpose(!cTranspose[cWinOrientation]);
            this->update();
            return 1;
            break;*/

          /*
          case 'A':
            m_ImageRenderMethod->SetViewAxisLabel(!m_ImageRenderMethod->GetViewAxisLabel()); 
            val = 1; 
            break;

          case 'C':
            m_ImageRenderMethod->SetViewCrosshairs(!m_ImageRenderMethod->GetViewCrosshairs());   
            val = 1;
            break;

          case 'V':
            m_ImageRenderMethod->SetViewValue(!m_ImageRenderMethod->GetViewValue());   
            val = 1;
            break;

          case 'D':
            m_ImageRenderMethod->SetViewDetails(!m_ImageRenderMethod->GetViewDetails());   
            val = 1;
            break;

          case 'O':
            m_ImageRenderMethod->SetViewOverlayData(!m_ImageRenderMethod->GetViewOverlayData());   
            val = 1;
            break;
          */
          
          default: 
            return 0;
  }

  if(val)
    {
    m_Renderer->Update();
    if(m_Display)
      {
      static_cast<Display*>(m_Display)->Redraw();
      }
    else
      {
      std::cout << "sovGlSlicerKeyboardInteractor: WARNING Display not set!" << std::endl;
      }
    }

  return val;

  }

  void 
  GlSlicerKeyboardInteractor
  ::SetOrientationCallback(void(*callback)(unsigned int))
    {
    m_OrientationCallback = callback;
    }

  void 
  GlSlicerKeyboardInteractor
  ::SetSliceCallback(void(*callback)(unsigned int))
    {
    m_SliceCallback = callback;
    }

}//end of namespace sov
