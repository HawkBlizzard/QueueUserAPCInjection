// main.cpp

#include "injector.h"

int wmain(int argc, wchar_t* argv[]) {

	// �����̐����`�F�b�N
	if (argc != 3) {
		std::wcout << "Usage: " << argv[0] << " [�v���Z�X��] [DLL�p�X]" << std::endl;
		return 1;
	}

	std::wstring processName = argv[1];
	std::wstring dllPath = argv[2];

	// DLL�p�X�T�C�Y���v�Z
	SIZE_T dllPathSize = (dllPath.size() + 1) * sizeof(wchar_t);

	// �v���Z�X������v���Z�XID���擾
	DWORD processId = GetProcessIdByProcessName(processName);

	// �v���Z�X�n���h�����擾
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

	// DLL�p�X���������ރ������̈���m��
	LPVOID dllPathMemory = VirtualAllocEx(processHandle, nullptr, dllPathSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	// DLL�p�X����������
	WriteProcessMemory(processHandle, dllPathMemory, dllPath.c_str(), dllPathSize, nullptr);

	// �X���b�h��APC���L���[
	InjectUsingQueueUserAPC(processId, LoadLibraryW, dllPathMemory);

	return 0;
}