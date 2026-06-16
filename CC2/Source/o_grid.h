
#ifndef LIBFUN_PL_H_O_GRID_H
#define LIBFUN_PL_H_O_GRID_H

extern void Grid_analysis(void);

//all structures inherited from o_static.h, to minimize number of structures

typedef struct
{
    double x;  //real position of supporting node on drawing
    double y;
    double f;  //force intensity per unit of model mm or in
    double l;  //length of segment in model units mm or in
    double a;  //length in mm of drawing
} ST_WINKLER_REACTION;


typedef struct
{
    double x1;
    double y1;
    double x2;
    double y2;
} ST_WINKLER_BEAM;

typedef struct
{
    LINIA Le;
    LINIA Ldsp;
    LINIA Ldsp2;
    LINIA Ldsp_;
    double x_iso;
    double value;
    FE_TORQUE t_value;
    double precision;
    char suffix[4];
    char prefix[4];
    BOOL bold;
} ST_WINKLER_FORCE, ST_MEMBER_FORCE;
#endif //LIBFUN_PL_H_O_GRID_H
