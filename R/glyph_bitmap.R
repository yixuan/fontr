glyph_bitmap = function(ch = "A", family = "mono", face = 1, pixel_size = 50, rot = 0)
{
    ch = as.character(ch)
    family = as.character(family)
    face = as.integer(face)
    pixel_size = as.integer(pixel_size)
    rot = as.numeric(rot) / 180 * pi
    mat = .Call("glyph_bitmap", ch, family, face, pixel_size, rot, PACKAGE = "fontr")
    class(mat) = "glyph_bitmap"
    mat
}

plot.glyph_bitmap = function(glyph, ...)
{
    m = nrow(glyph)
    n = ncol(glyph)
    image(1:n, 1:m, t(glyph[m:1, ]), col = grey((45:0) / 50), asp = 1,
          axes = FALSE, xlab = "", ylab = "", ...)
}