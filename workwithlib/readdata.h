#pragma once
#include <iostream>
#include <string>
#include <conio.h>
#include "Lusbapi.h"
#define CHANNELS_QUANTITY			(0x4)

// ��������� ����� �� ���������
void AbortProgram(char* ErrorString, bool AbortionFlag = true);
// ������� ������ ����� ������ � ���
DWORD WINAPI ServiceReadThread(PVOID /*Context*/);
// �������� ���������� ������������ ������� �� ���� ������
BOOL WaitingForRequestCompleted(OVERLAPPED* ReadOv);
// ������� ������ ��������� � ��������
void ShowThreadErrorMessage(void);
//�������� �������
void Reading(void);

// ����������� ��������� ���-�� ������������ 
// ����������� ������ (��� ������ ������)
const WORD MaxVirtualSoltsQuantity = 127;

// ������������� �����
HANDLE hFile;

// ������������� ������ ����� ������
HANDLE hReadThread;
DWORD ReadTid;

// ������ ����������
DWORD DllVersion;
// ��������� �� ��������� ������
ILE140* pModule;
// ���������� ����������
HANDLE ModuleHandle;
// �������� ������
char ModuleName[7];
// �������� ������ ���� USB
BYTE UsbSpeed;
// ��������� � ������ ����������� � ������
MODULE_DESCRIPTION_E140 ModuleDescription;
// ��������� ���������� ������ ��� ������
ADC_PARS_E140 ap;

// ���-�� ���������� �������� (������� 32) ��� �. ReadData()
DWORD DataStep = 64 * 1024;
// ����� �������� NDataBlock ������ �� DataStep �������� � ������
const WORD NDataBlock = 80;
// ������� ������ ��� � ���
const double AdcRate = 5000.0;
// ����� ������
SHORT* ReadBuffer;

// ������ ���������� ������ ������ ����� ������
bool IsReadThreadComplete;
// ����� ������ ��� ���������� ����� ������
WORD ReadThreadErrorNumber;

// �������� �������-���������
DWORD Counter = 0x0, OldCounter = 0xFFFFFFFF;
