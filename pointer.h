#pragma once
#include "address.h"

namespace pkodev
{
	namespace pointer
	{
		// int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
		typedef int(__stdcall* _tWinMain__Ptr)(HINSTANCE, HINSTANCE, LPSTR, int);
		_tWinMain__Ptr _tWinMain = (_tWinMain__Ptr)(void*)(address::MOD_EXE_VERSION::_tWinMain);

		// bool CLoginScene::_Init()
		typedef bool(__thiscall* CLoginScene___Init__Ptr)(void*);
		CLoginScene___Init__Ptr CLoginScene___Init = (CLoginScene___Init__Ptr)(void*)(address::MOD_EXE_VERSION::CLoginScene___Init);

		// void CSelectChaScene::_FrameMove ( DWORD dwTimeParam )
		typedef void(__thiscall* CSelectChaScene___FrameMove__Ptr)(void*, unsigned int);
		CSelectChaScene___FrameMove__Ptr CSelectChaScene___FrameMove = (CSelectChaScene___FrameMove__Ptr)(void*)(address::MOD_EXE_VERSION::CSelectChaScene___FrameMove);

		// bool CS_Connect(dbc::cChar *hostname, dbc::uShort port, dbc::uLong timeout)
		typedef bool(__cdecl* CS_Connect__Ptr)(const char*, unsigned short, unsigned int);
		CS_Connect__Ptr CS_Connect = (CS_Connect__Ptr)(void*)(address::MOD_EXE_VERSION::CS_Connect);

		// void CS_BeginPlay(const char *cha)
		typedef void(__cdecl* CS_BeginPlay__Ptr)(const char*);
		CS_BeginPlay__Ptr CS_BeginPlay = (CS_BeginPlay__Ptr)(void*)(address::MOD_EXE_VERSION::CS_BeginPlay);

		// BOOL	SC_Login(LPRPACKET pk)
		typedef bool(__cdecl* SC_Login__Ptr)(void*);
		SC_Login__Ptr SC_Login = (SC_Login__Ptr)(void*)(address::MOD_EXE_VERSION::SC_Login);

		// void CGameApp::Waiting( bool isWaiting )
		typedef void(__cdecl* CGameApp__Waiting__Ptr)(bool);
		CGameApp__Waiting__Ptr CGameApp__Waiting = (CGameApp__Waiting__Ptr)(void*)(address::MOD_EXE_VERSION::CGameApp__Waiting);

		// ??? - write const char * to edit box
		typedef int(__fastcall* Edit_StringAssign__Ptr)(void*, void*, const char*, unsigned int);
		Edit_StringAssign__Ptr Edit_StringAssign = (Edit_StringAssign__Ptr)(void*)(address::MOD_EXE_VERSION::Edit_StringAssign);

		// void NetLoginFailure(unsigned short Errno)
		typedef void(__cdecl* NetLoginFailure__Ptr)(unsigned short);
		NetLoginFailure__Ptr NetLoginFailure = (NetLoginFailure__Ptr)(void*)(address::MOD_EXE_VERSION::NetLoginFailure);

		// void CGameApp::MsgBox( const char *pszFormat, ... )
		typedef void(__cdecl* CGameApp__MsgBox__Ptr)(const char*, ...);
		CGameApp__MsgBox__Ptr CGameApp__MsgBox = (CGameApp__MsgBox__Ptr)(void*)(address::MOD_EXE_VERSION::CGameApp__MsgBox);

		// inline const char* g_GetServerError( int error_code )
		typedef const char* (__cdecl* g_GetServerError__Ptr)(int err);
		g_GetServerError__Ptr g_GetServerError = (g_GetServerError__Ptr)(void*)(address::MOD_EXE_VERSION::g_GetServerError);
	}
}