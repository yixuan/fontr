#ifndef FONTS_H_INCLUDED
#define FONTS_H_INCLUDED

#include <Rcpp.h>

#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct fontDesc {
    FT_Library library;
    FT_Face face;
} FontDesc;

int utf8toucs4(unsigned int *ucs4, const char *utf8, int n);
FT_Face GetFTFace(const pGEcontext gc);

#endif /* FONTS_H_INCLUDED */

