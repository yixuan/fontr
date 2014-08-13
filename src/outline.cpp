#include <Rcpp.h>
#include "outline.h"

int outlineMoveTo(const FT_Vector* to, void* user)
{
    OutlineData *data = (OutlineData *) user;
    data->append_ft_point(to, 'M');
    
    return 0;
}

int outlineLineTo(const FT_Vector* to, void* user)
{
    OutlineData *data = (OutlineData *) user;
    data->append_ft_point(to, 'L');
    
    return 0;
}

int outlineConicTo(const FT_Vector* control, const FT_Vector* to, void* user)
{
    OutlineData *data = (OutlineData *) user;
    data->append_ft_point(to, 'Q');
    data->append_ft_point(control, 'B');
    
    return 0;
}

int outlineCubicTo(const FT_Vector* control1, const FT_Vector* control2,
                   const FT_Vector* to, void* user)
{
    OutlineData *data = (OutlineData *) user;
    data->append_ft_point(to, 'C');
    data->append_ft_point(control1, 'B');
    data->append_ft_point(control2, 'B');
    
    return 0;
}





void SegData::append_scaled_conic(double to_x, double to_y,
                                  double ctrl_x, double ctrl_y)
{
    double from_x = x->back();
    double from_y = y->back();
    double delta = 1.0 / nseg;
    double lambda = delta;
    double pen_x = 0, pen_y = 0;
    double coef1 = 0, coef2 = 0, coef3 = 0;
    
    for(lambda = delta; lambda < 1; lambda += delta)
    {
        coef2 = 2 * lambda * (1 - lambda);
        coef3 = lambda * lambda;
        coef1 = 1 - coef2 - coef3;
        
        pen_x = coef1 * from_x + coef2 * ctrl_x + coef3 * to_x;
        pen_y = coef1 * from_y + coef2 * ctrl_y + coef3 * to_y;
        append_scaled_point(pen_x, pen_y);
    }
    append_scaled_point(to_x, to_y);
}

void SegData::append_scaled_cubic(double to_x, double to_y,
                                  double ctrl1_x, double ctrl1_y,
                                  double ctrl2_x, double ctrl2_y)
{
    double from_x = x->back();
    double from_y = y->back();
    double delta = 1.0 / nseg;
    double lambda = delta;
    double pen_x = 0, pen_y = 0;
    double coef1 = 0, coef2 = 0, coef3 = 0, coef4 = 0;
    
    for(lambda = delta; lambda < 1; lambda += delta)
    {
        coef2 = 3 * lambda * (1 - lambda) * (1 - lambda);
        coef3 = 3 * lambda * lambda * (1 - lambda);
        coef4 = lambda * lambda * lambda;
        coef1 = 1 - coef2 - coef3 - coef4;
        
        pen_x = coef1 * from_x + coef2 * ctrl1_x + coef3 * ctrl2_x + coef4 * to_x;
        pen_y = coef1 * from_y + coef2 * ctrl1_y + coef3 * ctrl2_y + coef4 * to_y;
        append_scaled_point(pen_x, pen_y);
    }
    append_scaled_point(to_x, to_y);
}

int segMoveTo(const FT_Vector* to, void* user)
{
    SegData *data = (SegData *) user;
    // If this is not the first point, it starts a new
    // subpath, so we need to add a pair of NA to
    // x and y.
    // This is the convention of polypath() in R
    if(data->size())
        data->append_scaled_point(NA_REAL, NA_REAL);
    data->append_ft_point(to, 0);
    
    return 0;
}

int segLineTo(const FT_Vector* to, void* user)
{
    SegData *data = (SegData *) user;
    data->append_ft_point(to, 0);
    
    return 0;
}

int segConicTo(const FT_Vector* control, const FT_Vector* to, void* user)
{
    SegData *data = (SegData *) user;
    data->append_scaled_conic(to->x / data->units(), to->y / data->units(),
                              control->x / data->units(), control->y / data->units());
    
    return 0;
}

int segCubicTo(const FT_Vector* control1, const FT_Vector* control2,
                   const FT_Vector* to, void* user)
{
    SegData *data = (SegData *) user;
    data->append_scaled_cubic(to->x / data->units(), to->y / data->units(),
                              control1->x / data->units(), control1->y / data->units(),
                              control2->x / data->units(), control2->y / data->units());
    
    return 0;
}
