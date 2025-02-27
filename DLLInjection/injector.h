// injector.h

#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <thread>

// ä÷êîêÈåæ
DWORD GetProcessIdByProcessName(std::wstring& processName);
bool InjectUsingQueueUserAPC(DWORD processId, LPVOID apcFunction, LPVOID apcParam);