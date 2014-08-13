#ifndef OUTLINE_H_INCLUDED
#define OUTLINE_H_INCLUDED

#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H

// These are data passed to outline drawing handlers
class OutlineData
{
protected:
    std::vector<double> *x;
    std::vector<double> *y;
    std::vector<char> *type;
    double units_per_EM;
public:
    OutlineData(std::vector<double> *_x,
                std::vector<double> *_y,
                std::vector<char> *_type,
                int _units) :
        x(_x), y(_y), type(_type), units_per_EM(_units) {}
    
    void append_scaled_point(double _x, double _y, char _type)
    {
        if(x)  x->push_back(_x);
        if(y)  y->push_back(_y);
        if(type)  type->push_back(_type);
    }
    
    void append_ft_point(const FT_Vector *_point, char _type)
    {
        append_scaled_point(_point->x / units_per_EM,
                            _point->y / units_per_EM,
                            _type);
    }
};

// Outline drawing functions used by FT_Outline_Decompose()
int outlineMoveTo(const FT_Vector* to, void* user);
int outlineLineTo(const FT_Vector* to, void* user);
int outlineConicTo(const FT_Vector* control, const FT_Vector* to, void* user);
int outlineCubicTo(const FT_Vector* control1, const FT_Vector* control2,
                   const FT_Vector* to, void* user);

#endif /* OUTLINE_H_INCLUDED */
