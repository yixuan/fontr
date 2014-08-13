char2outline = function(ch = "a", family = "sans")
{
    ch = as.character(ch)
    family = as.character(family)
    .Call("char2outline", ch, family, PACKAGE = "asciir")
}

char2seg = function(ch = "a", family = "sans", nseg = 10L)
{
    ch = as.character(ch)
    family = as.character(family)
    nseg = as.integer(nseg)
    .Call("char2seg", ch, family, nseg, PACKAGE = "asciir")
}
