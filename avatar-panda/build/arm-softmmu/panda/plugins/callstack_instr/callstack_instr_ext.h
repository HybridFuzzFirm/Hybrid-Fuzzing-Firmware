#ifndef __CALLSTACK_INSTR_EXT_H__
#define __CALLSTACK_INSTR_EXT_H__
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

typedef uint32_t(*get_callers_t)(target_ulong *callers, uint32_t n, CPUState *cpu);
extern get_callers_t __get_callers;
#ifdef PLUGIN_MAIN
get_callers_t __get_callers = NULL;
#endif
static inline uint32_t get_callers(target_ulong *callers, uint32_t n, CPUState *cpu);
static inline uint32_t get_callers(target_ulong *callers, uint32_t n, CPUState *cpu) {
    assert(__get_callers);
    return __get_callers(callers,n,cpu);
}

typedef uint32_t(*get_functions_t)(target_ulong *functions, uint32_t n, CPUState *cpu);
extern get_functions_t __get_functions;
#ifdef PLUGIN_MAIN
get_functions_t __get_functions = NULL;
#endif
static inline uint32_t get_functions(target_ulong *functions, uint32_t n, CPUState *cpu);
static inline uint32_t get_functions(target_ulong *functions, uint32_t n, CPUState *cpu) {
    assert(__get_functions);
    return __get_functions(functions,n,cpu);
}

typedef void(*get_prog_point_t)(CPUState *cpu, prog_point *p);
extern get_prog_point_t __get_prog_point;
#ifdef PLUGIN_MAIN
get_prog_point_t __get_prog_point = NULL;
#endif
static inline void get_prog_point(CPUState *cpu, prog_point *p);
static inline void get_prog_point(CPUState *cpu, prog_point *p) {
    assert(__get_prog_point);
    return __get_prog_point(cpu,p);
}

typedef Panda__CallStack *(*pandalog_callstack_create_t)(void);
extern pandalog_callstack_create_t __pandalog_callstack_create;
#ifdef PLUGIN_MAIN
pandalog_callstack_create_t __pandalog_callstack_create = NULL;
#endif
static inline Panda__CallStack * pandalog_callstack_create(void);
static inline Panda__CallStack * pandalog_callstack_create(void) {
    assert(__pandalog_callstack_create);
    return __pandalog_callstack_create();
}

typedef void(*pandalog_callstack_free_t)(Panda__CallStack *cs);
extern pandalog_callstack_free_t __pandalog_callstack_free;
#ifdef PLUGIN_MAIN
pandalog_callstack_free_t __pandalog_callstack_free = NULL;
#endif
static inline void pandalog_callstack_free(Panda__CallStack *cs);
static inline void pandalog_callstack_free(Panda__CallStack *cs) {
    assert(__pandalog_callstack_free);
    return __pandalog_callstack_free(cs);
}

bool init_callstack_instr_api(void);

#ifdef PLUGIN_MAIN
#define API_PLUGIN_NAME "callstack_instr"
#define IMPORT_PPP(module, func_name) { \
    __##func_name = (func_name##_t) dlsym(module, #func_name); \
    char *err = dlerror(); \
    if (err) { \
        printf("Couldn't find %s function in library %s.\n", #func_name, API_PLUGIN_NAME); \
        printf("Error: %s\n", err); \
        return false; \
    } \
}
bool init_callstack_instr_api(void) {
    void *module = panda_get_plugin_by_name(API_PLUGIN_NAME);
    if (!module) {
        fprintf(stderr, "Couldn't load %s plugin: %s\n", API_PLUGIN_NAME, dlerror());
        return false;
    }
    IMPORT_PPP(module, get_callers)
    IMPORT_PPP(module, get_functions)
    IMPORT_PPP(module, get_prog_point)
    IMPORT_PPP(module, pandalog_callstack_create)
    IMPORT_PPP(module, pandalog_callstack_free)
    return true;
}
#undef API_PLUGIN_NAME
#undef IMPORT_PPP
#endif

#endif
