/*
 * PtokaX - hub server for Direct Connect peer to peer network.

 * Copyright (C) 2002-2005  Ptaczek, Ptaczek at PtokaX dot org
 * Copyright (C) 2004-2008  Petr Kozelka, PPK at PtokaX dot org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//---------------------------------------------------------------------------
#include "stdinc.h"
//---------------------------------------------------------------------------
#include "LuaInc.h"
//---------------------------------------------------------------------------
#include "LuaUDPDbgLib.h"
//---------------------------------------------------------------------------
#include "LuaScriptManager.h"
#include "UdpDebug.h"
#include "utility.h"
//---------------------------------------------------------------------------
#include "LuaScript.h"
//---------------------------------------------------------------------------

static int Reg(lua_State * L) {
	if(lua_gettop(L) != 3) {
        luaL_error(L, "bad argument count to 'Reg' (3 expected, got %d)", lua_gettop(L));
        lua_settop(L, 0);
        lua_pushnil(L);
        return 1;
    }

    if(lua_type(L, 1) != LUA_TSTRING || lua_type(L, 2) != LUA_TNUMBER || lua_type(L, 3) != LUA_TBOOLEAN) {
        luaL_checktype(L, 1, LUA_TSTRING);
        luaL_checktype(L, 2, LUA_TNUMBER);
        luaL_checktype(L, 3, LUA_TBOOLEAN);

		lua_settop(L, 0);
		lua_pushnil(L);
        return 1;
    }

	Script * cur = ScriptManager->FindScript(L);
	if(cur == NULL || cur->bRegUDP == true) {
        lua_settop(L, 0);
		lua_pushnil(L);
        return 1;
    }

    size_t iLen;
    char * sIP = (char *)lua_tolstring(L, 1, &iLen);

    if(iLen < 7 || iLen > 15 || isIP(sIP, iLen) == false) {
		lua_settop(L, 0);
		lua_pushnil(L);
        return 1;
    }

    unsigned short usPort = (unsigned short)lua_tonumber(L, 2);

    bool bAllData = lua_toboolean(L, 3) == 0 ? false : true;

    if(UdpDebug->New(sIP, usPort, bAllData, cur->sName) == false) {
		lua_settop(L, 0);
		lua_pushnil(L);
        return 1;
    }

    cur->bRegUDP = true;

    lua_settop(L, 0);
    lua_pushboolean(L, 1);
    return 1;
}
//------------------------------------------------------------------------------

static int Unreg(lua_State * L) {
	if(lua_gettop(L) != 0) {
        luaL_error(L, "bad argument count to 'Unreg' (0 expected, got %d)", lua_gettop(L));
        lua_settop(L, 0);
        return 0;
    }

	Script * cur = ScriptManager->FindScript(L);
	if(cur == NULL || cur->bRegUDP == false) {
        lua_settop(L, 0);
        return 0;
    }

    UdpDebug->Remove(cur->sName);

    cur->bRegUDP = false;

    return 0;
}
//------------------------------------------------------------------------------

static int Send(lua_State * L) {
	if(lua_gettop(L) != 1) {
        luaL_error(L, "bad argument count to 'Send' (1 expected, got %d)", lua_gettop(L));
        lua_settop(L, 0);
        lua_pushnil(L);
        return 1;
    }

    if(lua_type(L, 1) != LUA_TSTRING) {
        luaL_checktype(L, 1, LUA_TSTRING);

		lua_settop(L, 0);
		lua_pushnil(L);
        return 1;
    }

    size_t iLen;
    char * sMsg = (char *)lua_tolstring(L, 1, &iLen);

    if(iLen == 0) {
		lua_settop(L, 0);
		lua_pushnil(L);
        return 1;
    }

	Script * cur = ScriptManager->FindScript(L);
	if(cur == NULL || cur->bRegUDP == false) {
        UdpDebug->Broadcast(sMsg, iLen);
        lua_settop(L, 0);
        lua_pushboolean(L, 1);
        return 1;
    }

    UdpDebug->Send(cur->sName, sMsg, iLen);

    lua_pushboolean(L, 1);
    return 1;
}
//------------------------------------------------------------------------------

static const luaL_reg udpdbg_funcs[] =  {
	{ "Reg", Reg }, 
	{ "Unreg", Unreg }, 
	{ "Send", Send }, 
	{ NULL, NULL }
};
//---------------------------------------------------------------------------

void RegUDPDbg(lua_State * L) {
    luaL_register(L, "UDPDbg", udpdbg_funcs);
}
//---------------------------------------------------------------------------
