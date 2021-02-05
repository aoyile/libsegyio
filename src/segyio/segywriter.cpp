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

#include "segyio/survey.h"
#include "segyio/segywriter.h"

namespace SegyIO {

/*                       format =   0, 1, 2, 3, 4, 5, 6, 7, 8 */
static const size_t SEGY_SIZE[] = { 0u, 4u, 4u, 2u, 4u, 4u, 0u, 0u, 1u };
#define SEGY_SIZE SEGY_SIZE

SegyWriter::SegyWriter(void) 
  : pFileW_(NULL)
{ /* nothing needs to do */ }


SegyWriter::SegyWriter(const SegyConfig& config)
  : config_(config), pFileW_(NULL)
{ /* nothing needs to do */ }


SegyWriter::~SegyWriter(void) 
{
  close();
  reader_.close();
}

bool SegyWriter::referTo(const std::string& referSegyPath) {
  if (reader_.open(referSegyPath)) {
    config_ = reader_.config();
    return true;
  }
  return false;
}

bool SegyWriter::referTo(const SegyReader& reader) {
  return referTo(reader.path());
}

bool SegyWriter::open(const std::string& segyPath)
{
  SegyWriter::close();
  FILE* pFileW = fopen(segyPath.c_str(), "wb");
  if (pFileW != NULL) {
    pFileW_ = pFileW;
    outputPath_ = segyPath;
    return true;
  }
  return false;
}

void SegyWriter::close(void)
{
  if (pFileW_ != NULL)
    fclose(pFileW_);
  outputPath_ = string();
  pFileW_ = NULL;
}

void SegyWriter::writeAsciiHeader(const std::string& ascii) const
{
  UINT8 cache[3200];
  memset(&cache, 0, sizeof(UINT8)*3200l);
  std::copy(ascii.begin(), ascii.begin()+3200l, cache);
  gSegyEncodeASCII(cache, 3200l);
  if (fwrite(cache, sizeof(UINT8), 3200l, pFileW_) != 3200l)
    throw std::runtime_error("SegyWriter: fwrite(...) failed when it dumps ASCII header !");
}

void SegyWriter::writeAsciiHeader(void) const
{
  std::stringstream ss;
  ss << "C01  Created by: package GeoSeis                                               "
     << "C02     INLINE OFFSET = 5                                                      "
     << "C03  CORSSLINE OFFSET = 21                                                     ";
  for (uword i = 4; i < 39; ++i) {
    if (i < 9)
      ss << "C0" << std::setw(1) << i;
    else
      ss << "C"  << std::setw(2) << i;
    ss << "                                                                             ";
  }
  ss << "C39  SEG Y REV1                                                                 ";
  ss << "C40  END TEXTUAL HEADER                                                         ";
  writeAsciiHeader(ss.str());
}

void SegyWriter::writeBinaryHeader(const SegyBinaryHeader& header) const
{
  SegyBinaryHeader bHeader = header;
  if (config_.bswap)
    gSegyBSWAP(&bHeader);
  fseek(pFileW_, 3200l, SEEK_SET);
  if (fwrite(&bHeader, sizeof(SegyBinaryHeader), 1u, pFileW_) != 1u)
    throw std::runtime_error("SegyWriter: fwrite(...) failed when it dumps binary header !");
}

void SegyWriter::writeBinaryHeader(void) const
{
  SegyBinaryHeader bHeader;
  memset(&bHeader, 0, sizeof(bHeader));
  bHeader.hns    = config_.nsample;
  bHeader.hdt    = uword(config_.step * 1000.0);
  bHeader.format = config_.format;
  writeBinaryHeader(bHeader);
}

void SegyWriter::writeTraceHeader(const uword index, const SegyTraceHeader& header) const
{
  if (index == SEGYIO_INVALID_INDEX) return;

  SegyTraceHeader tHeader = header;
  if (config_.bswap)
    gSegyBSWAP(&tHeader);
  fseek(pFileW_, (3600l + (240l + config_.nsample * SEGY_SIZE[config_.format])*index), SEEK_SET);
  if (fwrite(&tHeader, sizeof(SegyTraceHeader), 1u, pFileW_) != 1u)
    throw std::runtime_error("SegyWriter: fwrite(...) failed when it dumps trace header !");
}

void SegyWriter::writeTraceHeader(const uword index, const uword iline, const uword xline) const
{
  SegyTraceHeader header;
  memset(&header, 0, sizeof(SegyTraceHeader));
  header.tracr = iline;
  header.ep    = xline;
  writeTraceHeader(index, header);
}

void SegyWriter::_dump_trace_data(FLOAT32* pCache, const uword index) const
{
  if (config_.format == SEGY_FMT_IBM_FLOAT32)
  {
    for(uword i= 0; i < config_.nsample; i++)
      pCache[i] = gIEEEtoIBM(pCache[i]);
    fseek(pFileW_, (3600l + (240l + config_.nsample * SEGY_SIZE[config_.format])*index + 240L), SEEK_SET);
    if (fwrite(pCache, sizeof(FLOAT32), config_.nsample, pFileW_) != config_.nsample)
      throw std::runtime_error("SegyWriter: fwrite(...) failed when it dumps trace data !");
  }
  else if (config_.format == SEGY_FMT_IEEE_FLOAT32)
  {
    if (config_.bswap) {
      for(uword i = 0; i < config_.nsample; i++)
        pCache[i] = gBSWAP(pCache[i]);
    }
    fseek(pFileW_, (3600l + (240l + config_.nsample * SEGY_SIZE[config_.format])*index + 240L), SEEK_SET);
    if (fwrite(pCache, sizeof(FLOAT32), config_.nsample, pFileW_) != config_.nsample)
      throw std::runtime_error("SegyWriter: fwriter(...) failed when it dumps trace data !");
  }
}

void SegyWriter::writeTraceData(const uword index, const fvec& data) const
{
  if (index == SEGYIO_INVALID_INDEX) return;
  
  fvec trace = zeros<fvec>(config_.nsample);
  std::copy(data.begin(), data.begin()+min(trace.n_elem, data.n_elem), trace.begin());
  _dump_trace_data(trace.memptr(), index);
}

void gSaveProfile(const fmat& profile, const SegyWriter& writer, const uvec& indexes)
{
  for (uword ic = 0; ic < indexes.n_elem; ++ic) {
    writer.copyTraceHeader(indexes[ic]);
    writer.writeTraceData(indexes[ic], profile.col(ic));
  }
}

void gSaveVolume(const fcube& volume, const SegyWriter& writer, const Survey& survey)
{
  assert(volume.n_rows == survey.ilineCount());
  assert(volume.n_cols == survey.xlineCount());
  assert(volume.n_slices == writer.config().nsample);

  const uvec ilines = survey.ilines();
  const uvec xlines = survey.xlines();
  for (uword ic = 0; ic < xlines.n_elem; ic++) {
    for (uword ir = 0; ir < ilines.n_elem; ir++) {
      const uword index = survey.index(ilines[ir], xlines[ic]);
      writer.copyTraceHeader(index);
      writer.writeTraceData(index, volume.tube(ir, ic));
    }
  }
}

} /*namespace SegyIO*/