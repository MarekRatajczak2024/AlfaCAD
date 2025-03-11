#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include <stdbool.h>
#include "alfaplugin_funcs.h"

#ifndef NULL
#define NULL		((void *)(0))
#endif

#define CUSTOM_COMND_OFF 1000

typedef enum {
    PLUGIN_TYPE_UNKNOWN = 0,
    PLUGIN_TYPE_TXT,
    PLUGIN_TYPE_INFO,
    PLUGIN_TYPE_NUM,
    PLUGIN_TYPE_ALFA
} plugin_type_t;

typedef enum {
    PLUGIN_PART_UNKNOWN = 0,
    PLUGIN_PART_DRAW,
    PLUGIN_PART_EDIT,
    PLUGIN_PART_BLOCK,
    PLUGIN_PART_AUX
} plugin_part_t;

bool plugin_init(void);
void plugin_deinit(void);

plugin_type_t plugin_type(void);
const char *plugin_name(void);
int plugin_version(void);
int plugin_api_version(void);

#endif /* __PLUGIN_H__ */
