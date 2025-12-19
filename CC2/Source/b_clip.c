/*   ______   ___       ___         ____     ______  ____
*   /\  _  \ /\_ \    /'___\       /\  _`\  /\  _  \/\  _`\
*   \ \ \L\ \\//\ \  /\ \__/   __  \ \ \/\_\\ \ \L\ \ \ \/\ \
*    \ \  __ \ \ \ \ \ \ ,__\/'__`\ \ \ \/_/_\ \  __ \ \ \ \ \
*     \ \ \/\ \ \_\ \_\ \ \_/\ \L\.\_\ \ \L\ \\ \ \/\ \ \ \_\ \
*      \ \_\ \_\/\____\\ \_\\ \__/.\_\\ \____/ \ \_\ \_\ \____/
*       \/_/\/_/\/____/ \/_/ \/__/\/_/ \/___/   \/_/\/_/\/___/
*
*   A CAD
*
*   By Marek Ratajczak 2024
*
*   See readme_alfacad.txt for copyright information.
*
*/

#include <forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include <math.h>
#include <stddef.h>
#include "bib_e.h"
#include "b_clip.h"
#include "rysuj_e.h"

////#include "leak_detector_c.h"

#define MaxNumPolygonPoints 200

extern  BITMAP *screenplay;

extern void Get_View_Coord (int * x1, int *y1, int *x2, int *y2) ;
extern void fillpoly(int numpoints, void *polypoints);
extern void fillpoly3D(int numpoints, void *polypoints, void *polypoints_z, int bg);
extern void drawpoly(int numpoints, void *polypoints);
extern void putpixel_(int x, int y, int color);
extern int getcolor(void);

extern int get_palette_color(int color);

extern void fillpolypattern(int numpoints, AL_CONST int* polypoints, char* s_pattern, long origin_x, long origin_y);
extern void my_fillpoly(int numpoints, int* polypoints, int translucency, int kolory_paper);
extern void set_mode_trans();
extern int GTRANSLUCENCY;

void Clip_Solid (int num_we, T_PixelTVal  *poly_we, int *num_wy, int *poly_wy) ;
BOOL Clip_Solid0 (int num_we, T_PixelTVal  *poly_we, int *num_wy, int *poly_wy) ;

static void clip (int granica,
			T_PixelTVal  x1, T_PixelTVal  y1,
			T_PixelTVal  x2, T_PixelTVal  y2,
			T_PixelTVal  *x, T_PixelTVal  *y) ;
static BOOL widocznosc (int granica, T_PixelTVal  x1, T_PixelTVal  y1) ;

static void clipF(int granica,
	T_Float  x1, T_Float  y1,
	T_Float  x2, T_Float  y2,
	T_Float *x, T_Float  *y);
static BOOL widocznoscF(int granica, T_Float  x1, T_Float  y1);


static int wg, wp, wd, wl ;
static float wgf, wpf, wdf, wlf;

/*
//#include <stdio.h>  // For potential debugging, but not necessary
#include <stdbool.h>  // For bool type

typedef struct {
    double x, y;
} Point;

enum ClipSide { LEFT, RIGHT, BOTTOM, TOP };

bool inside(Point p, double bound, enum ClipSide side) {
    switch (side) {
        case LEFT:   return p.x >= bound;
        case RIGHT:  return p.x <= bound;
        case BOTTOM: return p.y >= bound;
        case TOP:    return p.y <= bound;
    }
    return false;  // Should not reach here
}

Point intersect(Point p1, Point p2, double bound, enum ClipSide side) {
    Point res;
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;

    if (side == LEFT || side == RIGHT) {
        // Vertical clip line x = bound
        double t = (bound - p1.x) / dx;
        res.x = bound;
        res.y = p1.y + t * dy;
    } else {
        // Horizontal clip line y = bound
        double t = (bound - p1.y) / dy;
        res.x = p1.x + t * dx;
        res.y = bound;
    }
    return res;
}

void clipEdge(Point input[], int inCount, Point output[], int *outCount, double bound, enum ClipSide side) {
    *outCount = 0;
    if (inCount < 2) return;

    Point s = input[inCount - 1];  // Start with the last point to close the polygon loop

    for (int i = 0; i < inCount; i++) {
        Point e = input[i];
        bool sInside = inside(s, bound, side);
        bool eInside = inside(e, bound, side);

        if (eInside) {
            if (!sInside) {
                // Crossing in: add intersection
                output[*outCount] = intersect(s, e, bound, side);
                (*outCount)++;
            }
            // Add the end point
            output[*outCount] = e;
            (*outCount)++;
        } else if (sInside) {
            // Crossing out: add intersection
            output[*outCount] = intersect(s, e, bound, side);
            (*outCount)++;
        }
        s = e;
    }
}

void clipTriangle(T_PixelTVal  *poly_we, double xmin, double ymin, double xmax, double ymax, int *poly_wy, int *outCount) {
    Point poly1[10];  // Buffer for intermediate results (max vertices for triangle clip ~6-7)
    Point poly2[10];
    Point poly3[10];
    Point poly4[10];

    Point output[10];

    int count1 = 3;
    for (int i = 0; i < 3; i++) {
        //poly1[i] = triangle[i];
        poly1[i].x = (double)poly_we[i*2];
        poly1[i].y = (double)poly_we[i*2+1];
    }

    // Clip against left edge
    int count2;
    clipEdge(poly1, count1, poly2, &count2, xmin, LEFT);

    // Clip against bottom edge
    int count3;
    clipEdge(poly2, count2, poly3, &count3, ymin, BOTTOM);

    // Clip against right edge
    int count4;
    clipEdge(poly3, count3, poly4, &count4, xmax, RIGHT);

    // Clip against top edge
    clipEdge(poly4, count4, output, outCount, ymax, TOP);

    for (int i = 0; i < *outCount; i++) {
        poly_wy[i*2]=(int)output[i].x;
        poly_wy[i*2+1]=(int)output[i].y;
    }
}
*/
// Example usage (optional, for testing):
/*
int main() {
    Point triangle[3] = {{0, 0}, {10, 0}, {5, 10}};
    Point output[10];
    int outCount;

    clipTriangle(triangle, 2, 2, 8, 8, output, &outCount);

    printf("Clipped polygon vertices: %d\n", outCount);
    for (int i = 0; i < outCount; i++) {
        printf("(%.2f, %.2f)\n", output[i].x, output[i].y);
    }

    return 0;
}
*/
/*
#include <stdbool.h>  // For bool type

typedef struct {
    float x, y, m;
} Vertex;

enum ClipSide { LEFT, RIGHT, BOTTOM, TOP };

bool inside(Vertex p, float bound, enum ClipSide side) {
    switch (side) {
        case LEFT:   return p.x >= bound;
        case RIGHT:  return p.x <= bound;
        case BOTTOM: return p.y >= bound;
        case TOP:    return p.y <= bound;
    }
    return false;  // Should not reach here
}

Vertex intersect(Vertex p1, Vertex p2, float bound, enum ClipSide side) {
    Vertex res;
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float dm = p2.m - p1.m;
    float t;

    if (side == LEFT || side == RIGHT) {
        // Vertical clip line x = bound
        t = (bound - p1.x) / dx;
        res.x = bound;
        res.y = p1.y + t * dy;
        res.m = p1.m + t * dm;
    } else {
        // Horizontal clip line y = bound
        t = (bound - p1.y) / dy;
        res.x = p1.x + t * dx;
        res.y = bound;
        res.m = p1.m + t * dm;
    }
    return res;
}

void clipEdge(Vertex input[], int inCount, Vertex output[], int *outCount, float bound, enum ClipSide side) {
    *outCount = 0;
    if (inCount < 2) return;

    Vertex s = input[inCount - 1];  // Start with the last point to close the polygon loop

    for (int i = 0; i < inCount; i++) {
        Vertex e = input[i];
        bool sInside = inside(s, bound, side);
        bool eInside = inside(e, bound, side);

        if (eInside) {
            if (!sInside) {
                // Crossing in: add intersection
                output[*outCount] = intersect(s, e, bound, side);
                (*outCount)++;
            }
            // Add the end point
            output[*outCount] = e;
            (*outCount)++;
        } else if (sInside) {
            // Crossing out: add intersection
            output[*outCount] = intersect(s, e, bound, side);
            (*outCount)++;
        }
        s = e;
    }
}

void clipTriangleWithMag(int xy[6], float mag[3], int xmin, int ymin, int xmax, int ymax, int new_xy[], float new_mag[], int *outCount) {
    Vertex input[3];
    input[0].x = (float)xy[0]; input[0].y = (float)xy[1]; input[0].m = mag[0];
    input[1].x = (float)xy[2]; input[1].y = (float)xy[3]; input[1].m = mag[1];
    input[2].x = (float)xy[4]; input[2].y = (float)xy[5]; input[2].m = mag[2];

    Vertex poly1[10];  // Buffers for intermediate results
    Vertex poly2[10];
    Vertex poly3[10];
    Vertex poly4[10];
    Vertex final[10];

    int count1 = 3;
    for (int i = 0; i < 3; i++) {
        poly1[i] = input[i];
    }

    float fxmin = (float)xmin;
    float fymin = (float)ymin;
    float fxmax = (float)xmax;
    float fymax = (float)ymax;

    // Clip against left edge
    int count2;
    clipEdge(poly1, count1, poly2, &count2, fxmin, LEFT);

    // Clip against bottom edge
    int count3;
    clipEdge(poly2, count2, poly3, &count3, fymin, BOTTOM);

    // Clip against right edge
    int count4;
    clipEdge(poly3, count3, poly4, &count4, fxmax, RIGHT);

    // Clip against top edge
    clipEdge(poly4, count4, final, outCount, fymax, TOP);

    // Output to arrays, rounding coordinates to int
    for (int i = 0; i < *outCount; i++) {
        new_xy[i * 2] = (int)round(final[i].x);
        new_xy[i * 2 + 1] = (int)round(final[i].y);
        new_mag[i] = final[i].m;
    }
}
*/
/*
//#include <stdio.h>  // For potential debugging
#include <stdbool.h>  // For bool type
//#include <math.h>   // For round

typedef struct {
    float x, y;
    unsigned int color;
} Vertex;

enum ClipSide { LEFT, RIGHT, BOTTOM, TOP };

unsigned char getR(unsigned int rgb) { return (rgb >> 16) & 0xFF; }
unsigned char getG(unsigned int rgb) { return (rgb >> 8) & 0xFF; }
unsigned char getB(unsigned int rgb) { return rgb & 0xFF; }
unsigned int packRGB(unsigned char r, unsigned char g, unsigned char b) {
    return ((unsigned int)r << 16) | ((unsigned int)g << 8) | (unsigned int)b;
}

bool inside(Vertex p, float bound, enum ClipSide side) {
    switch (side) {
        case LEFT:   return p.x >= bound;
        case RIGHT:  return p.x <= bound;
        case BOTTOM: return p.y >= bound;
        case TOP:    return p.y <= bound;
    }
    return false;  // Should not reach here
}

Vertex intersect(Vertex p1, Vertex p2, float bound, enum ClipSide side) {
    Vertex res;
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float t;

    if (side == LEFT || side == RIGHT) {
        // Vertical clip line x = bound
        t = (bound - p1.x) / dx;
        res.x = bound;
        res.y = p1.y + t * dy;
    } else {
        // Horizontal clip line y = bound
        t = (bound - p1.y) / dy;
        res.x = p1.x + t * dx;
        res.y = bound;
    }

    // Interpolate color
    float r1 = (float)getR(p1.color);
    float g1 = (float)getG(p1.color);
    float b1 = (float)getB(p1.color);
    float r2 = (float)getR(p2.color);
    float g2 = (float)getG(p2.color);
    float b2 = (float)getB(p2.color);

    unsigned char res_r = (unsigned char)round(r1 + t * (r2 - r1));
    unsigned char res_g = (unsigned char)round(g1 + t * (g2 - g1));
    unsigned char res_b = (unsigned char)round(b1 + t * (b2 - b1));

    res.color = packRGB(res_r, res_g, res_b);

    return res;
}

void clipEdge(Vertex input[], int inCount, Vertex output[], int *outCount, float bound, enum ClipSide side) {
    *outCount = 0;
    if (inCount < 2) return;

    Vertex s = input[inCount - 1];  // Start with the last point to close the polygon loop

    for (int i = 0; i < inCount; i++) {
        Vertex e = input[i];
        bool sInside = inside(s, bound, side);
        bool eInside = inside(e, bound, side);

        if (eInside) {
            if (!sInside) {
                // Crossing in: add intersection
                output[*outCount] = intersect(s, e, bound, side);
                (*outCount)++;
            }
            // Add the end point
            output[*outCount] = e;
            (*outCount)++;
        } else if (sInside) {
            // Crossing out: add intersection
            output[*outCount] = intersect(s, e, bound, side);
            (*outCount)++;
        }
        s = e;
    }
}

//void clipTriangleWithMag(int xy[6], float mag[3], int xmin, int ymin, int xmax, int ymax, int new_xy[], float new_mag[], int *outCount);
void clipTriangleWithColors(int xy[6], unsigned int colors[3], int xmin, int ymin, int xmax, int ymax, int new_xy[], unsigned int new_colors[], int *outCount) {
    Vertex input[3];
    input[0].x = (float)xy[0]; input[0].y = (float)xy[1]; input[0].color = colors[0];
    input[1].x = (float)xy[2]; input[1].y = (float)xy[3]; input[1].color = colors[1];
    input[2].x = (float)xy[4]; input[2].y = (float)xy[5]; input[2].color = colors[2];

    Vertex poly1[10];  // Buffers for intermediate results
    Vertex poly2[10];
    Vertex poly3[10];
    Vertex poly4[10];
    Vertex final[10];

    int count1 = 3;
    for (int i = 0; i < 3; i++) {
        poly1[i] = input[i];
    }

    float fxmin = (float)xmin;
    float fymin = (float)ymin;
    float fxmax = (float)xmax;
    float fymax = (float)ymax;

    // Clip against left edge
    int count2;
    clipEdge(poly1, count1, poly2, &count2, fxmin, LEFT);

    // Clip against bottom edge
    int count3;
    clipEdge(poly2, count2, poly3, &count3, fymin, BOTTOM);

    // Clip against right edge
    int count4;
    clipEdge(poly3, count3, poly4, &count4, fxmax, RIGHT);

    // Clip against top edge
    clipEdge(poly4, count4, final, outCount, fymax, TOP);

    // Output to arrays, rounding coordinates to int
    for (int i = 0; i < *outCount; i++) {
        new_xy[i * 2] = (int)round(final[i].x);
        new_xy[i * 2 + 1] = (int)round(final[i].y);
        new_colors[i] = final[i].color;
    }
}
*/
/*
//#include <stdio.h>  // For potential debugging
#include <stdbool.h>  // For bool type
//#include <math.h>   // For round

typedef struct {
    float x, y;
    unsigned int color;
} Vertex;

enum ClipSide { LEFT, RIGHT, BOTTOM, TOP };

unsigned char getR(unsigned int rgb) { return (rgb >> 16) & 0xFF; }
unsigned char getG(unsigned int rgb) { return (rgb >> 8) & 0xFF; }
unsigned char getB(unsigned int rgb) { return rgb & 0xFF; }
unsigned int packRGB(unsigned char r, unsigned char g, unsigned char b) {
    return ((unsigned int)r << 16) | ((unsigned int)g << 8) | (unsigned int)b;
}

bool inside(Vertex p, float bound, enum ClipSide side) {
    switch (side) {
        case LEFT:   return p.x >= bound;
        case RIGHT:  return p.x <= bound;
        case BOTTOM: return p.y >= bound;
        case TOP:    return p.y <= bound;
    }
    return false;  // Should not reach here
}

Vertex intersect(Vertex p1, Vertex p2, float bound, enum ClipSide side) {
    Vertex res = {0.0f, 0.0f, 0};  // Initialize to avoid uninitialized memory
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float t;

    // Handle division by zero
    if (side == LEFT || side == RIGHT) {
        if (fabsf(dx) < 1e-6f) {  // Vertical segment
            res.x = p1.x;
            res.y = p1.y;  // Keep original y if segment is vertical
        } else {
            t = (bound - p1.x) / dx;
            res.x = bound;
            res.y = p1.y + t * dy;
        }
    } else {  // BOTTOM or TOP
        if (fabsf(dy) < 1e-6f) {  // Horizontal segment
            res.x = p1.x;
            res.y = p1.y;  // Keep original y if segment is horizontal
        } else {
            t = (bound - p1.y) / dy;
            res.x = p1.x + t * dx;
            res.y = bound;
        }
    }

    // Interpolate color
    float r1 = (float)getR(p1.color);
    float g1 = (float)getG(p1.color);
    float b1 = (float)getB(p1.color);
    float r2 = (float)getR(p2.color);
    float g2 = (float)getG(p2.color);
    float b2 = (float)getB(p2.color);

    unsigned char res_r = (unsigned char)round(r1 + t * (r2 - r1));
    unsigned char res_g = (unsigned char)round(g1 + t * (g2 - g1));
    unsigned char res_b = (unsigned char)round(b1 + t * (b2 - b1));

    res.color = packRGB(res_r, res_g, res_b);

    return res;
}

void clipEdge(Vertex input[], int inCount, Vertex output[], int *outCount, float bound, enum ClipSide side) {
    *outCount = 0;
    if (inCount < 2) return;

    // Initialize output array to avoid uninitialized memory
    for (int i = 0; i < 10; i++) {
        output[i].x = 0.0f;
        output[i].y = 0.0f;
        output[i].color = 0;
    }

    Vertex s = input[inCount - 1];  // Start with the last point to close the polygon loop

    for (int i = 0; i < inCount; i++) {
        Vertex e = input[i];
        bool sInside = inside(s, bound, side);
        bool eInside = inside(e, bound, side);

        if (eInside) {
            if (!sInside) {
                // Crossing in: add intersection
                output[*outCount] = intersect(s, e, bound, side);
                // printf("Intersection: x=%.2f, y=%.2f, color=0x%06X\n", output[*outCount].x, output[*outCount].y, output[*outCount].color);
                (*outCount)++;
            }
            // Add the end point
            output[*outCount] = e;
            // printf("Vertex: x=%.2f, y=%.2f, color=0x%06X\n", output[*outCount].x, output[*outCount].y, output[*outCount].color);
            (*outCount)++;
        } else if (sInside) {
            // Crossing out: add intersection
            output[*outCount] = intersect(s, e, bound, side);
            // printf("Intersection: x=%.2f, y=%.2f, color=0x%06X\n", output[*outCount].x, output[*outCount].y, output[*outCount].color);
            (*outCount)++;
        }
        s = e;
    }
}

void clipTriangleWithColors(long *xy, unsigned int colors[3], int xmin, int ymin, int xmax, int ymax, int new_xy[], unsigned int new_colors[], int *outCount) {
    Vertex input[3];
    input[0].x = (float)xy[0]; input[0].y = (float)xy[1]; input[0].color = colors[0];
    input[1].x = (float)xy[2]; input[1].y = (float)xy[3]; input[1].color = colors[1];
    input[2].x = (float)xy[4]; input[2].y = (float)xy[5]; input[2].color = colors[2];

    Vertex poly1[10];  // Buffers for intermediate results
    Vertex poly2[10];
    Vertex poly3[10];
    Vertex poly4[10];
    Vertex final[10];

    // Initialize buffers to avoid uninitialized memory
    for (int i = 0; i < 10; i++) {
        poly1[i].x = poly1[i].y = poly2[i].x = poly2[i].y = poly3[i].x = poly3[i].y = poly4[i].x = poly4[i].y = final[i].x = final[i].y = 0.0f;
        poly1[i].color = poly2[i].color = poly3[i].color = poly4[i].color = final[i].color = 0;
    }

    int count1 = 3;
    for (int i = 0; i < 3; i++) {
        poly1[i] = input[i];
    }

    float fxmin = (float)xmin;
    float fymin = (float)ymin;
    float fxmax = (float)xmax;
    float fymax = (float)ymax;

    // Clip against left edge
    int count2;
    clipEdge(poly1, count1, poly2, &count2, fxmin, LEFT);

    // Clip against bottom edge
    int count3;
    clipEdge(poly2, count2, poly3, &count3, fymin, BOTTOM);

    // Clip against right edge
    int count4;
    clipEdge(poly3, count3, poly4, &count4, fxmax, RIGHT);

    // Clip against top edge
    clipEdge(poly4, count4, final, outCount, fymax, TOP);

    // Output to arrays, rounding coordinates to int
    for (int i = 0; i < *outCount; i++) {
        new_xy[i * 2] = (int)round(final[i].x);
        new_xy[i * 2 + 1] = (int)round(final[i].y);
        new_colors[i] = final[i].color;
        // printf("Final vertex %d: x=%d, y=%d, color=0x%06X\n", i, new_xy[i * 2], new_xy[i * 2 + 1], new_colors[i]);
    }
}
*/

//#include <stdio.h>  // For potential debugging
#include <stdbool.h>  // For bool type
//#include <math.h>   // For round

typedef struct {
    float x, y;
    unsigned int color;
} Vertex;

enum ClipSide { LEFT, RIGHT, BOTTOM, TOP };

unsigned char getR(unsigned int rgb) { return (rgb >> 16) & 0xFF; }
unsigned char getG(unsigned int rgb) { return (rgb >> 8) & 0xFF; }
unsigned char getB(unsigned int rgb) { return rgb & 0xFF; }
unsigned int packRGB(unsigned char r, unsigned char g, unsigned char b) {
    return ((unsigned int)r << 16) | ((unsigned int)g << 8) | (unsigned int)b;
}

bool inside(Vertex p, float bound, enum ClipSide side) {
    switch (side) {
        case LEFT:   return p.x >= bound;
        case RIGHT:  return p.x <= bound;
        case BOTTOM: return p.y >= bound;
        case TOP:    return p.y <= bound;
    }
    return false;  // Should not reach here
}

Vertex intersect(Vertex p1, Vertex p2, float bound, enum ClipSide side) {
    Vertex res = {0.0f, 0.0f, 0};  // Initialize to avoid uninitialized memory
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float t;

    if (side == LEFT || side == RIGHT) {
        if (fabsf(dx) < 1e-6f) {  // Vertical segment
            res.x = p1.x;
            res.y = p1.y;
        } else {
            t = (bound - p1.x) / dx;
            res.x = bound;
            res.y = p1.y + t * dy;
        }
    } else {  // BOTTOM or TOP
        if (fabsf(dy) < 1e-6f) {  // Horizontal segment
            res.x = p1.x;
            res.y = p1.y;
        } else {
            t = (bound - p1.y) / dy;
            res.x = p1.x + t * dx;
            res.y = bound;
        }
    }

    // Interpolate color
    float r1 = (float)getR(p1.color);
    float g1 = (float)getG(p1.color);
    float b1 = (float)getB(p1.color);
    float r2 = (float)getR(p2.color);
    float g2 = (float)getG(p2.color);
    float b2 = (float)getB(p2.color);

    unsigned char res_r = (unsigned char)round(r1 + t * (r2 - r1));
    unsigned char res_g = (unsigned char)round(g1 + t * (g2 - g1));
    unsigned char res_b = (unsigned char)round(b1 + t * (b2 - b1));

    res.color = packRGB(res_r, res_g, res_b);

    return res;
}

void clipEdge(Vertex input[], int inCount, Vertex output[], int *outCount, float bound, enum ClipSide side) {
    *outCount = 0;
    if (inCount < 2) return;

    // Initialize output array to avoid uninitialized memory
    for (int i = 0; i < 10; i++) {
        output[i].x = 0.0f;
        output[i].y = 0.0f;
        output[i].color = 0;
    }

    Vertex s = input[inCount - 1];  // Start with the last point to close the polygon loop

    for (int i = 0; i < inCount; i++) {
        Vertex e = input[i];
        bool sInside = inside(s, bound, side);
        bool eInside = inside(e, bound, side);

        if (eInside) {
            if (!sInside) {
                // Crossing in: add intersection
                output[*outCount] = intersect(s, e, bound, side);
                (*outCount)++;
            }
            // Add the end point
            output[*outCount] = e;
            (*outCount)++;
        } else if (sInside) {
            // Crossing out: add intersection
            output[*outCount] = intersect(s, e, bound, side);
            (*outCount)++;
        }
        s = e;
    }
}

void clipPolygonWithColors(long *xy, unsigned int *colors, int n_vertices, int xmin, int ymin, int xmax, int ymax, int *new_xy, unsigned int *new_colors, int *outCount) {
    if (n_vertices < 3 || n_vertices > 4) {
        *outCount = 0;  // Only handle triangles (3) or quads (4)
        return;
    }

    Vertex input[4];
    // Initialize input vertices
    for (int i = 0; i < n_vertices; i++) {
        input[i].x = (float)xy[i * 2];
        input[i].y = (float)xy[i * 2 + 1];
        input[i].color = colors[i];
    }

    Vertex poly1[10];  // Buffers for intermediate results (max vertices ~8 for quad)
    Vertex poly2[10];
    Vertex poly3[10];
    Vertex poly4[10];
    Vertex final[10];

    // Initialize buffers to avoid uninitialized memory
    for (int i = 0; i < 10; i++) {
        poly1[i].x = poly1[i].y = poly2[i].x = poly2[i].y = poly3[i].x = poly3[i].y = poly4[i].x = poly4[i].y = final[i].x = final[i].y = 0.0f;
        poly1[i].color = poly2[i].color = poly3[i].color = poly4[i].color = final[i].color = 0;
    }

    int count1 = n_vertices;
    for (int i = 0; i < n_vertices; i++) {
        poly1[i] = input[i];
    }

    float fxmin = (float)xmin;
    float fymin = (float)ymin;
    float fxmax = (float)xmax;
    float fymax = (float)ymax;

    // Clip against left edge
    int count2;
    clipEdge(poly1, count1, poly2, &count2, fxmin, LEFT);

    // Clip against bottom edge
    int count3;
    clipEdge(poly2, count2, poly3, &count3, fymin, BOTTOM);

    // Clip against right edge
    int count4;
    clipEdge(poly3, count3, poly4, &count4, fxmax, RIGHT);

    // Clip against top edge
    clipEdge(poly4, count4, final, outCount, fymax, TOP);

    // Output to arrays, rounding coordinates to int
    for (int i = 0; i < *outCount; i++) {
        new_xy[i * 2] = (int)round(final[i].x);
        new_xy[i * 2 + 1] = (int)round(final[i].y);
        new_colors[i] = final[i].color;
    }
}

unsigned int getRGB(double m, double max_m) {
    unsigned char r, g, b;
    double intensity;

    if (m > 0) {
        intensity = fmin(m / max_m, 1.0);
        r = 255;
        g = (unsigned char)(255 * (1 - intensity));
        b = (unsigned char)(255 * (1 - intensity));
    } else if (m < 0) {
        intensity = fmin(-m / max_m, 1.0);
        r = (unsigned char)(255 * (1 - intensity));
        g = (unsigned char)(255 * (1 - intensity));
        b = 255;
    } else {
        r = 255;
        g = 255;
        b = 255;
    }

    return (r << 16) | (g << 8) | b;
}

unsigned int getRGBgreen(double m, double max_m) {
    unsigned char r, g, b;
    double intensity;

    if (m > 0) {
        intensity = fmin(m / max_m, 1.0);
        g = 255;
        r = (unsigned char)(255 * (1 - intensity));
        b = (unsigned char)(255 * (1 - intensity));
    } else if (m < 0) {
        intensity = fmin(-m / max_m, 1.0);
        r = (unsigned char)(255 * (1 - intensity));
        b = (unsigned char)(255 * (1 - intensity));
        g = 255;
    } else {
        r = 255;
        g = 255;
        b = 255;
    }

    return (r << 16) | (g << 8) | b;
}

// Procedure to set colors for triangle vertices
void setTriangleVertexColors(double m1, double m2, double m3, double max_m, unsigned int colors[3]) {
    colors[0] = getRGB(m1, max_m);
    colors[1] = getRGB(m2, max_m);
    colors[2] = getRGB(m3, max_m);
}

/**
* Lightens a 24-bit RGB integer (0xRRGGBB).
* factor: 0.0 (no change) to 1.0 (pure white)
*/
unsigned int lighten_color(unsigned int color, float factor) {
    // 1. Extract RGB components
    unsigned int r = (color >> 16) & 0xFF;
    unsigned int g = (color >> 8) & 0xFF;
    unsigned int b = color & 0xFF;

    // 2. Apply linear tint: NewValue = OldValue + (255 - OldValue) * factor
    // We cast to int to ensure correct math before packing back
    r = (unsigned int)(r + (255 - r) * factor);
    g = (unsigned int)(g + (255 - g) * factor);
    b = (unsigned int)(b + (255 - b) * factor);

    // 3. Recombine into 24-bit integer
    return (r << 16) | (g << 8) | b;
}

void Draw_Solid (int numpoints, T_PixelTVal * polypoints, unsigned int pcx_solid, BOOL hatch_solid, double origin_x, double origin_y, char *s_pattern, int translucency, GRADIENT4 *gradient)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
{
  int num_wy ;
  int *poly_wy;
  int ret;
  V3D v[8];
#pragma pack(1)
  V3D_f v_f[8];
  GRADIENT8 gradient8;
  unsigned int gradient8_c1,gradient8_c2,gradient8_c3,gradient8_c4,gradient8_c5,gradient8_c6,gradient8_c7,gradient8_c8;
  float factor = 1.f-(float)GTRANSLUCENCY/255.f;

  int wl, wg, wp, wd;

  if (numpoints>4)
  {
      int a=0;
  }

  if (NULL == (poly_wy = (int*)malloc((MaxNumPolygonPoints + 2) * sizeof(poly_wy[0]))))
  {
	  return;
  }

  if (gradient!=NULL) memmove(&gradient8, gradient, sizeof(GRADIENT4));

  if (FALSE == Clip_Solid0 (numpoints, polypoints, &num_wy, poly_wy))
  {
    if (gradient==NULL)  Clip_Solid (numpoints, polypoints, &num_wy, poly_wy) ;
    else
    {
        Get_View_Coord(&wl, &wg, &wp, &wd);
        //clipTriangleWithMag(polypoints, (double) wl, (double) wg, (double) wp, (double) wd, poly_wy, &num_wy);
        //clipTriangleWithMag(polypoints, float mag[3], wl, wg, wp, wd, poly_wy, float new_mag[], &num_wy);
        //clipTriangleWithColors((long *) polypoints, (unsigned int *) gradient, wl, wg, wp, wd, poly_wy,(unsigned int *) &gradient6, &num_wy);
        clipPolygonWithColors(polypoints, (unsigned int *) gradient, numpoints, wl, wg, wp, wd, poly_wy,(unsigned int *) &gradient8, &num_wy);
    }
 }

  if ((pcx_solid==0) || (options1.fill_pcx_solid==1)) 
  {
      if (s_pattern != NULL)
      {
          fillpolypattern(num_wy, poly_wy, s_pattern, (long)origin_x, (long)origin_y);
      }
      else {
          if (gradient==NULL) my_fillpoly(num_wy, poly_wy, translucency, kolory.paper);
          else
          {

              //set_trans_blender(0, 0, 0, (int)GTRANSLUCENCY);
              //set_mode_trans();
              
              /*
              v[0].x=ftofix((float)poly_wy[0]);
              v[0].y=ftofix((float)poly_wy[1]);
              v[0].z=ftofix(0.0f);  //0
              v[0].u=0;
              v[0].v=0;
              v[0].c=gradient->c1;

              v[1].x=ftofix((float)poly_wy[2]);
              v[1].y=ftofix((float)poly_wy[3]);
              v[1].z=ftofix(0.0f);  //0
              v[1].u=0;
              v[1].v=0;
              v[1].c=gradient->c2;

              v[2].x=ftofix((float)poly_wy[4]);
              v[2].y=ftofix((float)poly_wy[5]);
              v[2].z=ftofix(0.0f);  //0
              v[2].u=0;
              v[2].v=0;
              v[2].c=gradient->c3;

              if (num_wy>3)
              {
                  v[3].x = ftofix((float) poly_wy[6]);
                  v[3].y = ftofix((float) poly_wy[7]);
                  v[3].z = ftofix(0.0f);  //0
                  v[3].u = 0;
                  v[3].v = 0;
                  v[3].c = gradient->c3;  //c4

                  if (num_wy>4)
                  {
                      v[4].x = ftofix((float) poly_wy[8]);
                      v[4].y = ftofix((float) poly_wy[9]);
                      v[4].z = ftofix(0.0f);  //0
                      v[4].u = 0;
                      v[4].v = 0;
                      v[4].c = gradient->c3;  //c4

                      if (num_wy>5)
                      {
                          v[5].x = ftofix((float) poly_wy[10]);
                          v[5].y = ftofix((float) poly_wy[11]);
                          v[5].z = ftofix(0.0f);  //0
                          v[5].u = 0;
                          v[5].v = 0;
                          v[5].c = gradient->c3;  //c4
                      }

                      polygon3d(screenplay, POLYTYPE_GRGB, NULL, min(num_wy, 6), (V3D **) v);   //(V3D **) &v);
                  }
                  else quad3d(screenplay, POLYTYPE_GRGB, NULL, &v[0], &v[1], &v[2], &v[3]);
              }
              else {

                  triangle3d(screenplay, POLYTYPE_GRGB, NULL, &v[0], &v[1], &v[2]);
              }
              */
              //////////////////////////
              gradient8_c1=lighten_color(gradient8.c1, factor);
              gradient8_c2=lighten_color(gradient8.c2, factor);
              gradient8_c3=lighten_color(gradient8.c3, factor);
              //////////////////////////
              v_f[0].x=(float)poly_wy[0];
              v_f[0].y=(float)poly_wy[1];
              v_f[0].z=0.0f;
              v_f[0].u=0;
              v_f[0].v=0;
              //v_f[0].c=gradient8.c1;
              v_f[0].c=gradient8_c1;

              v_f[1].x=(float)poly_wy[2];
              v_f[1].y=(float)poly_wy[3];
              v_f[1].z=0.0f;  //0
              v_f[1].u=0;
              v_f[1].v=0;
              //v_f[1].c=gradient8.c2;
              v_f[1].c=gradient8_c2;

              v_f[2].x=(float)poly_wy[4];
              v_f[2].y=(float)poly_wy[5];
              v_f[2].z=0.0f;
              v_f[2].u=0;
              v_f[2].v=0;
              //v_f[2].c=gradient8.c3;
              v_f[2].c=gradient8_c3;

              if (num_wy>3)
              {
                  gradient8_c4=lighten_color(gradient8.c4, factor);

                  v_f[3].x = (float) poly_wy[6];
                  v_f[3].y = (float) poly_wy[7];
                  v_f[3].z = 0.0f;  //0
                  v_f[3].u = 0;
                  v_f[3].v = 0;
                  //v_f[3].c = gradient8.c4;
                  v_f[3].c = gradient8_c4;

                  if (num_wy>4)
                  {
                      gradient8_c5=lighten_color(gradient8.c5, factor);

                      v_f[4].x = (float) poly_wy[8];
                      v_f[4].y = (float) poly_wy[9];
                      v_f[4].z = 0.0f;  //0
                      v_f[4].u = 0;
                      v_f[4].v = 0;
                      //v_f[4].c = gradient8.c5;
                      v_f[4].c = gradient8_c5;

                      if (num_wy>5)
                      {
                          gradient8_c6=lighten_color(gradient8.c6, factor);

                          v_f[5].x = (float) poly_wy[10];
                          v_f[5].y = (float) poly_wy[11];
                          v_f[5].z = 0.0f;  //0
                          v_f[5].u = 0;
                          v_f[5].v = 0;
                          //v_f[5].c = gradient8.c6;
                          v_f[5].c = gradient8_c6;
                      }
                      if (num_wy>6)
                      {
                          gradient8_c7=lighten_color(gradient8.c7, factor);

                          v_f[6].x = (float) poly_wy[12];
                          v_f[6].y = (float) poly_wy[13];
                          v_f[6].z = 0.0f;  //0
                          v_f[6].u = 0;
                          v_f[6].v = 0;
                          //v_f[6].c = gradient8.c7;
                          v_f[6].c = gradient8_c7;
                      }
                      if (num_wy>7)
                      {
                          gradient8_c8=lighten_color(gradient8.c8, factor);

                          v_f[7].x = (float) poly_wy[14];
                          v_f[7].y = (float) poly_wy[15];
                          v_f[7].z = 0.0f;  //0
                          v_f[7].u = 0;
                          v_f[7].v = 0;
                          //v_f[7].c = gradient8.c8;
                          v_f[7].c = gradient8_c8;
                      }

                      ////polygon3d_f(screenplay, POLYTYPE_GRGB, NULL, num_wy, (V3D_f **)v_f);   //this is crashing
                      ////temporary removed due to crash, instead is using quad3d_f
                      quad3d_f(screenplay, POLYTYPE_GRGB, NULL, &v_f[0], &v_f[1], &v_f[2], &v_f[3]);
                      if (num_wy==8)
                      {
                          quad3d_f(screenplay, POLYTYPE_GRGB, NULL, &v_f[3], &v_f[4], &v_f[5], &v_f[0]);
                          quad3d_f(screenplay, POLYTYPE_GRGB, NULL, &v_f[5], &v_f[6], &v_f[7], &v_f[0]);
                      }
                      else if (num_wy==7)
                      {
                          quad3d_f(screenplay, POLYTYPE_GRGB, NULL, &v_f[3], &v_f[4], &v_f[5], &v_f[0]);
                          triangle3d_f(screenplay, POLYTYPE_GRGB, NULL, &v_f[5], &v_f[6], &v_f[0]);
                      }
                      else if (num_wy==6)
                      {
                          quad3d_f(screenplay, POLYTYPE_GRGB, NULL, &v_f[3], &v_f[4], &v_f[5], &v_f[0]);
                          //triangle3d_f(screenplay, POLYTYPE_GRGB, NULL, &v_f[5], &v_f[6], &v_f[0]);
                      }
                      else if (num_wy==5)
                      {
                          triangle3d_f(screenplay, POLYTYPE_GRGB, NULL, &v_f[3], &v_f[4], &v_f[0]);
                      }
                  }
                  else quad3d_f(screenplay, POLYTYPE_GRGB, NULL, &v_f[0], &v_f[1], &v_f[2], &v_f[3]);
              }
              else {
                  triangle3d_f(screenplay, POLYTYPE_GRGB, NULL, &v_f[0], &v_f[1], &v_f[2]);
              }

              //solid_mode();
          }
      }
  }

  free(poly_wy);

}



BOOL Clip_Solid0 (int num_we, T_PixelTVal  *poly_we,
			int *num_wy, int *poly_wy)
/*--------------------------------------------------*/
{
  int i, num_we2 ;
  T_PixelTVal dist ;
  BOOL b_ret ;

  if ((num_we>4) || (num_we==0))
      return FALSE;

  *num_wy = num_we;
  b_ret = FALSE ;
  for (i = 2, dist = 0, num_we2 = num_we * 2 ; i < num_we2 ; i+= 2)
  {
    dist += labs (poly_we [0] - poly_we [i]) +
	    labs (poly_we [1] - poly_we [i + 1]) ;
  }
  if ( dist/ num_we < 100)
  {
    for (i = 0 ; i < num_we2 ; i++)
      poly_wy [i] = (int)poly_we [i] ;
    *num_wy = num_we ;
    b_ret = TRUE ;
  }
  return b_ret ;
}


void Clip_Solid (int num_we, T_PixelTVal  *poly_we, int *num_wy, int *poly_wy)
/*--------------------------------------------------------------------------*/
{
  int i, licz, granica ;
  T_PixelTVal   s1, s2, p1, p2, x, y ;
  T_PixelTVal  *temp_wy, *chwil ;

  if (NULL == (chwil = (T_PixelTVal*)malloc(MaxNumPolygonPoints * sizeof(chwil[0]))))
  {
	  return;
  }

  if (NULL == (temp_wy = (T_PixelTVal*)malloc(MaxNumPolygonPoints * sizeof(temp_wy[0]))))
  {
    free(chwil);
    return;
  }

  num_we *= 2 ;
  Get_View_Coord (&wl, &wg, &wp, &wd) ;
  for (i = 0 ; i < num_we ; i++)  /*dla kazdej granicy*/
  {
    chwil [i] = poly_we [i] ;
  }
 
  for ( granica = 0; granica < 4; granica++)
  {
    licz = 0 ;
    s1 = chwil [num_we - 2] ;	/*punk S jest ostatnim punktem*/
    s2 = chwil [num_we - 1] ;
    for (i = 0 ; i < num_we ; i += 2) 	/*dla kazdego wierzcholka*/
    {
      p1 = chwil [i] ;			/*punkt P jest nastepny*/
      p2 = chwil [i + 1] ;
      if (TRUE == widocznosc (granica, p1, p2))  /*punkt p jest widoczny*/
      {
	    if (TRUE == widocznosc (granica, s1, s2)) /*punkt s jest widoczny*/
	    {
	      temp_wy [licz++] = p1 ;
	      temp_wy [licz++] = p2 ;
	    }
	    else	/*punkt s jest niewidoczny*/
	    {
	      clip (granica, s1, s2, p1, p2, &x, &y) ;
	      temp_wy [licz++] = x ;
	      temp_wy [licz++] = y ;
	      temp_wy [licz++] = p1 ;
	      temp_wy [licz++] = p2 ;
	    }
      }
      else	/*punkt p jest niewidoczny*/
      {
	    if (TRUE == widocznosc (granica, s1, s2))
	    {
	      clip (granica, s1, s2, p1, p2, &x, &y) ;
	      temp_wy [licz++] = x ;
	      temp_wy [licz++] = y ;
	    }
      }
      s1 = p1 ;
      s2 = p2 ;
    } 			/*nastepny punkt*/
    num_we = licz ;

    for (i = 0; i < num_we ; i++)
    {
      chwil [i] = temp_wy [i] ;
    }
  }
  for (i = 0; i < num_we; i++)
  {
    poly_wy [i] = (int)temp_wy [i] ;
  }
  *num_wy = num_we / 2 ;

  free(chwil);
  free(temp_wy) ;
}


static void clip (int granica,
			T_PixelTVal  x1, T_PixelTVal  y1,
			T_PixelTVal  x2, T_PixelTVal  y2,
			T_PixelTVal  *x, T_PixelTVal  *y)
/*------------------------------------------------------*/
{
  double r ;

  switch (granica)
  {
    case 0 :
      r = ((double)(wg - y1)) / (y2 - y1) ;
      *x = x1 + (T_PixelTVal )floor(.5 + (x2 - x1) * r) ;
      *y = wg ;
      break ;
    case 1 :
      r = ((double)(wp - x1)) / (x2 - x1) ;
      *y = y1 + (T_PixelTVal )floor(.5 + (y2 - y1) * r) ;
      *x = wp ;
      break ;
    case 2 :
      r = ((double)(wd - y1)) / (y2 - y1) ;
      *x = x1 + (T_PixelTVal )floor(.5 + (x2 - x1) * r) ;
      *y = wd ;
      break ;
    case 3 :
      r = ((double)(wl - x1)) / (x2 - x1) ;
      *y = y1 + (T_PixelTVal )floor(.5 + (y2 - y1) * r) ;
      *x = wl ;
      break ;
  }
}


static BOOL widocznosc (int granica, T_PixelTVal  x1, T_PixelTVal  y1)
/*---------------------------------------------------------------*/
{
  BOOL b_retval ;
  T_PixelTVal  v1 ;

  switch (granica)
  {
    case 0 :
       v1 =  -(wp - wl) * (wg - y1) ;
       break ;
    case 1 :
       v1 = (wd -wg) * (wp - x1) ;
       break ;
    case 2 :
       v1 = - (wl - wp) * (wd - y1) ;
       break ;
    case 3 :
       v1 = (wg - wd) * (wl -x1) ;
       break ;
  }
  b_retval = TRUE ;
  if (v1 <=0)
  {
     b_retval = FALSE ;
  }
  return b_retval ;
}



static void near sinX(int *X,int N)
{ double dk,kat=0;
  int i;
  dk=Pi/(2*(N-1));
  for(i=0,kat=0;i<N;i++,X++,kat+=dk) *X=sin(kat)*1000+0.5;
}


#define NPC 14
void Draw_Kolo (T_PixelTVal xr, T_PixelTVal yr, T_PixelTVal R)
{
  static int y[NPC] = {0,121,239,355,465,568,663,749,823,885,935,971,993,1000};
  static int z[NPC], *x, wsk=0;
  static T_PixelTVal R0;
  T_PixelTVal  xp, yp ;
  double  Xfact, Yfact;
  int i, N;
  long N0;
  int num_we ;
  T_PixelTVal *poly_we ;
  int num_wy ;
  int *poly_wy ;

  if (R <= 1)
  {
    putpixel_ ((int)xr, (int)yr, getcolor ()) ;
    return ;
  }
  if (NULL == (poly_we= (T_PixelTVal*)malloc(MaxNumPolygonPoints * sizeof(poly_we[0]))))
  {
    return ;
  }
  if (NULL == (poly_wy= (int*)malloc((MaxNumPolygonPoints + 2) * sizeof(poly_wy[0]))))
  {
    free(poly_we);
    return ;
  }

  Xfact = fabs(R * 0.001);
  Yfact = Xfact / sk_x;
  if (Xfact > 0.0)
   { if(Pi*R<NPC*7.5)
     { N0=3+R/20;
	x = z;
	if(wsk==0 || R0!=R)
	 { sinX(z,(int)N0);
	   wsk=1;
	   R0=R;
	 }
     }
     else { N0=NPC; x=y;}
     N = (int)N0 ;
     num_we = 0 ;
     for (i = 0; i < N ; i++)
     {
	xp = xr + x [N - i - 1] * Xfact ;
	yp = yr - x [i] * Yfact ;
	poly_we [num_we++] = xp ;
	poly_we [num_we++] = yp ;
     }
     for (i = 1; i < N ; i++)
     {
	xp = xr - x [i] * Xfact ;
	yp = yr - x [N - i - 1] * Yfact ;
	poly_we [num_we++] = xp ;
	poly_we [num_we++] = yp ;
     }
     for (i = 1; i < N ; i++)
     {
	xp = xr - x [N - i - 1] * Xfact ;
	yp = yr + x [i] * Yfact ;
	poly_we [num_we++] = xp ;
	poly_we [num_we++] = yp ;
     }
     for (i = 1; i < N - 1; i++)
     {
	xp = xr + x [i] * Xfact ;
	yp = yr + x [N - i - 1] * Yfact ;
	poly_we [num_we++] = xp ;
	poly_we [num_we++] = yp ;
     }
  }
  if (R < 50)
  {        	/*obcinanie pozostawiono funkcji fillpoly*/
    for (i = 0 ; i < num_we; i++)
      poly_wy [i] = (int)poly_we [i] ;
    num_wy = num_we / 2 ;
  }
  else
  {
    Clip_Solid (num_we / 2, poly_we, &num_wy, poly_wy) ;
  }
  fillpoly (num_wy, poly_wy) ;
  free(poly_we);
  free(poly_wy);
}

