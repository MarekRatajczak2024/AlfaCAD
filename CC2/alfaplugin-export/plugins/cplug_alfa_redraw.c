#include "../alfaplugin_sys.h"

#include <stdbool.h>
#include "alfaplugin.h"
#include "../alfaplugin_enum.h"
#include "../bib_e_part.h"

static int language=-1;
static int iconno_plugin=0;
void *(*plugin_ptr)(int plugin_function, char *param1, char *param2, char *param3);

union void_cast {
    void* ptr;
    int value;
};

int VOID_TO_INT(void* ptr) {
    union void_cast u;
    u.ptr = ptr;
    return u.value;
}

plugin_type_t plugin_type(void)
{
    return PLUGIN_TYPE_ALFA;
}

plugin_part_t plugin_part(void)
{
    return PLUGIN_PART_AUX;
}

bool init(char *plug_ptr)
{
    plugin_ptr=(void*)plug_ptr;
    language = VOID_TO_INT(plugin_ptr(GETLANGUAGE, NULL, NULL, NULL));
    return true;
}

void deinit(void)
{
    //char goodbye[32]= "Bye bye\n";
    //int ret = VOID_TO_INT(plugin_ptr(NOTICE_STR_SHORT, (char*)goodbye, NULL, NULL));
    int ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, (char*)&iconno_plugin, NULL, NULL));
    return;
}

const char *plugin_name(void)
{
    switch (language)
    {
        case ENGLISH:
            return u8"Redraw (plugin)";
            break;
        case POLISH:
            return u8"Przerysuj (wtyczka)";
            break;
        case UKRAINIAN:
            return u8"Перемалювати (плагін)";
            break;
        case SPANISH:
            return u8"Redibujar (enchufar)";
            break;
        default:
            return u8"Redraw (plugin)";
            break;
    }
}

int plugin_icon(void)
{
    iconno_plugin=VOID_TO_INT(plugin_ptr(LOAD_CLIENT_BITMAP, "refresh.png", NULL, NULL));
    return iconno_plugin;
}

int plugin_version(void)
{
    return 1;
}

int plugin_api_version(void)
{
    return 2;
}

void alfa_func(int a)
{
    int ret;

    ret = VOID_TO_INT(plugin_ptr(REDRAW, NULL, NULL, NULL));
}
