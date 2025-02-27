// main.cpp

#include "injector.h"

int wmain(int argc, wchar_t* argv[]) {

	// 引数の数をチェック
	if (argc != 3) {
		std::wcout << "Usage: " << argv[0] << " [プロセス名] [DLLパス]" << std::endl;
		return 1;
	}

	std::wstring processName = argv[1];
	std::wstring dllPath = argv[2];

	// DLLパスサイズを計算
	SIZE_T dllPathSize = (dllPath.size() + 1) * sizeof(wchar_t);

	// プロセス名からプロセスIDを取得
	DWORD processId = GetProcessIdByProcessName(processName);

	// プロセスハンドルを取得
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

	// DLLパスを書き込むメモリ領域を確保
	LPVOID dllPathMemory = VirtualAllocEx(processHandle, nullptr, dllPathSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	// DLLパスを書き込む
	WriteProcessMemory(processHandle, dllPathMemory, dllPath.c_str(), dllPathSize, nullptr);

	// スレッドにAPCをキュー
	InjectUsingQueueUserAPC(processId, LoadLibraryW, dllPathMemory);

	return 0;
}