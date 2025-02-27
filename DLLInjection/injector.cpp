// injector.cpp

#include "injector.h"

// �v���Z�X������v���Z�XID���擾
DWORD GetProcessIdByProcessName(std::wstring& processName) {

	// �v���Z�X�̃X�i�b�v�V���b�g���擾
	HANDLE snapProcessHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32W processEntry = { sizeof(PROCESSENTRY32W) };

	// �ŏ��̃v���Z�X�����擾
	BOOL hasNextProcess = Process32First(snapProcessHandle, &processEntry);

	// �X�i�b�v�V���b�g�̃t�@�C�����ƃ^�[�Q�b�g�v���Z�X�����r
	for (; hasNextProcess; hasNextProcess = Process32Next(snapProcessHandle, &processEntry)) {
		if (processName == processEntry.szExeFile) {
			return processEntry.th32ProcessID;
		}
	}

	return 0;
}

// �^�[�Q�b�g�v���Z�X�̂��ׂẴX���b�h��APC���L���[�C���O
bool InjectUsingQueueUserAPC(DWORD processId, LPVOID apcFunction, LPVOID apcParam) {

	// �S�v���Z�X�̃X���b�h�̃X�i�b�v�V���b�g���쐬
	HANDLE threadSnapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	THREADENTRY32 threadEntry = { sizeof(THREADENTRY32) };

	// �ŏ��̃X���b�h���擾
	BOOL hasNextThread = Thread32First(threadSnapHandle, &threadEntry);

	// �X���b�h�̗�
	for (; hasNextThread; hasNextThread = Thread32Next(threadSnapHandle, &threadEntry)) {
		if (threadEntry.th32OwnerProcessID == processId) {

			// �X���b�h�n���h�����擾
			HANDLE threadHandle = OpenThread(THREAD_ALL_ACCESS, FALSE, threadEntry.th32ThreadID);

			// �L���[
			QueueUserAPC(static_cast<PAPCFUNC>(apcFunction), threadHandle, reinterpret_cast<ULONG_PTR>(apcParam));
		}
	}

	return true;
}
