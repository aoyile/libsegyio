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

#include "segyio/segyloader.h"

namespace SegyIO {

bool SegyLoader::open(const std::string& segyPath) {
  assert(! survey_.isInvalid());
  return SegyReader::open(segyPath);
}

bool SegyLoader::open(const std::string& segyPath, const Survey& survey) {
  if (SegyReader::open(segyPath)) {
    survey_ = survey;
    return true;
  }
  return false;
}

bool SegyLoader::open(const std::string& segyPath, const uword ilineOffset, const uword xlineOffset)
{
  if (SegyReader::open(segyPath)) {
    survey_ = Survey::From(*this, ilineOffset, xlineOffset);
    return true;
  }
  return false;
}

void SegyLoader::close(void) {
  SegyReader::close();
}

fmat SegyLoader::iline(const uword iline) const 
{
  const uvec indexes = survey_.indexes(iline, survey_.xlines());
  fmat profile = zeros<fmat>(config_.nsample, indexes.n_elem);
  for (uword ic = 0; ic < indexes.n_elem; ++ic)
    profile.col(ic) = traceData(indexes[ic]);
  return profile;
}

fmat SegyLoader::xline(const uword xline) const
{
  const uvec indexes = survey_.indexes(survey_.ilines(), xline);
  fmat profile = zeros<fmat>(config_.nsample, indexes.n_elem);
  for (uword ic = 0; ic < indexes.n_elem; ++ic)
    profile.col(ic) = traceData(indexes[ic]);
  return profile;
}

fmat SegyLoader::profile(const uvec& ilines, const uvec& xlines) const
{
  const uvec indexes = survey_.indexes(ilines, xlines);
  fmat profile = zeros<fmat>(config_.nsample, indexes.n_elem);
  for (uword ic = 0; ic < indexes.n_elem; ++ic)
    profile.col(ic) = traceData(indexes[ic]);
  return profile;
}

fcube SegyLoader::volume(void) const
{
  const uvec ilines = survey_.ilines();
  const uvec xlines = survey_.xlines();
  fcube volume = zeros<fcube>(ilines.n_elem, xlines.n_elem, config_.nsample);
  for (uword ic = 0; ic < xlines.n_elem; ic++) {
    for (uword ir = 0; ir < ilines.n_elem; ir++) {
      const uword index = survey_.index(ilines[ir], xlines[ic]);
      volume.tube(ir, ic) = traceData(index);
    }
  }
  return volume;
}

fcube SegyLoader::ilines(const uvec& ilines) const
{
  const uvec xlines = survey_.xlines();
  fcube volume = zeros<fcube>(ilines.n_elem, xlines.n_elem, config_.nsample);
  for (uword ic = 0; ic < xlines.n_elem; ic++) {
    for (uword ir = 0; ir < ilines.n_elem; ir++) {
      const uword index = survey_.index(ilines[ir], xlines[ic]);
      volume.tube(ir, ic) = traceData(index);
    }
  }
  return volume;
}

fcube SegyLoader::xlines(const uvec& xlines) const
{
  const uvec ilines = survey_.ilines();
  fcube volume = zeros<fcube>(ilines.n_elem, xlines.n_elem, config_.nsample);
  for (uword ic = 0; ic < xlines.n_elem; ic++) {
    for (uword ir = 0; ir < ilines.n_elem; ir++) {
      const uword index = survey_.index(ilines[ir], xlines[ic]);
      volume.tube(ir, ic) = traceData(index);
    }
  }
  return volume;
}

fmat SegyLoader::timeSlice(const float time) const
{
  const SegyTimes times = traceTime();
  const float ifloat = (times.correct(time) - times.start) / times.step;

  const uvec ilines = survey_.ilines();
  const uvec xlines = survey_.xlines();
  fmat slice = zeros<fmat>(ilines.n_elem, xlines.n_elem);
  for (uword ic = 0; ic < xlines.n_elem; ic++) {
    for (uword ir = 0; ir < ilines.n_elem; ir++) {
      const uword index = survey_.index(ilines[ir], xlines[ic]);
      const float xupper = traceData(index, ceil(ifloat));
      const float xlower = traceData(index, floor(ifloat));
      slice.at(ir, ic) = xlower + (ifloat - floor(ifloat)) * (xupper - xlower);
    }
  }
  return slice;
}

fmat SegyLoader::timeSlice(const float start, const float stop) const
{
  const uvec  ilines = survey_.ilines();
  const uvec  xlines = survey_.xlines();
  const uword istart = times().indexOf(start);
  const uword istop  = times().indexOf(stop);

  fmat slice = zeros<fmat>(ilines.n_elem, xlines.n_elem);
  for (uword ic = 0; ic < xlines.n_elem; ic++) {
    for (uword ir = 0; ir < ilines.n_elem; ir++) {
      const uword index = survey_.index(ilines[ir], xlines[ic]);
      slice.at(ir, ic) = mean(traceData(index, istart, istop));
    }
  }
  return slice;
}

fmat SegyLoader::layerSlice(const fmat& surface) const 
{
  const SegyTimes times = traceTime();
  const uvec     ilines = survey_.ilines();
  const uvec     xlines = survey_.xlines();
  fmat slice = zeros<fmat>(ilines.n_elem, xlines.n_elem);
  for (uword ic = 0; ic < xlines.n_elem; ic++) {
    for (uword ir = 0; ir < ilines.n_elem; ir++) {
      const uword index = survey_.index(ilines[ir], xlines[ic]);
      const float  time = times.correct(surface.at(ir, ic));
      if (isnan(time) || isinf(time)) {
        slice.at(ir, ic) = NAN;
      } else {
        const float ifloat = (time - times.start) / times.step;
        const float xupper = traceData(index, ceil(ifloat));
        const float xlower = traceData(index, floor(ifloat));
        slice.at(ir, ic) = xlower + (ifloat - floor(ifloat)) * (xupper - xlower);
      }
    }
  }
  return slice;
}

fmat SegyLoader::layerSlice(const fmat& start, const fmat& stop) const
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
      slice.at(ir, ic) = mean(traceData(index, istart, istop));
    }
  }
  return slice;
}

fmat SegyLoader::slice(const uword is) const 
{
  const uword itime = min(is, config_.nsample-1u);
  const uvec  ilines = survey_.ilines();
  const uvec  xlines = survey_.xlines();
  fmat slice = zeros<fmat>(ilines.n_elem, xlines.n_elem);
  for (uword ic = 0; ic < xlines.n_elem; ic++) {
    for (uword ir = 0; ir < ilines.n_elem; ir++) {
      const uword index = survey_.index(ilines[ir], xlines[ic]);
      slice.at(ir, ic) = traceData(index, itime);
    }
  }
  return slice;
}

} /*namespace SegyIO*/