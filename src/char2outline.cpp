#include <Rcpp.h>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "fonts.h"
#include "outline.h"

using namespace Rcpp;
using std::string;

RcppExport SEXP char2outline(SEXP _chr, SEXP _family, SEXP _fontface)
{
BEGIN_RCPP
    string str = as<string>(_chr);
    int maxlen = str.length();
    unsigned int *unicode = new unsigned int[maxlen + 1];
    int nchar = utf8toucs4(unicode, str.c_str(), maxlen);
    string family = as<string>(_family);
    int fontface = as<int>(_fontface);

    FT_Face       face = get_ft_face(family, fontface);
    FT_GlyphSlot  slot;
    FT_Error      error;

    slot = face->glyph;
    error = FT_Load_Char(face, unicode[0], FT_LOAD_NO_SCALE);
    
    FT_Outline_Funcs funs = {outlineMoveTo,
                             outlineLineTo,
                             outlineConicTo,
                             outlineCubicTo,
                             0,
                             0};
    
    std::vector<double> x;
    std::vector<double> y;
    std::vector<char> type;
    OutlineData data(&x, &y, &type, face->units_per_EM);
    
    FT_Outline_Decompose(&slot->outline, &funs, &data);
    
    return DataFrame::create(Named("x") = wrap(x),
                             Named("y") = wrap(y),
                             Named("type") = wrap(type));
END_RCPP
}

RcppExport SEXP char2seg(SEXP _chr, SEXP _family, SEXP _fontface, SEXP _nseg)
{
BEGIN_RCPP
    string str = as<string>(_chr);
    int maxlen = str.length();
    unsigned int *unicode = new unsigned int[maxlen + 1];
    int nchar = utf8toucs4(unicode, str.c_str(), maxlen);
    string family = as<string>(_family);
    int fontface = as<int>(_fontface);
    int nseg = as<int>(_nseg);

    FT_Face       face = get_ft_face(family, fontface);
    FT_GlyphSlot  slot;
    FT_Error      error;

    slot = face->glyph;
    error = FT_Load_Char(face, unicode[0], FT_LOAD_NO_SCALE);
    
    FT_Outline_Funcs funs = {segMoveTo,
                             segLineTo,
                             segConicTo,
                             segCubicTo,
                             0,
                             0};
    
    std::vector<double> x;
    std::vector<double> y;
    SegData data(&x, &y, face->units_per_EM, nseg);
    
    FT_Outline_Decompose(&slot->outline, &funs, &data);
    
    return DataFrame::create(Named("x") = wrap(x),
                             Named("y") = wrap(y));
END_RCPP
}
