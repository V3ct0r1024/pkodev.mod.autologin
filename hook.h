#pragma once

namespace pkodev
{
	namespace hook
	{
		// int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
		int __stdcall _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

		// bool CLoginScene::_Init()
		bool __fastcall CLoginScene___Init(void* This, void* notUsed);

		// void CSelectChaScene::_FrameMove ( DWORD dwTimeParam )
		void __fastcall CSelectChaScene___FrameMove(void* This, void* notUsed, unsigned int dwTimeParam);

		// BOOL	SC_Login(LPRPACKET pk)
		bool SC_Login(void* pk);

		// void NetLoginFailure(unsigned short Errno)
		void __cdecl NetLoginFailure(unsigned short err);
	}
}