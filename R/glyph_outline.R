glyph_outline = function(ch = "a", family = "sans", face = 1)
{
    ch = as.character(ch)
    family = as.character(family)
    face = as.integer(face)
    .Call("glyph_outline", ch, family, fontface, PACKAGE = "fontr")
}

glyph_polygon = function(ch = "a", family = "sans", face = 1, nseg = 10)
{
    ch = as.character(ch)
    family = as.character(family)
    face = as.integer(face)
    nseg = as.integer(nseg)
    .Call("glyph_polygon", ch, family, fontface, nseg, PACKAGE = "fontr")
}
