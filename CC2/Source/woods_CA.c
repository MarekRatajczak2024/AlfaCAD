#include <stdio.h>
#include <forwin.h>
#include "bib_e.h"

POLE pmWoodSpeciesCA[] = {
{u8"֎Aspen Quaking",L' ',1092,NULL},
{u8"֎Aspen Big-toothed",L' ',1092,NULL},
{u8"֎Cottonwood Balsam poplar",L' ',1092,NULL},
{u8"֎Cottonwood Black",L' ',1092,NULL},
{u8"֎Cottonwood Eastern",L' ',1092,NULL},
{u8"֎Cedar Northern white",L' ',1017,NULL},
{u8"֎Cedar Western redcedar",L' ',1017,NULL},
{u8"֎Cedar Yellow",L' ',1017,NULL},
{u8"֎Douglas-fir",L' ',1017,NULL},
{u8"֎Fir Subalpine",L' ',1017,NULL},
{u8"֎Fir Pacific silver",L' ',1017,NULL},
{u8"֎Fir Balsam",L' ',1017,NULL},
{u8"֎Hemlock Eastern",L' ',1017,NULL},
{u8"֎Hemlock Western",L' ',1017,NULL},
{u8"֎Larch, western",L' ',1017,NULL},
{u8"֎Pine Eastern white",L' ',1017,NULL},
{u8"֎Pine Jack",L' ',1017,NULL},
{u8"֎Pine Lodgepole",L' ',1017,NULL},
{u8"֎Pine Ponderosa",L' ',1017,NULL},
{u8"֎Pine Red",L' ',1017,NULL},
{u8"֎Pine Western white",L' ',1017,NULL},
{u8"֎Spruce Black",L' ',1017,NULL},
{u8"֎Spruce Engelmann",L' ',1017,NULL},
{u8"֎Spruce Red",L' ',1017,NULL},
{u8"֎Spruce Sitka",L' ',1017,NULL},
{u8"֎Spruce White",L' ',1017,NULL},
{u8"֎Tamarack",L' ',1017,NULL},
};

TMENU mWoodSpeciesCA = { 27,0,0,25,16,8,0,CMNU,CMBR,CMTX,0,1,0,0,0,(POLE(*)[]) &pmWoodSpeciesCA,NULL,NULL };

