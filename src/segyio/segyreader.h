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

#ifndef LIB_SEGYIO_SEGYREADER_H
#define LIB_SEGYIO_SEGYREADER_H

#include "segyio/segydef.h"

namespace SegyIO {

struct SegyConfig
{
public:

  bool  bswap;
  uword format;
  uword nsample;
  float start;
  float step;

public:

  SegyConfig(void)
    : bswap(false),
      format(SEGY_FMT_IBM_FLOAT32),
      nsample(0u),
      start(0.0f),
      step(0.0f)
  { /* nothing needs to do */ }
};

struct SegyTimes
{
public:

  float start;
  float stop;
  float step;
  uword nsample;

public:

  SegyTimes(void)
    : start(0.0), stop(0.0), step(0.0), nsample(0)
  { /* nothing needs to do */ }

  SegyTimes(const float start_, const float step_, const uword nsample_)
    : start(start_), stop(start_ + nsample_*step_), step(step_), nsample(nsample_)
  { /* nothing needs to do */ }

public:

  float correct(const float time) const {
    if (isnan(time) || isinf(time)) return NAN;
    return max(min(time, stop), start);
  }

  uword indexOf(const float time) const {
    if (isnan(time) || isinf(time)) return SEGYIO_INVALID_INDEX;
    return round((correct(time) - start)/step);
  }

  float at(const uword i) const { 
    return start + i*step; 
  }

  float operator [] (const uword i) const { 
    return start + i*step; 
  }

  operator fvec(void) const {
    fvec times = zeros<fvec>(nsample);
    for (uword i = 0; i < nsample; ++i)
      times[i] = start + step * i;
    return times;
  }
};

class SegyReader
{
protected:

  string     inputPath_;
  SegyConfig config_;
  FILE*      pFileR_;

private:

  bool _initilize(FILE* pFileR);

  void _load_trace_data(FLOAT32* pCache, const uword index) const;

  void _load_trace_data(FLOAT32* pCache, const uword index, const uword istart, const uword istop) const;

  void _load_trace_data(FLOAT32* pCache, const uword index, const uword itime) const;

public:

  SegyReader(void);

  SegyReader(const SegyReader& reader);

  virtual ~SegyReader(void);

  virtual SegyReader& operator= (const SegyReader& reader);

public:

  virtual bool open(const std::string& segyPath);

  virtual void close(void);

public:

  bool isINVALID(void) const {
    return pFileR_ == NULL;
  }

public:

  const SegyConfig& config(void) const;

  const string& path(void) const;

  std::string asciiHeader(void) const;

  SegyBinaryHeader binaryHeader(void) const;

  uword traceCount(void) const;

  SegyTraceHeader traceHeader(const uword index) const;

  SegyTimes traceTime(const uword index = 0) const;

  fvec traceData(const uword index) const;

  fvec traceData(const uword index, const uword istart, const uword istop) const;

  float traceData(const uword index, const uword itime) const;

  uword traceLine(const uword index, const uword offset) const;
};

class Survey;

fmat gLoadProfile(const SegyReader& reader, const uvec& indexes);

fcube gLoadVolume(const SegyReader& reader, const Survey& survey);

} /*namespace SegyIO*/

#endif // LIB_SEGYIO_SEGYREADER_H
