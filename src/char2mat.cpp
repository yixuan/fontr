#include <Rcpp.h>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "fonts.h"

using namespace Rcpp;
using std::string;

RcppExport SEXP char2mat(SEXP _chr, SEXP _dim, SEXP _family)
{
BEGIN_RCPP
    string str = as<string>(_chr);
    char ch = str[0];
    int pixel_size = as<int>(_dim);
    string family = as<string>(_family);

    FT_Face       face = get_ft_face(family);
    FT_GlyphSlot  slot;
    FT_Error      error;

    /* use 50 pixels */
    error = FT_Set_Pixel_Sizes(face, pixel_size, pixel_size);
    /* error handling omitted */

    int maxbrY, maxtail;
    get_global_metrics(face, &maxbrY, &maxtail);
    
    // Determine baseline: pixels from top canvas edge to baseline
    int baseline = maxbrY + (pixel_size - maxbrY - maxtail) / 2;

    slot = face->glyph;
    error = FT_Load_Char(face, ch, FT_LOAD_RENDER);
    
    int left = slot->bitmap_left;
    int top = baseline - slot->bitmap_top;
    int width = slot->bitmap.width;
    int rows = slot->bitmap.rows;
    int xmax = left + width;
    int ymax = top + rows;
    int advance = slot->advance.x / 64;
    
    NumericMatrix mat(pixel_size, advance);
    for(int i = top, p = 0; i < ymax; i++, p++)
    {
        for(int j = left, q = 0; j < xmax; j++, q++)
        {
            if(i < 0 || j < 0 || i >= pixel_size || j >= advance)
                continue;

            mat(i, j) = slot->bitmap.buffer[p * width + q];
        }
    }

    return mat;
END_RCPP
}
