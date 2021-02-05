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

#ifndef LIB_SEGYIO_SEGYWRITER_H
#define LIB_SEGYIO_SEGYWRITER_H

#include "segyio/segyreader.h"

namespace SegyIO {

class SegyWriter
{
protected:
  
  SegyConfig config_;
  string     outputPath_;
  FILE*      pFileW_;
  SegyReader reader_;
  
private:

  void _dump_trace_data(FLOAT32* pCache, const uword index) const;

public:

  SegyWriter(void);

  SegyWriter(const SegyConfig& config);

  virtual ~SegyWriter(void);

public:

  bool isINVALID(void) const {
    return pFileW_ == NULL;
  }

public:

  virtual bool referTo(const std::string& referSegyPath);

  virtual bool referTo(const SegyReader& reader);

  virtual bool open(const std::string& segyPath);

  virtual void close(void);

public:

  const string& path(void) const {
    return outputPath_;
  }

  const SegyConfig& config(void) const {
    return config_;
  }

public:

  void writeAsciiHeader(const std::string& ascii) const;

  void writeAsciiHeader(void) const;

  void writeBinaryHeader(const SegyBinaryHeader& header) const;

  void writeBinaryHeader(void) const;

  void writeTraceHeader(const uword index, const SegyTraceHeader& header) const;

  void writeTraceHeader(const uword index, const uword iline, const uword xline) const;

  void writeTraceData(const uword index, const fvec& data) const;
  
public:

  void copyAsciiHeader(void) const {
    writeAsciiHeader(reader_.asciiHeader());
  }

  void copyBinaryHeader(void) const {
    writeBinaryHeader(reader_.binaryHeader());
  }

  void copyTraceHeader(const uword index) const {
    if (index == SEGYIO_INVALID_INDEX) return;
    writeTraceHeader(index, reader_.traceHeader(index));
  }

  void copyTraceData(const uword index) const {
    if (index == SEGYIO_INVALID_INDEX) return;
    writeTraceData(index, reader_.traceData(index));
  }
};

class Survey;

void gSaveProfile(const fmat& profile, const SegyWriter& writer, const uvec& indexes);

void gSaveVolume(const fcube& volume, const SegyWriter& writer, const Survey& survey);

} /*namespace SegyIO*/

#endif // LIB_SEGYIO_SEGYWRITER_H
