glyph_outline = function(ch = "a", family = "sans", face = "regular")
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
    .Call("glyph_outline", ch, family, face, PACKAGE = "fontr")
}

#' Extracting glyph outline of characters as polygons
#' 
#' This function extracts the outline of character glyphs contained in a font,
#' and converts the outline into polygons.
#' 
#' @param ch     A character whose glyph is to be extracted.
#' @param family The family of the font. See
#'               \code{\link[sysfonts]{font.families}()}.
#' @param face   The font face. Possible values are "regular", "bold", "italic"
#'               and "bolditalic".
#' @param nseg   The number of line segments to approximate
#'               a piece of curve in the outline. The larger \code{nseg} is,
#'               the smoother text outlines would be. Usually a value
#'               between 5~20 would be enough.
#' 
#' @return A data frame with class "glyph_polygon" that contains two columns,
#'         giving the x and y coordinates of the outline. Rows that contain
#'         \code{NA} represents the start of a new polygon path.
#' 
#' @export
#' 
#' @author Yixuan Qiu <\url{http://statr.me/}>
#' 
#' @examples
#' R = glyph_polygon("R", family = "sans", face = "bold")
#' plot(R)
glyph_polygon = function(ch = "a", family = "sans", face = "regular", nseg = 10)
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
    nseg = as.integer(nseg)
    dat = .Call("glyph_polygon", ch, family, face, nseg, PACKAGE = "fontr")
    class(dat) = "glyph_polygon"
    dat
}

#' Plotting function for glyph outline in polygon form
#' 
#' This function plots glyph outline polygons of class "glyph_polygon", typically returned
#' by the function \code{\link{glyph_polygon}()}.
#' 
#' @export
#' 
#' @author Yixuan Qiu <\url{http://statr.me/}>
#' 
#' @examples
#' R = glyph_polygon("R", family = "sans", face = "bold")
#' plot(R)
#' 
#' @seealso \code{\link{glyph_polygon}()}
plot.glyph_polygon = function(glyph, ...)
{
    x0 = min(0, glyph$x, na.rm = TRUE)
    x1 = max(1, glyph$x, na.rm = TRUE)
    y0 = min(0, glyph$y, na.rm = TRUE)
    y1 = max(1, glyph$y, na.rm = TRUE)
    plot(1, type = "n", xlim = c(x0, x1), ylim = c(y0, y1),
         xlab = "x", ylab = "y", asp = 1, ...)
    rect(0, 0, 1, 1, lty = 2)
    polypath(glyph$x, glyph$y)
}
