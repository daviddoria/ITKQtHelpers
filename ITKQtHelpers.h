/*=========================================================================
 *
 *  Copyright David Doria 2011 daviddoria@gmail.com
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

#ifndef HELPERS_QT_H
#define HELPERS_QT_H

// Qt
#include <QColor>
#include <QImage>
#include <QMetaType>
class QGraphicsView;
class QTableWidget;

// ITK
#include "itkImageRegion.h"

// Custom
//#include "DisplayStyle.h"

Q_DECLARE_METATYPE(itk::ImageRegion<2>)

namespace HelpersQt
{

////////////////////////////////////
///////// Function templates ///////
////////////////////////////////////
// template <typename TImage>
// QImage GetQImage(const TImage* const image, const itk::ImageRegion<2>& region, const DisplayStyle& style);

template <typename TImage>
QImage GetQImageColor(const TImage* const image, const itk::ImageRegion<2>& region);

template <typename TImage>
QImage GetQImageMagnitude(const TImage* const image, const itk::ImageRegion<2>& region);

template <typename TImage>
QImage GetQImageScalar(const TImage* const image, const itk::ImageRegion<2>& region);

template <typename TImage>
QImage GetQImageChannel(const TImage* const image, const itk::ImageRegion<2>& region, const unsigned int channel);

/** Construct a QImage from a source and target patch.*/
// template <typename TImage>
// QImage GetQImageCombinedPatch(const TImage* const image, const itk::ImageRegion<2>& sourceRegion, const itk::ImageRegion<2>& targetRegion, const Mask* const mask);


} // end namespace

#include "QtHelpers.hpp"

#endif
