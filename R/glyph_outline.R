glyph_outline = function(ch = "a", family = "sans", face = 1)
{
    ch = as.character(ch)
    family = as.character(family)
    face = as.integer(face)
    .Call("glyph_outline", ch, family, face, PACKAGE = "fontr")
}

glyph_polygon = function(ch = "a", family = "sans", face = 1, nseg = 10)
{
    ch = as.character(ch)
    family = as.character(family)
    face = as.integer(face)
    nseg = as.integer(nseg)
    dat = .Call("glyph_polygon", ch, family, face, nseg, PACKAGE = "fontr")
    class(dat) = "glyph_polygon"
    dat
}

plot.glyph_polygon = function(glyph, ...)
{
    x0 = min(0, glyph$x, na.rm = TRUE)
    x1 = max(1, glyph$x, na.rm = TRUE)
    y0 = min(0, glyph$y, na.rm = TRUE)
    y1 = max(1, glyph$y, na.rm = TRUE)
    plot(1, type = "n", xlim = c(x0, x1), ylim = c(y0, y1),
         xlab = "x", ylab = "y", asp = 1, ...)
    rect(0, 0, 1, 1, lty = 2)
    polypath(glyph$x, glyph$y)
}