#include "outline.h"

int outlineMoveTo(const FT_Vector* to, void* user)
{
    OutlineData *data = (OutlineData *) user;
    data->append_point(to, 'M');
    
    return 0;
}

int outlineLineTo(const FT_Vector* to, void* user)
{
    OutlineData *data = (OutlineData *) user;
    data->append_point(to, 'L');
    
    return 0;
}

int outlineConicTo(const FT_Vector* control, const FT_Vector* to, void* user)
{
    OutlineData *data = (OutlineData *) user;
    data->append_point(to, 'Q');
    data->append_point(control, 'B');
    
    return 0;
}

int outlineCubicTo(const FT_Vector* control1, const FT_Vector* control2,
                   const FT_Vector* to, void* user)
{
    OutlineData *data = (OutlineData *) user;
    data->append_point(to, 'C');
    data->append_point(control1, 'B');
    data->append_point(control2, 'B');
    
    return 0;
}
