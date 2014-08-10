#include <Rcpp.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
using namespace Rcpp;
using std::string;

RcppExport SEXP char2mat(SEXP _chr, SEXP _dim)
{
BEGIN_RCPP
    string str = as<string>(_chr);
    char ch = str[0];
    int pixel_size = as<int>(_dim);
    
    FT_Library    library;
    FT_Face       face;
    FT_GlyphSlot  slot;
    FT_Error      error;

    char*         filename = "/home/qyx/consola.ttf";
    error = FT_Init_FreeType(&library);              /* initialize library */
    /* error handling omitted */

    error = FT_New_Face(library, filename, 0, &face);/* create face object */
    /* error handling omitted */

    /* use 50 pixels */
    error = FT_Set_Pixel_Sizes(face, pixel_size, pixel_size);
    /* error handling omitted */

    slot = face->glyph;
    error = FT_Load_Char(face, ch, FT_LOAD_RENDER);
    
    int left = slot->bitmap_left;
    int top = pixel_size - slot->bitmap_top;
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
    FT_Done_Face    ( face );
    FT_Done_FreeType( library );

    return mat;
END_RCPP
}