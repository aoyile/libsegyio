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

#ifndef LIB_SEGYIO_SURFACE_H
#define LIB_SEGYIO_SURFACE_H

#include "segyio/config.h"
#include "segyio/survey.h"

namespace SegyIO {

struct SurfaceTable {
  uvec ilines;
  uvec xlines;
  fvec values;
};

SurfaceTable gSurfaceToTable(
  const uvec& ilines,
  const uvec& xlines,
  const fmat& surface
);

fmat gTableToSurface(
  const uvec& ilines,
  const uvec& xlines,
  const fvec& values
);

inline
fmat gTableToSurface(const SurfaceTable& table) {
  return gTableToSurface(table.ilines, table.xlines, table.values);
}

fmat gTableToSurface(
  const Survey& survey,
  const uvec&   ilines,
  const uvec&   xlines,
  const fvec&   values
);

inline
fmat gTableToSurface(const Survey& survey, const SurfaceTable& table) {
  return gTableToSurface(survey, table.ilines, table.xlines, table.values);
}

} /*namespace SegyIO*/

#endif // LIB_SEGYIO_SURFACE_H
