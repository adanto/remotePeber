#include "remotePeber.h"


int NtQueryInfoTest(HANDLE hProcess);
int getProcessesPEB();
void printPEBInfo(MY_PPEB pPEB, WCHAR* ImagePathNameBuffer, WCHAR* CommandLineBuffer);

int main()
{
	getProcessesPEB();
	return 0;
}


int getProcessesPEB() {

	HANDLE hSnapshot;
	PROCESSENTRY32 pe;
	BOOL hResult;

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) return 0;

	pe.dwSize = sizeof(PROCESSENTRY32);

	hResult = Process32First(hSnapshot, &pe);

	while (hResult) {
		if (HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pe.th32ProcessID))
		{
			printf("[*] PID %i: ", pe.th32ProcessID);

			NtQueryInfoTest(hProcess);

			CloseHandle(hProcess);
		}
		hResult = Process32Next(hSnapshot, &pe);
	}

	CloseHandle(hSnapshot);
}

int NtQueryInfoTest(HANDLE hProcess)
{


	// Locating functions
	HINSTANCE hNtDll = GetModuleHandleW(L"ntdll.dll");
	NtQueryInformationProcessPtr NtQueryInformationProcess = (NtQueryInformationProcessPtr)GetProcAddress(hNtDll, "NtQueryInformationProcess");
	RtlNtStatusToDosErrorPtr RtlNtStatusToDosError = (RtlNtStatusToDosErrorPtr)GetProcAddress(hNtDll, "RtlNtStatusToDosError");

	if (!NtQueryInformationProcess || !RtlNtStatusToDosError)
	{
		fprintf(stderr, "\n\t[!] Could not load module or functions from ntdll.dll\n");
		return -1;
	}

	// Get PROCESS_BASIC_INFORMATION
	PROCESS_BASIC_INFORMATION pbi;
	ULONG len;
	NTSTATUS status = NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), &len);
	SetLastError(RtlNtStatusToDosError(status));
	if (NT_ERROR(status) || !pbi.PebBaseAddress)
	{
		fprintf(stderr, "\n\t[!] API call NtQueryInformationProcess failed\n");
		return -1;
	}

	void* buffer[sizeof(PEB)];
	SIZE_T bytesRead = 0;

	if (!ReadProcessMemory(hProcess, pbi.PebBaseAddress, buffer, sizeof(PEB), &bytesRead)) {
		fprintf(stderr, "\n\t[!] Could not read PEB base\n");
		return -1;
	}

	MY_PPEB pPEB = reinterpret_cast<MY_PPEB>(buffer);
	PPEB pPEB2 = reinterpret_cast<PPEB>(buffer);

	offsetof()

	LPVOID bufferPParams[sizeof(RTL_USER_PROCESS_PARAMETERS)];
	if (!ReadProcessMemory(hProcess, (LPCVOID)(pPEB->ProcessParameters), bufferPParams, sizeof(RTL_USER_PROCESS_PARAMETERS), &bytesRead))
	{
		fprintf(stderr, "\n\t[!] Could not read memory from PROCESS PARAMETERS\n");
		return -1;
	};

	PRTL_USER_PROCESS_PARAMETERS pProcParams = reinterpret_cast<PRTL_USER_PROCESS_PARAMETERS>(bufferPParams);

	WCHAR CommandLineBuffer[MAX_PATH];
	CommandLineBuffer[(pProcParams->CommandLine.Length / sizeof(wchar_t)) + 1] = L'\0';

	WCHAR ImagePathNameBuffer[MAX_PATH];
	ImagePathNameBuffer[(pProcParams->ImagePathName.Length / sizeof(wchar_t)) + 1] = L'\0';

	if (!ReadProcessMemory(hProcess, pProcParams->CommandLine.Buffer, CommandLineBuffer, pProcParams->CommandLine.MaximumLength, &bytesRead))
	{
		fprintf(stderr, "\n\t[!] Could not read memory from pProcParams->CommandLine.Buffer\n");
		return -1;
	};

	if (!ReadProcessMemory(hProcess, pProcParams->ImagePathName.Buffer, ImagePathNameBuffer, pProcParams->ImagePathName.MaximumLength, &bytesRead))
	{
		fprintf(stderr, "\n\t[!] Could not read memory from pProcParams->ImagePathName.Buffer\n");
		return -1;
	};

	printPEBInfo(pPEB, ImagePathNameBuffer, CommandLineBuffer);

	return true;
	
}

void printPEBInfo(MY_PPEB pPEB, WCHAR* ImagePathNameBuffer, WCHAR* CommandLineBuffer)
{
	wprintf(L"\n%20s: %s", L"CommandLine", CommandLineBuffer);
	wprintf(L"\n%20s: %s", L"ImagePathName", ImagePathNameBuffer);
	printf("\n\n");
}