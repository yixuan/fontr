#include "fonts.h"

using namespace Rcpp;
using std::string;

FT_Face get_ft_face(string &family)
{
    Environment sysfonts = Environment::namespace_env("sysfonts");
    Environment pkgenv = sysfonts[".pkg.env"];
    List font_list = pkgenv[".font.list"];
    CharacterVector font_names = font_list.names();
    
    int i;
    char *font_name;
    for(i = 0; i < font_names.length(); i++)
    {
        font_name = font_names[i];
        if(font_name == family)
            break;
    }
    if(i == font_names.length())
        i = 2; // mono
    
    List font_selected = font_list[i];
    SEXP extPtr = font_selected[0];
    FontDesc *font = (FontDesc *) R_ExternalPtrAddr(extPtr);
    
    return font->face;
}
