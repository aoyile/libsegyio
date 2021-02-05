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

#include <Rcpp.h>
#include "segyio/segyio.h"

using namespace arma;
using namespace SegyIO;

RcppExport
SEXP _SEGYIO_VERSION(void) {
  BEGIN_RCPP {
    return Rcpp::wrap(SEGYIO_VERSION);
  } END_RCPP
}

/*********************************************************************************************
 * 
 *                           Convert between Rcpp and arma
 * 
 ********************************************************************************************/

inline
fvec _toFVEC(SEXP rvec) {
  const Rcpp::NumericVector rcppVec = Rcpp::as<Rcpp::NumericVector>(rvec);
  fvec result(rcppVec.size());
  std::copy(rcppVec.begin(), rcppVec.end(), result.begin());
  return result;
}

inline
uvec _toUVEC(SEXP rvec) {
  const Rcpp::IntegerVector rcppVec = Rcpp::as<Rcpp::IntegerVector>(rvec);
  uvec result(rcppVec.size());
  std::copy(rcppVec.begin(), rcppVec.end(), result.begin());
  return result;
}

inline
fmat _toFMAT(SEXP rmat) {
  const Rcpp::NumericMatrix rcppMat = Rcpp::as<Rcpp::NumericMatrix>(rmat);
  fmat result(rcppMat.nrow(), rcppMat.ncol());
  std::copy(rcppMat.begin(), rcppMat.end(), result.begin());
  return result;
}

inline
umat _toUMAT(SEXP rmat) {
  const Rcpp::IntegerMatrix rcppMat = Rcpp::as<Rcpp::IntegerMatrix>(rmat);
  umat result(rcppMat.nrow(), rcppMat.ncol());
  std::copy(rcppMat.begin(), rcppMat.end(), result.begin());
  return result;
}

inline
fcube _toFCUBE(SEXP rcube) {
  const Rcpp::NumericVector rcppCube = Rcpp::as<Rcpp::NumericVector>(rcube);
  const Rcpp::IntegerVector dim = rcppCube.attr("dim");
  fcube result(dim[0], dim[1], dim[2]);
  std::copy(rcppCube.begin(), rcppCube.end(), result.begin());
  return result;
}

inline
SEXP _toSEXP(const fvec& vec) {
  const Rcpp::NumericVector rcppVec(vec.begin(), vec.end());
  return Rcpp::wrap(rcppVec);
}

inline
SEXP _toSEXP(const uvec& vec) {
  const Rcpp::IntegerVector rcppVec(vec.begin(), vec.end());
  return Rcpp::wrap(rcppVec);
}

inline
SEXP _toSEXP(const fmat& mat) {
  const Rcpp::NumericMatrix rcppMat(mat.n_rows, mat.n_cols, mat.begin());
  return Rcpp::wrap(rcppMat);
}

inline
SEXP _toSEXP(const umat& mat) {
  const Rcpp::IntegerMatrix rcppMat(mat.n_rows, mat.n_cols, mat.begin());
  return Rcpp::wrap(rcppMat);
}

inline
SEXP _toSEXP(const fcube& cube) {
  Rcpp::NumericVector rcppCube(cube.begin(), cube.end());
  rcppCube.attr("dim") = Rcpp::Dimension(cube.n_rows, cube.n_cols, cube.n_slices);
  return Rcpp::wrap(rcppCube);
}

/*********************************************************************************************
 * 
 *                                  XPtr for SegyIO Class
 * 
 ********************************************************************************************/

typedef Rcpp::XPtr<Survey>     SurveyXPtr;
typedef Rcpp::XPtr<SegyReader> SegyReaderXPtr;
typedef Rcpp::XPtr<SegyLoader> SegyLoaderXPtr;
typedef Rcpp::XPtr<SegyWriter> SegyWriterXPtr;
typedef Rcpp::XPtr<SegyDumper> SegyDumperXPtr;

/*********************************************************************************************
 * 
 *                                          Segy Survey
 * 
 ********************************************************************************************/

RcppExport
SEXP _RegularSurvey(SEXP ilineMin, SEXP ilineMax, SEXP xlineMin, SEXP xlineMax) 
{
  BEGIN_RCPP {
    Survey* survey = new Survey(Survey::Regular(
      Rcpp::as<uword>(ilineMin), 
      Rcpp::as<uword>(ilineMax), 
      Rcpp::as<uword>(xlineMin), 
      Rcpp::as<uword>(xlineMax)
    ));
    return Rcpp::wrap(SurveyXPtr(survey));
  } END_RCPP
}

RcppExport
SEXP _gSurveyFrom(SEXP reader, SEXP ilineOffset, SEXP xlineOffset) 
{
  BEGIN_RCPP {
    Survey* survey = new Survey(Survey::From(
      *Rcpp::as<SegyReaderXPtr>(reader), 
      Rcpp::as<uword>(ilineOffset), 
      Rcpp::as<uword>(xlineOffset)
    ));
    return Rcpp::wrap(SurveyXPtr(survey));
  } END_RCPP
}

RcppExport
SEXP _Survey_all_ilines(SEXP survey) {
  BEGIN_RCPP {
    const uvec line = Rcpp::as<SurveyXPtr>(survey)->ilines();
    return _toSEXP(line);
  } END_RCPP
}

RcppExport
SEXP _Survey_all_xlines(SEXP survey) {
  BEGIN_RCPP {
    const uvec line = Rcpp::as<SurveyXPtr>(survey)->xlines();
    return _toSEXP(line);
  } END_RCPP
}

RcppExport
SEXP _Survey_iline(SEXP survey, SEXP index) {
  BEGIN_RCPP {
    const uword line = Rcpp::as<SurveyXPtr>(survey)->iline(
      Rcpp::as<uword>(index)
    );
    return Rcpp::wrap(line);
  } END_RCPP
}

RcppExport
SEXP _Survey_ilines(SEXP survey, SEXP index) {
  BEGIN_RCPP {
    const uvec line = Rcpp::as<SurveyXPtr>(survey)->ilines(
      _toUVEC(index)
    );
    return _toSEXP(line);
  } END_RCPP
}

RcppExport
SEXP _Survey_xline(SEXP survey, SEXP index) {
  BEGIN_RCPP {
    const uword line = Rcpp::as<SurveyXPtr>(survey)->xline(
      Rcpp::as<uword>(index)
    );
    return Rcpp::wrap(line);
  } END_RCPP
}

RcppExport
SEXP _Survey_xlines(SEXP survey, SEXP index) {
  BEGIN_RCPP {
    const uvec line = Rcpp::as<SurveyXPtr>(survey)->xlines(
      _toUVEC(index)
    );
    return _toSEXP(line);
  } END_RCPP
}

RcppExport
SEXP _Survey_index(SEXP survey, SEXP iline, SEXP xline) {
  BEGIN_RCPP {
    const uword index = Rcpp::as<SurveyXPtr>(survey)->index(
      Rcpp::as<uword>(iline), Rcpp::as<uword>(xline)
    );
    return Rcpp::wrap(index);
  } END_RCPP
}

RcppExport
SEXP _Survey_indexes_sv(SEXP survey, SEXP iline, SEXP xlines) {
  BEGIN_RCPP {
    const uvec indexes = Rcpp::as<SurveyXPtr>(survey)->indexes(
      Rcpp::as<uword>(iline), _toUVEC(xlines)
    );
    return _toSEXP(indexes);
  } END_RCPP
}

RcppExport
SEXP _Survey_indexes_vs(SEXP survey, SEXP iline, SEXP xline) {
  BEGIN_RCPP {
    const uvec indexes = Rcpp::as<SurveyXPtr>(survey)->indexes(
      _toUVEC(iline), Rcpp::as<uword>(xline)
    );
    return _toSEXP(indexes);
  } END_RCPP
}

RcppExport
SEXP _Survey_indexes_vv(SEXP survey, SEXP ilines, SEXP xlines) {
  BEGIN_RCPP {
    const uvec indexes = Rcpp::as<SurveyXPtr>(survey)->indexes(
      _toUVEC(ilines), _toUVEC(xlines)
    );
    return _toSEXP(indexes);
  } END_RCPP
}

/*********************************************************************************************
 * 
 *                                          Segy Reader
 * 
 ********************************************************************************************/

RcppExport
SEXP _SegyReader(SEXP segyPath) {
  BEGIN_RCPP {
    SegyReader* reader = new SegyReader();
    reader->open(Rcpp::as<string>(segyPath));
    return Rcpp::wrap(SegyReaderXPtr(reader));
  } END_RCPP
}

RcppExport
SEXP _SegyReader_open(SEXP reader, SEXP segyPath) {
  BEGIN_RCPP {
    bool ok = Rcpp::as<SegyReaderXPtr>(reader)->open(Rcpp::as<string>(segyPath));
    return Rcpp::wrap(ok);
  } END_RCPP
}

RcppExport
SEXP _SegyReader_close(SEXP reader) {
  BEGIN_RCPP {
    Rcpp::as<SegyReaderXPtr>(reader)->close();
    return Rcpp::wrap(0);
  } END_RCPP
}

RcppExport
SEXP _SegyReader_asciiHeader(SEXP reader) {
  BEGIN_RCPP {
    const std::string ascii = Rcpp::as<SegyReaderXPtr>(reader)->asciiHeader();
    return Rcpp::wrap(ascii);
  } END_RCPP
}

RcppExport
SEXP _SegyReader_binaryHeader(SEXP reader) {
  BEGIN_RCPP {
    const SegyBinaryHeader header = Rcpp::as<SegyReaderXPtr>(reader)->binaryHeader();
    Rcpp::RawVector raw(sizeof(header));
    memcpy(&raw[0], &header, sizeof(header));
    return Rcpp::wrap(raw);
  } END_RCPP
}

RcppExport
SEXP _SegyReader_traceCount(SEXP reader) {
  BEGIN_RCPP {
    const uword n = Rcpp::as<SegyReaderXPtr>(reader)->traceCount();
    return Rcpp::wrap(n);
  } END_RCPP
}

RcppExport
SEXP _SegyReader_traceHeader(SEXP reader, SEXP index) {
  BEGIN_RCPP {
    const SegyTraceHeader header = Rcpp::as<SegyReaderXPtr>(reader)->traceHeader(
      Rcpp::as<uword>(index) );
    Rcpp::RawVector raw(sizeof(header));
    memcpy(&raw[0], &header, sizeof(header));
    return Rcpp::wrap(raw);
  } END_RCPP
}

RcppExport
SEXP _SegyReader_traceTime(SEXP reader, SEXP index) {
  BEGIN_RCPP {
    const fvec time = Rcpp::as<SegyReaderXPtr>(reader)->traceTime(
      Rcpp::as<uword>(index) );
    return _toSEXP(time);
  } END_RCPP
}

RcppExport
SEXP _SegyReader_traceData(SEXP reader, SEXP index) {
  BEGIN_RCPP {
    const fvec data = Rcpp::as<SegyReaderXPtr>(reader)->traceData(
      Rcpp::as<uword>(index) );
    return _toSEXP(data);
  } END_RCPP
}

RcppExport
SEXP _SegyReader_traceLine(SEXP reader, SEXP index, SEXP offset) {
  BEGIN_RCPP {
    const uword line = Rcpp::as<SegyReaderXPtr>(reader)->traceLine(
      Rcpp::as<uword>(index), Rcpp::as<uword>(offset) );
    return Rcpp::wrap(line);
  } END_RCPP
}

/*********************************************************************************************
 * 
 *                                      Segy Loader
 * 
 ********************************************************************************************/

RcppExport
SEXP _SegyLoader(SEXP segyPath, SEXP ilineOffset, SEXP xlineOffset) {
  BEGIN_RCPP {
    SegyLoader* loader = new SegyLoader();
    loader->open(
      Rcpp::as<string>(segyPath),
      Rcpp::as<uword>(ilineOffset),
      Rcpp::as<uword>(xlineOffset)
    );
    return Rcpp::wrap(SegyLoaderXPtr(loader));
  } END_RCPP
}

RcppExport
SEXP _SegyLoader2(SEXP segyPath, SEXP survey) {
  BEGIN_RCPP {
    SegyLoader* loader = new SegyLoader();
    loader->open(
      Rcpp::as<string>(segyPath),
      *(Rcpp::as<SurveyXPtr>(survey))
    );
    return Rcpp::wrap(SegyLoaderXPtr(loader));
  } END_RCPP
}

RcppExport
SEXP _SegyLoader_open(SEXP loader, SEXP segyPath, SEXP ilineOffset, SEXP xlineOffset) {
  BEGIN_RCPP {
    bool ok = Rcpp::as<SegyLoaderXPtr>(loader)->open(
      Rcpp::as<string>(segyPath),
      Rcpp::as<uword>(ilineOffset),
      Rcpp::as<uword>(xlineOffset)
    );
    return Rcpp::wrap(ok);
  } END_RCPP
}

RcppExport
SEXP _SegyLoader_close(SEXP loader) {
  BEGIN_RCPP {
    Rcpp::as<SegyLoaderXPtr>(loader)->close();
    return Rcpp::wrap(0);
  } END_RCPP
}

RcppExport
SEXP _SegyLoader_survey(SEXP loader) {
  BEGIN_RCPP {
    Survey survey = Rcpp::as<SegyLoaderXPtr>(loader)->survey();
    return Rcpp::wrap(SurveyXPtr(new Survey(survey)));
  } END_RCPP
}

RcppExport
SEXP _SegyLoader_trace(SEXP loader, SEXP iline, SEXP xline) {
  BEGIN_RCPP {
    fvec trace = Rcpp::as<SegyLoaderXPtr>(loader)->trace(
      Rcpp::as<uword>(iline), Rcpp::as<uword>(xline)
    );
    return _toSEXP(trace);
  } END_RCPP
}

RcppExport
SEXP _SegyLoader_iline(SEXP loader, SEXP iline) {
  BEGIN_RCPP {
    fmat profile = Rcpp::as<SegyLoaderXPtr>(loader)->iline(
      Rcpp::as<uword>(iline)
    );
    return _toSEXP(profile);
  } END_RCPP
}

RcppExport
SEXP _SegyLoader_xline(SEXP loader, SEXP xline) {
  BEGIN_RCPP {
    fmat profile = Rcpp::as<SegyLoaderXPtr>(loader)->xline(
      Rcpp::as<uword>(xline)
    );
    return _toSEXP(profile);
  } END_RCPP
}

RcppExport
SEXP _SegyLoader_profile(SEXP loader, SEXP ilines, SEXP xlines) {
  BEGIN_RCPP {
    fmat profile = Rcpp::as<SegyLoaderXPtr>(loader)->profile(
      _toUVEC(ilines), _toUVEC(xlines)
    );
    return _toSEXP(profile);
  } END_RCPP
}

RcppExport
SEXP _SegyLoader_volume(SEXP loader) {
  BEGIN_RCPP {
    fcube volume = Rcpp::as<SegyLoaderXPtr>(loader)->volume();
    return _toSEXP(volume);
  } END_RCPP
}

RcppExport
SEXP _SegyLoader_timeSlice(SEXP loader, SEXP time) {
  BEGIN_RCPP {
    fmat slice = Rcpp::as<SegyLoaderXPtr>(loader)->timeSlice(
      Rcpp::as<float>(time)
    );
    return _toSEXP(slice);
  } END_RCPP
}

RcppExport
SEXP _SegyLoader_layerSlice(SEXP loader, SEXP surface) {
  BEGIN_RCPP {
    fmat slice = Rcpp::as<SegyLoaderXPtr>(loader)->layerSlice(
      _toFMAT(surface)
    );
    return _toSEXP(slice);
  } END_RCPP
}

/*********************************************************************************************
 * 
 *                                          Segy Writer
 * 
 ********************************************************************************************/

RcppExport
SEXP _SegyWriter(SEXP referSegyPath) {
  BEGIN_RCPP {
    SegyWriter* writer = new SegyWriter();
    writer->referTo(Rcpp::as<string>(referSegyPath));
    return Rcpp::wrap(SegyWriterXPtr(writer));
  } END_RCPP
}

RcppExport
SEXP _SegyWriter_open(SEXP writer, SEXP segyPath) {
  BEGIN_RCPP {
    bool ok = Rcpp::as<SegyWriterXPtr>(writer)->open(Rcpp::as<string>(segyPath));
    return Rcpp::wrap(ok);
  } END_RCPP
}

RcppExport
SEXP _SegyWriter_close(SEXP writer) {
  BEGIN_RCPP {
    Rcpp::as<SegyWriterXPtr>(writer)->close();
    return Rcpp::wrap(0);
  } END_RCPP
}

RcppExport
SEXP _SegyWriter_writeAsciiHeader(SEXP writer, SEXP ascii) {
  BEGIN_RCPP {
    Rcpp::as<SegyWriterXPtr>(writer)->writeAsciiHeader(Rcpp::as<string>(ascii));
    return Rcpp::wrap(0);
  } END_RCPP
}

RcppExport
SEXP _SegyWriter_writeTraceData(SEXP writer, SEXP index, SEXP data) {
  BEGIN_RCPP {
    Rcpp::as<SegyWriterXPtr>(writer)->writeTraceData(
      Rcpp::as<uword>(index), _toFVEC(data)
    );
    return Rcpp::wrap(0);
  } END_RCPP
}

RcppExport
SEXP _SegyWriter_copyAsciiHeader(SEXP writer) {
  BEGIN_RCPP {
    Rcpp::as<SegyWriterXPtr>(writer)->copyAsciiHeader();
    return Rcpp::wrap(0);
  } END_RCPP
}

RcppExport
SEXP _SegyWriter_copyBinaryHeader(SEXP writer) {
  BEGIN_RCPP {
    Rcpp::as<SegyWriterXPtr>(writer)->copyBinaryHeader();
    return Rcpp::wrap(0);
  } END_RCPP
}

RcppExport
SEXP _SegyWriter_copyTraceHeader(SEXP writer, SEXP index) {
  BEGIN_RCPP {
    Rcpp::as<SegyWriterXPtr>(writer)->copyTraceHeader(Rcpp::as<uword>(index));
    return Rcpp::wrap(0);
  } END_RCPP
}

RcppExport
SEXP _SegyWriter_copyTraceData(SEXP writer, SEXP index) {
  BEGIN_RCPP {
    Rcpp::as<SegyWriterXPtr>(writer)->copyTraceData(
      Rcpp::as<uword>(index)
    );
    return Rcpp::wrap(0);
  } END_RCPP
}

/*********************************************************************************************
 * 
 *                                      Segy Dumper
 * 
 ********************************************************************************************/

RcppExport
SEXP _SegyDumper(SEXP segyPath, SEXP ilineOffset, SEXP xlineOffset) {
  BEGIN_RCPP {
    SegyDumper* dumper = new SegyDumper();
    dumper->referTo(
      Rcpp::as<string>(segyPath),
      Rcpp::as<uword>(ilineOffset),
      Rcpp::as<uword>(xlineOffset)
    );
    return Rcpp::wrap(SegyDumperXPtr(dumper));
  } END_RCPP
}

RcppExport
SEXP _SegyDumper2(SEXP segyPath, SEXP survey) {
  BEGIN_RCPP {
    SegyDumper* dumper = new SegyDumper();
    dumper->referTo(
      Rcpp::as<string>(segyPath),
      *(Rcpp::as<SurveyXPtr>(survey))
    );
    return Rcpp::wrap(SegyDumperXPtr(dumper));
  } END_RCPP
}

RcppExport
SEXP _SegyDumper3(SEXP loader) {
  BEGIN_RCPP {
    SegyDumper* dumper = new SegyDumper();
    dumper->referTo( *(Rcpp::as<SegyLoaderXPtr>(loader)) );
    return Rcpp::wrap(SegyDumperXPtr(dumper));
  } END_RCPP
}

RcppExport
SEXP _SegyDumper_open(SEXP dumper, SEXP segyPath) {
  BEGIN_RCPP {
    bool ok = Rcpp::as<SegyDumperXPtr>(dumper)->open(
      Rcpp::as<string>(segyPath)
    );
    return Rcpp::wrap(ok);
  } END_RCPP
}

RcppExport
SEXP _SegyDumper_close(SEXP dumper) {
  BEGIN_RCPP {
    Rcpp::as<SegyDumperXPtr>(dumper)->close();
    return Rcpp::wrap(0);
  } END_RCPP
}

RcppExport
SEXP _SegyDumper_dumpTrace(SEXP dumper, SEXP iline, SEXP xline, SEXP trace) {
  BEGIN_RCPP {
    Rcpp::as<SegyDumperXPtr>(dumper)->dumpTrace(
      Rcpp::as<uword>(iline), Rcpp::as<uword>(xline), _toFVEC(trace)
    );
    return Rcpp::wrap(0);
  } END_RCPP
}

RcppExport
SEXP _SegyDumper_dumpILINE(SEXP dumper, SEXP iline, SEXP profile) {
  BEGIN_RCPP {
    Rcpp::as<SegyDumperXPtr>(dumper)->dumpILINE(
      Rcpp::as<uword>(iline), _toFMAT(profile)
    );
    return Rcpp::wrap(0);
  } END_RCPP
}

RcppExport
SEXP _SegyDumper_dumpXLINE(SEXP dumper, SEXP xline, SEXP profile) {
  BEGIN_RCPP {
    Rcpp::as<SegyDumperXPtr>(dumper)->dumpXLINE(
      Rcpp::as<uword>(xline), _toFMAT(profile)
    );
    return Rcpp::wrap(0);
  } END_RCPP
}

RcppExport
SEXP _SegyDumper_dumpProfile(SEXP dumper, SEXP iline, SEXP xline, SEXP profile) {
  BEGIN_RCPP {
    Rcpp::as<SegyDumperXPtr>(dumper)->dumpProfile(
      _toUVEC(iline), _toUVEC(xline), _toFMAT(profile)
    );
    return Rcpp::wrap(0);
  } END_RCPP
}

RcppExport
SEXP _SegyDumper_dumpVolume(SEXP dumper, SEXP volume) {
  BEGIN_RCPP {
    Rcpp::as<SegyDumperXPtr>(dumper)->dumpVolume(_toFCUBE(volume));
    return Rcpp::wrap(0);
  } END_RCPP
}

/*********************************************************************************************
 * 
 *                               Surface Operation
 * 
 ********************************************************************************************/

RcppExport
SEXP _gSurfaceToTable(SEXP ilines, SEXP xlines, SEXP surface) {
  BEGIN_RCPP {
    const SurfaceTable table = gSurfaceToTable(
      _toUVEC(ilines), _toUVEC(xlines), _toFMAT(surface)
    );
    const Rcpp::DataFrame frame = Rcpp::DataFrame::create(
      Rcpp::Named("ilines") = _toSEXP(table.ilines),
      Rcpp::Named("xlines") = _toSEXP(table.xlines),
      Rcpp::Named("values") = _toSEXP(table.values)
    );
    return Rcpp::wrap(frame);
  } END_RCPP
}

RcppExport
SEXP _gTableToSurface(SEXP ilines, SEXP xlines, SEXP values) {
  BEGIN_RCPP {
    const fmat surface = gTableToSurface(
      _toUVEC(ilines), _toUVEC(xlines), _toFVEC(values)
    );
    return _toSEXP(surface);
  } END_RCPP
}

RcppExport
SEXP _gTableToSurface_survey(SEXP survey, SEXP ilines, SEXP xlines, SEXP values) {
  BEGIN_RCPP {
    fmat surface = gTableToSurface(
      *Rcpp::as<SurveyXPtr>(survey),
      _toUVEC(ilines), _toUVEC(xlines), _toFVEC(values)
    );
    return _toSEXP(surface);
  } END_RCPP
}
