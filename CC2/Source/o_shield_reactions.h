//
// Created by marek on 11/30/25.
//

#ifndef ALFACAD_O_SHIELD_REACTIONS_H
#define ALFACAD_O_SHIELD_REACTIONS_H

typedef enum {
    SUPPORT_FIXED = 7,         // blocked in both X and Y
    SUPPORT_ROLLER_X = 4,      // free in X, blocked in Y (vertical roller)
    SUPPORT_ROLLER_Y = 3,      // free in Y, blocked in X (horizontal roller)
    SUPPORT_SIMPLE = 6         // same as fixed for membrane (no moment anyway)
} SupportType;

#endif //ALFACAD_O_SHIELD_REACTIONS_H
