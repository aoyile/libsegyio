/*******************************************************************************
 *
 *                  libsegyio Copyright (C) 2021 aoyile
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#include "segyio/bitop.h"
#include "segyio/survey.h"

namespace SegyIO {

class _RegularSurvey 
  : public Survey::Interface 
{
private:

  const uword ilineMin_;
  const uword ilineMax_; 
  const uword xlineMin_; 
  const uword xlineMax_;
  const uword indexMax_;

public:

  _RegularSurvey(
    const uword ilineMin, const uword ilineMax, 
    const uword xlineMin, const uword xlineMax 
  ) : ilineMin_(ilineMin), 
      ilineMax_(ilineMax), 
      xlineMin_(xlineMin), 
      xlineMax_(xlineMax),
      indexMax_((ilineMax - ilineMin + 1) * (xlineMax - xlineMin + 1) - 1)
  { /* nothing needs to do */ }

  virtual ~_RegularSurvey(void)
  { /* nothing needs to do */ }

  virtual uword indexMax(void) const { return indexMax_; }
  virtual uword ilineMin(void) const { return ilineMin_; }
  virtual uword ilineMax(void) const { return ilineMax_; }
  virtual uword xlineMin(void) const { return xlineMin_; }
  virtual uword xlineMax(void) const { return xlineMax_; }
  virtual bool isRegular(void) const { return true; }

  virtual bool contains(const uword iline, const uword xline) const {
    return ( iline >= ilineMin_ && iline <= ilineMax_ && 
             xline >= xlineMin_ && xline <= xlineMax_ );
  }

  virtual uword iline(const uword index) const {
    if (index > indexMax_)
      return SEGYIO_INVALID_INDEX;
    else
      return ilineMin_ + index / (xlineMax_ - xlineMin_ + 1);
  }

  virtual uword xline(const uword index) const  {
    if (index > indexMax_)
      return SEGYIO_INVALID_INDEX;
    else
      return xlineMin_ + index % (xlineMax_ - xlineMin_ + 1);
  }

  virtual uword index(const uword iline, const uword xline) const {
    if ( iline < ilineMin_ || iline > ilineMax_ || 
         xline < xlineMin_ || xline > xlineMax_ )
      return SEGYIO_INVALID_INDEX;
    else
      return (iline - ilineMin_) * (xlineMax_ - xlineMin_ + 1) + (xline - xlineMin_);
  }
};

class _IrregularSurvey 
  : public Survey::Interface 
{
private:

  const uword ilineMin_;
  const uword ilineMax_; 
  const uword xlineMin_; 
  const uword xlineMax_;
  const uword indexMax_;

  uvec ilineTable_;
  uvec xlineTable_;
  umat indexTable_;

public:

  _IrregularSurvey(
    const uvec& indexes, 
    const uvec& ilines, 
    const uvec& xlines
  ) : ilineMin_(ilines.min()), 
      ilineMax_(ilines.max()), 
      xlineMin_(xlines.min()), 
      xlineMax_(xlines.max()),
      indexMax_(indexes.max())
  { 
    ilineTable_ = zeros<uvec>(indexMax_ + 1);
    xlineTable_ = zeros<uvec>(indexMax_ + 1);
    indexTable_ = zeros<umat>(ilineMax_ - ilineMin_ + 1, xlineMax_ - xlineMin_ + 1);

    ilineTable_.fill(SEGYIO_INVALID_INDEX);
    xlineTable_.fill(SEGYIO_INVALID_INDEX);
    indexTable_.fill(SEGYIO_INVALID_INDEX);

    for (uword i = 0; i < indexes.n_elem; ++i) {
      ilineTable_[ indexes[i] ] = ilines[i];
      xlineTable_[ indexes[i] ] = xlines[i];
      indexTable_.at(ilines[i] - ilineMin_, xlines[i] - xlineMin_) = indexes[i];
    }
  }

  virtual ~_IrregularSurvey(void)
  { /* nothing needs to do */ }

  virtual uword indexMax(void) const { return indexMax_; }
  virtual uword ilineMin(void) const { return ilineMin_; }
  virtual uword ilineMax(void) const { return ilineMax_; }
  virtual uword xlineMin(void) const { return xlineMin_; }
  virtual uword xlineMax(void) const { return xlineMax_; }
  virtual bool isRegular(void) const { return false; }

  virtual bool contains(const uword iline, const uword xline) const {
    return (index(iline, xline) != SEGYIO_INVALID_INDEX);
  }

  virtual uword iline(const uword index) const {
    if (index > indexMax_)
      return SEGYIO_INVALID_INDEX;
    else
      return ilineTable_[index];
  }

  virtual uword xline(const uword index) const {
    if (index > indexMax_)
      return SEGYIO_INVALID_INDEX;
    else
      return xlineTable_[index];
  }

  virtual uword index(const uword iline, const uword xline) const 
  {
    if ( iline < ilineMin_ || iline > ilineMax_ || 
         xline < xlineMin_ || xline > xlineMax_ )
      return SEGYIO_INVALID_INDEX;
    else
      return indexTable_.at(iline - ilineMin_, xline - xlineMin_);
  }
};


Survey Survey::Regular(
  const uword ilineMin, const uword ilineMax, 
  const uword xlineMin, const uword xlineMax ) 
{
  return Survey(new _RegularSurvey(ilineMin, ilineMax, xlineMin, xlineMax));
}

Survey Survey::From(const uvec& indexes, const uvec& ilines, const uvec& xlines)
{
  assert(indexes.n_elem == ilines.n_elem);
  assert(indexes.n_elem == xlines.n_elem);
  
  Survey surveyI(new _IrregularSurvey(indexes, ilines, xlines));
  Survey surveyR(new _RegularSurvey(
    surveyI.ilineMin(), surveyI.ilineMax(), 
    surveyI.xlineMin(), surveyI.xlineMax()
  ));
  for (uword xline = surveyI.xlineMax(); xline >= surveyI.xlineMax(); xline--) {
    for (uword iline = surveyI.ilineMax(); iline >= surveyI.ilineMax(); iline--) {
      if (surveyR.index(iline, xline) != surveyI.index(iline, xline))
        return surveyI;
    }
  }
  return surveyR;
}

Survey Survey::From(const uvec& ilines, const uvec& xlines) {
  assert(ilines.n_elem == xlines.n_elem);
  return Survey::From(regspace<uvec>(0, ilines.n_elem-1), ilines, xlines);
}

Survey Survey::From(const SegyReader& reader, const uword ilineOffset, const uword xlineOffset)
{
  const uword nTrace = reader.traceCount();
  uvec ilines  = zeros<uvec>(nTrace); 
  uvec xlines  = zeros<uvec>(nTrace);
  for (uword i = 0; i < nTrace; ++i) {
    xlines[i]  = reader.traceLine(i, xlineOffset);
    ilines[i]  = reader.traceLine(i, ilineOffset);
  }
  return Survey::From(ilines, xlines);
}

} /*namespace SegyIO*/