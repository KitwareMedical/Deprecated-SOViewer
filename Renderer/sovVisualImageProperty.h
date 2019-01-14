/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVisualImageProperty.h,v $
  Language:  C++
  Date:      $Date: 2005-09-19 20:29:23 $
  Version:   $Revision: 1.2 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __VisualImageProperty_h
#define __VisualImageProperty_h

#include "sovVisualObjectProperty.h"


namespace sov
{

class VisualImageProperty
: public VisualObjectProperty
{

public:

  typedef enum
    {
    GRAY = 1,
    COLD,
    WARM,
    SPECTRUM
    } LookupTableType;

  typedef enum
    {
    DEFAULT_MODE,
    TRANSPARENT_MODE,
    OVERLAY_MODE
    } DrawMode;

  VisualImageProperty()
  {
    m_Orientation = 0;
    m_MinimalAlphaValue = -1; // not used by default

    for(unsigned int i=0;i<3;i++)
    {
      m_Opacity[i] = 1;
    }
    m_PixelType="";
    m_NumberOfBins = 100;
    m_IntensityRange[0] = 0;
    m_IntensityRange[1] = 100;
    m_UseIntensityRange = false;
    m_FrequencyRange[0] = 0;
    m_FrequencyRange[1] = 1000;
    m_FrequencyScale = 1.0;
    m_IsoFrequency = 500;
    m_IsoFrequencyVisible = false;
    m_SmoothingIterations = 20;
    m_SmoothingFactor = 0.1;
    m_HueRange[0] = 0.0;
    m_HueRange[1] = 0.0;
    m_AlphaRange[0] = 1.0;
    m_AlphaRange[1] = 1.0;
    m_SaturationRange[0] = 0.0;
    m_SaturationRange[1] = 0.0;
    
    m_Tolerance = 0.1;
    m_Decimation = 0.2;
    m_Threshold = 100;
    m_PreserveTopology = false;
    m_SurfaceModified = true;
    m_StandardDeviation = 2.0;
    m_SurfaceColor[0] = 1.0;
    m_SurfaceColor[1] = 0.0;
    m_SurfaceColor[2] = 0.0;

    m_DrawMode = DEFAULT_MODE;

    m_Color[0] = 1.0;
    m_Color[1] = 1.0;
    m_Color[2] = 1.0;

    itk::Point<float,4> rgb1;
    rgb1[0] = 0;
    rgb1[1] = 1;
    rgb1[2] = 0;
    rgb1[3] = 0;
    
    itk::Point<float,4> rgb2;
    rgb2[0] = 255;
    rgb2[1] = 0;
    rgb2[2] = 0;
    rgb2[3] = 1;

    m_RGBPoint.push_back(rgb1);
    m_RGBPoint.push_back(rgb2);

    itk::Point<float,2> opacity1;
    opacity1[0] = 0;
    opacity1[1] = 0;

    itk::Point<float,2> opacity2;
    opacity2[0] = 255;
    opacity2[1] = 1;

    m_OpacityPoint.push_back(opacity1);
    m_OpacityPoint.push_back(opacity2);
  }

  ~VisualImageProperty(){};

  void SetOrientation(unsigned int orientation)
  {
    m_Orientation = orientation;
  }

  const char* GetPixelType()
    {
    return m_PixelType.c_str();
    }

  void SetPixelType(std::string type)
    {
    m_PixelType = type;
    }

  unsigned int GetOrientation(void)
  {
    return m_Orientation;
  }

  void SetOpacity(unsigned int orientation, double opacity)
  {
    m_Opacity[orientation] = opacity;
  }

  double GetOpacity(unsigned int orientation)
  {
    return m_Opacity[orientation];
  }

  void SetNumberOfBins( unsigned int n )
  {
    if( this->m_NumberOfBins != n )
      {
      this->m_NumberOfBins = n;
      this->AskRenderMethodToBeUpdated();
      this->Modified();
      }
  }
  
  unsigned int GetNumberOfBins()
  {
    return this->m_NumberOfBins;
  }

  void SetUseIntensityRange(bool val)
    {
    m_UseIntensityRange = val;
    }

  bool GetUseIntensityRange()
    {
    return m_UseIntensityRange;
    }

  void SetIntensityRange( double min, double max )
  {
    if( min != this->m_IntensityRange[0]
    ||  max != this->m_IntensityRange[1] )
      {
      this->m_IntensityRange[0] = min;
      this->m_IntensityRange[1] = max;
      this->AskRenderMethodToBeUpdated();
      this->Modified();
      }
  }
  
  const double * GetIntensityRange() const
  {
    return this->m_IntensityRange;
  }

  void SetFrequencyRange( unsigned int min, unsigned int max )
  {
    if( min != this->m_FrequencyRange[0]
    ||  max != this->m_FrequencyRange[1] )
      {
      this->m_FrequencyRange[0] = min;
      this->m_FrequencyRange[1] = max;
      this->AskRenderMethodToBeUpdated();
      this->Modified();
      }
  }
  
  const unsigned int * GetFrequencyRange() const
  {
    return this->m_FrequencyRange;
  }
  
  void SetHueRange( float min, float max )
  {
    if(   this->m_HueRange[0] != min 
      ||  this->m_HueRange[1] != max )
      {
      this->m_HueRange[0] = min;
      this->m_HueRange[1] = max;
      this->Modified();
      }
  }
  
  const float * GetHueRange() const
  {
    return this->m_HueRange;
  }

  void SetAlphaRange( float min, float max )
  {
    if(   this->m_AlphaRange[0] != min 
      ||  this->m_AlphaRange[1] != max )
      {
      this->m_AlphaRange[0] = min;
      this->m_AlphaRange[1] = max;
      this->Modified();
      }
  }
  
  const float * GetAlphaRange() const
  {
    return this->m_AlphaRange;
  }

  void SetSaturationRange( float min, float max )
  {
    if(   this->m_SaturationRange[0] != min 
      ||  this->m_SaturationRange[1] != max )
      {
      this->m_SaturationRange[0] = min;
      this->m_SaturationRange[1] = max;
      this->Modified();
      }
  }
  
  const float * GetSaturationRange() const
  {
    return this->m_SaturationRange;
  }


  void SetSmoothingIterations(unsigned int n)
  {
    if( n != this->m_SmoothingIterations )
      {
      this->m_SmoothingIterations = n;
      this->AskRenderMethodToBeUpdated();
      }
  }

  unsigned int GetSmoothingIterations() const
  {
    return this->m_SmoothingIterations;
  }

  void SetSmoothingFactor(float n)
  {
    if( n != this->m_SmoothingFactor )
      {
      this->m_SmoothingFactor = n;
      this->AskRenderMethodToBeUpdated();
      }
  }

  float GetSmoothingFactor() const
  {
    return this->m_SmoothingFactor;
  }

  void SetFrequencyScale(float n)
  {
    if( this->m_FrequencyScale != n )
      {
      this->m_FrequencyScale = n;
      this->Modified();
      }
  }

  float GetFrequencyScale() const
  {
    return this->m_FrequencyScale;
  }

  void SetIsoFrequency(unsigned int n)
  {
    if( this->m_IsoFrequency != n )
      {
      this->m_IsoFrequency = n;
      this->Modified();
      }
  }

  unsigned int GetIsoFrequency() const
  {
    return m_IsoFrequency;
  }

  void IsoFrequencyOn(void)
  {
    if( this->m_IsoFrequencyVisible != true )
      {
      this->m_IsoFrequencyVisible = true;
      this->Modified();
      }
  }

  void IsoFrequencyOff(void)
    {
    if( this->m_IsoFrequencyVisible != false )
      {
      m_IsoFrequencyVisible = false;
      this->Modified();
      }
    }

  void IsoFrequency(bool n)
    {
    if( this->m_IsoFrequencyVisible != n )
      {
      this->m_IsoFrequencyVisible = n;
      this->Modified();
      }
    }

  bool IsoFrequency(void)
    {
    if( m_IsoFrequencyVisible )
      {
      return true;
      }
    return false;
    }

  void AddRGBPoint( float value, float r, float g, float b )
    {
    float point[4];
    point[0] = value;
    point[1] = r;
    point[2] = g;
    point[3] = b;

    this->m_RGBPoint.push_back(point);
    }

  void AddRGBPoint( float * point )
    {
    this->m_RGBPoint.push_back(point);
    }

  std::list< itk::Point<float,4> > GetRGBPoints()
    {
    return this->m_RGBPoint;
    }

  void ClearRGBPoints()
    {
    this->m_RGBPoint.clear();
    }

  void AddOpacityPoint( float value, float opacity )
    {
    float point[2];
    point[0] = value;
    point[1] = opacity;

    this->m_OpacityPoint.push_back(point);
    }

  void AddOpacityPoint( float * point )
    {
    this->m_OpacityPoint.push_back(point);
    }

  std::list< itk::Point<float,2> > GetOpacityPoints()
    {
    return this->m_OpacityPoint;
    }

  void ClearOpacityPoints()
    {
    this->m_OpacityPoint.clear();
    }

  void SetDecimation( float decimation )
    {
    if( this->m_Decimation != decimation )
      {
      this->m_Decimation = decimation;
      this->AskRenderMethodToBeUpdated();
      //this->m_SurfaceModified = true;
      }
    }

  float GetDecimation() const
    {
    return this->m_Decimation;
    }


  void SetThreshold( float threshold )
    {
    if( this->m_Threshold != threshold )
      {
      this->m_Threshold = threshold;
      this->AskRenderMethodToBeUpdated();
      //this->m_SurfaceModified = true;
      }
    }

  float GetThreshold() const
    {
    return this->m_Threshold;
    }

  void SetTolerance( float tolerance )
    {
    if( this->m_Tolerance != tolerance )
      {
      this->m_Tolerance = tolerance;
      this->AskRenderMethodToBeUpdated();
      //this->m_SurfaceModified = true;
      }
    }

  float GetTolerance() const
    {
    return this->m_Tolerance;
    }

  void SetStandardDeviation( float dev )
    {
    if( this->m_StandardDeviation != dev )
      {
      this->m_StandardDeviation = dev;
      this->AskRenderMethodToBeUpdated();
      //this->m_SurfaceModified = true;
      }
    }

  float GetStandardDeviation() const
    {
    return this->m_StandardDeviation;
    }

  void SetPreserveTopology( bool value )
    {
    if( this->m_PreserveTopology != value )
      {
      this->m_PreserveTopology = value;
      this->AskRenderMethodToBeUpdated();
      //this->m_SurfaceModified = true;
      }
    }

  bool GetPreserveTopology() const
    {
    return this->m_PreserveTopology;
    }

  void SetColor( float * color )
    {
    this->m_Color[0] = color[0];
    this->m_Color[1] = color[1];
    this->m_Color[2] = color[2];
    }

  float * GetColor()
    {
    return this->m_Color;
    }

  void SetSurfaceColor( float * color )
    {
    if( this->m_SurfaceColor[0] != color[0]
    ||  this->m_SurfaceColor[1] != color[1]
    ||  this->m_SurfaceColor[2] != color[2] )
      {
      this->m_SurfaceColor[0] = color[0];
      this->m_SurfaceColor[1] = color[1];
      this->m_SurfaceColor[2] = color[2];
      this->Modified();
      }
    }

  float * GetSurfaceColor()
    {
    return this->m_SurfaceColor;
    }

  void SetSurfaceModified( bool value )
    {
    this->m_SurfaceModified = value;
    }

  bool GetSurfaceModified() const
    {
    return this->m_SurfaceModified;
    }

  void SetTableLookupToGray()
    {
    this->m_HueRange[0] = 0.0;
    this->m_HueRange[1] = 0.0;
    this->m_SaturationRange[0] = 0.0;
    this->m_SaturationRange[1] = 0.0;
    this->m_AlphaRange[0] = 1.0;
    this->m_AlphaRange[1] = 1.0;
    this->Modified();
    }

  void SetTableLookupToCold()
    {
    this->m_HueRange[0] = 0.5;
    this->m_HueRange[1] = 0.6667;
    this->m_SaturationRange[0] = 1.0;
    this->m_SaturationRange[1] = 1.0;
    this->m_AlphaRange[0] = 1.0;
    this->m_AlphaRange[0] = 1.0;
    this->Modified();
    }

  void SetTableLookupToWarm()
    {
    this->m_HueRange[0] = 0.0;
    this->m_HueRange[1] = 0.3333;
    this->m_SaturationRange[0] = 1.0;
    this->m_SaturationRange[1] = 1.0;
    this->m_AlphaRange[0] = 1.0;
    this->m_AlphaRange[0] = 1.0;
    this->Modified();
    }

  void SetTableLookupToSpectrum()
    {
    this->m_HueRange[0] = 0.0;
    this->m_HueRange[1] = 1.0;
    this->m_SaturationRange[0] = 1.0;
    this->m_SaturationRange[1] = 1.0;
    this->m_AlphaRange[0] = 1.0;
    this->m_AlphaRange[0] = 1.0;
    this->Modified();
    }

  void SetDrawMode( DrawMode mode )
    {
    m_DrawMode = mode;
    }

  void SetDrawModeToDefault()
    { 
    m_DrawMode = DEFAULT_MODE;
    }  

  void SetDrawModeToOverlay()
    { 
    m_DrawMode = OVERLAY_MODE;
    }

  void SetDrawModeToTransparent()
    { 
    m_DrawMode = TRANSPARENT_MODE;
    }

  DrawMode GetDrawMode() const
    {
    return m_DrawMode;
    }

  float GetMinimalAlphaValue()
    {
    return m_MinimalAlphaValue;
    }

  void SetMinimalAlphaValue(float alpha)
    {
    m_MinimalAlphaValue = alpha;
    this->Modified();
    }

  void SetTableLookupType( LookupTableType type )
    {
    switch(type)
      {
      case GRAY:
        this->m_HueRange[0] = 0.0;
        this->m_HueRange[1] = 1.0;
        this->m_SaturationRange[0] = 0.0;
        this->m_SaturationRange[1] = 0.0;
        this->m_AlphaRange[0] = 1.0;
        this->m_AlphaRange[0] = 1.0;
        break;

      case COLD:
        this->m_HueRange[0] = 0.5000;
        this->m_HueRange[1] = 0.6667;
        this->m_SaturationRange[0] = 1.0;
        this->m_SaturationRange[1] = 1.0;
        this->m_AlphaRange[0] = 1.0;
        this->m_AlphaRange[0] = 1.0;
        break;

      case WARM:
        this->m_HueRange[0] = 0.0;
        this->m_HueRange[1] = 0.3333;
        this->m_SaturationRange[0] = 1.0;
        this->m_SaturationRange[1] = 1.0;
        this->m_AlphaRange[0] = 1.0;
        this->m_AlphaRange[0] = 1.0;
        break;

      case SPECTRUM:
        this->m_HueRange[0] = 0.0;
        this->m_HueRange[1] = 1.0;
        this->m_SaturationRange[0] = 1.0;
        this->m_SaturationRange[1] = 1.0;
        this->m_AlphaRange[0] = 1.0;
        this->m_AlphaRange[0] = 1.0;
        break;
      }
    this->Modified();
    }

protected:

  unsigned int  m_Orientation;
  double        m_Opacity[3];
  std::string   m_PixelType;
  
  float         m_HueRange[2];
  float         m_AlphaRange[2];
  float         m_SaturationRange[2];
  
  float         m_Color[3];  
  DrawMode      m_DrawMode;

  // the following properties are used by the 
  // ImageSurfaceVTKRenderMethod3D

  float        m_Tolerance;
  float        m_Decimation;
  float        m_Threshold;
  bool         m_PreserveTopology;
  float        m_StandardDeviation;
  bool         m_SurfaceModified;
  float        m_SurfaceColor[3];
  

  // This values defines the alpha value to render the minimum value of the image
  float m_MinimalAlphaValue;

  // the following properties are used by the 
  // ImageRayCastVTKRenderMethod3D

  std::list< itk::Point<float,4> > m_RGBPoint;      //RGB piecewise function point list
  std::list< itk::Point<float,2> > m_OpacityPoint;  //opacity piecewise function point list

  // the following properties are used by the
  // ImageHistogramVTKRenderMethod3D....

  unsigned int  m_NumberOfBins;         //number of bins generated for the histogram
  double        m_IntensityRange[2];    //intensity range of the histogram
  unsigned int  m_IsoFrequency;         //frequency of the iso-level
  bool          m_IsoFrequencyVisible;  //visibility of the frequency iso-level
  unsigned int  m_FrequencyRange[2];    //frequency range of the histogram
  float         m_FrequencyScale;       //scale the frequency axis of the histogram
  unsigned int  m_SmoothingIterations;  //number of iterations for the smoothing of the histogram
  float         m_SmoothingFactor;      //strength of the smoothing of the histogram
  bool          m_UseIntensityRange; // should we use the user defined intensity range?
};

} //end of namespace sov

#endif //__sovVisualImageProperty_h
