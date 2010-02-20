//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) 2003-2010 Jason Zink 
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// ScriptManager
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#ifndef ScriptManager_h
#define ScriptManager_h
//--------------------------------------------------------------------------------
#include <vector>
#include <string>
#include <map>
//--------------------------------------------------------------------------------
#ifdef _DEBUG
	#pragma comment( lib, "lualibd.lib" )
	#pragma comment( lib, "lualiblibd.lib" )
#else
	#pragma comment( lib, "lualib.lib" )
	#pragma comment( lib, "lualiblib.lib" )
#endif
//--------------------------------------------------------------------------------
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
//--------------------------------------------------------------------------------
namespace Glyph3
{
	struct sClassData
	{
		std::wstring name;
		unsigned int count;
		unsigned int id;

	public:
		sClassData()
		{
			name = L"";
			count = 0;
			id = 0;
		};

		unsigned int ReserveHandle()
		{
			unsigned int handle = count + id;
			count++;
			return( handle );
		};
	};

	struct sObjectData
	{
		void* pointer;

	public:
		sObjectData()
		{
			pointer = 0;
		};
	};

	struct sPointerData
	{
		unsigned int handle;
	public:
		sPointerData()
		{
			handle = 0xffffffff;
		};
	};

	class ScriptManager
	{
	public:
		ScriptManager();
		~ScriptManager();

		static ScriptManager* Get();

		// Registration of standard static functions
		void RegisterFunction( const wchar_t* name, lua_CFunction function );
		void RegisterClassFunction( const wchar_t* classname, const wchar_t* funcname, lua_CFunction function );

		void ExecuteChunk( wchar_t* chunk );
		
		// Registration of classes, their class methods, and instances of
		// an object.
		unsigned int RegisterEngineClass( const wchar_t* name );
		unsigned int RegisterEngineObject( const wchar_t* name, void* pObject );

		bool UnRegisterObjectByHandle( unsigned int handle );
		bool UnRegisterObjectByPointer( void* pObject );
		bool IsRegistered( void* pObject );

		// Lookup a class by ID to verify the correct type reference. 
//		std::wstring& GetClass( int ClassID );
		
		// Retrieve an object by ID.  This will inherently check the type
		// of the object with this handle and return null if the handle
		// type doesn't match the stored object type.
		void* GetObjectPointer( unsigned int ID );
		unsigned int GetObjectHandle( void* pObject );

		void Run( wchar_t *fp_szFileName );
		lua_State* GetState( );

	protected:
		
		// ScriptManager pointer to ensure single instance
		static ScriptManager* ms_pScriptManager;
		
		// Base lua state
		lua_State* m_pLuaState;

		int m_iClassIndex;
		std::map< const wchar_t*, sClassData > m_kClassRegistry;
		std::map< unsigned int, sObjectData > m_kObjectRegistry;
		std::map< void*, sPointerData > m_kPointerRegistry;
	};
};
#endif // ScriptManager_h

