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

source("R/gcolors.R")

gimage = function(
  profile,
  zlim     = NULL,
  col      = gColors("flower", 512),
  lwd      = 2,
  mar      = 1.5,
  colorbar = TRUE,
  xaxis    = NULL,
  yaxis    = NULL,
  cex.text = 1.0,
  alpha    = 1.0, 
  add      = FALSE, ...)
{
  z  = t(profile)
  z  = z[, ncol(z):1]
  x  = (1 : nrow(z)) / nrow(z)
  y  = (1 : ncol(z)) / ncol(z)

  if (is.null(zlim))
    zlim = range(pretty(quantile(z, (2:98)/100, na.rm = TRUE)))
  z[z < zlim[1]] = zlim[1]
  z[z > zlim[2]] = zlim[2]

  prev.par = par(
    omi = c(0.0, 0.0, 0.0, 0.0),
    mar = c(1.5, 1.5, 1.5 + mar, 1.5 + mar),
    xpd = TRUE, ...)
  {
    image.col = col
    if (alpha < 1.0)
      image.col = sprintf("%s%02X", col, round(alpha*255))
    image(
      x = ((0 : nrow(z))+0.5) / (nrow(z) + 1),
      y = ((0 : ncol(z))+0.5) / (ncol(z) + 1),
      z = z,
      xlim = c(-0.1, 1.0),
      ylim = c(0.0,  1.0),
      zlim = zlim, col = image.col,
      axes = FALSE, xlab = "", ylab = "", 
      add  = add, ...
    )

    rect(0.0, 0.0, 1.0, 1.0, lwd = lwd)
    rect(-0.1, 0.0, 0.0, 1.0, lwd = lwd)

    # plot x-axis
    if (! is.null(xaxis)) {
      xscale = pretty(xaxis, n = 10)
      xscale = xscale[min(xaxis) <= xscale & xscale <= max(xaxis)]
      xat    = (xscale - min(xaxis)) / (max(xaxis) - min(xaxis))
      axis(side = 3, at = xat, labels = FALSE, lwd = lwd)
      mtext(xscale, side = 3, at = xat, cex = cex.text, family = 'serif', padj = -1.0)
    } else {
      by = diff(pretty(1:length(x), n = 10))[1]
      ix = seq.int(from = 1, to = length(z), by = by)
      axis(side = 3, at = x[ix], labels = FALSE, lwd = lwd)
    }

    # plot y-axis
    if (! is.null(yaxis)) {
      yscale = pretty(yaxis, n = 20)
      yscale = yscale[min(yaxis) <= yscale & yscale <= max(yaxis)]
      yat    = (max(yaxis) - yscale) / (max(yaxis) - min(yaxis))
      axis(side = 4, at = yat, labels = FALSE, lwd = lwd)
      if (max(yscale) > 1000)
        mtext(sprintf("%4d", yscale), side = 4, at = yat, las = 2, cex = cex.text, family = 'serif', adj = -0.2)
      else
        mtext(sprintf("%3d", yscale), side = 4, at = yat, las = 2, cex = cex.text, family = 'serif', adj = -0.2)
    } else {
      by = diff(pretty(1:length(y), n = 10))[1]
      iy = seq.int(from = 1, to = length(y), by = by)
      axis(side = 4, at = rev(y)[iy], labels = FALSE, lwd = lwd)
    }

    # plot colorbar
    if (colorbar)
    {
      if (length(col) > 128) {
        by = floor(length(col)/128)
        col = col[seq(1, length(col), by=by)]
      }
      for (i in 1:length(col)) {
        ybottom = 0.025 + ((i-1) / length(col)) * 0.95
        ytop    = 0.025 + (    i / length(col)) * 0.95
        rect(-0.01, ybottom, -0.03, ytop, col = col[i], border = col[i])
      }
      rect(-0.01, 0.025, -0.03, 0.975, lwd = lwd)

      if (length(col) > 32) {
        qz = pretty(zlim, 20)
        qz = qz[zlim[1] < qz & qz < zlim[2]]
      } else {
        qz = pretty(zlim, length(col))
      }
      tz = format(qz, width = 9, digits = 2, scientific = NA)
      step = qz[2] - qz[1]
      for (i in 1:length(qz)) {
        y = 0.025 + (qz[i] - zlim[1] + 0.5*step) / (zlim[2] - zlim[1] + step) * 0.95
        lines(x = c(-0.025, -0.035), y = c(y, y), lwd = lwd)
        text(-0.04, y, labels = tz[i], pos = 2, cex = cex.text, family = 'serif')
      }
    }
  }
  par(prev.par)
}
