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

INCLUDEPATH += $$PWD/segyio
DEPENDPATH += $$PWD/segyio

unix {
  R_HOME    = $$system(R RHOME)
  RCPPFLAGS = $$system($$R_HOME/bin/R CMD config --cppflags)
  RLDFLAGS  = $$system($$R_HOME/bin/R CMD config --ldflags)
  RBLAS     = $$system($$R_HOME/bin/R CMD config BLAS_LIBS)
  RLAPACK   = $$system($$R_HOME/bin/R CMD config LAPACK_LIBS)
  RCPPINCL  = $$system($$R_HOME/bin/Rscript -e \'Rcpp:::CxxFlags\(\)\')
  RCPPLIBS  = $$system($$R_HOME/bin/Rscript -e \'Rcpp:::LdFlags\(\)\')

  QMAKE_CXXFLAGS += $$RCPPWARNING $$RCPPFLAGS $$RCPPINCL
  QMAKE_LFLAGS += $$RLDFLAGS $$RCPPLIBS $$RBLAS $$RLAPACK
}

SOURCES += $$PWD/segyio/segyio-rcpp.cpp
