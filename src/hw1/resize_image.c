#include <math.h>
#include "image.h"


int closest_int(float pos)
{
    if((pos - (int)pos) >= 0.5) {
        return (int)pos + 1;
    }
    return (int)pos;
}

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    return 0;
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    return make_image(1,1,1);
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    return 0;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    return make_image(1,1,1);
}

