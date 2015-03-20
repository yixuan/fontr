#include <Rcpp.h>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "fonts.h"

using Rcpp::as;
using Rcpp::IntegerMatrix;
using std::string;

// Write bitmap to the matrix
// The upper-left corner of the bitmap will be written to matrix(i, j)
void write_matrix(const FT_Bitmap *bitmap, IntegerMatrix mat, int mi, int mj)
{
    // Bitmap is stored by row, starting from the upperleft corner
    // Therefore, the pixel in the p-th row and q-th column (0-based) is
    // buffer[p * bm_width + q]
    int bm_width = bitmap->width, bm_height = bitmap->rows;
    int mat_cols = mat.ncol(), mat_rows = mat.nrow();

    for(int i = mi, p = 0; p < bm_height; i++, p++)
    {
        for(int j = mj, q = 0; q < bm_width; j++, q++)
        {
            if(i < 0 || j < 0 || i >= mat_rows || j >= mat_cols)
                continue;

            mat(i, j) = bitmap->buffer[p * bm_width + q];
        }
    }
}

RcppExport SEXP char2mat(SEXP chr_, SEXP family_, SEXP fontface_, SEXP pixel_size_)
{
BEGIN_RCPP
    string str = as<string>(chr_);
    int maxlen = str.length();
    unsigned int *unicode = new unsigned int[maxlen + 1];
    int nchar = utf8toucs4(unicode, str.c_str(), maxlen);
    string family = as<string>(family_);
    int fontface = as<int>(fontface_);
    int pixel_size = as<int>(pixel_size_);

    FT_Face       face = get_ft_face(family, fontface);
    FT_GlyphSlot  slot;
    FT_Error      error;

    // Set pixel size
    error = FT_Set_Pixel_Sizes(face, pixel_size, pixel_size);
    // Error handling omitted

    // How many rows above baseline
    int bearingY;
    // How many rows below baseline
    int tail;
    // How many columns
    int advance;
    get_string_metrics(face, unicode, nchar, &bearingY, &tail, &advance);
    
    int mi = 0, mj = 0;
    slot = face->glyph;
    IntegerMatrix mat(bearingY + tail, advance);
    for(int i = 0; i < nchar; i++)
    {
        FT_Load_Char(face, unicode[i], FT_LOAD_RENDER);
        mi = bearingY - slot->bitmap_top;
        write_matrix(&(slot->bitmap), mat, mi, mj + slot->bitmap_left);
        mj += slot->advance.x / 64;
    }

    delete [] unicode;
    
    return mat;
END_RCPP
}
