/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovImageGlRenderMethodSlicer.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ImageGlRenderMethodSlicer_h
#define _ImageGlRenderMethodSlicer_h

#include "sovGlRenderMethodSlicer.h"
#include "itkImage.h"
#include "itkColorTable.h"
#include "itkImageSpatialObject.h"
#include "OpenGLSliceTexture.h"


#include <math.h>

const int sovNUM_ClickModeTypes = 3;
typedef enum {sovCM_NOP, sovCM_SELECT, sovCM_BOX} sovClickModeType;
const char sovClickModeTypeName[3][7] =
  {{'N', 'O', 'P', '\0', ' ', ' ', ' '},
  {'S', 'e', 'l', 'e', 'c', 't', '\0'},
  {'B', 'o', 'x', '\0', ' ', ' ', ' '}};

const int sovNUM_ImageModeTypes = 8;
typedef enum {sovIMG_VAL, sovIMG_INV, sovIMG_LOG, sovIMG_DX, sovIMG_DY, sovIMG_DZ,
  sovIMG_BLEND, sovIMG_MIP} sovImageModeType;
const char sovImageModeTypeName[8][8] =
  {{'V', 'a', 'l', 'u', 'e', '\0', ' ', ' '},
  {'I', 'n', 'v', 'e', 'r', 's', 'e', '\0'},
  {'L', 'o', 'g', '\0', ' ', ' ', ' ', ' '},
  {'D', 'e', 'r', 'i', 'v', '-', 'X', '\0'},
  {'D', 'e', 'r', 'i', 'v', '-', 'Y', '\0'},
  {'D', 'e', 'r', 'i', 'v', '-', 'Z', '\0'},
  {'B', 'l', 'e', 'n', 'd', '\0', ' ', ' '},
  {'M', 'I', 'P', '\0', ' ', ' ', ' ', ' '}};

const int sovNUM_IWModeTypes = 3;
typedef enum {sovIW_MIN, sovIW_MAX, sovIW_FLIP} sovIWModeType;
const char sovIWModeTypeName[3][5] =
  {{'M', 'i', 'n', '\0', ' '},
  {'M', 'a', 'x', '\0', ' '},
  {'F', 'l', 'i', 'p', '\0'}};

struct sovClickPoint 
  {
  float x, y, z;
  double value;
  
  sovClickPoint(float _x,float _y,float _z,double v)
    : x(_x),y(_y),z(_z),value(v){}
  };


namespace sov
{

template <class TImage>
class ImageGlRenderMethodSlicer
: public GlRenderMethodSlicer
{

  public:

  typedef TImage                           ImageType;
  typedef typename ImageType::PixelType    ImagePixelType;
  
  typedef unsigned char                    OverlayPixelType;
  typedef itk::Image<OverlayPixelType,3>   OverlayType;

  typedef itk::Image<ImagePixelType,2>     SliceType;
  typedef typename ImageType::ConstPointer ImagePointer;
  typedef typename OverlayType::Pointer    OverlayPointer;
  typedef typename ImageType::RegionType   RegionType;
  typedef typename ImageType::SizeType     SizeType;
  typedef typename ImageType::IndexType    IndexType;

  typedef ImageSpatialObject<3,ImagePixelType> ImageSpatialObjectType;
  
  typedef ImageGlRenderMethodSlicer        Self;
  typedef GlRenderMethodSlicer            Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;

  typedef Superclass::ObjectType      ObjectType;
  typedef Superclass::ObjectListType  ObjectListType;

  typedef OpenGLSliceTexture<ImagePixelType> GreyTextureType;

  itkNewMacro( ImageGlRenderMethodSlicer );
  itkTypeMacro( ImageGlRenderMethodSlicer, GlRenderMethodSlicer );

  virtual void draw( void );

  void SetMinimumIntensity(double min){cIWMin = min;}
  void SetMaximumIntensity(double max){cIWMax = max;}

  double GetMinimumIntensity(){return cIWMin;}
  double GetMaximumIntensity(){return cIWMax;}

  double GetDataMin() {return m_DataMin;}
  double GetDataMax() {return m_DataMax;}

  int GetWinCenter(unsigned int val) {return cWinCenter[val];}
  unsigned int GetWinOrder(unsigned int val) {return cWinOrder[val];}

  unsigned int GetOrientation() {return m_Orientation;}

  void SetZoom(double zoom){cWinZoom=zoom;}

  bool GetViewOverlayData(){return cViewOverlayData;}
  void SetViewOverlayData(bool val) {cViewOverlayData = val;}

  bool GetViewCrosshairs(){return cViewCrosshairs;}
  void SetViewCrosshairs(bool val) {cViewCrosshairs = val;}

  bool GetViewValue(){return cViewValue;}
  void SetViewValue(bool val) {cViewValue = val;}

  bool GetViewDetails(){return cViewDetails;}
  void SetViewDetails(bool val) {cViewDetails = val;}

  bool GetViewAxisLabel(){return cViewAxisLabel;}
  void SetViewAxisLabel(bool val) {cViewAxisLabel = val;}

  void SetWinZoom(float val){cWinZoom = val;}
  float GetWinZoom(){return cWinZoom;}

  void Reset()
    {
      cWinZoom = 1;
      cImageMode = sovIMG_VAL;
      cIWMin = m_DataMin;
      cIWMax = m_DataMax;
      cWinCenter[cWinOrder[0]] = m_DimSize[cWinOrder[0]]/2;
      cWinCenter[cWinOrder[1]] = m_DimSize[cWinOrder[1]]/2;
    }

  void SetImageMode(sovImageModeType mode) {cImageMode =  mode;}
  sovImageModeType GetImageMode() {return cImageMode;}

protected:

  ImageGlRenderMethodSlicer( void );
  ~ImageGlRenderMethodSlicer( void );

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();
  void DisplayText(int x, int y, const char *format);

private:

  unsigned long       m_DimSize[3];  // Actual dimensions of the image
  float               m_Spacing[3];  // Pixel spacing (units?)
  double              m_DataMax;
  double              m_DataMin;

  // The following was pre-itk code, hence the different prefix convention.
  int   cWinMinX;
  int   cWinMaxX;
  unsigned int   cWinSizeX;
  int   cWinMinY;
  int   cWinMaxY;
  unsigned int   cWinSizeY;
  int   cWinDataSizeX;
  int   cWinDataSizeY;
  unsigned int   inDataSizeX;
  unsigned int   inDataSizeY;
  unsigned char  *cWinImData;
  unsigned short *cWinZBuffer;
  float       cIWMin;
  float       cIWMax;
  sovIWModeType  cIWModeMin;
  sovIWModeType  cIWModeMax;
  sovImageModeType cImageMode;

  int         cWinCenter[3];
  void        (* cWinCenterCallBack)(void);
  void        * cWinCenterArg;
  void        (* cWinCenterArgCallBack)(void * winCenterArg);
    
  bool        cViewAxisLabel;
  char        cAxisLabelX[3][80];
  char        cAxisLabelY[3][80];
    
  bool        cViewOverlayData;
  bool        cViewCrosshairs;
  bool        cViewValue;
  bool        cViewDetails;

  float               cWinZoom;
  unsigned int        cWinOrder[3];
  //unsigned int        cWinOrientation;
    
  bool  cViewImData ;
  bool  cValidImData;
  bool  cValidOverlayData;
  void   (* cSliceNumCallBack)(void);
  void    * cSliceNumArg;
  void   (* cSliceNumArgCallBack)(void * sliceNumArg);
  void                    (* cViewImDataCallBack)(void);
  void                     * cViewImDataArg;
  void                    (* cViewImDataArgCallBack)(void *viewImDataArg);
    
  float         cClickSelect[3];
  float         cClickSelectV;
  void          (* cClickSelectCallBack)(float x,float y,float z,
                                         float v);
  void           * cClickSelectArg;
  void          (* cClickSelectArgCallBack)(float x, float y, float z, 
                                            float v, void *clickSelectArg);
  float       cBoxMin[3];
  float       cBoxMax[3];
  void        (* cClickBoxCallBack)(float minX, float minY, float minZ, 
                                    float maxX, float maxY, float maxZ);
  void        * cClickBoxArg;
//  void        (* cClickBoxArgCallBack)(float minX, float minY, float minZ,
//                                         float maxX, float maxY, float maxZ,
//                                         void * clickBoxArg)    
 
  void        (* cIWCallBack)(void);
  void         * cIWArg;
  void        (* cIWArgCallBack)(void * iwArg);
      
  bool        cFlipX[3];
  bool        cFlipY[3];
  bool        cFlipZ[3];
  bool        cTranspose[3];
    
  float       cOverlayOpacity;
  
  OverlayPointer cOverlayData;
  void     (* cViewOverlayCallBack)(void);
  
  unsigned char * cWinOverlayData;
  
  typedef itk::ColorTable<float>        ColorTableType;
  typedef ColorTableType::Pointer       ColorTablePointer;
  
  ColorTablePointer      cColorTable;

  ImagePointer m_ImData;
  typename SliceType::ConstPointer m_Slice;

  bool NotUpdated;

  GreyTextureType * m_GreyTexture;
  int   m_SlicePosition;
  
};

}

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovImageGlRenderMethodSlicer.txx"
#endif

#endif //_ImageGlRenderMethodSlicer_h
