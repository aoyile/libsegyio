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

#include "segyio/surface.h"

namespace SegyIO {

SurfaceTable gSurfaceToTable(
  const uvec& ilines,
  const uvec& xlines,
  const fmat& surface )
{
  assert(ilines.n_elem == surface.n_rows);
  assert(xlines.n_elem == surface.n_cols);

  std::vector<uword> table_ilines; table_ilines.reserve(surface.n_elem);
  std::vector<uword> table_xlines; table_xlines.reserve(surface.n_elem);
  std::vector<float> table_values; table_values.reserve(surface.n_elem);
  for (uword ir = 0; ir < ilines.n_elem; ++ir) {
    for (uword ic = 0; ic < xlines.n_elem; ++ic) {
      table_ilines.push_back(ilines[ir]);
      table_xlines.push_back(xlines[ic]);
      table_values.push_back(surface.at(ir, ic));
    }
  }
  SurfaceTable table;
  table.ilines = table_ilines;
  table.xlines = table_xlines;
  table.values = table_values;
  return table;
}

fmat gTableToSurface(
  const uvec& ilines,
  const uvec& xlines,
  const fvec& values )
{
  assert(ilines.n_elem == values.n_elem);
  assert(xlines.n_elem == values.n_elem);
  
  const uword ilineMin = ilines.min();
  const uword ilineMax = ilines.max();
  const uword xlineMin = xlines.min();
  const uword xlineMax = xlines.max();

  fmat surface(ilineMax-ilineMin+1, xlineMax-xlineMin+1);
  surface.fill(NAN);
  for (uword i = 0; i < ilines.n_elem; ++i) {
    const uword ir = ilines[i] - ilineMin;
    const uword ic = xlines[i] - xlineMin;
    surface.at(ir, ic) = values[i];
  }
  return surface;
}

fmat gTableToSurface(
  const Survey& survey,
  const uvec&   ilines,
  const uvec&   xlines,
  const fvec&   values )
{
  assert(ilines.n_elem == values.n_elem);
  assert(xlines.n_elem == values.n_elem);
  
  const uword ilineMin = survey.ilineMin();
  const uword ilineMax = survey.ilineMax();
  const uword xlineMin = survey.xlineMin();
  const uword xlineMax = survey.xlineMax();

  fmat surface(ilineMax-ilineMin+1, xlineMax-xlineMin+1);
  surface.fill(NAN);
  for (uword i = 0; i < ilines.n_elem; ++i) {
    if (survey.contains(ilines[i], xlines[i])) {
      const uword ir = ilines[i] - ilineMin;
      const uword ic = xlines[i] - xlineMin;
      surface.at(ir, ic) = values[i];
    }
  }
  return surface;
}

} /*namespace SegyIO*/