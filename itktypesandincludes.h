#ifndef DICOMTYPEDEFS_H
#define DICOMTYPEDEFS_H

#include <itkImage.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesReader.h>
#include <itkImageRegistrationMethod.h>

//using non rigid transform instead of Translation Transform
//#include <itkTranslationTransform.h>


#include <itkMeanSquaresImageToImageMetric.h>

// Using LBFGSOptimizer instead
//#include <itkRegularStepGradientDescentOptimizer.h>


#include <itkResampleImageFilter.h>
#include <itkSubtractImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include "itkImageToVTKImageFilter.h"
#include <itkMetaImageIOFactory.h>
#include <itkDCMTKImageIOFactory.h>
#include <itkCastImageFilter.h>
#include <itkHistogramMatchingImageFilter.h>


#include "itkBSplineTransform.h"
#include "itkLBFGSBOptimizer.h"


// Some Typedefs and Definitions for ITK
typedef  signed short PixelType;
static const unsigned int dimension = 3;
typedef float InternalPixelType;
typedef itk::Image< InternalPixelType, dimension> InternalImageType;

typedef itk::Image<PixelType, dimension> ImageType;
typedef itk::ImageSeriesReader< ImageType > ReaderType;
typedef itk::ImageToVTKImageFilter <ImageType> FilterType;
typedef itk::GDCMImageIO ImageIOType;
typedef itk::GDCMSeriesFileNames NamesGeneratorType;

typedef itk::CastImageFilter <ImageType, InternalImageType> ImageCasterType;
typedef itk::HistogramMatchingImageFilter<InternalImageType, InternalImageType> MatchingFilterType;


//typedef itk::TranslationTransform<double, dimension> TransformType;

//typedef itk::RegularStepGradientDescentOptimizer OptimizerType;


typedef itk::MeanSquaresImageToImageMetric<InternalImageType, InternalImageType> MetricType;
typedef itk::LinearInterpolateImageFunction< InternalImageType, double> InterpolatorType;
typedef itk::ImageRegistrationMethod<InternalImageType, InternalImageType> RegistrationType;
typedef itk::ResampleImageFilter<ImageType, InternalImageType> ResampleFilterType;
typedef itk::SubtractImageFilter<ImageType,InternalImageType,ImageType> DifferenceFilterType;
typedef itk::RescaleIntensityImageFilter<ImageType,ImageType> RescalerType;

//non-rigid Transform typedefs
static const unsigned int SpaceDimension = dimension;
//maybe change SplineOrder later
static const unsigned int SplineOrder = 3;
typedef double CoordinateRepType;
typedef itk::BSplineTransform<CoordinateRepType,SpaceDimension,SplineOrder> TransformType;

typedef itk::LBFGSBOptimizer OptimizerType;

#endif // DICOMTYPEDEFS_H
