#include <Windows.h>
#include<stdio.h>
#include<locale.h>
#include<string.h>
#include<stdlib.h>
#include<tchar.h>
#include<windows.h>
#include<tlhelp32.h>
#include<conio.h>

void ListProcessInfo(void);
void KillProcess(TCHAR* TargetProcess);

BOOL flag;

int main() {

	//TCHAR TargetProcess[BUFSIZ];
	//while (1) {
		//ListProcessInfo();
		//_tprintf(_T("Input TargetProcess name (End :: EXIT) :: "));
		//_tscanf_s(_T("%s"), TargetProcess);
		//_tprintf(_T("input :: %s"), TargetProcess);
	//}

	flag = FALSE;
	ListProcessInfo();
	if (flag == TRUE) {
		wchar_t target[BUFSIZ];// = "sdwExeDetector.exe";
		wcscpy_s(target, L"sdwExeDetector.exe");
		KillProcess(target);
		wcscpy_s(target, L"notepad.exe");
		KillProcess(target);
	}else{
		_tprintf(_T("not opening"));
	}
	//}
	while (_getch() != 'q');
	return 0;
}

void ListProcessInfo(void) {

	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		_tprintf(_T("CreateToolhelp32Snapshot error \n"));
		exit(EXIT_FAILURE);
	}

	PROCESSENTRY32W pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32W);

	if (!Process32First(hProcessSnap, &pe32)) {
		_tprintf(_T("Process32First error ! \n"));
		CloseHandle(hProcessSnap);
		return;
	}
	_tprintf(_T("\t[Process name] \t\t[PID]\t[PPID]\t[ThreadID] \n"));
	
	do {
		_tprintf(_T("%25s %8d %8d %8d \n"),	pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID, pe32.cntThreads);
		if (_tcscmp(pe32.szExeFile, L"mspaint.exe") == 0) {
			flag = TRUE;
		}
	} while (Process32Next(hProcessSnap, &pe32));
	

	return;
}

void KillProcess(TCHAR* TargetProcess) {
	
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		_tprintf(_T("CreateToolhelp32Snapshot error! \n"));
		return;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32)) {
		_tprintf(_T("Process32First error ! \n"));
		CloseHandle(hProcessSnap);
		return;
	}

	HANDLE hProcess = NULL;
	BOOL isKill = FALSE;

	do { 
		if (_tcscmp(pe32.szExeFile, TargetProcess) == 0)
			hProcess = OpenProcess(	PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);

		if (hProcess != NULL) {
			TerminateProcess(hProcess, -1);

			isKill = TRUE;
			CloseHandle(hProcess);
			break;
		}

	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	if (isKill == FALSE)
		_tprintf(_T("Kill process fail. Try again! \n"));
}