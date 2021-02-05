# libsegyio

The `libsegyio` library is an `armadillo` (http://arma.sourceforge.net/) based c++ library for `SEGY` file reading and writing, which provides efficient APIs of trace, profile, slice, and volume IO for the `SEGY` file. Meawhile, `R` APIs of `libsegyio` (see the `R/gsegyio.R` file) are also supported with the help of `Rcpp` (http://rcpp.org/).

Currently, the `libsegyio` is not fully completed, documented and systematically tested (or packaged as a `R` library). However, its reading and writing functionality is completed and works well. The whole project is organized with the `qmake` tool of `qt`. If you want to use it now, please include the `libsegyio.pri` file and all related c++ files in `src/segyio` into your c++/qt projects. I will try to finish all the developments in the future. At the same time, everyone is welcomed to participate in its development and improvement. `(^_^)`

By the way, the `libsegyio` library is licensed under the GNU General Public License. Please See the GNU General Public License (https://www.gnu.org/licenses/) for more details.

# Contact

**Ao Yile** (aoyile@yeah.net)

The Easysignal Team, Department of Automation, Tsinghua University
