char2mat = function(ch = "A", family = "mono", fontface = 1, size = 50)
{
    ch = as.character(ch)
    family = as.character(family)
    fontface = as.integer(fontface)
    size = as.integer(size)
    .Call("char2mat", ch, family, fontface, size, PACKAGE = "fontr")
}

viewmat = function(mat)
{
    m = nrow(mat)
    n = ncol(mat)
    image(1:n, 1:m, t(mat[m:1, ]), col = grey(45:0/50), asp = 1)
}