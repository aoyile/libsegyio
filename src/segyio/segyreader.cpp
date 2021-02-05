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

#include "segyio/segyreader.h"
#include "segyio/survey.h"

namespace SegyIO {

/*                       format =   0, 1, 2, 3, 4, 5, 6, 7, 8 */
static const size_t SEGY_SIZE[] = { 0u, 4u, 4u, 2u, 4u, 4u, 0u, 0u, 1u };
#define SEGY_SIZE SEGY_SIZE

SegyReader::SegyReader(void)
  : pFileR_(NULL)
{ /* nothing needs to do */ }

SegyReader::SegyReader(const SegyReader& reader)
  : pFileR_(NULL)
{ open(reader.path()); }

SegyReader::~SegyReader(void) {
  close();
}

SegyReader& SegyReader::operator= (const SegyReader& reader) {
  open(reader.path());
  return *this;
}

bool SegyReader::_initilize(FILE* pFileR)
{
  pFileR_ = pFileR;

  SegyBinaryHeader bHeader;
  memset(&bHeader, 0, sizeof(SegyBinaryHeader));
  fseek(pFileR_, 3200l, SEEK_SET);
  if (fread(&bHeader, sizeof(SegyBinaryHeader), 1u, pFileR_) != 1u) {
    close();
    return false;
  }
  config_.bswap = bHeader.format > 16;

  if (config_.bswap)
    gSegyBSWAP(&bHeader);
  config_.nsample = bHeader.hns;
  config_.step    = bHeader.hdt / 1000.0f;
  config_.format  = bHeader.format;

  SegyTraceHeader tHeader;
  memset(&tHeader, 0, sizeof(SegyTraceHeader));
  fseek(pFileR_, 3600l, SEEK_SET);
  if (fread(&tHeader, sizeof(SegyTraceHeader), 1u, pFileR_) != 1u) {
    close();
    return false;
  }
  if (config_.bswap)
    gSegyBSWAP(&tHeader);
  config_.start = tHeader.delrt;

  return true;
}

bool SegyReader::open(const std::string& segyPath)
{
  close();
  FILE* pFileR = fopen(segyPath.c_str(), "rb");
  if (pFileR != NULL) {
    inputPath_ = segyPath;
    return _initilize(pFileR);
  }
  return false;
}

void SegyReader::close(void)
{
  if (pFileR_ != NULL)
    fclose(pFileR_);
  inputPath_ = string();
  config_ = SegyConfig();
  pFileR_ = NULL;
}

const SegyConfig& SegyReader::config(void) const {
  return config_;
}

const string& SegyReader::path(void) const {
  return inputPath_;
}

std::string SegyReader::asciiHeader(void) const
{
  UINT8 cache[3200];
  memset(&cache, 0, sizeof(UINT8)*3200l);
  fseek(pFileR_, 0, SEEK_SET);
  if (fread(cache, sizeof(UINT8), 3200l, pFileR_) != 3200l)
    throw std::runtime_error("SegyReader: fread(...) failed when it loads ASCII header !");
  if(! std::all_of(cache, cache+32, ::isprint))
    gSegyDecodeASCII(cache, 3200l);

  std::string ascii(3200, '\0');
  for (uword i = 0; i < 3200; ++i)
    ascii[i] = cache[i];
  return ascii;
}

SegyBinaryHeader SegyReader::binaryHeader(void) const
{
  SegyBinaryHeader header;
  memset(&header, 0, sizeof(SegyBinaryHeader));
  fseek(pFileR_, 3200l, SEEK_SET);
  if (fread(&header, sizeof(SegyBinaryHeader), 1u, pFileR_) != 1u)
    throw std::runtime_error("SegyReader: fread(...) failed when it loads binary header !");
  if (config_.bswap)
    gSegyBSWAP(&header);
  return header;
}

uword SegyReader::traceCount(void) const
{
  fseek(pFileR_, 0, SEEK_END);
  return (ftell(pFileR_) - 3600l) / (240l + config_.nsample * SEGY_SIZE[config_.format]);
}

SegyTraceHeader SegyReader::traceHeader(const uword index) const
{
  if (index == SEGYIO_INVALID_INDEX)
    return SegyTraceHeader();

  SegyTraceHeader header;
  memset(&header, 0, sizeof(SegyTraceHeader));
  fseek(pFileR_, (3600l + (240l + config_.nsample * SEGY_SIZE[config_.format])*index), SEEK_SET);
  if (fread(&header, sizeof(SegyTraceHeader), 1u, pFileR_) != 1u)
    throw std::runtime_error("SegyReader: fread(...) failed when it loads trace header !");
  if (config_.bswap)
    gSegyBSWAP(&header);
  return header;
}

SegyTimes SegyReader::traceTime(const uword index) const
{
  if (index == SEGYIO_INVALID_INDEX)
    return SegyTimes();

  SegyTraceHeader header;
  memset(&header, 0, sizeof(SegyTraceHeader));
  fseek(pFileR_, (3600l + (240l + config_.nsample * SEGY_SIZE[config_.format])*index), SEEK_SET);
  if (fread(&header, sizeof(SegyTraceHeader), 1u, pFileR_) != 1u)
    throw std::runtime_error("SegyReader: fread(...) failed when it loads trace header !");
  if (config_.bswap)
    gSegyBSWAP(&header);

  return SegyTimes(header.delrt, config_.step, config_.nsample);
}

void SegyReader::_load_trace_data(FLOAT32* pCache, const uword index) const
{
  fseek(pFileR_, (3600l + (240l + config_.nsample * SEGY_SIZE[config_.format])*index + 240L), SEEK_SET);
  if (fread(pCache, sizeof(FLOAT32), config_.nsample, pFileR_) != config_.nsample)
    throw std::runtime_error("SegyReader: fread(...) failed when it loads trace data !");

  if (config_.format == SEGY_FMT_IBM_FLOAT32) {
    for(uword i= 0; i < config_.nsample; i++)
      pCache[i] = gIBMtoIEEE(pCache[i]);
  } else if (config_.format == SEGY_FMT_IEEE_FLOAT32) {
    if (config_.bswap) {
      for(uword i = 0; i < config_.nsample; i++)
        pCache[i] = gBSWAP(pCache[i]);
    }
  }
}

fvec SegyReader::traceData(const uword index) const
{
  if (index == SEGYIO_INVALID_INDEX)
    return zeros<fvec>(config_.nsample);

  fvec trace = zeros<fvec>(config_.nsample);
  _load_trace_data(trace.memptr(), index);
  return trace;
}

void SegyReader::_load_trace_data(FLOAT32* pCache, const uword index, const uword istart, const uword istop) const
{
  const uword nsample = istop - istart + 1;
  const long int off = (3600l + (240l + config_.nsample * SEGY_SIZE[config_.format])*index + 240L);
  fseek(pFileR_, off + sizeof(FLOAT32)*istart, SEEK_SET);
  if (fread(pCache, sizeof(FLOAT32), nsample, pFileR_) != nsample)
    throw std::runtime_error("SegyReader: fread(...) failed when it loads trace data !");

  if (config_.format == SEGY_FMT_IBM_FLOAT32) {
    for(uword i= 0; i < nsample; i++)
      pCache[i] = gIBMtoIEEE(pCache[i]);
  } else if (config_.format == SEGY_FMT_IEEE_FLOAT32) {
    if (config_.bswap) {
      for(uword i = 0; i < nsample; i++)
        pCache[i] = gBSWAP(pCache[i]);
    }
  }
}

fvec SegyReader::traceData(const uword index, const uword istart, const uword istop) const 
{
  assert(istart <= config_.nsample);
  assert(istop  <= config_.nsample);
  
  if (index == SEGYIO_INVALID_INDEX)
    return zeros<fvec>(config_.nsample);

  fvec trace = zeros<fvec>(istop - istart + 1);
  _load_trace_data(trace.memptr(), index, istart, istop);
  return trace;
}

void SegyReader::_load_trace_data(FLOAT32* pCache, const uword index, const uword itime) const
{
  const long int off = (3600l + (240l + config_.nsample * SEGY_SIZE[config_.format])*index + 240L);
  fseek(pFileR_, off + sizeof(FLOAT32)*itime, SEEK_SET);
  if (fread(pCache, sizeof(FLOAT32), 1, pFileR_) != 1)
    throw std::runtime_error("SegyReader: fread(...) failed when it loads trace data !");

  if (config_.format == SEGY_FMT_IBM_FLOAT32) {
    pCache[0] = gIBMtoIEEE(pCache[0]);
  } else if (config_.format == SEGY_FMT_IEEE_FLOAT32) {
    if (config_.bswap)
      pCache[0] = gBSWAP(pCache[0]);
  }
}

float SegyReader::traceData(const uword index, const uword itime) const 
{
  assert(itime <= config_.nsample);
  
  if (index == SEGYIO_INVALID_INDEX)
    return 0.0;

  float trace = 0.0;
  _load_trace_data(&trace, index, itime);
  return trace;
}

uword SegyReader::traceLine(const uword index, const uword offset) const
{
  if (index == SEGYIO_INVALID_INDEX)
    return SEGYIO_INVALID_INDEX;

  UINT16 line = 0u;
  const long int fpos = 3600l + (240l + config_.nsample * SEGY_SIZE[config_.format])*index + offset + 1l;
  fseek(pFileR_, fpos, SEEK_SET);
  if (fread(&line, sizeof(UINT16), 1u, pFileR_) != 1u)
    throw std::runtime_error("SegyReader: fread(...) failed when it loads trace line !");
  line = config_.bswap ? gBSWAP(line) : line;
  return line;
}

fmat gLoadProfile(const SegyReader& reader, const uvec& indexes)
{
  const uword nsample = reader.config().nsample;
  fmat profile = zeros<fmat>(nsample, indexes.n_elem);
  for (uword ic = 0; ic < indexes.n_elem; ++ic)
    profile.col(ic) = reader.traceData(indexes[ic]);
  return profile;
}

fcube gLoadVolume(const SegyReader& reader, const Survey& survey)
{
  const uvec ilines = survey.ilines();
  const uvec xlines = survey.xlines();
  fcube volume = zeros<fcube>(ilines.n_elem, xlines.n_elem, reader.config().nsample);
  for (uword ic = 0; ic < xlines.n_elem; ic++) {
    for (uword ir = 0; ir < ilines.n_elem; ir++) {
      const uword index = survey.index(ilines[ir], xlines[ic]);
      volume.tube(ir, ic) = reader.traceData(index);
    }
  }
  return volume;
}

} /*namespace SegyIO*/