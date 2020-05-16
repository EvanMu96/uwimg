#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

#define PI 3.14

float get_pixel(image im, int x, int y, int c)
{
    // clamp strategy
    x = x >= im.w ? im.w - 1 : x;
    y = y >= im.h ? im.h - 1  : y;
    c = c >= im.c ? im.c - 1 : c;

    return im.data[c * im.h * im.w + y * im.w + x ];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if(x >= im.w || x < 0) return;
    if(y >= im.h || y < 0) return;
    if(c >= im.c || c < 0) return;

    v = v < 0 ? 0 : v;

    im.data[c * im.h * im.w + y * im.w + x ] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    copy.c = im.c;
    copy.w = im.w;
    copy.h = im.h;

    for(int i = 0; i < im.w * im.h * im.c; i++) {
        copy.data[i] = im.data[i];
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3); 
    image gray = make_image(im.w, im.h, 1);
    for(int i = 0; i < im.w; i++) {
        for(int j = 0; j < im.h; j++) {
            float luma = 0.299 * get_pixel(im, i, j, 0) + 0.587 * get_pixel(im, i, j, 1) + 0.114 * get_pixel(im, i, j, 2);
            set_pixel(gray, i, j, 0, luma);
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    if(c >= im.c) return;
    // TODO Fill this in
    for(int i = 0; i < im.w; i++) {
        for(int j = 0; j < im.h; j++) {
            set_pixel(im, i, j, c,  v + get_pixel(im, i, j, c));
        }
    }
}

void clamp_image(image im)
{
    for(int i = 0; i < im.w; i++) {
        for(int j = 0; j < im.h; j++) {
            for(int k = 0; k < im.c; k++) {
                if(get_pixel(im, i, j, k) > 1) set_pixel(im, i, j, k, 1.0);
                if(get_pixel(im, i, j, k) < 0) set_pixel(im, i, j, k, 0.0);
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

float abs_float(float a) {
    return (a < 0) ? -a : a;
}


#define R rgb[0]
#define G rgb[1]
#define B rgb[2]

#define H hsv[0]
#define S hsv[1]
#define V hsv[2]
// in-place transform
void rgb_to_hsv(image im)
{
    float rgb[3];
    float hsv[3];

    assert(im.c == 3);
    for(int i = 0; i < im.w; i++) {
        for(int j = 0; j < im.h; j++) {
            
            for(int k = 0; k < im.c; k++) {
                rgb[k] = get_pixel(im, i, j, k);
            }
            V = three_way_max(R, G, B);
            float m = three_way_min(R, G, B);
            float C = V - m;
            S = C / V;
            for(int k = 0; k < 3; k++) {
                H = 0;
                if(C != 0) {
                    if(V == R) H = (G - B) / C;
                    else if(V == G) H = (B - R) / C + 2;
                    else H = (R - G) / C + 4;
                }
                if(H < 0) {
                    H = (H / 6) + 1;
                }
                else {
                    H = (H / 6);
                }
                set_pixel(im, i, j, k, hsv[k]);
            }
        }
    }
}

void hsv_to_rgb(image im)
{
    float rgb[3];
    float hsv[3];
    
    assert(im.c == 3);
    const int factor =  60;
    float C = 0;
    float X = 0;
    float m = 0;
    for(int i = 0; i < im.w; i++) {
        for(int j = 0; j < im.h; j++) {
            
            for(int k = 0; k < im.c; k++) {
                if(k == 0) 
                    hsv[k] = get_pixel(im, i, j, 360 * k);
                else 
                    hsv[k] = get_pixel(im, i, j, k);
            }
            // well this part of code is extremely ugly
            C = V * S;
            X = C * (1 - abs((int)(H / factor) % 2 - 1));
            m = V - C;

            if(H >= 0 && H < factor) {
                R = C;
                G = X;
                B = 0;    
            }
            else if(H >= factor && H < 2*factor) {
                R = X;
                G = C;
                B = 0;
            }
            else if(H >= 2*factor && H < 3*factor) {
                R = 0;
                G = C;
                B = X;
            }
            else if(H >= 3*factor && H < 4*factor) {
                R = 0;
                G = X;
                B = C;
            }
            else if(H >= 4*factor && H < 5*factor) {
                R = X;
                G = 0;
                B = C;
            }
            else if(H >= 5*factor && H < 1) {
                R = C;
                G = 0;
                B = X;
            }
            else {return ;}
            
            #define PROC_RGB(v) (v + m)
            
            for(int k = 0; k < 3; k++) {
                set_pixel(im, i, j, k, PROC_RGB(rgb[k]));
            }
        }
    }


}
