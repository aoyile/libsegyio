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

include(libsegyio.pri)

CONFIG += console c++11
CONFIG -= app_bundle qt
TEMPLATE = lib
TARGET = $$PWD/../lib/segyio
OBJECTS_DIR = $$PWD/../lib

################################################################################
#
#                           lib armadillo configure
#
################################################################################

unix {
  DEFINES += NDEBUG
  DEFINES += ARMA_NO_DEBUG ARMA_USE_CXX11
  DEFINES += ARMA_USE_BLAS ARMA_USE_LAPACK ARMA_DONT_USE_OPENMP
  LIBS += -lblas -llapack -larmadillo
}
win32 {
  include(libarma.pri)
  DEFINES += ARMA_NO_DEBUG ARMA_USE_CXX11
  DEFINES += ARMA_DONT_USE_BLAS ARMA_DONT_USE_LAPACK ARMA_DONT_USE_OPENMP
}

################################################################################
#
#                           Rcpp Export API configure
#
################################################################################

DEFINES += SEGYIO_RCPP_EXPORT

if (contains(DEFINES, SEGYIO_RCPP_EXPORT)) {
  include(libsegyio-rcpp.pri)
}

