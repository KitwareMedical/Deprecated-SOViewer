#include <math.h>
#include "sovColorScheme.h"

namespace sov
{

  ColorScheme::ColorScheme()
  {
    m_ColorSchemeType = SIG_MAP ;
    
    // the default values for significance map settings
    m_SigMapSettings.t = 0.05 ;
    m_SigMapSettings.color[0].h = 0 ;
    m_SigMapSettings.color[0].s = 1 ;
    m_SigMapSettings.color[0].v = 0.5 ;
    m_SigMapSettings.color[1].h = 0.33 ;
    m_SigMapSettings.color[1].s = 1 ;
    m_SigMapSettings.color[1].v = 0.5 ;
    m_SigMapSettings.color[2].h = 0.67 ;
    m_SigMapSettings.color[2].s = 1 ;
    m_SigMapSettings.color[2].v = 0.5 ;

    // the default values for distance map settings
    m_DistMapSettings.t[0] = -400 ;
    m_DistMapSettings.t[1] = 400 ;
    m_DistMapSettings.color[0].h = 0 ;
    m_DistMapSettings.color[0].s = 1 ;
    m_DistMapSettings.color[0].v = 0.5 ;
    m_DistMapSettings.color[1].h = 0.5 ;
    m_DistMapSettings.color[1].s = 1 ;
    m_DistMapSettings.color[1].v = 0.5 ;

    // default values for solid color 
    m_SolidSettings.h = 1 ;
    m_SolidSettings.s = 1 ;
    m_SolidSettings.v = 0.5 ;
  }
  
  ColorScheme::~ColorScheme()
  {
    
  }
  
  void ColorScheme::MakeSignificanceMap(vtkPolyDataMapper *mapper)
  {
    vtkLookupTable *lut = vtkLookupTable::New () ;
    
    lut->SetHueRange (m_SigMapSettings.color[0].h, m_SigMapSettings.color[1].h) ;
    lut->SetSaturationRange (m_SigMapSettings.color[0].s, m_SigMapSettings.color[1].s) ;
    lut->SetValueRange (m_SigMapSettings.color[0].v, m_SigMapSettings.color[1].v) ;
    lut->SetTableRange (0.0, m_SigMapSettings.t) ;
    lut->SetRampToLinear () ;
    
    lut->Build() ;
    int n = lut->GetNumberOfColors() ;
    
    double red, green, blue ;
    HSV2RGB ( m_SigMapSettings.color[2], red, green, blue ) ;
    lut->SetTableValue (n-1, red, green, blue ) ;
    
    mapper->SetColorModeToMapScalars () ;
    mapper->SetLookupTable (lut) ;
    mapper->SetScalarRange(0, m_SigMapSettings.t) ;
  }
  
  void ColorScheme::MakeDistanceMap(vtkPolyDataMapper *mapper)
  { 
    vtkLookupTable *lut = vtkLookupTable::New () ;
    
    lut->SetHueRange (m_DistMapSettings.color[0].h, m_DistMapSettings.color[1].h) ;
    lut->SetSaturationRange (m_DistMapSettings.color[0].s, m_DistMapSettings.color[1].s) ;
    lut->SetValueRange (m_DistMapSettings.color[0].v, m_DistMapSettings.color[1].v) ;
    lut->SetTableRange (m_DistMapSettings.t[0], m_DistMapSettings.t[1]) ;
    lut->SetRampToLinear () ;
    
    lut->Build() ;
    
    mapper->SetColorModeToMapScalars () ;
    mapper->SetLookupTable (lut) ;
    mapper->SetScalarRange(m_DistMapSettings.t[0], m_DistMapSettings.t[1]) ;
  }
  
  void ColorScheme::SetThreshold (int index, float value) 
  {
    if (( m_ColorSchemeType == SIG_MAP ) && ( index == 0 ))
    {
      m_SigMapSettings.t = value ;
    }
    
    else if (( m_ColorSchemeType == DIST_MAP ) && ( index >= 0 ) && ( index < 2 ) )
    {
      m_DistMapSettings.t[index] = value ;
    }
  }
  
  void ColorScheme::SetColorHSV (int index, float hue, float sat, float val) 
  {
    if (( m_ColorSchemeType == SIG_MAP ) && ( index >= 0 ) && ( index < 3 ) )
    {
      m_SigMapSettings.color[index].h = hue ;
      m_SigMapSettings.color[index].s = sat ;
      m_SigMapSettings.color[index].v = val ;
    }
    
    else if (( m_ColorSchemeType == DIST_MAP ) && ( index >= 0 ) && ( index < 2 ) )
    {
      m_DistMapSettings.color[index].h = hue ;
      m_DistMapSettings.color[index].s = sat ;
      m_DistMapSettings.color[index].v = val ;
    }
    
    else if ( m_ColorSchemeType == SOLID ) 
    {
      m_SolidSettings.h = hue ;
      m_SolidSettings.s = sat ;
      m_SolidSettings.v = val ;
    }
  }
  
  /*void ColorScheme::SetColorRGB (int index, float r, float g, float b) 
  {
    
  }*/
  
  void ColorScheme::SetMode (enum colorMode newMode) 
  {
    m_ColorSchemeType = newMode ;
  }

  enum ColorScheme::colorMode ColorScheme::GetMode () 
  {
    return m_ColorSchemeType ;
  }
  
  void ColorScheme::HSV2RGB ( struct hsv hsv_value, double &r, double &g, double &b ) 
  {
    int i;
    double f, p, q, t;
    double hue = hsv_value.h * 6 ;
    double  s = hsv_value.s ;
    double  v = hsv_value.v ;
    
    //printf("%f %f %f\n", hue, s, v);
    if( s == 0 ) {
      // achromatic (grey)
      r = g = b = v;
      return;
    }
    
    i = (int)floor( hue );
    f = hue - i;      // factorial part of h
    p = v * ( 1 - s );
    q = v * ( 1 - s * f );
    t = v * ( 1 - s * ( 1 - f ) );
    
    switch( i ) {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    default:    // case 5:
      r = v;
      g = p;
      b = q;
      break;
    }
    
    //printf("%f %f %f\n", r, g, b);
  }
  
  void ColorScheme::Update (vtkPolyDataMapper *mapper)
  {
    if ( m_ColorSchemeType == SIG_MAP ) 
      MakeSignificanceMap ( mapper ) ;
    else if ( m_ColorSchemeType == DIST_MAP )
      MakeDistanceMap ( mapper ) ;
  }
  
  /*void ColorScheme::MakeSignificanceFunction(vtkPolyDataMapper *mapper)
  {
  // not used any more 
  vtkColorTransferFunction *lut = vtkColorTransferFunction::New () ;
  lut->AddHSVSegment (0.0, 0, 1, 1, 0.05, 0.25, 1, 1) ;
  lut->AddHSVSegment (0.05, 0.6, 1, 0.5, 1, 0.6, 1, 0.5) ;
  
    mapper->SetColorModeToMapScalars () ;
    mapper->SetLookupTable (lut) ;
    }
  */
  float ColorScheme::GetThreshold (int index)   
  {
    if ( m_ColorSchemeType == SIG_MAP )
    {
      return m_SigMapSettings.t ;
    }
    else if ( m_ColorSchemeType == DIST_MAP )
      return m_DistMapSettings.t[index] ;
    else
      return 0 ;
  }

  
  void ColorScheme::GetColor (int colorMode, int index, float &h, float &s, float &v)
  {
    if ( colorMode == 1 )
    {
      h = m_SigMapSettings.color[index].h ;
      s = m_SigMapSettings.color[index].s ;
      v = m_SigMapSettings.color[index].v ;
    }
    else if ( colorMode == 2 )
    {
      h = m_DistMapSettings.color[index].h ;
      s = m_DistMapSettings.color[index].s ;
      v = m_DistMapSettings.color[index].v ;
    }
    else 
    {
      h = m_SolidSettings.h ;
      s = m_SolidSettings.s ;
      v = m_SolidSettings.v ;
    }
  }

  void ColorScheme::Load (const char *filename)
  {
    ColorSchemeXMLReader *pReader ;
    pReader = new ColorSchemeXMLReader ;
    pReader->SetFilename (filename) ;
    pReader->SetObject ( this ) ;
    pReader->GenerateOutputInformation () ;
  }

  void ColorScheme::Save (const char *filename)
  {
    ColorSchemeXMLWriter *pWriter ;
    pWriter = new ColorSchemeXMLWriter ;
    pWriter->SetFilename (filename) ;
    pWriter->SetObject (this ) ;
    
    pWriter->WriteFile () ;
  }
}

