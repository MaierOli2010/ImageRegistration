#ifndef DICOMTYPEDEFS_H
#define DICOMTYPEDEFS_H

/*!
 * \brief itktypesandincludes This header contains the importen typedefs and includes for the
 * itk registration process. If you want to switch the registration method or transforms
 * start here.
*/

#include <itkImage.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesReader.h>
#include <itkImageRegistrationMethodv4.h>

//using non rigid transform instead of Translation Transform
#include <itkTranslationTransform.h>


//#include <itkMeanSquaresImageToImageMetric.h>
#include <itkMattesMutualInformationImageToImageMetricv4.h>

// Using LBFGSOptimizer instead
#include <itkRegularStepGradientDescentOptimizerv4.h>


#include <itkResampleImageFilter.h>
#include <itkSubtractImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include "itkImageToVTKImageFilter.h"
#include <itkMetaImageIOFactory.h>
#include <itkDCMTKImageIOFactory.h>
#include <itkCastImageFilter.h>
#include <itkHistogramMatchingImageFilter.h>

#include <itkCompositeTransform.h>
#include "itkBSplineTransform.h"
#include "itkLBFGSBOptimizerv4.h"

#include "itkBSplineTransformParametersAdaptor.h"
#include "itkBSplineTransformInitializer.h"

// Some Typedefs and Definitions for ITK
typedef double PixelType;
static const unsigned int dimension = 3;
typedef double InternalPixelType;
typedef itk::Image< InternalPixelType, dimension> InternalImageType;

typedef itk::Image<PixelType, dimension> ImageType;
typedef itk::ImageSeriesReader< ImageType > ReaderType;
typedef itk::ImageToVTKImageFilter <ImageType> FilterType;
typedef itk::GDCMImageIO ImageIOType;
typedef itk::GDCMSeriesFileNames NamesGeneratorType;

typedef itk::CastImageFilter <ImageType, InternalImageType> ImageCasterType;
typedef itk::HistogramMatchingImageFilter<InternalImageType, InternalImageType> MatchingFilterType;


typedef itk::TranslationTransform<InternalPixelType, dimension> TransformType_1st;

typedef itk::RegularStepGradientDescentOptimizerv4<InternalPixelType> OptimizerType_1st;

//non-rigid Transform typedefs
static const unsigned int SpaceDimension = dimension;
//maybe change SplineOrder later
static const unsigned int SplineOrder = 3;
typedef double CoordinateRepType;
typedef itk::BSplineTransform<CoordinateRepType,SpaceDimension,SplineOrder> TransformType;

typedef itk::BSplineTransformInitializer<TransformType, ImageType> InitializerType;

//typedef itk::MeanSquaresImageToImageMetric<InternalImageType, InternalImageType> MetricType;
typedef itk::MattesMutualInformationImageToImageMetricv4<ImageType, InternalImageType> MetricType;
typedef itk::LinearInterpolateImageFunction< InternalImageType, double> InterpolatorType;
typedef itk::ImageRegistrationMethodv4<ImageType, InternalImageType, TransformType> RegistrationType;
typedef itk::ImageRegistrationMethodv4<ImageType, InternalImageType, TransformType_1st> TRegistrationType;
typedef itk::ResampleImageFilter<ImageType, InternalImageType> ResampleFilterType;
typedef itk::SubtractImageFilter<ImageType,InternalImageType,ImageType> DifferenceFilterType;
typedef itk::RescaleIntensityImageFilter<ImageType,ImageType> RescalerType;


typedef itk::CompositeTransform<InternalPixelType, dimension> CompositeTransformType;


typedef itk::LBFGSBOptimizerv4 OptimizerType;

#endif // DICOMTYPEDEFS_H
