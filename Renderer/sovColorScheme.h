#if !defined __ColorScheme_h
#define __ColorScheme_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4284 )
#pragma warning ( disable : 4018 )
#endif

#include <sovObjectViewerWin32Header.h>
#include <vtkPolyDataMapper.h>
#include <vtkLookupTable.h>
#include <vtkColorTransferFunction.h>
#include <assert.h>
#include <sovColorSchemeXML.h>


namespace sov
{
  class SOViewer_EXPORT ColorScheme  
  {
  public:
    void Update (vtkPolyDataMapper *mapper);
    
    ColorScheme();
    virtual ~ColorScheme();
    
    enum colorMode {SOLID, SIG_MAP, DIST_MAP} ;
    
    void SetThreshold (int index, float value) ;
    void SetColorHSV (int index, float hue, float sat, float val) ;
    //void SetColorRGB (int index, float r, float g, float b) ;
    void SetMode (enum colorMode newMode) ;
    enum colorMode GetMode () ;
    float GetThreshold (int index=0) ;
    void GetColor (int colorMode, int index, float &h, float &s, float &v) ;
    
    void Load (const char *filename) ;
    void Save (const char *filename) ;
  
  private:
    enum colorMode m_ColorSchemeType ;
    
    struct hsv {float h; float s; float v;} ;
    
    struct   {
      float t ;
      struct hsv color[3] ;
    } m_SigMapSettings ;
    
    struct   {
      float t[2];
      struct hsv color[2] ;
    } m_DistMapSettings ;
    
    hsv m_SolidSettings ;
    
    void HSV2RGB ( struct hsv hsv_value, double &r, double &g, double &b ) ;
    
    void MakeDistanceMap (vtkPolyDataMapper *mapper);
    //void MakeSignificanceFunction(vtkPolyDataMapper *mapper);
    void MakeSignificanceMap(vtkPolyDataMapper *mapper);
  };
}

#endif // !defined __ColorScheme_h

