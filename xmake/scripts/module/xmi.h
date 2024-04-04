/*!A cross-platform build utility based on Lua
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Copyright (C) 2015-present, TBOOX Open Source Group.
 *
 * @author      ruki
 * @file        xmi.h
 *
 */
#ifndef XMI_H
#define XMI_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include <stdio.h>
#include <stdlib.h>
#ifndef LUA_VERSION
#   include "luaconf.h"
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// pseudo-indices
#ifdef XMI_USE_LUAJIT
#   define XMI_LUA_REGISTRYINDEX	(-10000)
#   define XMI_LUA_ENVIRONINDEX	    (-10001)
#   define XMI_LUA_GLOBALSINDEX	    (-10002)
#   define xmi_lua_upvalueindex(i)	(XMI_LUA_GLOBALSINDEX - (i))
#else
#   define XMI_LUA_REGISTRYINDEX	(-LUAI_MAXSTACK - 1000)
#   define xmi_lua_upvalueindex(i)	(XMI_LUA_REGISTRYINDEX - (i))
#endif

// lua interfaces
#define xmi_lua_createtable(lua, narr, nrec)    (g_lua_ops)->_lua_createtable(lua, narr, nrec)
#define xmi_lua_tointegerx(lua, idx, isnum)     (g_lua_ops)->_lua_tointegerx(lua, idx, isnum)
#define xmi_lua_touserdata(lua, idx)            (g_lua_ops)->_lua_touserdata(lua, idx)
#define xmi_lua_pushinteger(lua, n)             (g_lua_ops)->_lua_pushinteger(lua, n)
#define xmi_lua_newtable(lua)		            xmi_lua_createtable(lua, 0, 0)
#define xmi_lua_tointeger(lua, i)               lua_tointegerx(lua, (i), NULL)

// luaL interfaces
#define xmi_luaL_setfuncs(lua, narr, nrec)      (g_lua_ops)->_luaL_setfuncs(lua, narr, nrec)
#if defined(_MSC_VER)
#   define xmi_luaL_error(lua, fmt, ...)        (g_lua_ops)->_luaL_error(lua, fmt, __VA_ARGS__)
#else
#   define xmi_luaL_error(lua, fmt, arg ...)    (g_lua_ops)->_luaL_error(lua, fmt, ## arg)
#endif

/* we cannot redefine lua functions in loadxmi.c,
 * because original lua.h has been included
 */
#ifndef LUA_VERSION
#   define lua_createtable          xmi_lua_createtable
#   define lua_tointegerx           xmi_lua_tointegerx
#   define lua_touserdata           xmi_lua_touserdata
#   define lua_pushinteger          xmi_lua_pushinteger
#   define lua_newtable             xmi_lua_newtable
#   define lua_tointeger            xmi_lua_tointeger
#   define lua_upvalueindex         xmi_lua_upvalueindex

#   define luaL_setfuncs            xmi_luaL_setfuncs
#   define luaL_error               xmi_luaL_error

#   define luaL_Reg                 xmi_luaL_Reg
#   define lua_State                xmi_lua_State
#   define lua_Integer              xmi_lua_Integer
#endif

// extern c
#ifdef __cplusplus
#   define xmi_extern_c_enter       extern "C" {
#   define xmi_extern_c_leave       }
#else
#   define xmi_extern_c_enter
#   define xmi_extern_c_leave
#endif

// define lua module entry function
#define luaopen(name, lua) \
__dummy = 1; \
xmi_lua_ops_t* g_lua_ops = NULL; \
xmi_extern_c_enter \
int xmiopen_##name(lua); \
xmi_extern_c_leave \
int xmisetup(xmi_lua_ops_t* ops) { \
    g_lua_ops = ops; \
    return __dummy; \
} \
int xmiopen_##name(lua)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */
typedef LUA_INTEGER xmi_lua_Integer;

typedef struct xmi_lua_State_ {
    int dummy;
}xmi_lua_State;

typedef struct xmi_luaL_Reg_ {
    char const* name;
    int (*func)(lua_State* lua);
}xmi_luaL_Reg;

typedef struct xmi_lua_ops_t_ {
    void        (*_lua_createtable)(lua_State* lua, int narr, int nrec);
    lua_Integer (*_lua_tointegerx)(lua_State* lua, int idx, int* isnum);
    void        (*_lua_pushinteger)(lua_State* lua, lua_Integer n);
    void*       (*_lua_touserdata)(lua_State* lua, int idx);

    void        (*_luaL_setfuncs)(lua_State* lua, const luaL_Reg* l, int nup);
    int         (*_luaL_error)(lua_State* lua, const char* fmt, ...);
}xmi_lua_ops_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */
extern xmi_lua_ops_t* g_lua_ops;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
xmi_extern_c_enter

// setup lua interfaces
int xmisetup(xmi_lua_ops_t* ops);

xmi_extern_c_leave
#endif


