#
#  Convert images from different formats
#  into Encapsulated Postscript to be 
#  included in LaTeX files.
#


#
#  Find fig2dev : this program convert Xfig figures 
#  to Encapsulated Postscript on the command line.
#
FIND_PROGRAM(FIG2DEV_EXECUTABLE
  NAMES fig2dev
)


# 
# Find ImageMagick tools
# This is used to convert image formats
#
INCLUDE (${CMAKE_ROOT}/Modules/FindImageMagick.cmake)


#
#
#  Error checking for the components we need
#
#
IF(NOT IMAGEMAGICK_CONVERT_EXECUTABLE)
  MESSAGE("ImageMagick convert utility was not found. Please pass to advanced mode and provide its full path")
ENDIF(NOT IMAGEMAGICK_CONVERT_EXECUTABLE)




ADD_CUSTOM_TARGET(SoftwareGuideArt  ALL echo)


#
#  At some point we should replace XFIG_FIGURES with the FILE GLOB expression.
#
FILE(GLOB XFIG_FIGURES *.fig)
FILE(GLOB PNG_IMAGES *.png)
FILE(GLOB JPEG_IMAGES *.jpg)
FILE(GLOB EPS_IMAGES *.eps)

#
# If we built the figures, remove figures that were already generated 
# from the list
#
IF (BUILD_FIGURES)
  FIND_FILE(GENERATED_FILES "GeneratedFiles.txt" "${SoftwareGuide_BINARY_DIR}/Art/Generated")
  SET(GENERATED_FILES "${GENERATED_FILES}" CACHE INTERNAL "These files were generated" FORCE) #Just make the variable internal
  IF(GENERATED_FILES)
    FILE(READ ${GENERATED_FILES} REGULAR_EXPRESSIONS)
    FILE(GLOB FILES_TO_REMOVE ${REGULAR_EXPRESSIONS})
    REMOVE(XFIG_FIGURES ${FILES_TO_REMOVE})
    REMOVE(PNG_IMAGES ${FILES_TO_REMOVE})
    REMOVE(JPEG_IMAGES ${FILES_TO_REMOVE})
    REMOVE(EPS_IMAGES ${FILES_TO_REMOVE})
  ENDIF(GENERATED_FILES)
ENDIF(BUILD_FIGURES)




#SET(XFIG_FIGURES
#  BayesianPluginClassifier
#  BinaryThresholdTransferFunction
#  CannySegmentationLevelSetImageFilterCollaborationDiagram1
#  DiscreteGaussian
#  DeformableModelCollaborationDiagram
#  DudaClassifier
#  GeometricalObjects
#  GeodesicActiveContoursCollaborationDiagram1
#  GeodesicActiveContourShapePriorCollaborationDiagram
#  FastMarchingCollaborationDiagram1
#  FuzzyConnectednessClassDiagram1
#  FuzzyConnectednessCollaborationDiagram1
#  FuzzyVoronoiCollaborationDiagram1
#  FuzzyVoronoiDeformableCollaborationDiagram1
#  Histogram
#  HybridSegmentationEngine1
#  HybridSegmentationFCVDDM
#  ImageAdaptorConcept
#  ImageIOCollaborationDiagram
#  ImageIOFactoriesClassDiagram
#  ImageIOFactoriesUseCases
#  ImageOriginAndSpacing
#  IteratorFigure1
#  ImageOverlapedRegions
#  ImageOverlap
#  ImageOverlapInterpolator
#  ImageOverlapIterator
#  ImageRegistrationConcept
#  ImageRegistration1Pipeline
#  ImageRegistration1TraceMetric
#  ImageRegistration1TraceTranslations
#  ImageRegistration2TraceTranslations
#  ImageRegistration2TraceTranslations2
#  ImageRegistration2TraceMetric
#  ImageRegistration2TraceMetric2
#  ImageRegistration3Observer
#  ImageRegistration4TraceMetric
#  ImageRegistration4TraceTranslations
#  ImageRegistration5TraceMetric
#  ImageRegistration5TraceAngle
#  ImageRegistration5TraceTranslations
#  ImageRegistration5TraceMetric2
#  ImageRegistration5TraceAngle2
#  ImageRegistration5TraceTranslations2
#  ImageRegistration6TraceMetric
#  ImageRegistration6TraceAngle
#  ImageRegistration6TraceTranslations
#  ImageRegistration8TraceMetric
#  ImageRegistration8TraceAngle
#  ImageRegistration8TraceTranslations
#  ImageRegistration9TraceMetric
#  ImageRegistration9TraceAngle
#  ImageRegistration9TraceTranslations
#  LaplacianSegmentationLevelSetImageFilterCollaborationDiagram1
#  LevelSetZeroSet
#  LevelSetSegmentationFigure1
#  MinMaxCurvatureFlowFunctionDiagram
#  ModelToImageRegistrationConcept
#  ModelToImageRegistrationTraceAngle
#  ModelToImageRegistrationTraceTranslations
#  MultiResImageRegistration1TraceMetric
#  MultiResImageRegistration1TraceTranslations
#  MultiResImageRegistration2TraceMetric
#  MultiResImageRegistration2TraceTranslations
#  MultiResRegistrationComponents
#  MultiResRegistrationConcept
#  ModelToImageRegistrationComponentsDiagram
#  NeighborhoodIteratorFig1
#  NeighborhoodIteratorFig2
#  OptimizersHierarchy
#  ParzenWindowing1
#  ParzenWindowing2
#  ParzenWindowing3
#  ParzenWindowing13
#  PointSetWithVectors
#  RegistrationComponentsDiagram
#  ResampleImageFilterOutput1Analysis
#  ResampleImageFilterOutput2Analysis
#  ResampleImageFilterOutput3Analysis
#  ResampleImageFilterTransformComposition1
#  ResampleImageFilterTransformComposition2
#  ResampleImageFilterTransformComposition3
#  ResampleImageFilterTransformComposition4
#  ResampleImageFilterTransformComposition5
#  ResampleImageFilterTransformComposition6
#  SampleInheritanceTree
#  SigmoidBase
#  SigmoidParameterAlpha
#  SigmoidParameterBeta
#  ShapeDetectionCollaborationDiagram1
#  SpatialObjectTransforms
#  StatisticalClassificationFramework
#  ThresholdSegmentationLevelSetImageFilterFigure1
#  ThresholdSegmentationLevelSetImageFilterCollaborationDiagram1
#  ThresholdTransferFunctionAbove
#  ThresholdTransferFunctionBelow
#  ThresholdTransferFunctionOutside
#  TranslationAndRotationCenter
#  VoronoiSegmentationClassDiagram1
#  VoronoiSegmentationCollaborationDiagram1
#  VoronoiSegmentationCollaborationDiagram2
#  WatershedCatchmentBasins
#  WatershedsHierarchy
#  WatershedImageFilter
#)
#
#
SET(JPEG_IMAGES
  sovLogo
)

FOREACH(image ${JPEG_IMAGES})
  GET_FILENAME_COMPONENT(Imagebase ${image} NAME)
  GET_FILENAME_COMPONENT(ImageBase ${Imagebase} NAME_WE)
  ADD_CUSTOM_COMMAND( 
  SOURCE    ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.jpg
  COMMAND   ${IMAGEMAGICK_CONVERT_EXECUTABLE}
  ARGS      ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.jpg
            ${ImageBase}.eps
  TARGET    SoftwareGuideArt
  OUTPUTS   ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps
  )
  SET(EPS_DEPS ${EPS_DEPS} ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps)
ENDFOREACH(image)

FOREACH(image ${PNG_IMAGES})
  GET_FILENAME_COMPONENT(Imagebase ${image} NAME)
  GET_FILENAME_COMPONENT(ImageBase ${Imagebase} NAME_WE)
  ADD_CUSTOM_COMMAND( 
  SOURCE    ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.png
  COMMAND   ${IMAGEMAGICK_CONVERT_EXECUTABLE}
  ARGS      ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.png
            ${ImageBase}.eps
  TARGET    SoftwareGuideArt
  OUTPUTS   ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps
  )
  SET(EPS_DEPS ${EPS_DEPS} ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps)
ENDFOREACH(image)

FOREACH(image ${XFIG_FIGURES})
  GET_FILENAME_COMPONENT(Imagebase ${image} NAME)
  GET_FILENAME_COMPONENT(ImageBase ${Imagebase} NAME_WE)
  ADD_CUSTOM_COMMAND( 
  SOURCE    ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.fig
  COMMAND   ${FIG2DEV_EXECUTABLE}
  ARGS      -L eps ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.fig ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps
  TARGET    SoftwareGuideArt
  OUTPUTS   ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps
  )
  SET(EPS_DEPS ${EPS_DEPS} ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps)
ENDFOREACH(image)

FOREACH(image ${EPS_IMAGES})
  GET_FILENAME_COMPONENT(Imagebase ${image} NAME)
  GET_FILENAME_COMPONENT(ImageBase ${Imagebase} NAME_WE)
  CONFIGURE_FILE(
                "${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.eps" 
                "${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps" 
                COPYONLY IMMEDIATE)
  SET(EPS_DEPS ${EPS_DEPS} ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps)
ENDFOREACH(image)

ADD_CUSTOM_COMMAND(
  SOURCE SoftwareGuideArt
  COMMAND echo
  TARGET SoftwareGuideArt
  DEPENDS ${EPS_DEPS}
)

