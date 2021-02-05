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

#ifndef LIB_SEGYIO_SURVEY_H
#define LIB_SEGYIO_SURVEY_H

#include <memory>
#include "segyio/config.h"
#include "segyio/segyreader.h"

namespace SegyIO {

class Survey
{
public:

  static Survey Regular(
    const uword ilineMin, const uword ilineMax, 
    const uword xlineMin, const uword xlineMax
  );

  static Survey From(const uvec& indexes, const uvec& ilines, const uvec& xlines);

  static Survey From(const uvec& ilines, const uvec& xlines);

  static Survey From(
    const SegyReader& reader,
    const uword       ilineOffset,
    const uword       xlineOffset
  );

public:

  struct Interface {
    virtual ~Interface(void) {}
    virtual bool contains(const uword iline, const uword xline) const = 0;
    virtual uword iline(const uword index) const = 0;
    virtual uword xline(const uword index) const = 0;
    virtual uword index(const uword iline, const uword xline) const = 0;
    virtual uword indexMax(void) const = 0;
    virtual uword ilineMin(void) const = 0;
    virtual uword ilineMax(void) const = 0;
    virtual uword xlineMin(void) const = 0;
    virtual uword xlineMax(void) const = 0;
    virtual bool isRegular(void) const = 0;
  };

private:

  std::shared_ptr<Interface> pImpl_;

private:

  Survey(Interface* pImpl)
    : pImpl_(pImpl) 
  { /* nothing needs to do */ }

public:

  Survey(void) : pImpl_(NULL) 
  { /* nothing needs to do */ }

public:

  bool isInvalid(void) const { 
    return pImpl_ == NULL; 
  }

public:

  uword indexMax(void) const { return pImpl_->indexMax(); }
  uword ilineMin(void) const { return pImpl_->ilineMin(); }
  uword ilineMax(void) const { return pImpl_->ilineMax(); }
  uword xlineMin(void) const { return pImpl_->xlineMin(); }
  uword xlineMax(void) const { return pImpl_->xlineMax(); }
  bool isRegular(void) const { return pImpl_->isRegular(); }

  uword ilineCount(void) const { 
    return pImpl_->ilineMax() - pImpl_->ilineMin() + 1; 
  }

  uword xlineCount(void) const { 
    return pImpl_->xlineMax() - pImpl_->xlineMin() + 1; 
  }

  bool contains(const uword iline, const uword xline) const {
    return pImpl_->contains(iline, xline);
  }

  uword iline(const uword index) const { 
    return pImpl_->iline(index);
  }

  uword xline(const uword index) const {
    return pImpl_->xline(index);
  }

  uword index(const uword iline, const uword xline) const {
    return pImpl_->index(iline, xline);
  }

public:

  uvec ilines(void) const {
    return regspace<uvec>(ilineMin(), ilineMax());
  }

  uvec xlines(void) const {
    return regspace<uvec>(xlineMin(), xlineMax());
  }

  uvec ilines(const uvec& indexes) const {
    uvec results = zeros<uvec>(indexes.n_elem);
    for (uword i = 0; i < indexes.n_elem; ++i)
      results[i] = pImpl_->iline(indexes[i]);
    return results;
  }

  uvec xlines(const uvec& indexes) const {
    uvec results = zeros<uvec>(indexes.n_elem);
    for (uword i = 0; i < indexes.n_elem; ++i)
      results[i] = pImpl_->xline(indexes[i]);
    return results;
  }

  uvec indexes(const uvec& ilines, const uvec& xlines) const {
    assert(ilines.n_elem == xlines.n_elem);
    uvec results = zeros<uvec>(ilines.n_elem);
    for (uword i = 0; i < ilines.n_elem; ++i)
      results[i] = pImpl_->index(ilines[i], xlines[i]);
    return results;
  }

  uvec indexes(const uvec& ilines, const uword xline) const {
    uvec results = zeros<uvec>(ilines.n_elem);
    for (uword i = 0; i < ilines.n_elem; ++i)
      results[i] = pImpl_->index(ilines[i], xline);
    return results;
  }

  uvec indexes(const uword iline, const uvec& xlines) const {
    uvec results = zeros<uvec>(xlines.n_elem);
    for (uword i = 0; i < xlines.n_elem; ++i)
      results[i] = pImpl_->index(iline, xlines[i]);
    return results;
  }
};

} /*namespace SegyIO*/

#endif // LIB_SEGYIO_SURVEY_H
