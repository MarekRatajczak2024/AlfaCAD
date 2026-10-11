#ifndef __PLUGIN_MANAGER_H__
#define __PLUGIN_MANAGER_H__

#include "ds/list_str.h"
#include "alfaplugin.h"

struct pm_manager;
typedef struct pm_manager pm_manager_t;

pm_manager_t *pm_load(char *plug_ptr);
void pm_unload(pm_manager_t *pm);

int pm_plugin_version(pm_manager_t *pm, const char *name);
plugin_type_t pm_plugin_type(pm_manager_t *pm, const char *name);

list_str_t *pm_plugins_txt_transform(const pm_manager_t *pm);
list_str_t *pm_plugins_txt_info(const pm_manager_t *pm);
list_str_t *pm_plugins_num(const pm_manager_t *pm);
list_str_t *pm_plugins_alfa(const pm_manager_t *pm);

char *pm_plugin_txt_tranform(pm_manager_t *pm, const char *name, const char *txt);
char *pm_plugin_txt_info(pm_manager_t *pm, const char *name, const char *txt);
void pm_plugin_num_math(pm_manager_t *pm, const char *name, int a);
char *pm_plugin_alfa(pm_manager_t *pm, const char *name, void *txt);

#endif /* __PLUGIN_MANAGER_H__ */
