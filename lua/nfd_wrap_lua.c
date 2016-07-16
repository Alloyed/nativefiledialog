/// @module nfd
#include <assert.h>
#define false 0

#include "nfd.h"

#include "lua.h"
#include "lauxlib.h"

/***
 * Prompt the user to open a file, and return their selection.
 * @function open
 * @param filter
 * @param initial
 * @return path
 */
static int l_open(lua_State* L)
{
    nfdchar_t* out;
    const nfdchar_t* filter  = luaL_optstring(L, 1, NULL);
    const nfdchar_t* initial = luaL_optstring(L, 2, NULL);

    nfdresult_t status = NFD_OpenDialog(filter, initial, &out);

    switch (status) {
	case NFD_OKAY:
	    lua_pushstring(L, out);
	    return 1;
	case NFD_CANCEL:
	    lua_pushboolean(L, false);
	    return 1;
	case NFD_ERROR:
	    lua_pushstring(L, NFD_GetError());
	    lua_error(L); // always returns here
	    return -1;
	default:
		return 0;
    }
}

static void push_pathset(lua_State* L, nfdpathset_t* set)
{
    size_t count = NFD_PathSet_GetCount(set);
    lua_createtable(L, count, 0);
    int tbl = lua_gettop(L);
    for (size_t i = 0; i < count; ++i) {
	lua_pushstring(L, NFD_PathSet_GetPath(set, i));
	lua_rawseti(L, tbl, i);
    }
    assert(tbl == lua_gettop(L));
    NFD_PathSet_Free(set);
}

/***
 * Prompt the user to open a multiple files, and return their selection.
 * @function openMany
 * @param filter
 * @param initial
 * @return path
 */
static int l_openMany(lua_State* L)
{
    nfdpathset_t out;
    const nfdchar_t* filter  = luaL_optstring(L, 1, NULL);
    const nfdchar_t* initial = luaL_optstring(L, 2, NULL);

    nfdresult_t status = NFD_OpenDialogMultiple(filter, initial, &out);

    switch (status) {
	case NFD_OKAY:
	    push_pathset(L, &out);
	    return 1;
	case NFD_CANCEL:
	    lua_pushboolean(L, false);
	    return 1;
	case NFD_ERROR:
	    lua_pushstring(L, NFD_GetError());
	    lua_error(L); // always returns here
	    return -1;
	default:
		return 0;
    }
}

/***
 * Prompt the user to save a file, and return their selection.
 * @function save
 * @param filter
 * @param initial
 * @return path
 */
static int l_save(lua_State* L)
{
    nfdchar_t* out;
    const nfdchar_t* filter  = luaL_optstring(L, 1, NULL);
    const nfdchar_t* initial = luaL_optstring(L, 2, NULL);

    nfdresult_t status = NFD_SaveDialog(filter, initial, &out);

    switch (status) {
	case NFD_OKAY:
	    lua_pushstring(L, out);
	    return 1;
	case NFD_CANCEL:
	    lua_pushboolean(L, false);
	    return 1;
	case NFD_ERROR:
	    lua_pushstring(L, NFD_GetError());
	    lua_error(L); // always returns here
	    return -1;
	default:
		return 0;
    }
}

static void luax_setfuncs(lua_State *L, const luaL_Reg *l, int nup)
{
	luaL_checkstack(L, nup+1, "too many upvalues");
	for (; l->name != NULL; l++) {  /* fill the table with given functions */
		int i;
		lua_pushstring(L, l->name);
		for (i = 0; i < nup; i++)  /* copy upvalues to the top */
			lua_pushvalue(L, -(nup + 1));
		lua_pushcclosure(L, l->func, nup);  /* closure with those upvalues */
		/* table must be below the upvalues, the name and the closure */
		lua_settable(L, -(nup + 3));
	}
	lua_pop(L, nup);  /* remove upvalues */
}

static const struct luaL_Reg mod [] = {
	{"open",     l_open},
	{"openMany", l_openMany},
	{"save",     l_save},
	{NULL, NULL}
};

int luaopen_nfd(lua_State* L)
{
    lua_newtable(L);
    luax_setfuncs(L, mod, 0);
    return 1;
}
