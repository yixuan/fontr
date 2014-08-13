char2outline = function(ch = "a", family = "sans")
{
    ch = as.character(ch)
    family = as.character(family)
    .Call("char2outline", ch, family, PACKAGE = "asciir")
}
