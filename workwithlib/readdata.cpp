//******************************************************************************
// ������ E14-140.
// ���������� ��������� � ������������ ���������� ����� ������ � ���
// � ������������� ������� ���������� ������ �� ���� � �������� �������� �������.
// ���� �������������� � ������ ������ ������� ��� �� ������� 100 ���.
//******************************************************************************
// 
#include "readdata.h"

//------------------------------------------------------------------------
// �������� ���������
//------------------------------------------------------------------------
void Reading(void)
{
	WORD i;
	//	WORD DacSample;

		// ������� ������ ���������� ������ ����� ������
	IsReadThreadComplete = false;
	// ���� ������ �� �������� ��� ����� ������
	ReadBuffer = NULL;
	// ���� �� ������ ����� ����� ������
	hReadThread = NULL;
	// ���� �������� ����� ��� :(
	hFile = INVALID_HANDLE_VALUE;
	// ������� ���� ������ ������ ����� ������
	ReadThreadErrorNumber = 0x0;

	// �������� ����� ��������	
	system("cls");

	std:: cout<<(" *******************************\n");
	std:: cout<<(" Module E14-140                 \n");
	std:: cout<<(" Console example for ADC Stream \n");
	std:: cout<<(" *******************************\n\n");

	// �������� ������ ������������ ���������� Lusbapi.dll
	if ((DllVersion = GetDllVersion()) != CURRENT_VERSION_LUSBAPI)
	{
		char String[128];
		std:: cout<<(String, " Lusbapi.dll Version Error!!!\n   Current: %1u.%1u. Required: %1u.%1u",
			DllVersion >> 0x10, DllVersion & 0xFFFF,
			CURRENT_VERSION_LUSBAPI >> 0x10, CURRENT_VERSION_LUSBAPI & 0xFFFF);

		AbortProgram(String);
	}
	else std:: cout<<(" Lusbapi.dll Version --> OK\n");

	// ��������� �������� ��������� �� ���������
	pModule = static_cast<ILE140*>(CreateLInstance("e140"));
	if (!pModule) AbortProgram(" Module Interface --> Bad\n");
	else std:: cout<<(" Module Interface --> OK\n");

	// ��������� ���������� ������ E14-140 � ������ 256 ����������� ������
	for (i = 0x0; i < MaxVirtualSoltsQuantity; i++) if (pModule->OpenLDevice(i)) break;
	// ���-������ ����������?
	if (i == MaxVirtualSoltsQuantity) AbortProgram(" Can't find any module E14-140 in first 127 virtual slots!\n");
	else std:: cout<<(" OpenLDevice(%u) --> OK\n", i);

	// ��������� ��������� ���������� ����������
	ModuleHandle = pModule->GetModuleHandle();
	if (ModuleHandle == INVALID_HANDLE_VALUE) AbortProgram(" GetModuleHandle() --> Bad\n");
	else std:: cout<<(" GetModuleHandle() --> OK\n");

	// ��������� �������� ������ � ������������ ����������� �����
	if (!pModule->GetModuleName(ModuleName)) AbortProgram(" GetModuleName() --> Bad\n");
	else std:: cout<<(" GetModuleName() --> OK\n");
	// ��������, ��� ��� 'E14-140'
	if (strcmp(ModuleName, "E140")) AbortProgram(" The module is not 'E14-140'\n");
	else std:: cout<<(" The module is 'E14-140'\n");

	// ��������� �������� �������� ������ ���� USB
	if (!pModule->GetUsbSpeed(&UsbSpeed)) AbortProgram(" GetUsbSpeed() --> Bad\n");
	else std:: cout<<(" GetUsbSpeed() --> OK\n");
	// ������ ��������� �������� ������ ���� USB
	std:: cout<<("   USB is in %s\n", UsbSpeed ? "High-Speed Mode (480 Mbit/s)" : "Full-Speed Mode (12 Mbit/s)");

	// ������� ���������� �� ���� ������
	if (!pModule->GET_MODULE_DESCRIPTION(&ModuleDescription)) AbortProgram(" GET_MODULE_DESCRIPTION() --> Bad\n");
	else std:: cout<<(" GET_MODULE_DESCRIPTION() --> OK\n");

	// �������� �������� MCU ������
	if ((ModuleDescription.Module.Revision == REVISIONS_E140[REVISION_B_E140]) &&
		(strtod((char*)ModuleDescription.Mcu.Version.Version, NULL) < 3.05)) AbortProgram(" For module E14-140(Rev.'B') firmware version must be 3.05 or above --> !!! ERROR !!!\n");

	// ������� ������� ��������� ������ ���
	if (!pModule->GET_ADC_PARS(&ap)) AbortProgram(" GET_ADC_PARS() --> Bad\n");
	else std:: cout<<(" GET_ADC_PARS() --> OK\n");
	// ��������� �������� ��������� ������ ���
	ap.ClkSource = INT_ADC_CLOCK_E140;							// ���������� ������ ���
	ap.EnableClkOutput = ADC_CLOCK_TRANS_DISABLED_E140; 	// ��� ���������� �������� �������� ���
	ap.InputMode = NO_SYNC_E140;									// ��� ������������� ����� ������
	ap.ChannelsQuantity = CHANNELS_QUANTITY; 					// ���-�� �������� �������
	// ��������� ����������� ������� 
	for (i = 0x0; i < ap.ChannelsQuantity; i++) ap.ControlTable[i] = (WORD)(i | (ADC_INPUT_RANGE_2500mV_E140 << 0x6));
	ap.AdcRate = AdcRate;								// ������� ������ ��� � ���
	ap.InterKadrDelay = 0.0;							// ����������� �������� � ��
	// ��������� ��������� ��������� ������ ��� � ������
	if (!pModule->SET_ADC_PARS(&ap)) AbortProgram(" SET_ADC_PARS() --> Bad\n");
	else std:: cout<<(" SET_ADC_PARS() --> OK\n");

	// ������� ������ ��� �����
	ReadBuffer = new SHORT[2 * DataStep];
	if (!ReadBuffer) AbortProgram(" Can not allocate memory\n");

	// ������� ���� ��� ������ ���������� � ������ ������
	hFile = CreateFile(L"Test.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_WRITE_THROUGH, NULL);
	if (hFile == INVALID_HANDLE_VALUE) AbortProgram("\n Can't create file 'Test.dat'!\n");

	// ������ � ��������� ����� ����� ������
	std:: cout<<(" \n");
	hReadThread = CreateThread(0, 0x2000, ServiceReadThread, 0, 0, &ReadTid);
	if (!hReadThread) AbortProgram(" ServiceReadThread() --> Bad\n");
	else std:: cout<<(" ServiceReadThread() --> OK\n");

	// ��������� ��������� ����� ������ ������ �� ������ ��������
	std:: cout<<(" \n");
	std:: cout<<(" Module E14-140 (S/N %s) is ready ... \n", ModuleDescription.Module.SerialNumber);
	std:: cout<<("   Module Info:\n");
	std:: cout<<("     Module  Revision   is '%c'\n", ModuleDescription.Module.Revision);
	std:: cout<<("     MCU Driver Version is %s (%s)\n", ModuleDescription.Mcu.Version.Version, ModuleDescription.Mcu.Version.Date);
	std:: cout<<("   Adc parameters:\n");
	std:: cout<<("     ChannelsQuantity = %2d\n", ap.ChannelsQuantity);
	std:: cout<<("     AdcRate = %8.3f kHz\n", ap.AdcRate);
	std:: cout<<("     InterKadrDelay = %2.4f ms\n", ap.InterKadrDelay);
	std:: cout<<("     KadrRate = %8.3f kHz\n", ap.KadrRate);

	// ���� ������ ���������� ������ � �������� ��������� ������ ����������
	std:: cout<<("\n Press any key if you want to terminate this program...\n\n");
	while (!IsReadThreadComplete)
	{
		if (OldCounter != Counter) { std:: cout<<(" Counter %3u from %3u\r", Counter, NDataBlock); OldCounter = Counter; }
		else Sleep(20);
	}

	// ��� ��������� ������ ������ ����� ������
	WaitForSingleObject(hReadThread, INFINITE);

	// �������� ���� �� ������ ���������� ������ ����� ������
	std:: cout<<("\n\n");
	if (ReadThreadErrorNumber) { AbortProgram(NULL, false); ShowThreadErrorMessage(); }
	else AbortProgram(" The program was completed successfully!!!\n", false);
}

//------------------------------------------------------------------------
// �����, � ������� �������������� ���� ������
//------------------------------------------------------------------------
DWORD WINAPI ServiceReadThread(PVOID /*Context*/)
{
	WORD i;
	WORD RequestNumber;
	DWORD FileBytesWritten;
	// ������ OVERLAPPED �������� �� ���� ���������
	OVERLAPPED ReadOv[2];
	// ������ �������� � ����������� ������� �� ����/����� ������
	IO_REQUEST_LUSBAPI IoReq[2];

	// ��������� ������ ��� � ������������ ������� USB-����� ������ ������
	if (!pModule->STOP_ADC()) { ReadThreadErrorNumber = 0x1; IsReadThreadComplete = true; return 0x0; }

	// ��������� ����������� ��� ����� ������ ���������
	for (i = 0x0; i < 0x2; i++)
	{
		// ������������� ��������� ���� OVERLAPPED
		ZeroMemory(&ReadOv[i], sizeof(OVERLAPPED));
		// ������ ������� ��� ������������ �������
		ReadOv[i].hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		// ��������� ��������� IoReq
		IoReq[i].Buffer = ReadBuffer + i * DataStep;
		IoReq[i].NumberOfWordsToPass = DataStep;
		IoReq[i].NumberOfWordsPassed = 0x0;
		IoReq[i].Overlapped = &ReadOv[i];
		IoReq[i].TimeOut = (DWORD)(DataStep / ap.AdcRate + 1000);
	}

	// ������ ��������������� ������ �� ���� ������
	RequestNumber = 0x0;
	if (!pModule->ReadData(&IoReq[RequestNumber])) { CloseHandle(ReadOv[0].hEvent); CloseHandle(ReadOv[1].hEvent); ReadThreadErrorNumber = 0x2; IsReadThreadComplete = true; return 0x0; }

	// �������� ���
	if (pModule->START_ADC())
	{
		// ���� ����� ������
		for (i = 0x1; i < NDataBlock; i++)
		{
			// ������� ������ �� ��������� ������ ������
			RequestNumber ^= 0x1;
			if (!pModule->ReadData(&IoReq[RequestNumber])) { ReadThreadErrorNumber = 0x2; break; }
			if (ReadThreadErrorNumber) break;

			// ��� ���������� �������� ����� ���������� ������ ������
			if (!WaitingForRequestCompleted(IoReq[RequestNumber ^ 0x1].Overlapped)) break;
			//			if(WaitForSingleObject(ReadOv[RequestNumber^0x1].hEvent, IoReq[RequestNumber^0x1].TimeOut) == WAIT_TIMEOUT) { ReadThreadErrorNumber = 0x3; break; }
			if (ReadThreadErrorNumber) break;

			// ������� ���������� ������ ������ � ����
			if (!WriteFile(hFile,													// handle to file to write to
				IoReq[RequestNumber ^ 0x1].Buffer,					// pointer to data to write to file
				2 * DataStep,	 											// number of bytes to write
				&FileBytesWritten,									// pointer to number of bytes written
				NULL			  											// pointer to structure needed for overlapped I/O
			)) {
				ReadThreadErrorNumber = 0x4; break;
			}

			if (ReadThreadErrorNumber) break;
			else if (kbhit()) { ReadThreadErrorNumber = 0x5; break; }
			else Sleep(20);
			Counter++;
		}

		// ��������� ������ ������
		if (!ReadThreadErrorNumber)
		{
			RequestNumber ^= 0x1;
			// ��� ��������� �������� ����� ��������� ������ ������
			if (!WaitingForRequestCompleted(IoReq[RequestNumber ^ 0x1].Overlapped)) ReadThreadErrorNumber = 0x3;
			//			if(WaitForSingleObject(ReadOv[RequestNumber^0x1].hEvent, IoReq[RequestNumber^0x1].TimeOut) == WAIT_TIMEOUT) ReadThreadErrorNumber = 0x3;
						// ������� ��������� ������ ������ � ����
			if (!WriteFile(hFile,													// handle to file to write to
				IoReq[RequestNumber ^ 0x1].Buffer,					// pointer to data to write to file
				2 * DataStep,	 											// number of bytes to write
				&FileBytesWritten,									// pointer to number of bytes written
				NULL			  											// pointer to structure needed for overlapped I/O
			)) ReadThreadErrorNumber = 0x4;
			Counter++;
		}
	}
	else { ReadThreadErrorNumber = 0x6; }

	// ��������� ������ ���
	if (!pModule->STOP_ADC()) ReadThreadErrorNumber = 0x1;
	// ������ �������� ������������� ����������� ������ �� ���� ������
	if (!CancelIo(ModuleHandle)) { ReadThreadErrorNumber = 0x7; }
	// ��������� ��� �������������� �������
	for (i = 0x0; i < 0x2; i++) CloseHandle(ReadOv[i].hEvent);
	// ��������� ��������
	Sleep(100);
	// ��������� ������ ���������� ������ ������ ����� ������
	IsReadThreadComplete = true;
	// ������ ����� �������� �������� �� ������
	return 0;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
BOOL WaitingForRequestCompleted(OVERLAPPED* ReadOv)
{
	DWORD ReadBytesTransferred;

	while (TRUE)
	{
		if (GetOverlappedResult(ModuleHandle, ReadOv, &ReadBytesTransferred, FALSE)) break;
		else if (GetLastError() != ERROR_IO_INCOMPLETE) { ReadThreadErrorNumber = 0x3; return FALSE; }
		else if (kbhit()) { ReadThreadErrorNumber = 0x5; return FALSE; }
		else Sleep(20);
	}
	return TRUE;
}

//------------------------------------------------------------------------
// ��������� ��������� � �������
//------------------------------------------------------------------------
void ShowThreadErrorMessage(void)
{
	switch (ReadThreadErrorNumber)
	{
	case 0x1:
		std:: cout<<(" ADC Thread: STOP_ADC() --> Bad\n");
		break;

	case 0x2:
		std:: cout<<(" ADC Thread: ReadData() --> Bad\n");
		break;

	case 0x3:
		std:: cout<<(" ADC Thread: Timeout is occured!\n");
		break;

	case 0x4:
		std:: cout<<(" ADC Thread: Writing data file error!\n");
		break;

	case 0x5:
		// ���� ��������� ���� ������ ��������, ��������� ���� ��������
		std:: cout<<(" ADC Thread: The program was terminated!\n");
		break;

	case 0x6:
		std:: cout<<(" ADC Thread: START_ADC() --> Bad\n");
		break;

	case 0x7:
		std:: cout<<(" ADC Thread: Can't cancel ending input and output (I/O) operations!\n");
		break;

	default:
		std:: cout<<(" Unknown error!\n");
		break;
	}

	return;
}

//------------------------------------------------------------------------
// ��������� ���������� ���������
//------------------------------------------------------------------------
void AbortProgram(std::string ErrorString, bool AbortionFlag = true)
{
	// ��������� ��������� ������
	if (pModule)
	{
		// ��������� ��������� ������
		if (!pModule->ReleaseLInstance()) std:: cout<<(" ReleaseLInstance() --> Bad\n");
		else std:: cout<<(" ReleaseLInstance() --> OK\n");
		// ������� ��������� �� ��������� ������
		pModule = NULL;
	}

	// ��������� ������ ������
	if (ReadBuffer) { delete[] ReadBuffer; ReadBuffer = NULL; }
	// ��������� ������������� ������ ����� ������
	if (hReadThread) { CloseHandle(hReadThread); hReadThread = NULL; }
	// ��������� ������������� ����� ������
	if (hFile != INVALID_HANDLE_VALUE) { CloseHandle(hFile); hFile = INVALID_HANDLE_VALUE; }

	// ������� ����� ���������
	if (!ErrorString.empty()) std:: cout<<(ErrorString);

	// ��������� ������� ����������
	if (kbhit()) { while (kbhit()) getch(); }

	// ���� ����� - �������� ��������� ���������
	if (AbortionFlag) exit(-1);
	// ��� �������� ������� �� �������   
	else return;
}

