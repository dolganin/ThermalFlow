#pragma once
#include <iostream>
#include <string>
#include <conio.h>
#include "Lusbapi.h"
#define CHANNELS_QUANTITY			(0x4)

// аварийный выход из программы
void AbortProgram(char* ErrorString, bool AbortionFlag = true);
// функция потока ввода данных с АЦП
DWORD WINAPI ServiceReadThread(PVOID /*Context*/);
// ожидание завершения асинхронного запроса на ввод данных
BOOL WaitingForRequestCompleted(OVERLAPPED* ReadOv);
// функция вывода сообщений с ошибками
void ShowThreadErrorMessage(void);
//основная функция
void Reading(void);

// максимально возможное кол-во опрашиваемых 
// виртуальных слотов (при поиске модуля)
const WORD MaxVirtualSoltsQuantity = 127;

// идентификатор файла
HANDLE hFile;

// идентификатор потока сбора данных
HANDLE hReadThread;
DWORD ReadTid;

// версия библиотеки
DWORD DllVersion;
// указатель на интерфейс модуля
ILE140* pModule;
// дескриптор устройства
HANDLE ModuleHandle;
// название модуля
char ModuleName[7];
// скорость работы шины USB
BYTE UsbSpeed;
// структура с полной информацией о модуле
MODULE_DESCRIPTION_E140 ModuleDescription;
// структура параметров работы АЦП модуля
ADC_PARS_E140 ap;

// кол-во получаемых отсчетов (кратное 32) для Ф. ReadData()
DWORD DataStep = 64 * 1024;
// будем собирать NDataBlock блоков по DataStep отсчётов в каждом
const WORD NDataBlock = 80;
// частота работы АЦП в кГц
const double AdcRate = 5000.0;
// буфер данных
SHORT* ReadBuffer;

// флажок завершения работы потока сбора данных
bool IsReadThreadComplete;
// номер ошибки при выполнении сбора данных
WORD ReadThreadErrorNumber;

// экранный счетчик-индикатор
DWORD Counter = 0x0, OldCounter = 0xFFFFFFFF;
