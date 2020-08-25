#ifndef SCALU_H
#define SCALU_H

/*
MIT License

scalu.h
Copyright (c) 2020 Nick Trebes

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#define SCALU_API extern
#define SCALU_CLS(mod,type) class_scalu_##mod##_##type
#define SCALU_CLSENT(mod,type,fn) {#fn, class_scalu_##mod##_##type##_##fn}
#define SCALU_CLSFN(mod,type,fn) class_scalu_##mod##_##type##_##fn
#define SCALU_META(mod,type) meta_scalu_##mod##_##type
#define SCALU_METAENT(mod,type,fn) {#fn, meta_scalu_##mod##_##type##_##fn}
#define SCALU_METAFN(mod,type,fn) meta_scalu_##mod##_##type##_##fn
#define SCALU_MOD(mod) lualib_scalu_##mod
#define SCALU_MODENT(mod,fn) {#fn, lualib_scalu_##mod##_##fn}
#define SCALU_MODFN(mod,fn) lualib_scalu_##mod##_##fn
#define SCALU_OPEN luaopen_libscalu

#include <lua5.3/lua.h>
#include <lua5.3/lualib.h>
#include <lua5.3/lauxlib.h>

SCALU_API int SCALU_OPEN(lua_State *L);

#endif /* ! SCALU_H */
