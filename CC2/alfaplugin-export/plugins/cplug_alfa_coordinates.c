#include "../alfaplugin_sys.h"

#include <stdbool.h>
#include <string.h>
#include "alfaplugin.h"
#include "../alfaplugin_enum.h"
#include "../bib_e_part.h"

static int language=-1;
static int iconno_plugin=0;
void *(*plugin_ptr)(int plugin_function, char *param1, char *param2, char *param3);
double coords_angle=0.0;
double local_angle=0.0;

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
            return u8"LCS angle (plugin)";
            break;
        case POLISH:
            return u8"Kąt LUW (wtyczka)";
            break;
        case UKRAINIAN:
            return u8"Кут МСК (плагін)";
            break;
        case SPANISH:
            return u8"Ángulo del SCL (enchufar)";
            break;
        default:
            return u8"LCS angle (plugin)";
            break;
    }
}

int plugin_icon(void)
{
    iconno_plugin=VOID_TO_INT(plugin_ptr(LOAD_CLIENT_BITMAP, "coordinates.png", NULL, NULL));
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

static void restore_coordinates(void)
{
    int ret;
    int key=ESC;
    ret=VOID_TO_INT(plugin_ptr(SET_LCSANGLE, &coords_angle, NULL, NULL));
    ret=VOID_TO_INT(plugin_ptr(SIMULATE_KEYPRESSED, &key, NULL, NULL));

    return;
}


    void alfa_func(int a)
{
    int ret;
    char *obj_ptr;
    unsigned int typ;
    char kom[MaxTextLen];
    PLINIA PL ;
    double kat;

    switch (language) {
        case ENGLISH:
            strcpy(kom, u8"Select a line (or press BACKSPACE to restore original angle)");
            break;
        case POLISH:
            strcpy(kom, u8"Wskaż linię (lub naciśnij BACKSPACE, aby przywrócić oryginalny kąt)");
            break;
        case UKRAINIAN:
            strcpy(kom, u8"Виберіть лінію (або натисніть BACKSPACE, щоб відновити початковий кут)");
            break;
        case SPANISH:
            strcpy(kom, u8"Seleccione una línea (o presione BACKSPACE para restaurar el ángulo original)");
            break;
        default:
            strcpy(kom, u8"Select a line (or press BACKSPACE to restore original angle)");
            break;
    }

    typ=Blinia;

    ret=VOID_TO_INT(plugin_ptr(GET_LCSANGLE, &kat, NULL, NULL));

    if (VOID_TO_INT(plugin_ptr(CHECK_IF_EQUAL, &kat, &local_angle, NULL))==FALSE)
    {
       coords_angle=kat; //in the meantime was changed
    }

    ret=VOID_TO_INT(plugin_ptr(SETBACKSPACEKEY, (char*)restore_coordinates, NULL, NULL));

    obj_ptr=plugin_ptr(SELECT_SINGLE_OBJECT, (unsigned int*)&typ, kom, NULL);
    if (obj_ptr ==NULL) return;
    ret=VOID_TO_INT(plugin_ptr(LINEPARAMETERS, obj_ptr, (PLINIA*)&PL, NULL));
    kat=PL.kat;
    ret=VOID_TO_INT(plugin_ptr(SET_LCSANGLE, &kat, NULL, NULL));
    local_angle=kat;

    ret=VOID_TO_INT(plugin_ptr(RESTOREBACKSPACEKEY, NULL, NULL, NULL));

    return;

}
