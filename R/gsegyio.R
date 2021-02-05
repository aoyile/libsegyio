################################################################################
#
#                  libsegyio Copyright (C) 2021 aoyile
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
################################################################################

library("Rcpp")

if (! is.loaded("_SEGYIO_VERSION")) {
  if (file.exists("lib/libsegyio.so"))
    dyn.load("lib/libsegyio.so")
  else
    dyn.load("lib/libsegyio.dll")
}

SEGYIO_VERSION = .Call('_SEGYIO_VERSION')
cat(sprintf('load libsegyio version : %s\n', SEGYIO_VERSION))

################################################################################
#
#                                             Class of Segy File
#
################################################################################

gSegyFile = function(path, ilineOffset, xlineOffset) {
  structure(list(
    path        = path, 
    ilineOffset = ilineOffset, 
    xlineOffset = xlineOffset
  ), type = "gSegyFile")
}

gIsSegyFile = function(x) {
  class(x) == "gSegyFile"
}

################################################################################
#
#                                             Segy Survey
#
################################################################################

gSurvey = function(iline.range, xline.range) {
  .Call('_RegularSurvey', iline.range[1], iline.range[2], xline.range[1], xline.range[2])
}

gSurveyFrom = function(segyReader, ilineOffset, xlineOffset) {
  .Call('_gSurveyFrom', segyReader, ilineOffset, xlineOffset)
}

gILINE = function(survey, index = NULL) {
  if (length(index) <= 0)
    .Call('_Survey_all_ilines', survey)
  else if (length(index) > 1)
    .Call('_Survey_ilines', survey, index-1)
  else
    .Call('_Survey_iline', survey, index-1)
}

gXLINE = function(survey, index = NULL) {
  if (length(index) <= 0)
    .Call('_Survey_all_xlines', survey)
  else if (length(index) > 1)
    .Call('_Survey_xlines', survey, index-1)
  else
    .Call('_Survey_xline', survey, index-1)
}

gINDEX = function(survey, iline, xline) {
  if (length(iline) * length(xline) == 1)
    .Call('_Survey_index', survey, iline, xline) + 1
  else if (length(iline) == 1)
    .Call('_Survey_indexes_sv', survey, iline, xline) + 1
  else if (length(xline) == 1)
    .Call('_Survey_indexes_vs', survey, iline, xline) + 1
  else
    .Call('_Survey_indexes_vv', survey, iline, xline) + 1
}

################################################################################
#
#                                             Segy Reader
#
################################################################################

gSegyReader = function(segyPath) {
  .Call("_SegyReader", segyPath)
}

gSegyReaderOpen = function(segyReader, segyPath) {
  .Call("_SegyReader_open", segyReader, segyPath)
}

gSegyReaderClose = function(segyReader) {
  .Call("_SegyReader_close", segyReader)
}

gSegyAsciiHeader = function(segyReader) {
  .Call("_SegyReader_asciiHeader", segyReader)
}

gSegyBinaryHeader = function(segyReader) {
  .Call("_SegyReader_binaryHeader", segyReader)
}

gSegyTraceCount = function(segyReader) {
  .Call("_SegyReader_traceCount", segyReader)
}

gSegyTraceHeader = function(segyReader, index = 1) {
  if (length(index) > 1) {
    data = lapply(index, function(i) {
      .Call("_SegyReader_traceHeader", segyReader, as.integer(i - 1))
    })
    return(data)
  }
  .Call("_SegyReader_traceHeader", segyReader, as.integer(index - 1))
}

gSegyTraceTime = function(segyReader, index = 1) {
  if (length(index) > 1) {
    data = sapply(index, function(i) {
      .Call("_SegyReader_traceTime", segyReader, as.integer(i - 1))
    })
    return(data)
  }
  .Call("_SegyReader_traceTime", segyReader, as.integer(index - 1))
}

gSegyTraceLine = function(segyReader, index = 1, offset) {
  if (length(index) > 1) {
    data = sapply(index, function(i) {
      .Call("_SegyReader_traceLine", segyReader, as.integer(i - 1), as.integer(offset))
    })
    return(data)
  }
  .Call("_SegyReader_traceLine", segyReader, as.integer(index - 1), as.integer(offset))
}

gSegyTraceData = function(segyReader, index = 1) 
{
  if (length(index) > 1) {
    data = sapply(index, function(i) {
      .Call("_SegyReader_traceData", segyReader, as.integer(i - 1))
    })
    return(data)
  }
  .Call("_SegyReader_traceData", segyReader, as.integer(index - 1))
}

################################################################################
#
#                                   Segy Loader
#
################################################################################

gSegyLoader = function(segyPath, ilineOffset, xlineOffset, survey = NULL) {
  if (is.null(survey))
    .Call("_SegyLoader", segyPath, ilineOffset, xlineOffset)
  else
    .Call("_SegyLoader2", segyPath, survey)
}

gSegyLoaderOpen = function(loader, segyPath, ilineOffset, xlineOffset) {
  .Call("_SegyLoader_open", loader, segyPath, ilineOffset, xlineOffset)
} 

gSegyLoaderClose = function(loader) {
  .Call("_SegyLoader_close", loader)
} 

gSegyLoaderSurvey = function(loader) {
  .Call("_SegyLoader_survey", loader)
}

gSegyVolume = function(loader) {
  .Call("_SegyLoader_volume", loader)
}

gSegyProfile = function(loader, iline = NULL, xline = NULL) {
  if (is.null(xline))
    .Call("_SegyLoader_iline", loader, iline)
  else if (is.null(iline))
    .Call("_SegyLoader_xline", loader, xline)
  else
    .Call("_SegyLoader_profile", loader, iline, xline)
}

gSegySlice = function(loader, at) {
  if (length(at) == 1)
    .Call("_SegyLoader_timeSlice", loader, at)
  else
    .Call("_SegyLoader_layerSlice", loader, at)
}

gSegyTrace = function(loader, iline, xline) {
  .Call("_SegyLoader_trace", loader, iline, xline)
}

################################################################################
#
#                                         Segy Writer
#
################################################################################

gSegyWriter = function(referPath) {
  .Call("_SegyWriter", referPath)
}

gSegyWriterOpen = function(segyWriter, segyPath) {
  .Call("_SegyWriter_open", segyWriter, segyPath)
}

gSegyWriterClose = function(segyWriter) {
  .Call("_SegyWriter_close", segyWriter)
}

gSegyWriteAsciiHeader = function(segyWriter, ascii) {
  .Call("_SegyWriter_writeAsciiHeader", segyWriter, ascii)
}

gSegyWriteTraceData = function(segyWriter, index, data) {
  .Call("_SegyWriter_writeTraceData", segyWriter, as.integer(index - 1), data)
}

gSegyCopyAsciiHeader = function(segyWriter) {
  .Call("_SegyWriter_copyAsciiHeader", segyWriter)
}

gSegyCopyBinaryHeader = function(segyWriter) {
  .Call("_SegyWriter_copyBinaryHeader", segyWriter)
}

gSegyCopyTraceHeader = function(segyWriter, index) {
  .Call("_SegyWriter_copyTraceHeader", segyWriter, as.integer(index - 1))
}

gSegyCopyTraceData = function(segyWriter, index) {
  .Call("_SegyWriter_copyTraceData", segyWriter, as.integer(index - 1))
}

################################################################################
#
#                                   Segy Dumper
#
################################################################################

gSegyDumper = function(referPath, ilineOffset = 9, xlineOffset = 21, survey = NULL, referLoader = NULL) {
  if (is.null(survey))
    if (is.null(referLoader))
      .Call("_SegyDumper", referPath, ilineOffset, xlineOffset)
    else
      .Call("_SegyDumper3", referLoader)
  else
    .Call("_SegyDumper2", referPath, survey)
}

gSegyDumperOpen = function(dumper, segyPath) {
  .Call("_SegyDumper_open", dumper, segyPath)
} 

gSegyDumperClose = function(dumper) {
  .Call("_SegyDumper_close", dumper)
} 

gSegyDumpVolume = function(dumper, volume) {
  .Call("_SegyDumper_dumpVolume", dumper, volume)
}

gSegyDumpProfile = function(dumper, iline = NULL, xline = NULL, profile) {
  if (is.null(xline))
    .Call("_SegyDumper_dumpILINE", dumper, iline, profile)
  else if (is.null(iline))
    .Call("_SegyDumper_dumpXLINE", dumper, xline, profile)
  else
    .Call("_SegyDumper_dumpProfile", dumper, iline, xline, profile)
}

gSegyDumpTrace = function(dumper, iline, xline, trace) {
  .Call("_SegyDumper_dumpTrace", dumper, iline, xline, trace)
}
