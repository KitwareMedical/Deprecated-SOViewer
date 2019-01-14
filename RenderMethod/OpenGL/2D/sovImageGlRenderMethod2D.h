/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovImageGlRenderMethod2D.h,v $
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
#ifndef _ImageGlRenderMethod2D_h
#define _ImageGlRenderMethod2D_h

#include "sovGlRenderMethod2D.h"
#include "itkImage.h"
#include "itkColorTable.h"
#include "itkImageSpatialObject.h"
#include "OpenGLSliceTexture.h"

#include <math.h>

/*! Clicking in a window will cause different events
*  NOP = nothing
*  SELECT = report pixel info
*/
const int NUM_ClickModeTypes2D = 3;
typedef enum {CM_NOP2D, CM_SELECT2D, CM_BOX2D} ClickModeType2D;
const char ClickModeTypeName2D[3][7] =
  {{'N', 'O', 'P', '\0', ' ', ' ', ' '},
  {'S', 'e', 'l', 'e', 'c', 't', '\0'},
  {'B', 'o', 'x', '\0', ' ', ' ', ' '}};

  /*! Handling of values outside intensity window range - values above 
  *    and below can be handled separately
  *  IW_MIN = set values outside range to min value
  *  IW_MAX = set values outside range to max value
  *  IW_FLIP = rescale values to be within range by flipping
*/
const int NUM_ImageModeTypes2D = 8;
typedef enum {IMG_VAL2D, IMG_INV2D, IMG_LOG2D, IMG_DX2D, IMG_DY2D, IMG_DZ2D,
  IMG_BLEND2D, IMG_MIP2D} ImageModeType2D;
const char ImageModeTypeName2D[8][8] =
  {{'V', 'a', 'l', 'u', 'e', '\0', ' ', ' '},
  {'I', 'n', 'v', 'e', 'r', 's', 'e', '\0'},
  {'L', 'o', 'g', '\0', ' ', ' ', ' ', ' '},
  {'D', 'e', 'r', 'i', 'v', '-', 'X', '\0'},
  {'D', 'e', 'r', 'i', 'v', '-', 'Y', '\0'},
  {'D', 'e', 'r', 'i', 'v', '-', 'Z', '\0'},
  {'B', 'l', 'e', 'n', 'd', '\0', ' ', ' '},
  {'M', 'I', 'P', '\0', ' ', ' ', ' ', ' '}};

const int NUM_IWModeTypes2D = 3;
typedef enum {IW_MIN2D, IW_MAX2D, IW_FLIP2D} IWModeType2D;
const char IWModeTypeName2D[3][5] =
  {{'M', 'i', 'n', '\0', ' '},
  {'M', 'a', 'x', '\0', ' '},
  {'F', 'l', 'i', 'p', '\0'}};

  /*! Structure clickPoint to store the x,y and intensity value of a
  * point in the image
*/
struct ClickPoint2D 
  {
  float x, y;
  double value;
  
  ClickPoint2D(float _x,float _y,double v)
    : x(_x),y(_y),value(v){}
  };



namespace sov
{

template <class TImage>
class ImageGlRenderMethod2D
: public GlRenderMethod2D
{

  public:

  typedef TImage                           ImageType;
  typedef typename ImageType::PixelType    ImagePixelType;
  
  typedef unsigned char                    OverlayPixelType;
  typedef itk::Image<OverlayPixelType,2>   OverlayType;
  typedef typename ImageType::ConstPointer ImagePointer;
  typedef typename OverlayType::Pointer    OverlayPointer;
  typedef typename ImageType::RegionType   RegionType;
  typedef typename ImageType::SizeType     SizeType;
  typedef typename ImageType::IndexType    IndexType;

  typedef ImageSpatialObject<2,ImagePixelType> ImageSpatialObjectType;
  typedef OpenGLSliceTexture<ImagePixelType> GreyTextureType;

  typedef ImageGlRenderMethod2D        Self;
  typedef GlRenderMethod2D            Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;

  typedef Superclass::ObjectType      ObjectType;
  typedef Superclass::ObjectListType  ObjectListType;

  itkNewMacro( ImageGlRenderMethod2D );
  itkTypeMacro( ImageGlRenderMethod2D, GlRenderMethod2D );

  virtual void draw( void );

  void SetMinimumIntensity(double min){cIWMin = min;}
  void SetMaximumIntensity(double max){cIWMax = max;}

  double GetMinimumIntensity(){return cIWMin;}
  double GetMaximumIntensity(){return cIWMax;}

protected:

  ImageGlRenderMethod2D( void );
  ~ImageGlRenderMethod2D( void );

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();

private:

  unsigned long       m_DimSize[2];
  float               m_Spacing[2];
  double              m_DataMax;
  double              m_DataMin;
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
  IWModeType2D    cIWModeMin;
  IWModeType2D    cIWModeMax;
  ImageModeType2D cImageMode;

  int         cWinCenter[2];
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
  unsigned int        cWinOrder[2];
  unsigned int        cWinOrientation;
    
  bool  cViewImData ;
  bool  cValidImData;
  bool  cValidOverlayData;
  void   (* cSliceNumCallBack)(void);
  void    * cSliceNumArg;
  void   (* cSliceNumArgCallBack)(void * sliceNumArg);
  void                    (* cViewImDataCallBack)(void);
  void                     * cViewImDataArg;
  void                    (* cViewImDataArgCallBack)(void *viewImDataArg);
    
  float         cClickSelect[2];
  float         cClickSelectV;
  void          (* cClickSelectCallBack)(float x,float y,float z,
                                           float v);
  void           * cClickSelectArg;
  void          (* cClickSelectArgCallBack)(float x, float y, float z, 
                                              float v, void *clickSelectArg);
  float       cBoxMin[2];
  float       cBoxMax[2];
  void        (* cClickBoxCallBack)(float minX, float minY, float minZ, 
                                      float maxX, float maxY, float maxZ);
  void        * cClickBoxArg;
//  void        (* cClickBoxArgCallBack)(float minX, float minY, float minZ,
//                                         float maxX, float maxY, float maxZ,
//                                         void * clickBoxArg)    
 
  void        (* cIWCallBack)(void);
  void         * cIWArg;
  void        (* cIWArgCallBack)(void * iwArg);
      
  bool        cFlipX[2];
  bool        cFlipY[2];
  bool        cFlipZ[2];
  bool        cTranspose[2];
    
  float       cOverlayOpacity;
  
  OverlayPointer cOverlayData;
  void     (* cViewOverlayCallBack)(void);
  
  unsigned char * cWinOverlayData;
  
  typedef itk::ColorTable<float>        ColorTableType;
  typedef ColorTableType::Pointer       ColorTablePointer;
  
  ColorTablePointer      cColorTable;

  ImagePointer m_ImData;
  GreyTextureType * m_GreyTexture;
};

}

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovImageGlRenderMethod2D.txx"
#endif

#endif //_ImageGlRenderMethod2D_h
