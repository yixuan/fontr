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

            unsigned char intensity = bitmap->buffer[p * bm_width + q];
            if(intensity > 0)
                mat(i, j) = intensity;
        }
    }
}

RcppExport SEXP char2mat(SEXP chr_, SEXP family_, SEXP fontface_,
                         SEXP pixel_size_, SEXP rot_)
{
BEGIN_RCPP
    string str = as<string>(chr_);
    int maxlen = str.length();
    unsigned int *unicode = new unsigned int[maxlen + 1];
    int nchar = utf8toucs4(unicode, str.c_str(), maxlen);
    string family = as<string>(family_);
    int fontface = as<int>(fontface_);
    int pixel_size = as<int>(pixel_size_);
    double rot = as<double>(rot_);

    FT_Face       face = get_ft_face(family, fontface);
    FT_GlyphSlot  slot = face->glyph;
    FT_Error      error;

    // Set pixel size
    error = FT_Set_Pixel_Sizes(face, pixel_size, pixel_size);
    // Error handling omitted

    // Bounding box
    int xmin, xmax, ymin, ymax;
    get_string_bbox(face, unicode, nchar, rot, &xmin, &xmax, &ymin, &ymax);
    
    // Set rotation for transformation
    FT_Matrix trans;
    FT_Vector pen;
    trans.xx = trans.yy = (FT_Fixed)( cos(rot) * 0x10000L);
    trans.xy = (FT_Fixed)(-sin(rot) * 0x10000L);
    trans.yx = -trans.xy;
    pen.x = pen.y = 0;
    
    int mi = 0, mj = 0;
    IntegerMatrix mat(ymax - ymin, xmax - xmin);
    for(int i = 0; i < nchar; i++)
    {
        FT_Set_Transform(face, &trans, &pen);
        FT_Load_Char(face, unicode[i], FT_LOAD_RENDER);
        mi = ymax - slot->bitmap_top;
        mj = slot->bitmap_left - xmin;
        write_matrix(&(slot->bitmap), mat, mi, mj);
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }
    
    // Restore to identity
    trans.xx = trans.yy = 0x10000L;
    trans.xy = trans.yx = 0;
    pen.x = pen.y = 0;
    FT_Set_Transform(face, &trans, &pen);

    delete [] unicode;
    
    return mat;
END_RCPP
}
