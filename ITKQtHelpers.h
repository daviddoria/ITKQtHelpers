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

#ifndef ITKQTHELPERS_H
#define ITKQTHELPERS_H

// Qt
#include <QColor>
#include <QImage>
#include <QMetaType>
class QGraphicsView;
class QTableWidget;

// ITK
#include "itkCovariantVector.h"
#include "itkImageRegion.h"
#include "itkVariableLengthVector.h"

Q_DECLARE_METATYPE(itk::ImageRegion<2>)

namespace ITKQtHelpers
{

////////////////////////////////////
///////// Function templates ///////
////////////////////////////////////

  /** These two functions expect scalar images. */
template <typename TImage>
QImage GetQImageColor(const TImage* const image);

template <typename TImage>
QImage GetQImageColor(const TImage* const image, const itk::ImageRegion<2>& region);

/** These two functions just call GetQImageColor_Vector */
template <typename TPixel>
QImage GetQImageColor(const itk::VectorImage<TPixel, 2>* const image);

template <typename TPixel>
QImage GetQImageColor(const itk::VectorImage<TPixel, 2>* const image,
                      const itk::ImageRegion<2>& region);

/** These two functions just call GetQImageColor_Vector */
template <typename TPixel, unsigned int DComponents>
QImage GetQImageColor(const itk::Image<itk::CovariantVector<TPixel, DComponents> >* const image);

template <typename TPixel, unsigned int DComponents>
QImage GetQImageColor(const itk::Image<itk::CovariantVector<TPixel, DComponents> >* const image,
                      const itk::ImageRegion<2>& region);

/** These two functions just require the image to have 3 channels and an operator[] */
template <typename TImage>
QImage GetQImageColor_Vector(const TImage* const image);

template <typename TImage>
QImage GetQImageColor_Vector(const TImage* const image,
                             const itk::ImageRegion<2>& region);

template <typename TImage>
QImage GetQImageMagnitude(const TImage* const image);

template <typename TImage>
QImage GetQImageMagnitude(const TImage* const image, const itk::ImageRegion<2>& region);

template <typename TImage>
QImage GetQImageScalar(const TImage* const image);

template <typename TImage>
QImage GetQImageScalar(const TImage* const image, const itk::ImageRegion<2>& region);

template <typename TImage>
QImage GetQImageChannel(const TImage* const image, const itk::ImageRegion<2>& region, const unsigned int channel);

template <typename TValue>
QColor GetQColor(const itk::VariableLengthVector<TValue>& vec);

template <typename TComponent>
QColor GetQColor(const itk::CovariantVector<TComponent, 3>& vec);

/** Convert a QImage to an ITK image. */
template <typename TImage>
void QImageToITKImage(const QImage& qimage, TImage* const image);

} // end namespace

#include "ITKQtHelpers.hpp"

#endif
