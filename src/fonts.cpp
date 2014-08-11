#include "fonts.h"

using namespace Rcpp;
using std::string;

FT_Face get_ft_face(string &family)
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
    SEXP extPtr = font_selected[0];
    FontDesc *font = (FontDesc *) R_ExternalPtrAddr(extPtr);
    
    return font->face;
}
