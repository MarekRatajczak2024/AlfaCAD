#include "../alfaplugin_sys.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <../ds/list.h>

#include "alfaplugin_enum.h"
#include "alfaplugin_manager.h"

static pm_manager_t *pm = NULL;

void *plugin_functions(int plug_function, void *param1, void *param2, void *param3)
{
    switch (plug_function) {
        case GETLANGUAGE:
            return ENGLISH;
            break;
        case NOTICE_STR_SHORT:
            printf((char*)param1);
        default:
            return 0;
            break;
    }
}

int main(int argc, char **argv)
{
    const char *txt = "This is a test of AlfaCAD plugins";
    const char *name;
    list_str_t *plugs;
    char       *out;
    size_t      len;
    size_t      i;
    char *plug_ptr;

    plug_ptr=(char*)&plugin_functions;

    pm = pm_load(plug_ptr);

    plugs = pm_plugins_alfa(pm);
    len = list_str_len(plugs);
    printf(u8"Alfa Plugins (%zu):\n", len);

    for (i=0; i<len; i++) {
        name = list_str_get(plugs, i);
        printf(u8"PLUGIN: %s\n", name);
    }
    list_str_destroy(plugs);

    pm_unload(pm);
    return 0;
}
