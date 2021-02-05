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

#ifndef LIB_SEGYIO_CONFIG_H
#define LIB_SEGYIO_CONFIG_H

#include <cstdio>
#include <cmath>
#include <armadillo>
#include <vector>
#include <algorithm>
#include <memory>
#include <cassert>
#include <omp.h>
#include <iomanip>

#define SEGYIO_VERSION "0.0.1"

namespace SegyIO {

using namespace arma;
using namespace std;
using std::string;
using std::pair;

#ifndef SEGYIO_INVALID_INDEX
#define SEGYIO_INVALID_INDEX __UINT32_MAX__
#endif

} /*namespace SegyIO*/

#endif // LIB_GEOSEIS_CONFIG_H

