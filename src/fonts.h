#ifndef FONTS_H_INCLUDED
#define FONTS_H_INCLUDED

#include <Rcpp.h>
#include <string>
using std::string;

#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct fontDesc {
    FT_Library library;
    FT_Face face;
} FontDesc;

FT_Face get_ft_face(string &family, int fontface);

void get_char_metrics(FT_Face face, unsigned int ch,
                      int *bearingY, int *tail, int *advance);
                      
void get_string_metrics(FT_Face face, const unsigned int *str, int nchar,
                        int *bearingY, int *tail, int *advance);

int utf8toucs4(unsigned int *ucs4, const char *utf8, int n);

#endif /* FONTS_H_INCLUDED */

