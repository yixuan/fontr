char2mat = function(ch = "A", dim = 50)
{
    ch = as.character(ch)
    dim = as.integer(dim)
    .Call("char2mat", ch, dim, PACKAGE = "asciir")
}

viewmat = function(mat)
{
    m = nrow(mat)
    n = ncol(mat)
    image(1:n, 1:m, t(mat[m:1, ]), col = grey(45:0/50), asp = 1)
}