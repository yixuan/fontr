#' Extracting bitmap glyph from fonts loaded by the 'sysfonts' package
#' 
#' This function returns a bitmap matrix that represents the glyph of a given
#' string with the specified font.
#' 
#' @param ch         A character string whose glyph is to be extracted.
#' @param family     The family of the font. See
#'                   \code{\link[sysfonts]{font.families}()}.
#' @param face       The font face. Possible values are "regular", "bold", "italic"
#'                   and "bolditalic".
#' @param pixel_size The pixel size of the font.
#' @param rot        The angle in degrees of the counterclockwise string rotation.
#' 
#' @return A matrix with class "glyph_bitmap" that represents the bitmap glyph.
#'         Values in the matrix are integers ranging from 0 to 255. Larger
#'         number indicates a darker color.
#' 
#' @export
#' 
#' @author Yixuan Qiu <\url{http://statr.me/}>
#' 
#' @examples
#' f = glyph_bitmap("fontr", family = "serif", face = "italic")
#' plot(f)
glyph_bitmap = function(ch = "A", family = "mono", face = "regular", pixel_size = 50, rot = 0)
{
    ch = as.character(ch)
    family = as.character(family)
    face = switch(face,
                  regular    = 1L,
                  bold       = 2L,
                  italic     = 3L,
                  bolditalic = 4L,
                  stop("unsupported font face"))
    face = as.integer(face)
    pixel_size = as.integer(pixel_size)
    rot = as.numeric(rot) / 180 * pi
    mat = .Call("glyph_bitmap", ch, family, face, pixel_size, rot, PACKAGE = "fontr")
    class(mat) = "glyph_bitmap"
    mat
}

#' Plotting function for bitmap glyph
#' 
#' This function plots a bitmap glyph of class "glyph_bitmap", typically returned
#' by the function \code{\link{glyph_bitmap}()}.
#' 
#' @param x A bitmap glyph of class "glyph_bitmap"
#' @param \dots Additional arguments passed to \code{image()}
#' 
#' @export
#' 
#' @author Yixuan Qiu <\url{http://statr.me/}>
#' 
#' @examples
#' f = glyph_bitmap("fontr", family = "serif", face = "italic")
#' plot(f)
#' 
#' @seealso \code{\link{glyph_bitmap}()}
plot.glyph_bitmap = function(x, ...)
{
    m = nrow(x)
    n = ncol(x)
    image(1:n, 1:m, t(x[m:1, ]), col = grey((45:0) / 50), asp = 1,
          axes = FALSE, xlab = "", ylab = "", ...)
}