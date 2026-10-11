#include "../alfaplugin_sys.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include<sys/stat.h>
#include <../ds/list.h>

#include "alfaplugin_enum.h"
#include "alfaplugin_manager.h"

static pm_manager_t *pm = NULL;

int getlanguage_file(void)
{
    /*
#ifdef ENGLISH
    #ifdef UKRAINIAN
    return 2;
#else
#ifdef SPANISH
return 3;
#else
    return 0;
#endif
#endif
#else
    return 1;
#endif
     */

    int ret;
    struct stat buffer;

    if (stat("ALFACADES.INI", &buffer) == 0)
        return 3;
    if (stat("ALFACADUA.INI", &buffer) == 0)
        return 2;
    if (stat("ALFACADPL.INI", &buffer) == 0)
        return 1;
    return 0;  //ALFACAD.INI
}

void *plugin_functions(int plug_function, void *param1, void *param2, void *param3)
{
    switch (plug_function) {
        case GETLANGUAGE:
            //return ENGLISH;
            return (void *)(intptr_t)getlanguage_file();
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
    Plugs_Parts *plugs_parts;
    int icon;

    char part_name[][8]={"UNKNOWN", "DRAW", "EDIT", "BLOCK", "AUX"};

    plug_ptr=(char*)plugin_functions;

    pm = pm_load(plug_ptr);

    plugs_parts = pm_plugins_alfa(pm);
    len = list_str_len(plugs_parts->plugs);

    printf(u8"Alfa Plugins (%zu):\n", len);


    for (i=0; i<len; i++)
    {
        name = (char*)list_str_get(plugs_parts->plugs, i);
        icon=plugs_parts->icons[i];
        printf("PLUGIN: %s   %s\n", name, part_name[plugs_parts->parts[i]]);
    }

    list_str_destroy(plugs_parts->plugs);

    pm_unload(pm);
    return 0;
}
