#ifndef __PRI_DWARF_EXT_H__
#define __PRI_DWARF_EXT_H__
/*
 * DO NOT MODIFY. This file is automatically generated by scripts/apigen.py,
 * based on the <plugin>_int.h file in your plugin directory.
 *
 * Note: Function pointers for API calls are declared as extern.
 * The definition of the pointers is guarded by the PLUGIN_MAIN macro.
 * This plugin is defined only for the compilation unit matching the
 * name of the plugin.
 * This allows us to initialize API function pointers once, in the main
 * compilation unit, rather than in every compilation unit.
 */
#include <dlfcn.h>
#include "panda/plugin.h"

typedef void(*dwarf_type_iter_t)(CPUState *env, target_ulong base_addr, LocType loc_t, DwarfVarType *var_ty, dwarfTypeCB cb, int recursion_level);
extern dwarf_type_iter_t __dwarf_type_iter;
#ifdef PLUGIN_MAIN
dwarf_type_iter_t __dwarf_type_iter = NULL;
#endif
static inline void dwarf_type_iter(CPUState *env, target_ulong base_addr, LocType loc_t, DwarfVarType *var_ty, dwarfTypeCB cb, int recursion_level);
static inline void dwarf_type_iter(CPUState *env, target_ulong base_addr, LocType loc_t, DwarfVarType *var_ty, dwarfTypeCB cb, int recursion_level) {
    assert(__dwarf_type_iter);
    return __dwarf_type_iter(env,base_addr,loc_t,var_ty,cb,recursion_level);
}

typedef const char *(*dwarf_type_to_string_t)(DwarfVarType *var_ty);
extern dwarf_type_to_string_t __dwarf_type_to_string;
#ifdef PLUGIN_MAIN
dwarf_type_to_string_t __dwarf_type_to_string = NULL;
#endif
static inline const char * dwarf_type_to_string(DwarfVarType *var_ty);
static inline const char * dwarf_type_to_string(DwarfVarType *var_ty) {
    assert(__dwarf_type_to_string);
    return __dwarf_type_to_string(var_ty);
}

bool init_pri_dwarf_api(void);

#ifdef PLUGIN_MAIN
#define API_PLUGIN_NAME "pri_dwarf"
#define IMPORT_PPP(module, func_name) { \
    __##func_name = (func_name##_t) dlsym(module, #func_name); \
    char *err = dlerror(); \
    if (err) { \
        printf("Couldn't find %s function in library %s.\n", #func_name, API_PLUGIN_NAME); \
        printf("Error: %s\n", err); \
        return false; \
    } \
}
bool init_pri_dwarf_api(void) {
    void *module = panda_get_plugin_by_name(API_PLUGIN_NAME);
    if (!module) {
        fprintf(stderr, "Couldn't load %s plugin: %s\n", API_PLUGIN_NAME, dlerror());
        return false;
    }
    IMPORT_PPP(module, dwarf_type_iter)
    IMPORT_PPP(module, dwarf_type_to_string)
    return true;
}
#undef API_PLUGIN_NAME
#undef IMPORT_PPP
#endif

#endif
