/*=========================================================================
 *
 *  Copyright David Doria 2012 daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

// ITK
#include "itkRegionOfInterestImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "itkVectorMagnitudeImageFilter.h"

// Custom
#include "ITKHelpers/ITKHelpers.h"
#include "ITKHelpers/Helpers/Helpers.h"

// Qt
#include <QColor>

namespace ITKQtHelpers
{

template <typename TImage>
QImage GetQImageColor(const TImage* const image, QImage::Format format)
{
  return GetQImageColor(image, image->GetLargestPossibleRegion(), format);
}

/** Get a color QImage from a scalar ITK image. */
template <typename TImage>
QImage GetQImageColor(const TImage* const image,
                      const itk::ImageRegion<2>& region, QImage::Format format)
{
  QImage qimage(region.GetSize()[0], region.GetSize()[1], format);

  typedef itk::RegionOfInterestImageFilter< TImage, TImage >
      RegionOfInterestImageFilterType;
  typename RegionOfInterestImageFilterType::Pointer
      regionOfInterestImageFilter = RegionOfInterestImageFilterType::New();
  regionOfInterestImageFilter->SetRegionOfInterest(region);
  regionOfInterestImageFilter->SetInput(image);
  regionOfInterestImageFilter->Update();

  itk::ImageRegionIterator<TImage>
      imageIterator(regionOfInterestImageFilter->GetOutput(),
                    regionOfInterestImageFilter->GetOutput()->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
  {
    typename TImage::PixelType pixel = imageIterator.Get();

    itk::Index<2> index = imageIterator.GetIndex();

    int gray = 0; // initialize

    if(Helpers::IsValidRGB(pixel, pixel, pixel))
    {
      gray = pixel;
    }
    else
    {
      gray = static_cast<int>(Helpers::Force0to255(pixel));
      // Convert to float to output so that we see the actual values in a representable way.
//      std::cerr << "Can't set r,g,b to " << static_cast<float>(pixel) << " "
//                                         << static_cast<float>(pixel )<< " "
//                                         << static_cast<float>(pixel) << std::endl;
    }

    QColor pixelColor(gray, gray, gray);
    qimage.setPixel(index[0], index[1], pixelColor.rgb());

    ++imageIterator;
  }

  return qimage; // The actual image region
  // The flipped image region - the logic for this needs to be outside of this function
  //return qimage.mirrored(false, true); // (horizontal, vertical)
}

template <typename TPixel, unsigned int DComponents>
QImage GetQImageColor(const itk::Image<itk::CovariantVector<TPixel, DComponents> >* const image,
                      QImage::Format format)
{
  return GetQImageColor(image, image->GetLargestPossibleRegion(), format);
}

template <typename TPixel, unsigned int DComponents>
QImage GetQImageColor(const itk::Image<itk::CovariantVector<TPixel, DComponents> >* const image,
                      const itk::ImageRegion<2>& region,
                      QImage::Format format)
{
  return GetQImageColor_Vector(image, region, format);
}

template <typename TPixel>
QImage GetQImageColor(const itk::VectorImage<TPixel, 2>* const image, QImage::Format format)
{
  return GetQImageColor(image, image->GetLargestPossibleRegion(), format);
}

template <typename TPixel>
QImage GetQImageColor(const itk::VectorImage<TPixel, 2>* const image,
                      const itk::ImageRegion<2>& region, QImage::Format format)
{
  return GetQImageColor_Vector(image, region, format);
}

template <typename TImage>
QImage GetQImageColor_Vector(const TImage* const image, QImage::Format format)
{
  return GetQImageColor_Vector(image, image->GetLargestPossibleRegion(), format);
}

/** Get a color QImage from a multi-channel ITK image. */
template <typename TImage>
QImage GetQImageColor_Vector(const TImage* const image,
                             const itk::ImageRegion<2>& region, QImage::Format format)
{
  QImage qimage(region.GetSize()[0], region.GetSize()[1], format);

  typedef itk::RegionOfInterestImageFilter< TImage, TImage >
      RegionOfInterestImageFilterType;
  typename RegionOfInterestImageFilterType::Pointer regionOfInterestImageFilter =
      RegionOfInterestImageFilterType::New();
  regionOfInterestImageFilter->SetRegionOfInterest(region);
  regionOfInterestImageFilter->SetInput(image);
  regionOfInterestImageFilter->Update();

  itk::ImageRegionIterator<TImage>
      imageIterator(regionOfInterestImageFilter->GetOutput(),
                    regionOfInterestImageFilter->GetOutput()->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
  {
    typename TImage::PixelType pixel = imageIterator.Get();

    itk::Index<2> index = imageIterator.GetIndex();

    if(!Helpers::IsValidRGB(pixel[0], pixel[1], pixel[2]))
    {
      pixel[0] = static_cast<int>(Helpers::Force0to255(pixel[0]));
      pixel[1] = static_cast<int>(Helpers::Force0to255(pixel[1]));
      pixel[2] = static_cast<int>(Helpers::Force0to255(pixel[2]));
    }

    QColor pixelColor(pixel[0], pixel[1], pixel[2]);
    qimage.setPixel(index[0], index[1], pixelColor.rgb());

    ++imageIterator;
  }

  return qimage;
}

template <typename TImage>
QImage GetQImageMagnitude(const TImage* const image)
{
  return GetQImageMagnitude(image, image->GetLargestPossibleRegion());
}

template <typename TImage>
QImage GetQImageMagnitude(const TImage* image, const itk::ImageRegion<2>& region)
{
  // Get a QImage from a single channel ITK image. Each channel of the output image is the same.
  QImage qimage(region.GetSize()[0], region.GetSize()[1], QImage::Format_RGB888);

  typedef itk::Image<typename TImage::InternalPixelType, 2> ScalarImageType;
  typedef itk::VectorMagnitudeImageFilter<TImage, ScalarImageType>  VectorMagnitudeFilterType;
  typename VectorMagnitudeFilterType::Pointer magnitudeFilter = VectorMagnitudeFilterType::New();
  magnitudeFilter->SetInput(image);
  magnitudeFilter->Update();

  typedef itk::Image<unsigned char, 2> UnsignedCharScalarImageType;
  typedef itk::RescaleIntensityImageFilter<ScalarImageType, UnsignedCharScalarImageType> RescaleFilterType;
  typename RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->SetInput( magnitudeFilter->GetOutput() );
  rescaleFilter->Update();

  typedef itk::RegionOfInterestImageFilter< UnsignedCharScalarImageType, UnsignedCharScalarImageType> RegionOfInterestImageFilterType;
  typename RegionOfInterestImageFilterType::Pointer regionOfInterestImageFilter = RegionOfInterestImageFilterType::New();
  regionOfInterestImageFilter->SetRegionOfInterest(region);
  regionOfInterestImageFilter->SetInput(rescaleFilter->GetOutput());
  regionOfInterestImageFilter->Update();

  itk::ImageRegionIterator<UnsignedCharScalarImageType> imageIterator(regionOfInterestImageFilter->GetOutput(),
                                                                      regionOfInterestImageFilter->GetOutput()->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    unsigned char pixelValue = imageIterator.Get();

    QColor pixelColor(static_cast<int>(pixelValue), static_cast<int>(pixelValue), static_cast<int>(pixelValue));

    itk::Index<2> index = imageIterator.GetIndex();
    qimage.setPixel(index[0], index[1], pixelColor.rgb());

    ++imageIterator;
    }

  //return qimage; // The actual image region
  return qimage.mirrored(false, true); // The flipped image region
}

template <typename TImage>
QImage GetQImageChannel(const TImage* image, const itk::ImageRegion<2>& region, const unsigned int channel)
{
  typedef itk::Image<float, 2> ScalarImageType;
  typedef itk::VectorIndexSelectionCastImageFilter<TImage, ScalarImageType> IndexSelectionType;
  typename IndexSelectionType::Pointer indexSelectionFilter = IndexSelectionType::New();
  indexSelectionFilter->SetIndex(channel);
  indexSelectionFilter->SetInput(image);
  indexSelectionFilter->Update();

  return GetQImageScalar(indexSelectionFilter->GetOutput(), region);
}

template <typename TImage>
QImage GetQImageScalar(const TImage* const image)
{
  return GetQImageScalar(image, image->GetLargestPossibleRegion());
}

template <typename TImage>
QImage GetQImageScalar(const TImage* image, const itk::ImageRegion<2>& region)
{
  QImage qimage(region.GetSize()[0], region.GetSize()[1], QImage::Format_RGB888);

  typedef itk::RegionOfInterestImageFilter< TImage, TImage>
      RegionOfInterestImageFilterType;
  typename RegionOfInterestImageFilterType::Pointer
      regionOfInterestImageFilter = RegionOfInterestImageFilterType::New();
  regionOfInterestImageFilter->SetRegionOfInterest(region);
  regionOfInterestImageFilter->SetInput(image);
  regionOfInterestImageFilter->Update();

  itk::ImageRegionIterator<TImage>
      imageIterator(regionOfInterestImageFilter->GetOutput(),
                    regionOfInterestImageFilter->GetOutput()->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
  {
    typename TImage::PixelType pixelValue = imageIterator.Get();

    QColor pixelColor(static_cast<int>(pixelValue),
                      static_cast<int>(pixelValue),
                      static_cast<int>(pixelValue));

    itk::Index<2> index = imageIterator.GetIndex();
    qimage.setPixel(index[0], index[1], pixelColor.rgb());

    ++imageIterator;
  }

  //return qimage; // The actual image region
  return qimage.mirrored(false, true); // The flipped image region
}

/*
// TODO: Replace this with a CombineImages function in the Mask class, then simply convert that image to a QImage as normal.
template <typename TImage>
QImage GetQImageCombinedPatch(const TImage* const image, const itk::ImageRegion<2>& sourceRegion, const itk::ImageRegion<2>& targetRegion, const Mask* const mask)
{
  assert(sourceRegion.GetSize() == targetRegion.GetSize());

  QImage qimage(sourceRegion.GetSize()[0], sourceRegion.GetSize()[1], QImage::Format_RGB888);

  typedef itk::RegionOfInterestImageFilter<TImage, TImage> RegionOfInterestImageFilterType;

  typename RegionOfInterestImageFilterType::Pointer sourcePatchExtractor = RegionOfInterestImageFilterType::New();
  sourcePatchExtractor->SetRegionOfInterest(sourceRegion);
  sourcePatchExtractor->SetInput(image);
  sourcePatchExtractor->Update();

  typename RegionOfInterestImageFilterType::Pointer targetPatchExtractor = RegionOfInterestImageFilterType::New();
  targetPatchExtractor->SetRegionOfInterest(targetRegion);
  targetPatchExtractor->SetInput(image);
  targetPatchExtractor->Update();

  typedef itk::RegionOfInterestImageFilter<Mask, Mask> RegionOfInterestMaskFilterType;
  typename RegionOfInterestMaskFilterType::Pointer regionOfInterestMaskFilter = RegionOfInterestMaskFilterType::New();
  regionOfInterestMaskFilter->SetRegionOfInterest(targetRegion);
  regionOfInterestMaskFilter->SetInput(mask);
  regionOfInterestMaskFilter->Update();

  itk::ImageRegionIterator<TImage> sourcePatchIterator(sourcePatchExtractor->GetOutput(),
                                                       sourcePatchExtractor->GetOutput()->GetLargestPossibleRegion());

  itk::ImageRegionIterator<TImage> targetPatchIterator(targetPatchExtractor->GetOutput(),
                                                       targetPatchExtractor->GetOutput()->GetLargestPossibleRegion());

  while(!sourcePatchIterator.IsAtEnd())
    {
    itk::Index<2> index = targetPatchIterator.GetIndex();

    typename TImage::PixelType pixel;
    if(regionOfInterestMaskFilter->GetOutput()->IsHole(index))
      {
      pixel = sourcePatchIterator.Get();
      }
    else
      {
      pixel = targetPatchIterator.Get();
      }

    QColor pixelColor(static_cast<int>(pixel[0]), static_cast<int>(pixel[1]), static_cast<int>(pixel[2]));
    qimage.setPixel(index[0], index[1], pixelColor.rgb());

    ++targetPatchIterator;
    ++sourcePatchIterator;
    }

  // std::cout << "There were " << numberOfHolePixels << " hole pixels." << std::endl;

  //return qimage; // The actual image region
  return qimage.mirrored(false, true); // The flipped image region
}*/

template <typename TValue>
QColor GetQColor(const itk::VariableLengthVector<TValue>& vec)
{
  QColor color;

  color.setRed(vec[0]);
  color.setGreen(vec[1]);
  color.setBlue(vec[2]);

  return color;
}

template <typename TComponent>
QColor GetQColor(const itk::CovariantVector<TComponent, 3>& vec)
{
  QColor color;

  color.setRed(vec[0]);
  color.setGreen(vec[1]);
  color.setBlue(vec[2]);

  return color;
}

template <typename TImage>
void QImageToITKImage(const QImage& qimage, TImage* const image)
{
  itk::Index<2> corner = {{0,0}};
  itk::Size<2> size = {{static_cast<itk::SizeValueType>(qimage.size().width()),
                        static_cast<itk::SizeValueType>(qimage.size().height())}};
  itk::ImageRegion<2> region(corner, size);
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(itk::NumericTraits<typename TImage::PixelType>::Zero);

  itk::ImageRegionIteratorWithIndex<TImage>
      imageIterator(image, image->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
  {
    itk::Index<2> currentIndex = imageIterator.GetIndex();
    typename TImage::PixelType pixel;
//    pixel[0] = qimage.pixel(currentIndex[0], currentIndex[1]).red();
    pixel[0] = qRed(qimage.pixel(currentIndex[0], currentIndex[1]));
    pixel[1] = qBlue(qimage.pixel(currentIndex[0], currentIndex[1]));
    pixel[2] = qGreen(qimage.pixel(currentIndex[0], currentIndex[1]));

    imageIterator.Set(pixel);

    ++imageIterator;
  }
}

} // end namespace
