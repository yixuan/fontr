# fontr

**fontr** helps you to extract character glyphs from a specific font, either as
bitmaps or outline polygons. This is exactly what
[showtext](https://github.com/yixuan/showtext) does internally, and **fontr**
simply makes it work in the R level.

## Quick Examples

The code below generates a matrix representing the bitmap image of the character
"C", given the specification of the font.


```r
library(fontr)
ch_C = glyph_bitmap("C", family = "sans", face = "regular", pixel_size = 20)
ch_C
```

```
##       [,1] [,2] [,3] [,4] [,5] [,6] [,7] [,8] [,9] [,10] [,11]
##  [1,]    0    0   22  141  217  247  234  194   93     2     0
##  [2,]    0   48  234  232   90   15   14  104  243   165     3
##  [3,]    9  221  248   43    0    0    0    0   82   255   103
##  [4,]  102  255  151    0    0    0    0    0    0   185   119
##  [5,]  183  255   69    0    0    0    0    0    0     0     0
##  [6,]  230  255   24    0    0    0    0    0    0     0     0
##  [7,]  249  255    5    0    0    0    0    0    0     0     0
##  [8,]  248  255    7    0    0    0    0    0    0     0     0
##  [9,]  226  255   31    0    0    0    0    0    0     0     0
## [10,]  177  255   82    0    0    0    0    0    0     0     0
## [11,]   93  255  172    0    0    0    0    0    0   130   141
## [12,]    5  213  254   65    0    0    0    0   48   246   129
## [13,]    0   39  229  243  105   19   18   90  232   169     5
## [14,]    0    0   18  139  217  247  244  195   88     2     0
## attr(,"class")
## [1] "glyph_bitmap"
```

```r
plot(ch_C)
```

<div align="center">
  <img src="http://i.imgur.com/xnMYGgZ.png" alt="fontr_c" />
</div>

The glyph can also be extracted as outline curves that are similar to the SVG
path elements.


```r
ch_H = glyph_outline("H", family = "sans", face = "regular")
ch_H
```

```
##             x         y type
## 1  0.54736328 0.0000000    M
## 2  0.54736328 0.3188477    L
## 3  0.17529297 0.3188477    L
## 4  0.17529297 0.0000000    L
## 5  0.08203125 0.0000000    L
## 6  0.08203125 0.6879883    L
## 7  0.17529297 0.6879883    L
## 8  0.17529297 0.3969727    L
## 9  0.54736328 0.3969727    L
## 10 0.54736328 0.6879883    L
## 11 0.64062500 0.6879883    L
## 12 0.64062500 0.0000000    L
## 13 0.54736328 0.0000000    L
```

The meaning of the characters in the `type` column can be found in sections
8.3.2 to 8.3.7 of the
[SVG specification document](http://www.w3.org/TR/SVG/paths.html).

This form is an exact representation of the glyph outlines that are contained
in the font file. However, it is usually more convenient to use polygons to
approximate those curves, as the following example shows:


```r
ch_n = glyph_polygon("n", family = "serif", face = "italic", nseg = 10)
head(ch_n, 15)
```

```
##            x         y
## 1  0.3686523 0.3652344
## 2  0.3685352 0.3695459
## 3  0.3681836 0.3736914
## 4  0.3675977 0.3776709
## 5  0.3667773 0.3814844
## 6  0.3657227 0.3851318
## 7  0.3644336 0.3886133
## 8  0.3629102 0.3919287
## 9  0.3611523 0.3950781
## 10 0.3591602 0.3980615
## 11 0.3569336 0.4008789
## 12 0.3544531 0.4034766
## 13 0.3516992 0.4058008
## 14 0.3486719 0.4078516
## 15 0.3453711 0.4096289
```

```r
plot(ch_n)
```

<div align="center">
  <img src="http://i.imgur.com/Y6CqPnt.png" alt="fontr_n" />
</div>

The returned data frame contains the x and y coordinates of the vertices of
the polygons.

## Loading Fonts

The method to load fonts is the same as the **showtext** package. See the
**Loading Fonts** section of the [README file](https://github.com/yixuan/showtext)
of **showtext**.


```r
library(sysfonts)
font.add.google("Lobster", "lobster")
plot(glyph_bitmap("Welcome", family = "lobster", rot = 30))
```

<div align="center">
  <img src="http://i.imgur.com/HYePkvZ.png" alt="fontr_welcome" />
</div>
