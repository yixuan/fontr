#' Extracting the glyph outline of a character
#' 
#' This function extracts the outline of character glyphs contained in a font.
#' 
#' @param ch     A character whose glyph is to be extracted.
#' @param family The family of the font. See
#'               \code{\link[sysfonts]{font.families}()}.
#' @param face   The font face. Possible values are "regular", "bold", "italic"
#'               and "bolditalic".
#' 
#' @return A data frame with class "glyph_outline".
#'         Each row of the data frame contains two numeric values, giving the
#'         x and y coordinates of a point, and a character, indicating the action
#'         to take on that point in order to draw the outline.
#'         The meaning of different characters are given below:
#'         
#'         \item{M}{Move the pen to the point}
#'         \item{L}{Draw a straight line to the point}
#'         \item{Q}{Draw a quadratic Bezier curve to the point, followed by a
#'                  "B" row that gives the coordinates of the control point}
#'         \item{C}{Draw a cubic Bezier curve to the point, followed by two
#'                  "B" rows that give the coordinates of the control points}
#'         
#'         More details can be found in sections 8.3.4 to 8.3.7 of the SVG
#'         specification. (\url{http://www.w3.org/TR/SVG/paths.html})
#' 
#' @export
#' 
#' @author Yixuan Qiu <\url{http://statr.me/}>
#' 
#' @examples
#' R = glyph_outline("R", family = "sans", face = "bold")
#' head(R, 20)
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
    dat = .Call("glyph_outline", ch, family, face, PACKAGE = "fontr")
    class(dat) = c("glyph_outline", "data.frame")
    dat
}

#' Extracting the glyph outline of a character as polygons
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
    class(dat) = c("glyph_polygon", "data.frame")
    dat
}

#' Plotting function for glyph outline in polygon form
#' 
#' This function plots glyph outline polygons of class "glyph_polygon", typically returned
#' by the function \code{\link{glyph_polygon}()}.
#' 
#' @param x A glyph outline object of class "glyph_polygon"
#' @param \dots Additional arguments passed to \code{plot()}
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
plot.glyph_polygon = function(x, ...)
{
    x0 = min(0, x$x, na.rm = TRUE)
    x1 = max(1, x$x, na.rm = TRUE)
    y0 = min(0, x$y, na.rm = TRUE)
    y1 = max(1, x$y, na.rm = TRUE)
    plot(1, type = "n", xlim = c(x0, x1), ylim = c(y0, y1),
         xlab = "x", ylab = "y", asp = 1, ...)
    rect(0, 0, 1, 1, lty = 2)
    polypath(x$x, x$y)
}
