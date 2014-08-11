char2mat = function(ch = "A", dim = 50, family = "mono")
{
    ch = as.character(ch)
    dim = as.integer(dim)
    family = as.character(family)
    .Call("char2mat", ch, dim, family, PACKAGE = "asciir")
}

viewmat = function(mat)
{
    m = nrow(mat)
    n = ncol(mat)
    image(1:n, 1:m, t(mat[m:1, ]), col = grey(45:0/50), asp = 1)
}