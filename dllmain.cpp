#include <Windows.h>
#include <detours.h>

#include <string>
#include <sstream>

#include "address.h"
#include "pointer.h"
#include "hook.h"
#include "loader.h"

namespace pkodev
{
    // Autologin data structure
    struct connection
    {
        // Server IP
        std::string ip;

        // Server port
        unsigned short int port;

        // Server version
        unsigned short int version;

        // Account login
        std::string login;

        // Account password
        std::string password;

        // Character
        std::string character;


        // Constructor
        connection() :
            ip(""),
            port(0),
            version(0),
            login(""),
            password(""),
            character("")
        {

        }
    } autologin;

    // Is autologin enabled?
    bool autologin_enabled = false;

    // Timer
    unsigned __int64 timer = 0;
}

// Entry point
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )

{
    // Nothing to do . . .
    return TRUE;
}

// Get mod information
void GetModInformation(mod_info& info)
{
    strcpy_s(info.name, TOSTRING(MOD_NAME));
    strcpy_s(info.version, TOSTRING(MOD_VERSION));
    strcpy_s(info.author, TOSTRING(MOD_AUTHOR));
    info.exe_version = MOD_EXE_VERSION;
}

// Start the mod
void Start(const char* path)
{
    // Enable hooks
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)pkodev::pointer::_tWinMain, pkodev::hook::_tWinMain);
    DetourAttach(&(PVOID&)pkodev::pointer::CLoginScene___Init, pkodev::hook::CLoginScene___Init);
    DetourAttach(&(PVOID&)pkodev::pointer::CSelectChaScene___FrameMove, pkodev::hook::CSelectChaScene___FrameMove);
    DetourAttach(&(PVOID&)pkodev::pointer::SC_Login, pkodev::hook::SC_Login);
    DetourAttach(&(PVOID&)pkodev::pointer::NetLoginFailure, pkodev::hook::NetLoginFailure);
    DetourTransactionCommit();
}

// Stop the mod
void Stop()
{
    // Disable hooks
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)pkodev::pointer::_tWinMain, pkodev::hook::_tWinMain);
    DetourDetach(&(PVOID&)pkodev::pointer::CLoginScene___Init, pkodev::hook::CLoginScene___Init);
    DetourDetach(&(PVOID&)pkodev::pointer::CSelectChaScene___FrameMove, pkodev::hook::CSelectChaScene___FrameMove);
    DetourDetach(&(PVOID&)pkodev::pointer::SC_Login, pkodev::hook::SC_Login);
    DetourDetach(&(PVOID&)pkodev::pointer::NetLoginFailure, pkodev::hook::NetLoginFailure);
    DetourTransactionCommit();
}

// int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
int __stdcall pkodev::hook::_tWinMain(HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Get startup parameters
    char buf[256]{ 0x00 };
    CharToOemBuffA(lpCmdLine, buf, sizeof(buf));

    // Make string stream
    std::istringstream input(buf);

    // Current parameter
    std::string param{ "" };

    // Trim string
    auto trim = [](std::string str) -> std::string
    {
        // Remove spaces from the beginning of the string
        const std::size_t start_pos = str.find_first_not_of(" \t");
        if ( std::string::npos != start_pos )
        {
            str = str.substr(start_pos);
        }

        // Remove spaces from the ending of the string
        const std::size_t end_pos = str.find_last_not_of(" \t");
        if ( std::string::npos != end_pos )
        {
            str = str.substr(0, end_pos + 1);
        }

        return str;
    };

    // Convert std::string to unsigned short int 
    auto string_to_usint = [](const std::string& str) -> unsigned short int
    {
        // Result
        unsigned short int ret = 0;

        // Try to rerform the conversion
        try
        {
            // Conversion . . .
            ret = static_cast<unsigned short int>( std::stoul(str) );
        }
        catch (...)
        {
            // Conversion error!
        }

        return ret;
    };


    /*
        Parse startup parameters string:
       
        1. ip:127.0.0.1
        2. port:1973
        3. version:136
        4. login:V3ct0r
        5. password:112233
        6. character:V3ct0r
    */
    while (std::getline(input, param, ' '))
    {
        // Trim current string
        param = trim(param);

        // Remove spaces from string
        param.erase(std::remove_if(param.begin(), param.end(), isspace), param.end());

        // Check that string is not empty
        if ( param.empty() == true )
        {
            continue;
        }

        // Search ':' delimiter
        const std::size_t p = param.find(':');

        // Check that the delimiter is found
        if (p == std::string::npos)
        {
            continue;
        }

        // Get param name
        const std::string name = trim( param.substr(0,  p) );

        // Get param value
        const std::string value = trim( param.substr(p + 1, param.length() - p) );

        // Check that param name and value are not empty
        if ( (name.empty() == true) || (value.empty() == true) )
        {
            continue;
        }

        // Parse parameters
        {
            if (name == "ip")        { pkodev::autologin.ip = value;                       }
            if (name == "port")      { pkodev::autologin.port = string_to_usint(value);    }
            if (name == "version")   { pkodev::autologin.version = string_to_usint(value); }
            if (name == "login")     { pkodev::autologin.login = value;                    }
            if (name == "password")  { pkodev::autologin.password = value;                 }
            if (name == "character") { pkodev::autologin.character = value;                }
        }
    }

    // Set autologin flag
    pkodev::autologin_enabled = (
        (pkodev::autologin.ip.empty()       == false) &&
        (pkodev::autologin.login.empty()    == false) &&
        (pkodev::autologin.password.empty() == false)
    );

    // Check that autologin is enabled
    if (pkodev::autologin_enabled == true)
    {
        // Check that server port is not specified
        if (pkodev::autologin.port == 0)
        {
            // Read port from Game.exe memory
            pkodev::autologin.port = *reinterpret_cast<unsigned short int*>(pkodev::address::MOD_EXE_VERSION::CS_Connect__port);
        }

        // Check that server version is not specified
        if (pkodev::autologin.version == 0)
        {
            // Read the version from Game.exe memory
            pkodev::autologin.version = *reinterpret_cast<unsigned short int*>(pkodev::address::MOD_EXE_VERSION::g_sClientVer);
        }
        else
        {
            // Write the version to the Game.exe memory
            *reinterpret_cast<unsigned short int*>(pkodev::address::MOD_EXE_VERSION::g_sClientVer) = pkodev::autologin.version;
        }
    }

    // Call original function int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
    return pkodev::pointer::_tWinMain(hInstance,  hPrevInstance,  lpCmdLine, nCmdShow);
}

// bool CLoginScene::_Init()
bool __fastcall pkodev::hook::CLoginScene___Init(void* This, void* notUsed)
{
    // Call original method bool CLoginScene::_Init()
    const bool ret = pkodev::pointer::CLoginScene___Init(This);

    // Check it's result
    if (ret == true)
    {
        // Check that we are in the autologin mode
        if (pkodev::autologin_enabled == true)
        {
            // Set login
            pkodev::pointer::Edit_StringAssign(
                reinterpret_cast<void*>(reinterpret_cast<unsigned int>(This) + 0x274),
                notUsed,
                pkodev::autologin.login.c_str(),
                pkodev::autologin.login.length()
            );

            // Set password
            pkodev::pointer::Edit_StringAssign(
                reinterpret_cast<void*>(reinterpret_cast<unsigned int>(This) + 0x290),
                notUsed,
                pkodev::autologin.password.c_str(),
                pkodev::autologin.password.length()
            );

            // Switch login scene to the connecting state
            *reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(This) + 0x2B0) = 0x01;

            // Connect to the server
            pkodev::pointer::CS_Connect(pkodev::autologin.ip.c_str(), pkodev::autologin.port, 40000);
        }
    }

    return ret;
}

// void CSelectChaScene::_FrameMove ( DWORD dwTimeParam )
void __fastcall pkodev::hook::CSelectChaScene___FrameMove(void* This, void* notUsed, unsigned int dwTimeParam)
{
    // Call original method void CSelectChaScene::_FrameMove ( DWORD dwTimeParam )
    pkodev::pointer::CSelectChaScene___FrameMove(This, dwTimeParam);

    // Check that we are in the autologin mode
    if (pkodev::autologin_enabled == true)
    {
        // Check that we need to enter the character into the game
        if (pkodev::autologin.character.empty() == false)
        {
            // Get current time delta
            const unsigned __int64 delta = GetTickCount64() - pkodev::timer;

            // Check delta
            if (delta > 3000.0f)
            {
                // Hide waiting box
                pkodev::pointer::CGameApp__Waiting(false);

                // Disable autologin mode
                pkodev::autologin_enabled = false;

                // Enter the map
                pkodev::pointer::CS_BeginPlay(pkodev::autologin.character.c_str());
            }
        }
    }
}

// BOOL	SC_Login(LPRPACKET pk)
bool pkodev::hook::SC_Login(void* pk)
{
    // Call original function BOOL SC_Login(LPRPACKET pk)
    const bool ret = pkodev::pointer::SC_Login(pk);

    // Check it's result
    if (ret == true)
    {
        // Check that we are in the autologin mode
        if (pkodev::autologin_enabled == true)
        {
            // Check that we need to enter the character into the game
            if (pkodev::autologin.character.empty() == false)
            {
                // Save current time
                pkodev::timer = GetTickCount64();

                // Show waiting box
                pkodev::pointer::CGameApp__Waiting(true);
            }
            else
            {
                // Disable autologin mode
                pkodev::autologin_enabled = false;
            }
        }
    }

    return ret;
}

// void NetLoginFailure(unsigned short Errno)
void __cdecl pkodev::hook::NetLoginFailure(unsigned short err)
{
    // Cancel autologin
    pkodev::autologin_enabled = false;

    // Show message box with error
    pkodev::pointer::CGameApp__MsgBox("%s", pkodev::pointer::g_GetServerError(err));
}