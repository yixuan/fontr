char2mat = function(ch = "A")
{
    ch = as.character(ch)
    .Call("char2mat", ch, PACKAGE = "asciir")
}