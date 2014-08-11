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
    int maxlen = str.length();
    unsigned int *unicode = new unsigned int[maxlen + 1];
    int nchar = utf8toucs4(unicode, str.c_str(), maxlen);
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
    int baseline = pixel_size - maxtail;

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
