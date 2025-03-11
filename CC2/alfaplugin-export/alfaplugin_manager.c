#include "alfaplugin_sys.h"
#ifdef LINUX
#include <dlfcn.h>
#include <dirent.h>
#else
#include "dirent.h"
#endif
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ds/htable.h"
#include "ds/list_str.h"

#include "alfaplugin_manager.h"


typedef bool (*cpm_init)(char *plug_ptr);
typedef void (*cpm_deinit)(void);
typedef const char *(*cpm_name)(void);
typedef int (*cpm_version)(void);
typedef int (*cpm_api_version)(void);
typedef char *(*cpm_txt_transform)(const char *in);
typedef char *(*cpm_txt_info)(const char *in);
typedef void (*cpm_num_math)(int a);
typedef char *(*cpm_alfa_func)(void *in);
typedef plugin_type_t (*cpm_type)(void);


typedef struct {
    void          *handle;
    cpm_init       init;
    cpm_deinit     deinit;
    cpm_name       name;
    cpm_version    version;
    plugin_type_t  type;
} pgen_t;

typedef struct {
   pgen_t           *gen;
   cpm_txt_transform transform;
} ptxt_transform_t;

typedef struct {
   pgen_t      *gen;
   cpm_txt_info info;
} ptxt_info_t;

typedef struct {
   pgen_t      *gen;
   cpm_num_math num;
} pnum_math_t;

typedef struct {
    pgen_t      *gen;
    cpm_alfa_func param;
} palfa_func_t;


struct pm_manager {
    htable_t *ptxt_transform;
    htable_t *ptxt_info;
    htable_t *pnum_math;
    htable_t *palfa_func;
};


static int api_version = 2;


static void ptxt_transform_destroy(ptxt_transform_t *p)
{
    //free(p->gen);
    if (p->gen->deinit != NULL)
        p->gen->deinit();
    free(p->gen);
    free(p);
}

static void ptxt_info_destroy(ptxt_info_t *p)
{
    //free(p->gen);
    if (p->gen->deinit != NULL)
        p->gen->deinit();
    free(p->gen);
    free(p);
}

static void pnum_math_destroy(pnum_math_t *p)
{
    //free(p->gen);
    if (p->gen->deinit != NULL)
        p->gen->deinit();
    free(p->gen);
    free(p);
}

static void palfa_func_destroy(palfa_func_t *p)
{
    //free(p->gen);
    if (p->gen->deinit != NULL)
        p->gen->deinit();
    free(p->gen);
    free(p);
}


static unsigned int fnv1a_hash_str(const void *in, unsigned int seed)
{
    unsigned int  h = seed; /* default offset_basis: 2166136261 */
    unsigned int  c;
    size_t        len;
    size_t        i;

    len = strlen((const char *)in);
    for (i=0; i<len; i++) {
        c = ((const unsigned char *)in)[i];
        h ^= c;
        h *= 16777619; /* FNV prime */
    }

    return h;
}

static bool htable_str_eq(const void *a, const void *b)
{
    return (strcmp(a, b)==0)?true:false;
}


static bool pm_load_txt_transform(pm_manager_t *pm, pgen_t *gen)
{
    ptxt_transform_t *p;
    ptxt_transform_t *p2;
    p = calloc(1, sizeof(p));
    p->gen = gen;
#ifdef LINUX
    p->transform = dlsym(p->gen->handle, "txt_transform");
#else
    p->transform = GetProcAddress(p->gen->handle, "plugin_version");
#endif
    if (p->transform == NULL) {
        ptxt_transform_destroy(p);
        return false;
    }
    if (htable_get(pm->ptxt_transform, (void *)gen->name(), (void **)&p2) &&
            gen->version() > p2->gen->version())
    {
        ptxt_transform_destroy(p);
        return false;
    }
    htable_insert(pm->ptxt_transform, (void *)p->gen->name(), p);
    return true;
}

static bool pm_load_txt_info(pm_manager_t *pm, pgen_t *gen)
{
    ptxt_info_t *p;
    ptxt_info_t *p2;
    p = calloc(1, sizeof(p));
    p->gen = gen;
#ifdef LINUX
    p->info = dlsym(p->gen->handle, "txt_info");
#else
    p->info = GetProcAddress(p->gen->handle, "txt_info");
#endif
    if (p->info == NULL) {
        ptxt_info_destroy(p);
        return false;
    }
    if (htable_get(pm->ptxt_info, (void *)gen->name(), (void **)&p2) &&
            gen->version() > p2->gen->version())
    {
        ptxt_info_destroy(p);
        return false;
    }
    htable_insert(pm->ptxt_info, (void *)p->gen->name(), p);
    return true;
}

static bool pm_load_num_math(pm_manager_t *pm, pgen_t *gen)
{
    pnum_math_t *p;
    pnum_math_t *p2;
    p = calloc(1, sizeof(p));
    p->gen = gen;
#ifdef LINUX
    p->num = dlsym(p->gen->handle, "num_math");
#else
    p->num = GetProcAddress(p->gen->handle, "num_math");
#endif
    if (p->num == NULL) {
        pnum_math_destroy(p);
        return false;
    }
    if (htable_get(pm->pnum_math, (void *)gen->name(), (void **)&p2) &&
            gen->version() > p2->gen->version())
    {
        pnum_math_destroy(p);
        return false;
    }
    htable_insert(pm->pnum_math, (void *)p->gen->name(), p);
    return true;
}

static bool pm_load_alfa_func(pm_manager_t *pm, pgen_t *gen)
{
    palfa_func_t *p;
    palfa_func_t *p2;
    p = calloc(1, sizeof(p));
    p->gen = gen;
#ifdef LINUX
    p->param = dlsym(p->gen->handle, "alfa_func");
#else
    p->param = GetProcAddress(p->gen->handle, "alfa_func");
#endif
    if (p->param == NULL) {
        palfa_func_destroy(p);
        return false;
    }
    if (htable_get(pm->palfa_func, (void *)gen->name(), (void **)&p2) &&
        gen->version() > p2->gen->version())
    {
        palfa_func_destroy(p);
        return false;
    }
    htable_insert(pm->palfa_func, (void *)p->gen->name(), p);
    return true;
}


static void pm_load_gen(pm_manager_t *pm, void *handle, char *plug_ptr)
{
    pgen_t          *gen;
    const char      *const_temp;
    cpm_type         typef;
    cpm_api_version  api_ver;
    bool             ret;

    gen = calloc(1, sizeof(*gen));

#ifdef LINUX
    api_ver = dlsym(handle, "plugin_api_version");
#else
    api_ver = GetProcAddress(handle, "plugin_api_version");
#endif
    if (api_ver == NULL || api_ver() > api_version) {
        free(gen);
        return;
    }

#ifdef LINUX
    typef = dlsym(handle, "plugin_type");
#else
    typef = GetProcAddress(handle, "plugin_type");
#endif

    if (typef == NULL) {
        free(gen);
        return;
    }
    gen->type = typef();
#ifdef LINUX
    gen->name = dlsym(handle, "plugin_name");
#else
    gen->name = GetProcAddress(handle, "plugin_name");
#endif

    if (gen->name == NULL) {
        free(gen);
        return;
    }
    const_temp = gen->name();
    if (const_temp == NULL || *const_temp == '\0') {
        free(gen);
        return;
    }

#ifdef LINUX
    gen->init = dlsym(handle, "init");
#else
    gen->init = GetProcAddress(handle, "init");
#endif

    if (gen->init != NULL) {
        if (!gen->init(plug_ptr)) {
            free(gen);
            return;
        }
    }

    gen->handle = handle;
#ifdef LINUX
    gen->deinit = dlsym(handle, "deinit");
#else
    gen->deinit = GetProcAddress(handle, "deinit");
#endif

#ifdef LINUX
    gen->version = dlsym(handle, "plugin_version");
#else
    gen->version = GetProcAddress(handle, "plugin_version");
#endif


    switch (gen->type) {
        case PLUGIN_TYPE_TXT:
            ret = pm_load_txt_transform(pm, gen);
            break;
        case PLUGIN_TYPE_INFO:
            ret = pm_load_txt_info(pm, gen);
            break;
        case PLUGIN_TYPE_NUM:
            ret = pm_load_num_math(pm, gen);
            break;
        case PLUGIN_TYPE_ALFA:
            ret = pm_load_alfa_func(pm, gen);
            break;
        case PLUGIN_TYPE_UNKNOWN:
            ret = false;
            break;
    }

    if (!ret)
        free(gen);
}


static void pm_load_plugs(pm_manager_t *pm, char *plug_ptr)
{
    DIR           *dp;
    struct dirent *ep;
    void          *handle;
    char           path[256];
    char *error;

    dp = opendir("./plugins");
    if (dp == NULL)
        return;

    ep = readdir(dp);
    while (ep != NULL) {
        if (ep->d_type != DT_REG) {
            ep = readdir(dp);
            continue;
        }

        
#ifdef LINUX
        snprintf(path, sizeof(path), "./plugins/%s", ep->d_name);
        handle = dlopen(path, RTLD_NOW);
        if (!handle) {
            printf("Error: %s\n", dlerror());
        }
        //dlerror();
        if ((error = dlerror()) != NULL) {
            printf("dlerror: %s\n", error);
        }
        if (handle != NULL)
            pm_load_gen(pm, handle, plug_ptr);
#else
        snprintf(path, sizeof(path), "plugins/%s", ep->d_name);
        handle = LoadLibrary(path);
        if (!handle) {
            printf("Error: %s\n", path);
        }
        
        if (handle != NULL)
            pm_load_gen(pm, handle, plug_ptr);
#endif
        ep = readdir(dp);
    }
}

pm_manager_t *pm_load(char *plug_ptr)
{
    pm_manager_t *pm;
    htable_cbs    cbs = {
        (htable_kcopy)strdup,
        (htable_kfree)free,
        NULL,
        NULL,
    };

    pm = calloc(1, sizeof(*pm));
    cbs.val_free       = (htable_vfree)ptxt_transform_destroy;
    pm->ptxt_transform = htable_create(fnv1a_hash_str, htable_str_eq, &cbs);

    cbs.val_free  = (htable_vfree)ptxt_info_destroy;
    pm->ptxt_info = htable_create(fnv1a_hash_str, htable_str_eq, &cbs);

    cbs.val_free  = (htable_vfree)pnum_math_destroy;
    pm->pnum_math = htable_create(fnv1a_hash_str, htable_str_eq, &cbs);

    cbs.val_free  = (htable_vfree)palfa_func_destroy;
    pm->palfa_func = htable_create(fnv1a_hash_str, htable_str_eq, &cbs);

    pm_load_plugs(pm, plug_ptr);
    return pm;
}


void pm_unload(pm_manager_t *pm)
{
    if (pm == NULL)
        return;
    htable_destroy(pm->ptxt_transform);
    htable_destroy(pm->ptxt_info);
    htable_destroy(pm->pnum_math);
#ifdef LINUX
    htable_destroy(pm->palfa_func);
#endif
    free(pm);
}


static list_str_t *pm_plugins(htable_t *ht)
{
    list_str_t    *ls;
    htable_enum_t *he;
    const char    *name;

    ls = list_str_create(LIST_STR_NONE);
    he = htable_enum_create(ht);
    while (htable_enum_next(he, (void **)&name, NULL)) {
        list_str_append(ls, name);
    }
    htable_enum_destroy(he);

    return ls;
}

list_str_t *pm_plugins_txt_transform(const pm_manager_t *pm)
{
    if (pm == NULL)
        return NULL;
    return pm_plugins(pm->ptxt_transform);
}

list_str_t *pm_plugins_txt_info(const pm_manager_t *pm)
{
    if (pm == NULL)
        return NULL;
    return pm_plugins(pm->ptxt_info);
}

list_str_t *pm_plugins_num(const pm_manager_t *pm)
{
    if (pm == NULL)
        return NULL;
    return pm_plugins(pm->pnum_math);
}

list_str_t *pm_plugins_alfa(const pm_manager_t *pm)
{
    if (pm == NULL)
        return NULL;
    return pm_plugins(pm->palfa_func);
}


char *pm_plugin_txt_tranform(pm_manager_t *pm, const char *name, const char *txt)
{
    ptxt_transform_t *p;

    if (pm == NULL)
        return NULL;
    p = htable_get_direct(pm->ptxt_transform, (void *)name);
    if (p == NULL)
        return NULL;

    if (p->transform == NULL)
        return NULL;
    return p->transform(txt);
}

char *pm_plugin_txt_info(pm_manager_t *pm, const char *name, const char *txt)
{
    ptxt_info_t *p;

    if (pm == NULL)
        return NULL;
    p = htable_get_direct(pm->ptxt_info, (void *)name);
    if (p == NULL)
        return NULL;

    if (p->info == NULL)
        return NULL;
    return p->info(txt);
}

void pm_plugin_num_math(pm_manager_t *pm, const char *name, int a)
{
    pnum_math_t *p;

    if (pm == NULL)
        return;
    p = htable_get_direct(pm->pnum_math, (void *)name);
    if (p == NULL)
        return;

    if (p->num == NULL)
        return;
    p->num(a);
}

char *pm_plugin_alfa(pm_manager_t *pm, const char *name, void *a)
{
    palfa_func_t *p;

    if (pm == NULL)
        return NULL;
    p = htable_get_direct(pm->palfa_func, (void *)name);
    if (p == NULL)
        return NULL;

    if (p->param == NULL)
        return NULL;
    p->param(a);
}


