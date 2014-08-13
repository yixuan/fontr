char2mat = function(ch = "A", family = "mono", fontface = 1, dim = 50)
{
    ch = as.character(ch)
    family = as.character(family)
    fontface = as.integer(fontface)
    dim = as.integer(dim)
    .Call("char2mat", ch, family, fontface, dim, PACKAGE = "asciir")
}

viewmat = function(mat)
{
    m = nrow(mat)
    n = ncol(mat)
    image(1:n, 1:m, t(mat[m:1, ]), col = grey(45:0/50), asp = 1)
}