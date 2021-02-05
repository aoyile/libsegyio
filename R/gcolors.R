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
gColorMapper = function(colors, scales = NULL)
{
  if (length(colors) == 1) {
    colors = gColors(colors[[1]], n = 32)
    scales = seq(min(scales), max(scales), length.out = 32)
  }

  mRGB = col2rgb(colors)
  rFn  = approxfun(x = scales, y = mRGB[1, ], rule = 2)
  gFn  = approxfun(x = scales, y = mRGB[2, ], rule = 2)
  bFn  = approxfun(x = scales, y = mRGB[3, ], rule = 2)
  srg  = range(scales)
  mapper = function(x) {
    x[x < srg[1]] = srg[1]
    x[x > srg[2]] = srg[2]
    rgb(rFn(x), gFn(x), bFn(x), max = 255)
  }
  return(compiler::cmpfun(mapper))
}

gColors = function(name, n)
{
  if (name == "rainbow")
    return(rainbow(n, start = 0.0, end = 0.85))
  
  if (name == "levels") {
    colors = c(
      "#0066CC", "#DD00DD", "#00C0BB", "#FFC000", "#FF0000", "#CCFF22", "#8888FF", 
      "#00C0FF", "#0F8554", "#CF1C90", "#E17C05", "#73AF48", "#1D6996", "#EDAD08", 
      "#6F4070", "#CC503E", "#666666", "#94346E", "#F6CF71", "#994E95", "#66C5CC",
      "#DCB0F2", "#FE88B1", "#87C55F", "#F89C74", "#9EB9F3", "#B497E7", "#C9DB74",
      "#B3B3B3", "#8BE0A4", "#D3B484", "#11A579", "#7F3C8D", "#F2B701", "#E73F74",
      "#3969AC", "#80BA5A", "#f97b72", "#008695", "#855C75", "#D9AF6B", "#AF6458",
      "#736F4C", "#526A83", "#625377", "#68855C", "#9C9C5E", "#A06177", "#8C785D"
    )
    while (length(colors) < n)
      colors = c(colors, colors)
    return(colors[1:n])
  }

  mapper = list(
    "flower" = gColorMapper(
      c("#000044", "#0000FF", "#00C0FF", "#00C0BB", "#CFFF00", "#FFC000", "#FF0000", "#DD00DD", "#440044"),
      seq(from = 0.0, to = 1.0, length.out = 9)
    ),
    "offsets" = gColorMapper(
      c("#000044", "#0000FF", "#00CCFF", "#00FFCC", "#CCFF00", '#FFFFFF', "#FFFF00", "#FFC000", "#FF0000", "#DD00DD", "#440044"),
      seq(from = 0.0, to = 1.0, length.out = 11)
    ),
    "curvature" = gColorMapper(
      c("#000044", "#0000FF", "#0066CC", "#00CCFF", "#AAEEFF", "#FFFFFF", "#FFEEAA", "#FFCC00", "#FF0000", "#FF00FF", "#660066"),
      seq(from = 0.0, to = 1.0, length.out = 11)
    ),    
    "festival" = gColorMapper(
      c("#1d4f60", "#04693C", "#88CC00", "#FFEE00", "#FCB245", "#FF0000", "#660000"),
      seq(from = 0.0, to = 1.0, length.out = 7)
    ),
    "earth" = gColorMapper(
      c("#0033CC", "#0066CC", "#049FF1", "#007788", "#1d4f60", "#446600", "#814908", "#bd925a", "#d6bd8d", "#FFFFF0"),
      seq(from = 0.0, to = 1.0, length.out = 10)
    ),
    "desert" = gColorMapper(
      c("#004488", "#0091A9", "#66CCDD", "#edeac2", "#FFDD00", "#FCB245", "#814908"),
      seq(from = 0.0, to = 1.0, length.out = 7)
    ),
    "summer" = gColorMapper(
      c("#04693C", "#3CB64C", "#AAEE44", "#FFFF00", "#FCB245", "#E12066", "#642E94"),
      seq(from = 0.0, to = 1.0, length.out = 7)
    ),
    "charm" = gColorMapper(
      c("#AA4C40", "#A475A1", "#7D98C6", "#3DB3C6", "#36D0B1", "#85E489", "#DFEF66"),
      seq(from = 0.0, to = 1.0, length.out = 7)
    ),
    "winter" = gColorMapper(
      c("#888888", "#BBBBBB", "#EEF3F3", "#66CCDD", "#0091A9", "#004488", "#002244"),
      seq(from = 0.0, to = 1.0, length.out = 7)
    ),  

    "extreme" = gColorMapper(
      c("#000044", "#000088", "#0000FF", "#0066CC", "#00CCFF", "#CCFFFF", "#FFFFFF", "#FFFFCC", "#FFCC44", "#FF8800", "#FF0000", "#880000", "#440000"),
      seq(from = 0.0, to = 1.0, length.out = 13)
    ),
    "seismic" = gColorMapper(
      c("#000000",  "#112288", "#2244CC",  "#3366DD", "#6699CC", "#FFFFFF", "#FFAA00", "#FF8800", "#FF2200", "#AA0044", "#440022"),
      seq(from = 0.0, to = 1.0, length.out = 11)
    ),
    "seismic.sketch" = gColorMapper(
      c("#0000FF", "#0066FF", "#2288FF", "#44BBFF", "#99EEFF", "#CCFFFF", "#EEEEEE", "#888888", "#444444", "#222222", "#440000", "#880000", "#FF0000", "#FF0000"),
      seq(from = 0.0, to = 1.0, length.out = 14)
    ),
    "grayred" = gColorMapper(
      c("#222222", "#444444", "#888888", "#CCCCCC", "#FFFFFF", "#FFCC44", "#FF0000", "#880000", "#440000"),
      seq(from = 0.0, to = 1.0, length.out = 9)
    ),
    "grayblue" = gColorMapper(
      c("#222222", "#444444", "#888888", "#CCCCCC", "#FFFFFF", "#44CCFF", "#0044FF", "#0000CC", "#000044"),
      seq(from = 0.0, to = 1.0, length.out = 9)
    ),
    "graygreen" = gColorMapper(
      c("#222222", "#444444", "#888888", "#CCCCCC", "#FFFFFF", "#CCFF88", "#88DD00", "#008800", "#004400"),
      seq(from = 0.0, to = 1.0, length.out = 9)
    ),
    "graybrown" = gColorMapper(
      c("#222222", "#444444", "#888888", "#CCCCCC", "#FFFFFF", "#FFCC88", "#FF8800", "#DD5500", "#442200"),
      seq(from = 0.0, to = 1.0, length.out = 9)
    ),

    "fault.red" = gColorMapper(
      c("#FFFFFF", "#FFFFFF", "#FFFFDD", "#FFEEAA", "#FFCC44", "#FFCC00", "#FF8800", "#FF0000", "#880000", "#440000"),
      seq(from = 0.0, to = 1.0, length.out = 10)
    ),
    "fault.green" = gColorMapper(
      c("#FFFFFF", "#FFFFFF", "#FFFFDD", "#EEFFAA", "#CCFF44", "#CCFF00", "#88FF00", "#00FF00", "#008800", "#004400"),
      seq(from = 0.0, to = 1.0, length.out = 10)
    ),
    "fault.blue" = gColorMapper(
      c("#FFFFFF", "#EEFFFF","#DDFFFF", "#CCFFFF", "#BBFFFF", "#AAFFFF", "#70E1FF", "#44CCFF", "#049FF1", "#0088FF", "#0066CC", "#0044AA", "#002288", "#000144"),
      seq(from = 0.0, to = 1.0, length.out = 14)
    ),
    "blood" = gColorMapper(
      c("#FFFFFF", "#F3ECCF", "#FAD6A2", "#FCB245", "#F49F80", "#C02626", "#880022", "#440000"),
      seq(from = 0.0, to = 1.0, length.out = 8)
    ),
    "sunset" = gColorMapper(
      c("#FFFFFF", "#f3e79b", "#fac484", "#f8a07e", "#eb7f86", "#ce6693", "#a059a0", "#662266"),
      seq(from = 0.0, to = 1.0, length.out = 8)
    ),
    "orange" = gColorMapper(
      c("#FFFFFF", "#ecda9a", "#efc47e", "#f3ad6a", "#f7945d", "#f97b57", "#f66356", "#ee4d5a"),
      seq(from = 0.0, to = 1.0, length.out = 8)
    ),
    "purple" = gColorMapper(
      c("#FFFFFF", "#f9ddda", "#f2b9c4", "#e597b9", "#ce78b3", "#ad5fad", "#834ba0", "#573b88"),
      seq(from = 0.0, to = 1.0, length.out = 8)
    ),
    "magenta" = gColorMapper(
      c("#FFFFFF", "#f3cbd3", "#eaa9bd", "#dd88ac", "#ca699d", "#b14d8e", "#91357d", "#6c2167"),
      seq(from = 0.0, to = 1.0, length.out = 8)
    ),
    "brown" = gColorMapper(
      c("#FFFFFF", "#ede5cf", "#e0c2a2", "#d39c83", "#c1766f", "#a65461", "#813753", "#541f3f"),
      seq(from = 0.0, to = 1.0, length.out = 8)
    ),
    "green" = gColorMapper(
      c("#FFFFFF", "#EEEEEE", "#c4e6c3", "#96d2a4", "#4da284", "#36877a", "#266b6e", "#1d4f60"),
      seq(from = 0.0, to = 1.0, length.out = 8)
    ),
    "leave" = gColorMapper(
      c("#FFFFFF", "#E4FB6D", "#B3E972", "#87D477", "#3EA77D", "#147872", "#155544", "#1B3542", "#002200"),
      seq(from = 0.0, to = 1.0, length.out = 9)
    ),
    "blue" = gColorMapper(
      c("#FFFFFF", "#CBF3FB", "#70E1FF", "#42B3D5", "#049FF1", "#0066CC", "#003388", "#001188", "#000144"),
      seq(from = 0.0, to = 1.0, length.out = 9)
    ),
    "gray" = gray
  )[[name]]
  
  mapper(((1:n) - 1.0) / (n - 1.0))
}
