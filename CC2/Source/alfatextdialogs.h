//
// Created by marek on 4/26/25.
//

#ifndef ALFACAD_ALFATEXTDIALOGS_H
#define ALFACAD_ALFATEXTDIALOGS_H

#endif //ALFACAD_ALFATEXTDIALOGS_H

#ifdef __cplusplus
extern "C" {
#endif

extern char * alfa_editBox(
        char const * aTitle ,
        char const * aMessage ,
        int const * aDefaultInput,
        char const *fontface,
        char const *fontfile,
        char const *fontsize,
        char const *fontwidth,
        char const *etype,
        char const *params,
        int *single
);

#ifdef __cplusplus
}
#endif