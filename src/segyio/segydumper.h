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

#ifndef LIB_SEGYIO_SEGYDUMPER_H
#define LIB_SEGYIO_SEGYDUMPER_H

#include "segyio/survey.h"
#include "segyio/segywriter.h"
#include "segyio/segyloader.h"

namespace SegyIO {

class SegyDumper : public SegyWriter
{
private:

  Survey survey_;

public:

  SegyDumper(void)
  { /* nothing needs to do */ }

  SegyDumper(const SegyConfig& config, const Survey& survey)
   : SegyWriter(config), survey_(survey)
  { /* nothing needs to do */ }

  virtual ~SegyDumper(void)
  { /* nothing needs to do */ }

public:

  virtual bool referTo(const std::string& referSegyPath, const uword ilineOffset, const uword xlineOffset);

  virtual bool referTo(const std::string& referSegyPath, const Survey& survey);

  virtual bool referTo(const SegyReader& reader, const uword ilineOffset, const uword xlineOffset);

  virtual bool referTo(const SegyReader& reader, const Survey& survey);

  virtual bool referTo(const SegyLoader& loader);

  virtual bool open(const std::string& segyPath);

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

  void dumpTrace(const uword iline, const uword xline, const fvec& trace) const;

  void dumpProfile(const uvec& ilines, const uvec& xlines, const fmat& profile) const;

  void dumpILINE(const uword iline, const fmat& profile) const;

  void dumpXLINE(const uword xline, const fmat& profile) const;

public:

  void dumpVolume(const fcube& volume) const;

  void dumpILINE(const uvec& ilines, const fcube& volume) const;

  void dumpXLINE(const uvec& xlines, const fcube& volume) const;

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

public:

  void dumpTube(const uword ir, const uword ic, const fvec& trace) const {
    dumpTrace(survey_.ilineMin()+ir, survey_.xlineMin()+ic, trace);
  }

  void dumpRow(const uword ir, const fmat& profile) const {
    dumpILINE(survey_.ilineMin()+ir, profile);
  }

  void dumpCol(const uword ic, const fmat& profile) const {
    dumpXLINE(survey_.xlineMin()+ic, profile);
  }
};

} /*namespace SegyIO*/

#endif // LIB_SEGYIO_SEGYDUMPER_H
