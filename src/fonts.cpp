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

/* Get the bounding box of a string, with a possible rotation */
void get_string_bbox(FT_Face face, const unsigned int *str, int nchar, double rot,
                     int *xmin, int *xmax, int *ymin, int *ymax)
{
    int char_xmin, char_xmax, char_ymin, char_ymax;
    FT_GlyphSlot slot = face->glyph;
    FT_Matrix trans;
    FT_Vector pen;
    FT_Error err;
    int i;
    
    *xmin = *xmax = *ymin = *ymax = 0;
    
    /* Set rotation transformation */
    trans.xx = trans.yy = (FT_Fixed)( cos(rot) * 0x10000L);
    trans.xy = (FT_Fixed)(-sin(rot) * 0x10000L);
    trans.yx = -trans.xy;
    pen.x = pen.y = 0;

    for(i = 0; i < nchar; i++)
    {
        FT_Set_Transform(face, &trans, &pen);
        err = FT_Load_Char(face, str[i], FT_LOAD_RENDER);
        char_xmin = slot->bitmap_left;
        char_xmax = char_xmin + slot->bitmap.width;
        char_ymax = slot->bitmap_top;
        char_ymin = char_ymax - slot->bitmap.rows;
        if(i == 0)
        {
            *xmin = char_xmin;
            *xmax = char_xmax;
            *ymin = char_ymin;
            *ymax = char_ymax;
        } else {
            *xmin = char_xmin < *xmin ? char_xmin : *xmin;
            *xmax = char_xmax > *xmax ? char_xmax : *xmax;
            *ymin = char_ymin < *ymin ? char_ymin : *ymin;
            *ymax = char_ymax > *ymax ? char_ymax : *ymax;
        }
        /* Move the pen for the next character */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }
    
    /* Identity transformation */
    trans.xx = trans.yy = 0x10000L;
    trans.xy = trans.yx = 0;
    pen.x = pen.y = 0;
    /* Restore to identity */
    FT_Set_Transform(face, &trans, &pen);
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

