char2outline = function(ch = "a", family = "sans", fontface = 1)
{
    ch = as.character(ch)
    family = as.character(family)
    fontface = as.integer(fontface)
    .Call("char2outline", ch, family, fontface, PACKAGE = "fontr")
}

char2seg = function(ch = "a", family = "sans", fontface = 1, nseg = 10)
{
    ch = as.character(ch)
    family = as.character(family)
    fontface = as.integer(fontface)
    nseg = as.integer(nseg)
    .Call("char2seg", ch, family, fontface, nseg, PACKAGE = "fontr")
}
