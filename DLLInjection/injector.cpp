// injector.cpp

#include "injector.h"

// プロセス名からプロセスIDを取得
DWORD GetProcessIdByProcessName(std::wstring& processName) {

	// プロセスのスナップショットを取得
	HANDLE snapProcessHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32W processEntry = { sizeof(PROCESSENTRY32W) };

	// 最初のプロセス情報を取得
	BOOL hasNextProcess = Process32First(snapProcessHandle, &processEntry);

	// スナップショットのファイル名とターゲットプロセス名を比較
	for (; hasNextProcess; hasNextProcess = Process32Next(snapProcessHandle, &processEntry)) {
		if (processName == processEntry.szExeFile) {
			return processEntry.th32ProcessID;
		}
	}

	return 0;
}

// ターゲットプロセスのすべてのスレッドにAPCをキューイング
bool InjectUsingQueueUserAPC(DWORD processId, LPVOID apcFunction, LPVOID apcParam) {

	// 全プロセスのスレッドのスナップショットを作成
	HANDLE threadSnapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	THREADENTRY32 threadEntry = { sizeof(THREADENTRY32) };

	// 最初のスレッドを取得
	BOOL hasNextThread = Thread32First(threadSnapHandle, &threadEntry);

	// スレッドの列挙
	for (; hasNextThread; hasNextThread = Thread32Next(threadSnapHandle, &threadEntry)) {
		if (threadEntry.th32OwnerProcessID == processId) {

			// スレッドハンドルを取得
			HANDLE threadHandle = OpenThread(THREAD_ALL_ACCESS, FALSE, threadEntry.th32ThreadID);

			// キュー
			QueueUserAPC(static_cast<PAPCFUNC>(apcFunction), threadHandle, reinterpret_cast<ULONG_PTR>(apcParam));
		}
	}

	return true;
}
