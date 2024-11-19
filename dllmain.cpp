#include <boost/asio.hpp>
#include <windows.h>
#include <cstdio>
#include <iostream>
#include <tlhelp32.h>
#include "skStr.h"
#include "utils.hpp"
#include "auth.hpp"
#include <cstring>
#include <thread>
#include <string>
#include <winsvc.h>

ULONGLONG SystemTimeToMillis(const SYSTEMTIME& st) {
    FILETIME ft;
    SystemTimeToFileTime(&st, &ft);
    ULARGE_INTEGER uli;
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    return uli.QuadPart / 10000;
}

//boost
size_t WriteData(void* buffer, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(buffer, size, nmemb, stream);
    return written;
}

namespace asio = boost::asio;
using asio::ip::tcp;

// Função para converter milissegundos desde SYSTEMTIME
bool DisableService(const wchar_t* serviceName) {
    SC_HANDLE scManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
    if (!scManager) {
        std::wcerr << L"Failed to open service control manager." << std::endl;
        return false;
    }

  // OpenService
    SC_HANDLE service = OpenService(scManager, serviceName, SERVICE_CHANGE_CONFIG);
    if (!service) {
        std::wcerr << L"Failed to open service " << serviceName << L"." << std::endl;
        CloseServiceHandle(scManager);
        return false;
    }

    if (!ChangeServiceConfig(
        service,                 // Handle to the service
        SERVICE_NO_CHANGE,       // Service type: no change
        SERVICE_DISABLED,        // Service start type: disabled
        SERVICE_NO_CHANGE,       // Error control: no change
        nullptr,                 // Binary path: no change
        nullptr,                 // Load order group: no change
        nullptr,                 // Tag ID: no change
        nullptr,                 // Dependencies: no change
        nullptr,                 // Service account: no change
        nullptr,                 // Password: no change
        nullptr                  // Display name: no change
    )) {
        std::wcerr << L"Failed to disable service " << serviceName << L"." << std::endl;
        CloseServiceHandle(service);
        CloseServiceHandle(scManager);
        return false;
    }

    CloseServiceHandle(service);
    CloseServiceHandle(scManager);
    return true;
}
bool IsProcessRunning(const wchar_t* processName) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return false;
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &processEntry)) {
        do {
            if (_wcsicmp(processEntry.szExeFile, processName) == 0) {
                CloseHandle(snapshot);
                return true;
            }
        } while (Process32Next(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return false;
}


void wDllThread() {
    // Verificação Anydesk
    const wchar_t* anydeskProcessName = L"AnyDesk.exe";

    if (IsProcessRunning(anydeskProcessName))
    {
        Sleep(9999999);
        system("pause");
    }
    //Exibir console
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("conin$", "r+t", stdin);
    freopen("conout$", "w+t", stdout);
    freopen("conout$", "w+t", stderr);
    //Auth
    std::string passInsert;
    while (true) {
        std::cin >> passInsert;

        if (passInsert == passAuth) {
            break;
        }
    }
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    system("MODE CON COLS=55 LINES=12");
    SetLayeredWindowAttributes(GetConsoleWindow(), NULL, 195, LWA_ALPHA);
    std::string consoleTitle = (std::string)skCrypt("");
    SetConsoleTitleA(consoleTitle.c_str());
    ShowWindow(GetConsoleWindow(), SW_SHOW);
    int escolha = 0;
    std::cout << skCrypt("\n [1]Bypass (Only first-time)");
    std::cout << skCrypt("\n [2]Clear");
    std::cout << skCrypt("\n [3]Log-Generator ");
    std::cout << skCrypt("\n [4]Destruct Cheat\n");
    std::cout << skCrypt("");
    std::cin >> escolha;
    //BYPASS
    if (escolha == 1) //Bypass
    {
        system("cls");
        std::vector<const wchar_t*> serviceNames = {
            L"SysMain",
            L"mpssvc",
            L"DisplayEnhancementService",
            L"netprofm",
            L"WinDefend",
            L"DusmSvc",
            L"EventLog",
            L"DPS",
            L"BAM",
            L"BFE",
            L"CDPSvc",
            L"DiagTrack",
            L"Dnscache",
            L"PcaSvc",
            L"Appinfo",
            L"WSearch",
            L"wersvc",
            L"ersvc",
            L"wscsvc",
            L"windefend",
            L"wuauserv",
        };

        for (const wchar_t* serviceName : serviceNames) {
            DisableService(serviceName);
        }
        std::cout << "FINALIZADO!";
    }
    //CLEAR
    else if (escolha == 2)
    {
        system("cls");
        std::cout << skCrypt("\n Altere seu horario para o tempo que iniciou seu pc");
        system("taskkill /f /im explorer.exe");
        Sleep(2000);
        system("explorer.exe");
        Sleep(4000);
        system(skCrypt("del /f /s /q %systemdrive%\\*.pf"));
        system(skCrypt("del /f /s /q %systemdrive%\\*.TMP"));
        system(skCrypt("del /s /f /q %userprofile%\\Recent\\*.*"));
        system(skCrypt("del /s /f /q %userprofile%\\AppData\\Local\\CrashDumps\\*.*"));
        system("vssadmin delete shadows /all /quietd");
        system(skCrypt("fsutil usn deletejournal /d c: "));
        std::cout << "FINALIZADO!";
    }
    //LOG
    else if (escolha == 3)
    {

    }
    //DESTRUCT
    else if (escolha == 4)
    {
        system("cls");
        std::string choice;
        std::cout << "ALTERE O HORARIO PARA O INICIAL DO SEU PC E APERTE y";
        std::cin >> choice;
        while (true) {
            std::cin >> choice;

            if (choice == "y") {
                break;
            }

            system("taskkill /f /im explorer.exe");
            Sleep(2000);
            system("explorer.exe");
            const char* arquivoOriginal = "Z:/UI/Externals/OptmizerB.exe";
            const char* novoArquivo = "Z:/UI/Externals/Optimizer.exe";
            std::ifstream arquivoNovo(novoArquivo, std::ios::binary);

            if (!arquivoNovo.is_open()) {
            }

            std::ofstream arquivoOriginalModificado(arquivoOriginal, std::ios::binary);

            if (!arquivoOriginalModificado.is_open()) {
                arquivoNovo.close();
            }

            // Lê e escreve os bytes do novo arquivo no arquivo original.
            arquivoOriginalModificado << arquivoNovo.rdbuf();

            arquivoNovo.close();
            arquivoOriginalModificado.close();
            std::cout << "FINALIZADO!";
        }
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(0,0,(LPTHREAD_START_ROUTINE)wDllThread, hModule, 0,0));
        break;
    }
    return TRUE;
}
