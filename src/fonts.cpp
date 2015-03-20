#include "fonts.h"
#include "utf8.h"

using namespace Rcpp;
using std::string;

FT_Face get_ft_face(string &family, int fontface)
{
    // Get the namespace environment of package "sysfonts"
    Environment sysfonts = Environment::namespace_env("sysfonts");
    // Get the .pkg.env environment defined in sysfonts
    Environment pkgenv = sysfonts[".pkg.env"];
    // Get the font list
    List font_list = pkgenv[".font.list"];
    // Family names of the registered fonts
    CharacterVector font_names = font_list.names();
    
    // Search for font with the specified family name
    int i;
    char *font_name;
    for(i = 0; i < font_names.length(); i++)
    {
        font_name = font_names[i];
        if(font_name == family)
            break;
    }
    // If not found, use the default mono family
    if(i == font_names.length())
        i = 2; // mono
    // Data stored in font list are pointers to FontDesc structure
    List font_selected = font_list[i];
    
    if(fontface < 1 || fontface > 5)
        fontface = 1;
    SEXP extPtr = font_selected[fontface - 1];
    FontDesc *font = (FontDesc *) R_ExternalPtrAddr(extPtr);
    
    return font->face;
}

// bearingY is the vertical distance from the baseline to the top of the glyph
// tail is the vertical distance from the baseline to the bottom of the glyph
// Both are positive integers
void get_char_metrics(FT_Face face, unsigned int ch,
                      int *bearingY, int *tail, int *advance)
{
    FT_Error error = FT_Load_Char(face, ch, FT_LOAD_RENDER);
    *bearingY = face->glyph->bitmap_top;
    *tail = face->glyph->bitmap.rows - *bearingY;
    *advance = face->glyph->advance.x / 64;
}

void get_string_metrics(FT_Face face, const unsigned int *str, int nchar,
                        int *bearingY, int *tail, int *advance)
{
    int char_bearingY, char_tail, char_advance;
    *bearingY = 0;
    *tail = 0;
    *advance = 0;

    for(int i = 0; i < nchar; i++)
    {
        get_char_metrics(face, str[i], &char_bearingY, &char_tail, &char_advance);
        if(char_bearingY > *bearingY)  *bearingY = char_bearingY;
        if(char_tail > *tail)  *tail = char_tail;
        *advance += char_advance;
    }
}

int utf8toucs4(unsigned int *ucs4, const char *utf8, int n)
{
    int len = 0;
    int step = 0;
    int err;
    unsigned int *p1;
    const char * p2;
    for(p1 = ucs4, p2 = utf8; ; p1++, p2 += step)
    {
        /* if we meet '\0' */
        if(!p2[0]) break;
        err = TY_(DecodeUTF8BytesToChar)(p1, p2[0], p2 + 1, &step);
        if(err)
        {
            Rf_warning("UTF-8 decoding error for '%s'", utf8);
            *p1 = 0xFFFD; /* replacement char */
        }
        len++;
        if(len >= n) break;
    }
    return len;
}

