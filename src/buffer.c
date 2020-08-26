/*
MIT License

buffer.c
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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <scalu/buffer.h>

static int SCALU_METAFN(,Buffer,free)(lua_State *L);
static int SCALU_METAFN(,Buffer,get8)(lua_State *L);
static int SCALU_METAFN(,Buffer,get16)(lua_State *L);
static int SCALU_METAFN(,Buffer,get32)(lua_State *L);
static int SCALU_METAFN(,Buffer,len)(lua_State *L);
static int SCALU_METAFN(,Buffer,realloc)(lua_State *L);
static int SCALU_METAFN(,Buffer,set8)(lua_State *L);
static int SCALU_METAFN(,Buffer,set16)(lua_State *L);
static int SCALU_METAFN(,Buffer,set32)(lua_State *L);
static int SCALU_METAFN(,Buffer,size)(lua_State *L);

static const luaL_Reg idx_Buffer[] = {
	SCALU_METAENT(,Buffer,free),
	SCALU_METAENT(,Buffer,get8),
	SCALU_METAENT(,Buffer,get16),
	SCALU_METAENT(,Buffer,get32),
	SCALU_METAENT(,Buffer,realloc),
	SCALU_METAENT(,Buffer,set8),
	SCALU_METAENT(,Buffer,set16),
	SCALU_METAENT(,Buffer,set32),
	SCALU_METAENT(,Buffer,size),
	{NULL, NULL}
};

SCALU_API void SCALU_META(,Buffer)(lua_State *L) {
	if (luaL_newmetatable(L, SCALU_TYPE_BUFFER)) { // t
		lua_pushliteral(L, "__index"); // t s
		luaL_newlibtable(L, idx_Buffer); // t s t
		luaL_setfuncs(L, idx_Buffer, 0); // t s t
		lua_settable(L, -3); // t

		lua_pushliteral(L, "__len"); // t s
		lua_pushcfunction(L, SCALU_METAFN(,Buffer,len)); // t s f
		lua_settable(L, -3); // t

		lua_pushliteral(L, "name"); // t s
		lua_pushliteral(L, "Buffer"); // t s s
		lua_settable(L, -3); // t
	}
}

SCALU_API int SCALU_MOD(Buffer)(lua_State *L) {
	int size = luaL_checkinteger(L, -1);
	void *ptr = malloc((size_t)size);
	if (ptr == NULL) {
		return luaL_error(L, "Scalu: failed to allocate buffer of size %d", size);
	}

	lua_pushlightuserdata(L, ptr); // u
	SCALU_META(,Buffer)(L); // u t
	lua_newtable(L); // u t t
	lua_pushliteral(L, "__index"); // u t t s
	lua_pushvalue(L, -1); // u t t s s
	lua_gettable(L, -4); // u t t s t
	lua_settable(L, -3); // u t t
	lua_pushliteral(L, "size"); // u t t s
	lua_pushinteger(L, size); // u t t s i
	lua_settable(L, -3); // u t t
	lua_setmetatable(L, -3); // u t
	lua_pop(L, 1); // u

	return 1;
}

static int SCALU_METAFN(,Buffer,free)(lua_State *L) {
	free(luaL_checkudata(L, 1, SCALU_TYPE_BUFFER));
	return 0;
}

static int SCALU_METAFN(,Buffer,get8)(lua_State *L) {
	uint8_t *ptr = (uint8_t *)luaL_checkudata(L, 1, SCALU_TYPE_BUFFER);
	int off = luaL_checkinteger(L, 2);
	luaL_getmetafield(L, 1, "size"); // i
	int size = luaL_checkinteger(L, -1);

	if (off >= size) {
		lua_pushliteral(L, "Scalu: Buffer offset out of bounds");
		return lua_error(L);
	}

	lua_pushinteger(L, ptr[off]);

	return 1;
}

static int SCALU_METAFN(,Buffer,get16)(lua_State *L) {
	uint8_t *ptr = (uint8_t *)luaL_checkudata(L, 1, SCALU_TYPE_BUFFER);
	int off = luaL_checkinteger(L, 2);
	luaL_getmetafield(L, 1, "size"); // i
	int size = luaL_checkinteger(L, -1);

	if (off >= size - 1) {
		lua_pushliteral(L, "Scalu: Buffer offset out of bounds");
		return lua_error(L);
	}

	lua_pushinteger(L, *(uint16_t *)&ptr[off]);

	return 1;
}

static int SCALU_METAFN(,Buffer,get32)(lua_State *L) {
	uint8_t *ptr = (uint8_t *)luaL_checkudata(L, 1, SCALU_TYPE_BUFFER);
	int off = luaL_checkinteger(L, 2);
	luaL_getmetafield(L, 1, "size"); // i
	int size = luaL_checkinteger(L, -1);

	if (off >= size - 3) {
		lua_pushliteral(L, "Scalu: Buffer offset out of bounds");
		return lua_error(L);
	}

	lua_pushinteger(L, *(uint32_t *)&ptr[off]);

	return 1;
}

static int SCALU_METAFN(,Buffer,len)(lua_State *L) {
	luaL_checkudata(L, 1, SCALU_TYPE_BUFFER);
	lua_getmetatable(L, 1); // t
	lua_pushliteral(L, "size"); // t s
	lua_gettable(L, -2); // t s i
	return 1;
}

static int SCALU_METAFN(,Buffer,realloc)(lua_State *L) {
	void *ptr = luaL_checkudata(L, 1, SCALU_TYPE_BUFFER);
	int size = luaL_checkinteger(L, 2);
	lua_getmetatable(L, 1); // t

	ptr = realloc(ptr, (size_t)size);
	if (ptr == NULL) {
		return luaL_error(L, "Scalu: failed to realloc Buffer to size %d", size);
	}

	lua_pushlightuserdata(L, ptr); // u
	lua_pushliteral(L, "size"); // u t s
	lua_pushinteger(L, size); // u t s i
	lua_settable(L, -3); // u t
	lua_setmetatable(L, -2); // u

	return 1;
}

static int SCALU_METAFN(,Buffer,set8)(lua_State *L) {
	uint8_t *ptr = (uint8_t *)luaL_checkudata(L, 1, SCALU_TYPE_BUFFER);
	int off = luaL_checkinteger(L, 2);
	int val = luaL_checkinteger(L, 3);
	luaL_getmetafield(L, 1, "size"); // i
	int size = luaL_checkinteger(L, -1);

	if (off >= size) {
		lua_pushliteral(L, "Scalu: Buffer offset out of bounds");
		return lua_error(L);
	}

	ptr[off] = (uint8_t)val;

	return 0;
}

static int SCALU_METAFN(,Buffer,set16)(lua_State *L) {
	uint8_t *ptr = (uint8_t *)luaL_checkudata(L, 1, SCALU_TYPE_BUFFER);
	int off = luaL_checkinteger(L, 2);
	int val = luaL_checkinteger(L, 3);
	luaL_getmetafield(L, 1, "size"); // i
	int size = luaL_checkinteger(L, -1);

	if (off >= size - 1) {
		lua_pushliteral(L, "Scalu: Buffer offset out of bounds");
		return lua_error(L);
	}

	*(uint16_t *)&ptr[off] = (uint16_t)val;

	return 0;
}

static int SCALU_METAFN(,Buffer,set32)(lua_State *L) {
	uint8_t *ptr = (uint8_t *)luaL_checkudata(L, 1, SCALU_TYPE_BUFFER);
	int off = luaL_checkinteger(L, 2);
	int val = luaL_checkinteger(L, 3);
	luaL_getmetafield(L, 1, "size"); // i
	int size = luaL_checkinteger(L, -1);

	if (off >= size - 1) {
		lua_pushliteral(L, "Scalu: Buffer offset out of bounds");
		return lua_error(L);
	}

	*(uint32_t *)&ptr[off] = (uint32_t)val;

	return 0;
}

static int SCALU_METAFN(,Buffer,size)(lua_State *L) {
	luaL_checkudata(L, 1, SCALU_TYPE_BUFFER);
	luaL_getmetafield(L, 1, "size"); // i
	return 1;
}
