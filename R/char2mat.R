char2mat = function(ch = "A", family = "mono", fontface = 1, size = 50, rot = 0)
{
    ch = as.character(ch)
    family = as.character(family)
    fontface = as.integer(fontface)
    size = as.integer(size)
    rot = as.numeric(rot) / 180 * pi
    .Call("char2mat", ch, family, fontface, size, rot, PACKAGE = "fontr")
}

viewmat = function(mat)
{
    m = nrow(mat)
    n = ncol(mat)
    image(1:n, 1:m, t(mat[m:1, ]), col = grey(45:0/50), asp = 1)
}