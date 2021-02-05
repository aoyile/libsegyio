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

HEADERS +=  \
  $$PWD/segyio.h \
  $$PWD/segyio/config.h \
  $$PWD/segyio/bitop.h \
  $$PWD/segyio/survey.h \
  $$PWD/segyio/surface.h \
  $$PWD/segyio/segydef.h \
  $$PWD/segyio/segyreader.h \
  $$PWD/segyio/segyloader.h \
  $$PWD/segyio/segywriter.h \
  $$PWD/segyio/segydumper.h

SOURCES += \
  $$PWD/segyio/bitop.cpp \
  $$PWD/segyio/survey.cpp \
  $$PWD/segyio/surface.cpp \
  $$PWD/segyio/segydef.cpp \
  $$PWD/segyio/segyreader.cpp \
  $$PWD/segyio/segyloader.cpp \
  $$PWD/segyio/segywriter.cpp \
  $$PWD/segyio/segydumper.cpp
