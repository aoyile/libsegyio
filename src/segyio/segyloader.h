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

#ifndef LIB_SEGYIO_SEGYLOADER_H
#define LIB_SEGYIO_SEGYLOADER_H

#include "segyio/survey.h"
#include "segyio/segyreader.h"

namespace SegyIO {

class SegyLoader : public SegyReader
{
protected:

  Survey survey_;
  
public:

  SegyLoader(void) { /* nothing needs to do */ }

  SegyLoader(const Survey& survey)
   : SegyReader(), survey_(survey)
  { /* nothing needs to do */ }

  SegyLoader(const SegyLoader& loader) 
   : SegyReader()
  { open(loader.path(), loader.survey()); }

  SegyLoader(const SegyReader& reader, const Survey& survey) 
   : SegyReader()
  { open(reader.path(), survey); }

  SegyLoader(const SegyReader& reader, const uword ilineOffset, const uword xlineOffset)
   : SegyReader()
  { open(reader.path(), ilineOffset, xlineOffset); }

  virtual ~SegyLoader(void) 
  { /* nothing needs to do */ }

  virtual SegyLoader& operator= (const SegyLoader& loader) { 
    open(loader.path(), loader.survey());
    return *this;
  }

public:

  virtual bool open(const std::string& segyPath);

  virtual bool open(const std::string& segyPath, const Survey& survey);

  virtual bool open(const std::string& segyPath, const uword ilineOffset, const uword xlineOffset);

  virtual void close(void);

public:

  inline 
  const Survey& survey(void) const { 
    return survey_; 
  }

  inline
  uword ilineCount(void) const { 
    return survey_.ilineCount(); 
  }

  inline
  uword xlineCount(void) const { 
    return survey_.ilineCount(); 
  } 
  
  inline 
  uword timesCount(void) const { 
    return config().nsample; 
  }

public:

  inline 
  SegyTimes times(void) const { 
    return traceTime(0); 
  }

  inline
  SegyTimes times(const uword iline, const uword xline) const {
    return traceTime(survey_.index(iline, xline));
  }

  inline
  fvec trace(const uword iline, const uword xline) const {
    return traceData(survey_.index(iline, xline));
  }

public:

  fmat iline(const uword iline) const;

  fmat xline(const uword xline) const;

  fmat profile(const uvec& ilines, const uvec& xlines) const;

public:

  fcube volume(void) const;

  fcube ilines(const uvec& ilines) const;

  fcube xlines(const uvec& xlines) const;
  
public:

  fmat timeSlice(const float time) const;

  fmat timeSlice(const float start, const float stop) const;

  template <class FnT>
  fmat timeSlice(const float start, const float stop, const FnT& aggregate) const
  {
    const uvec  ilines = survey_.ilines();
    const uvec  xlines = survey_.xlines();
    const uword istart = times().indexOf(start);
    const uword istop  = times().indexOf(stop);

    fmat slice = zeros<fmat>(ilines.n_elem, xlines.n_elem);
    for (uword ic = 0; ic < xlines.n_elem; ic++) {
      for (uword ir = 0; ir < ilines.n_elem; ir++) {
        const uword index = survey_.index(ilines[ir], xlines[ic]);
        slice.at(ir, ic) = aggregate(traceData(index, istart, istop));
      }
    }
    return slice;
  }

public:

  fmat layerSlice(const fmat& surface) const;

  fmat layerSlice(const fmat& start, const fmat& stop) const;

  template <class FnT>
  fmat layerSlice(const fmat& start, const fmat& stop, const FnT& aggragate) const
  {
    const SegyTimes times = traceTime();
    const uvec     ilines = survey_.ilines();
    const uvec     xlines = survey_.xlines();
    fmat slice = zeros<fmat>(ilines.n_elem, xlines.n_elem);
    for (uword ic = 0; ic < xlines.n_elem; ic++) {
      for (uword ir = 0; ir < ilines.n_elem; ir++) {
        const uword index  = survey_.index(ilines[ir], xlines[ic]);
        const uword istart = times.indexOf(start.at(ir, ic));
        const uword istop  = times.indexOf(stop.at(ir, ic));
        slice.at(ir, ic) = aggragate(traceData(index, istart, istop));
      }
    }
    return slice;
  }

public:

  inline 
  uword nrow(void) const { 
    return survey_.ilineCount(); 
  }

  inline 
  uword ncol(void) const { 
    return survey_.ilineCount(); 
  } 
  
  inline 
  uword nslice(void) const { 
    return config().nsample; 
  }

  inline
  fvec tube(const uword ir, const uword ic) const {
    return trace(survey_.ilineMin() + ir, survey_.xlineMin() + ic);
  }

  inline
  fmat row(const uword ir) const {
    return iline(survey_.ilineMin() + ir);
  }

  inline
  fmat col(const uword ic) const {
    return xline(survey_.xlineMin() + ic);
  }

  fmat slice(const uword is) const;
};

} /*namespace SegyIO*/

#endif // LIB_SEGYIO_SEGYLOADER_H
