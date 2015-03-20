#include <Rcpp.h>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "fonts.h"

using Rcpp::as;
using Rcpp::NumericMatrix;
using std::string;

RcppExport SEXP char2mat(SEXP chr_, SEXP family_, SEXP fontface_, SEXP dim_)
{
BEGIN_RCPP
    string str = as<string>(chr_);
    int maxlen = str.length();
    unsigned int *unicode = new unsigned int[maxlen + 1];
    int nchar = utf8toucs4(unicode, str.c_str(), maxlen);
    string family = as<string>(family_);
    int fontface = as<int>(fontface_);
    int pixel_size = as<int>(dim_);

    FT_Face       face = get_ft_face(family, fontface);
    FT_GlyphSlot  slot;
    FT_Error      error;

    /* use 50 pixels */
    error = FT_Set_Pixel_Sizes(face, pixel_size, pixel_size);
    /* error handling omitted */

    int max_bearingY, max_tail;
    get_global_metrics(face, &max_bearingY, &max_tail);
    
    // Determine baseline: pixels from top canvas edge to baseline
    int baseline = pixel_size - max_tail;

    slot = face->glyph;
    error = FT_Load_Char(face, unicode[0], FT_LOAD_RENDER);
    
    int advance = slot->advance.x / 64;
    int left = slot->bitmap_left + (pixel_size - advance) / 2;
    int top = baseline - slot->bitmap_top;
    int width = slot->bitmap.width;
    int rows = slot->bitmap.rows;
    int xmax = left + width;
    int ymax = top + rows;

    NumericMatrix mat(pixel_size, pixel_size);
    for(int i = top, p = 0; i < ymax; i++, p++)
    {
        for(int j = left, q = 0; j < xmax; j++, q++)
        {
            if(i < 0 || j < 0 || i >= pixel_size || j >= pixel_size)
                continue;

            mat(i, j) = slot->bitmap.buffer[p * width + q];
        }
    }

    delete [] unicode;
    
    return mat;
END_RCPP
}
