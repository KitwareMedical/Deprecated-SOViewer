/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovColorImageGlRenderMethod2D.h,v $
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
#ifndef _ColorImageGlRenderMethod2D_h
#define _ColorImageGlRenderMethod2D_h

#include "sovGlRenderMethod2D.h"
#include "itkImage.h"
#include "itkColorTable.h"
#include "itkImageSpatialObject.h"

namespace sov
{

template <class TImage>
class ColorImageGlRenderMethod2D
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
  
  typedef ColorImageGlRenderMethod2D        Self;
  typedef GlRenderMethod2D            Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;

  typedef Superclass::ObjectType      ObjectType;
  typedef Superclass::ObjectListType  ObjectListType;

  itkNewMacro( ColorImageGlRenderMethod2D );
  itkTypeMacro( ColorImageGlRenderMethod2D, GlRenderMethod2D );

  virtual void draw( void );

  void SetMinimumIntensity(double min){cIWMin = min;}
  void SetMaximumIntensity(double max){cIWMax = max;}

  double GetMinimumIntensity(){return cIWMin;}
  double GetMaximumIntensity(){return cIWMax;}

protected:

  ColorImageGlRenderMethod2D( void );
  ~ColorImageGlRenderMethod2D( void );

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();
  virtual void Update2();

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
  bool        cTranspose[2];
    
  float       cOverlayOpacity;
  
  OverlayPointer cOverlayData;
  void     (* cViewOverlayCallBack)(void);
  
  unsigned char * cWinOverlayData;
  
  typedef itk::ColorTable<float>        ColorTableType;
  typedef ColorTableType::Pointer       ColorTablePointer;
  
  ColorTablePointer      cColorTable;

  ImagePointer m_ImData;
  
};

}

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovColorImageGlRenderMethod2D.txx"
#endif

#endif //_ColorImageGlRenderMethod2D_h
