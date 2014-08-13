#ifndef OUTLINE_H_INCLUDED
#define OUTLINE_H_INCLUDED

#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H

// These are data passed to outline drawing handlers
class OutlineData
{
private:
    double units_per_EM;
public:
    std::vector<double> *x;
    std::vector<double> *y;
    std::vector<char> *type;
    
    OutlineData(std::vector<double> *_x,
                std::vector<double> *_y,
                std::vector<char> *_type,
                int _units) :
        x(_x), y(_y), type(_type), units_per_EM(_units) {}
    
    void append_point(const FT_Vector *_point, char _type)
    {
        x->push_back(_point->x / units_per_EM);
        y->push_back(_point->y / units_per_EM);
        type->push_back(_type);
    }
};

// Outline drawing functions used by FT_Outline_Decompose()
int outlineMoveTo(const FT_Vector* to, void* user);
int outlineLineTo(const FT_Vector* to, void* user);
int outlineConicTo(const FT_Vector* control, const FT_Vector* to, void* user);
int outlineCubicTo(const FT_Vector* control1, const FT_Vector* control2,
                   const FT_Vector* to, void* user);

#endif /* OUTLINE_H_INCLUDED */
