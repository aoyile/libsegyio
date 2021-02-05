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

#include "segyio/segydumper.h"

namespace SegyIO {

bool SegyDumper::referTo(const std::string& referSegyPath, const uword ilineOffset, const uword xlineOffset) {
  if (SegyWriter::referTo(referSegyPath)) {
    survey_ = Survey::From(reader_, ilineOffset, xlineOffset);
    return true;
  }
  return false;
}

bool SegyDumper::referTo(const std::string& referSegyPath, const Survey& survey) {
  if (SegyWriter::referTo(referSegyPath)) {
    survey_ = survey;
    return true;
  }
  return false;
}

bool SegyDumper::referTo(const SegyReader& reader, const uword ilineOffset, const uword xlineOffset) {
  return referTo(reader.path(), ilineOffset, xlineOffset);
}

bool SegyDumper::referTo(const SegyReader& reader, const Survey& survey) {
  return referTo(reader.path(), survey);
}

bool SegyDumper::referTo(const SegyLoader& loader) {
  return referTo(loader.path(), loader.survey());
}

bool SegyDumper::open(const std::string& segyPath) {
  if (SegyWriter::open(segyPath)) {
    if (reader_.isINVALID()) {
      writeAsciiHeader();
      writeBinaryHeader();
    } else {
      copyAsciiHeader();
      copyBinaryHeader();
    }
    return true;
  }
  return false;
}

void SegyDumper::close(void) {
  SegyWriter::close();
}

void SegyDumper::dumpTrace(const uword iline, const uword xline, const fvec& trace) const 
{
  const uword index = survey_.index(iline, xline);
  if (reader_.isINVALID())
    writeTraceHeader(index, iline, xline);
  else
    copyTraceHeader(index);
  writeTraceData(index, trace);
}

void SegyDumper::dumpProfile(const uvec& ilines, const uvec& xlines, const fmat& profile) const
{
  assert(ilines.n_elem  == xlines.n_elem);
  assert(profile.n_cols == ilines.n_elem);
  assert(profile.n_rows == config_.nsample);
  
  const uvec indexes = survey_.indexes(ilines, xlines);
  for (uword ic = 0; ic < indexes.n_elem; ++ic) {
    if (reader_.isINVALID())
      writeTraceHeader(indexes[ic], ilines[ic], xlines[ic]);
    else
      copyTraceHeader(indexes[ic]);
    writeTraceData(indexes[ic], profile.col(ic));
  }
}

void SegyDumper::dumpILINE(const uword iline, const fmat& profile) const
{
  assert(profile.n_cols == survey().xlineCount());
  assert(profile.n_rows == config_.nsample);
  
  const uvec xlines  = survey_.xlines();
  const uvec indexes = survey_.indexes(iline, xlines);
  for (uword ic = 0; ic < indexes.n_elem; ++ic) {
    if (reader_.isINVALID())
      writeTraceHeader(indexes[ic], iline, xlines[ic]);
    else
      copyTraceHeader(indexes[ic]);
    writeTraceData(indexes[ic], profile.col(ic));
  }
}

void SegyDumper::dumpXLINE(const uword xline, const fmat& profile) const
{
  assert(profile.n_cols == survey().ilineCount());
  assert(profile.n_rows == config_.nsample);
  
  const uvec ilines  = survey_.ilines();
  const uvec indexes = survey_.indexes(ilines, xline);
  for (uword ic = 0; ic < indexes.n_elem; ++ic) {
    if (reader_.isINVALID())
      writeTraceHeader(indexes[ic], ilines[ic], xline);
    else
      copyTraceHeader(indexes[ic]);
    writeTraceData(indexes[ic], profile.col(ic));
  }
}

void SegyDumper::dumpVolume(const fcube& volume) const
{
  assert(volume.n_rows == survey().ilineCount());
  assert(volume.n_cols == survey().xlineCount());
  assert(volume.n_slices == config().nsample);

  const uvec ilines = survey_.ilines();
  const uvec xlines = survey_.xlines();
  for (uword ic = 0; ic < xlines.n_elem; ic++) {
    for (uword ir = 0; ir < ilines.n_elem; ir++) {
      const uword index = survey_.index(ilines[ir], xlines[ic]);
      if (reader_.isINVALID())
        writeTraceHeader(index, ilines[ir], xlines[ic]);
      else
        copyTraceHeader(index);
      writeTraceData(index, volume.tube(ir, ic));
    }
  }
}

void SegyDumper::dumpILINE(const uvec& ilines, const fcube& volume) const
{
  assert(volume.n_rows == ilines.n_elem);
  assert(volume.n_cols == survey().xlineCount());
  assert(volume.n_slices == config().nsample);

  const uvec xlines = survey_.xlines();
  for (uword ic = 0; ic < xlines.n_elem; ic++) {
    for (uword ir = 0; ir < ilines.n_elem; ir++) {
      const uword index = survey_.index(ilines[ir], xlines[ic]);
      if (reader_.isINVALID())
        writeTraceHeader(index, ilines[ir], xlines[ic]);
      else
        copyTraceHeader(index);
      writeTraceData(index, volume.tube(ir, ic));
    }
  }
}

void SegyDumper::dumpXLINE(const uvec& xlines, const fcube& volume) const
{
  assert(volume.n_rows == survey().ilineCount());
  assert(volume.n_cols == xlines.n_elem);
  assert(volume.n_slices == config().nsample);

  const uvec ilines = survey_.ilines();
  for (uword ic = 0; ic < xlines.n_elem; ic++) {
    for (uword ir = 0; ir < ilines.n_elem; ir++) {
      const uword index = survey_.index(ilines[ir], xlines[ic]);
      if (reader_.isINVALID())
        writeTraceHeader(index, ilines[ir], xlines[ic]);
      else
        copyTraceHeader(index);
      writeTraceData(index, volume.tube(ir, ic));
    }
  }
}

} /*namespace SegyIO*/