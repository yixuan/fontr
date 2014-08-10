#include <Rcpp.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
using namespace Rcpp;
using std::string;

RcppExport SEXP char2mat(SEXP _chr) {
BEGIN_RCPP
    string str = as<string>(_chr);
    char ch = str[0];
    
    FT_Library    library;
    FT_Face       face;
    FT_GlyphSlot  slot;
    FT_Error      error;

    char*         filename = "/home/qyx/consola.ttf";
    error = FT_Init_FreeType(&library);              /* initialize library */
    /* error handling omitted */

    error = FT_New_Face(library, filename, 0, &face);/* create face object */
    /* error handling omitted */

    /* use 50pt at 100dpi */
    error = FT_Set_Char_Size(face, 50 * 64, 0,
                             100, 0 );                /* set character size */
    /* error handling omitted */

    slot = face->glyph;
    error = FT_Load_Char(face, ch, FT_LOAD_RENDER);
    
    int width = slot->bitmap.width;
    int height = slot->bitmap.rows;
    
    NumericMatrix mat(width, height);
    for(int i = 0; i < width; i++)
        for(int j = 0; j < height; j++)
            mat(i, height - j - 1) = slot->bitmap.buffer[i + width * j];

    FT_Done_Face    ( face );
    FT_Done_FreeType( library );

    return mat;
END_RCPP
}