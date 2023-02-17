#include <windows.h>
#include "lusbapi.h"
#include "wlusbapi.h"
#pragma argsused

static ILE140 *pModule;
// ���������� ����������
static HANDLE ModuleHandle;
// �������� ������
static char ModuleName[7];
// �������� ������ ���� USB
static BYTE UsbSpeed;
// ��������� � ������ ����������� � ������
static MODULE_DESCRIPTION_E140 E140DescriptionStruct;
// ��������� ���������� ������ ��� ������
static ADC_PARS_E140 ap;

DWORD WINAPI ServiceRealTImeReadThread(PVOID /*Context*/);

//BOOL DescriptorReadFlag;
//MODULE_DESCRIPTION_E140 E140DescriptionStruct;

BOOL ProcessOnePoint(SHORT source, double *dest, DWORD channel, BOOL calibr, BOOL volt)
{
	double K_offset, K_scale;
	int gain_index;
	MODULE_DESCRIPTION_E140 md;
	double ScaleGain[4]={10.0/8000., 2.5/8000., 0.625/8000., 0.15625/8000.};

   // ���������� ������������
	gain_index=(channel >> 6) & 3;
	if(calibr == FALSE)
	{
		// ��� ����������
		if(volt == FALSE)
         	{
            // ���� ���, ��� ����������
            K_offset=0;
            K_scale=1.0;
            }
           else
           	{
            // ������, ��� ����������
            K_offset=0;
            K_scale=ScaleGain[gain_index];
            }
         }
        else
      	{
         // � �����������
         if(volt == FALSE)
         	{
            // ���� ���, � �����������
            K_offset=E140DescriptionStruct.Adc.OffsetCalibration[gain_index];
            K_scale=E140DescriptionStruct.Adc.ScaleCalibration[gain_index];
            }
           else
           	{
            // ������, � �����������
            K_offset=E140DescriptionStruct.Adc.OffsetCalibration[gain_index];
            K_scale=E140DescriptionStruct.Adc.ScaleCalibration[gain_index]*ScaleGain[gain_index];
            }
         }
    //  }

   *dest=(source+K_offset)*K_scale;

	// ��� ������ :)))))
	return TRUE;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
    return 1;
}
//------------------------------------------------------------------------
// ���������� ������� ������ ����������
//------------------------------------------------------------------------
__declspec(dllexport) DWORD WINAPI WE140GetDllVersion(VOID)
{
        return GetDllVersion();
}
//------------------------------------------------------------------------
// ���������� ������� ������ ����������
//------------------------------------------------------------------------
void BadExit(void);

__declspec(dllexport) DWORD WINAPI LV_OpenE140(VOID)
{
int i;

	BadExit();

	// ��������� �������� ��������� �� ���������
	pModule = static_cast<ILE140 *>(CreateLInstance("e140"));
	if(!pModule) { BadExit(); return 0; }

	// ��������� ���������� ������ E14-1140 � ������ WORD MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI ����������� ������
	for(i = 0x0; i < MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI; i++) if(pModule->OpenLDevice(i)) break;
	// ���-������ ����������?
	if(i == MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI) { BadExit(); return 0; }

	// ��������� ��������� ���������� ����������
	ModuleHandle = pModule->GetModuleHandle();
	if(ModuleHandle == INVALID_HANDLE_VALUE) { BadExit(); return 0; }

	// ��������� �������� ������ � ������������ ����������� �����
	if(!pModule->GetModuleName(ModuleName)) { BadExit(); return 0; }

	// ��������, ��� ��� 'E14-140'
	if(strcmp(ModuleName, "E140")) { BadExit(); return 0; }

	// ��������� �������� �������� ������ ���� USB
	if(!pModule->GetUsbSpeed(&UsbSpeed)) { BadExit(); return 0; }

	// ������� ���������� �� ���� ������
	if(!pModule->GET_MODULE_DESCRIPTION(&E140DescriptionStruct)) { BadExit(); return 0; }

   return 1;
}

__declspec(dllexport) DWORD WINAPI LV_CloseE140(VOID)
{
BadExit();
return 0;
}

void BadExit(void)
{
	// ��������� ��������� ������
	if(pModule)
	{
		// ��������� ��������� ������
		pModule->ReleaseLInstance();

		// ������� ��������� �� ��������� ������
		pModule = NULL;
	}
}
// -----------------------------------------------------------------------------
// ---------------------------------- E140 -------------------------------------
// -----------------------------------------------------------------------------
//------------------------------------------------------------------------
// �������� ���������� ������������ ������
//------------------------------------------------------------------------
__declspec(dllexport) LPVOID WINAPI WE140CreateInstance(VOID)
{
    return CreateLInstance("E140");
}
//------------------------------------------------------------------------
//  ������������ ���������� ����������
//------------------------------------------------------------------------
__declspec(dllexport) HANDLE WINAPI WE140GetModuleHandle(LPVOID lptr)
{
    return ((ILE140*)lptr)->GetModuleHandle();
}
//------------------------------------------------------------------------
//  ������� ����������� ���� ��� ������� � USB ������
//------------------------------------------------------------------------
__declspec(dllexport) BOOL WINAPI WE140OpenLDevice(LPVOID lptr, WORD VirtualSlot)
{
    return (BOOL)((ILE140*)lptr)->OpenLDevice(VirtualSlot);
}
//------------------------------------------------------------------------
//  ��������� ������� ����������� ����
//------------------------------------------------------------------------
__declspec(dllexport) BOOL WINAPI WE140CloseLDevice(LPVOID lptr)
{
    return (BOOL)((ILE140*)lptr)->CloseLDevice();
}
// -----------------------------------------------------------------------
//  ��������� ��������� �� ����������
// -----------------------------------------------------------------------
__declspec(dllexport) BOOL WINAPI WE140ReleaseLDevice(LPVOID lptr)
{
    return (BOOL)((ILE140*)lptr)->ReleaseLInstance();
}
// -----------------------------------------------------------------------
//  ������ �������� ������
// -----------------------------------------------------------------------
__declspec(dllexport) BOOL WINAPI WE140GetModuleName(LPVOID lptr, BYTE *ModuleName)
{
    return (BOOL)((ILE140*)lptr)->GetModuleName(ModuleName);
}
// -----------------------------------------------------------------------
// ��������� ������� ���������� ������ ���
// -----------------------------------------------------------------------
__declspec(dllexport) BOOL WINAPI WE140GET_CUR_ADC_PARS(LPVOID lptr, ADC_PARS_E140 *wam)
{
 if(((ILE140*)lptr)->GET_ADC_PARS(wam)) {
    return TRUE;
  } else return FALSE;
}
// -----------------------------------------------------------------------
//  ���������� ��������� ���������� ������ ���
// -----------------------------------------------------------------------
__declspec(dllexport) BOOL WINAPI WE140FILL_ADC_PARS(LPVOID lptr, ADC_PARS_E140 *wam)
{
 if(((ILE140*)lptr)->SET_ADC_PARS(wam)) {
    return TRUE;
  } else return FALSE;
}
// -----------------------------------------------------------------------
//  ������ ������ ���
// -----------------------------------------------------------------------
__declspec(dllexport) BOOL WINAPI WE140START_ADC(LPVOID lptr)
{
    return (BOOL)((ILE140*)lptr)->START_ADC();
}
// -----------------------------------------------------------------------
//  ������� ���
// -----------------------------------------------------------------------
__declspec(dllexport) BOOL WINAPI WE140STOP_ADC(LPVOID lptr)
{
    return (BOOL)((ILE140*)lptr)->STOP_ADC();
}
// -----------------------------------------------------------------------
//  ���� ����� �������� � ��� ������
// -----------------------------------------------------------------------
__declspec(dllexport) BOOL WINAPI WE140ADC_KADR(LPVOID lptr, SHORT *Data)
{
    return (BOOL)((ILE140*)lptr)->ADC_KADR(Data);
}
// -----------------------------------------------------------------------
//  ����������� ���� � ��������� ����������� ������ ��� ������
// -----------------------------------------------------------------------
static int test;
__declspec(dllexport) double WINAPI LV_SAMPLE_E140(DWORD Channel, DWORD UseCalibr, DWORD *error)
{
double f=0.0;
SHORT adc;
BOOL result;

result=pModule->ADC_SAMPLE(&adc, Channel);
*error=(result == TRUE) ? 1 : 0;
if(result == FALSE) return f;

result=ProcessOnePoint(adc, &f, Channel, UseCalibr, TRUE);
*error=(result == TRUE) ? 1 : 0;

return f;
}
// -----------------------------------------------------------------------
//  ��������� ������ ������ � ��� ������
// -----------------------------------------------------------------------
__declspec(dllexport) BOOL WINAPI WE140ReadData(LPVOID lptr, IO_REQUEST_LUSBAPI *ReadRequest)
{

    return (BOOL)((ILE140*)lptr)->ReadData(ReadRequest);
}
// -----------------------------------------------------------------------
//  ����������� ����� �� �������� ����� ���
// -----------------------------------------------------------------------
__declspec(dllexport) BOOL WINAPI LV_OUTDA_E140(double value, int dac_number)
{
short val;

    val=value*2048./5.00;
    return (BOOL)pModule->DAC_SAMPLE(&val, dac_number);
}
// -----------------------------------------------------------------------
//  ������� ���������� �������� ����� �������� ��������� �������
// -----------------------------------------------------------------------
static int TtlOutEnabled;
__declspec(dllexport) BOOL WINAPI WE140ENABLE_TTL_OUT(LPVOID lptr, BOOL flag)
{
    return (BOOL)((ILE140*)lptr)->ENABLE_TTL_OUT(flag ? true : false);
}
// -----------------------------------------------------------------------
//   ������� ������ ������� ����� �������� ��������� �������
// -----------------------------------------------------------------------
__declspec(dllexport) DWORD WINAPI LV_GETTTL_E140(DWORD *error)
{
BOOL result;
unsigned short ttl;

result=pModule->TTL_IN(&ttl);
if(result == FALSE)
	{
   *error=0;
   return 0;
   }

*error=1;
return (DWORD) ttl;
}
// -----------------------------------------------------------------------
//  ������� ������ �� �������� ����� �������� ��������� �������
// -----------------------------------------------------------------------
__declspec(dllexport) BOOL WINAPI LV_SETTTL_E140(DWORD TtlOut)
{
BOOL result;

if(!TtlOutEnabled)
	{
   if(!pModule->ENABLE_TTL_OUT(true)) { return 0; }
   TtlOutEnabled=1;
   }

return (BOOL) pModule->TTL_OUT(TtlOut);
}
// -----------------------------------------------------------------------
//  ����������/���������� ������ ������ � ���� ������
// -----------------------------------------------------------------------
__declspec(dllexport) BOOL WINAPI WE140ENABLE_FLASH_WRITE(LPVOID lptr, BOOL EnableFlashWrite)
{
    return (BOOL)((ILE140*)lptr)->ENABLE_FLASH_WRITE(EnableFlashWrite ? true : false);
}

// -----------------------------------------------------------------------
//  ������� ��������� ���������� � ������ �� ����
// -----------------------------------------------------------------------
__declspec(dllexport) BOOL WINAPI WE140GET_MODULE_DESCR(LPVOID lptr, MODULE_DESCRIPTION_E140 *wmd)
{
 if(((ILE140*)lptr)->GET_MODULE_DESCRIPTION(wmd)) {
    return TRUE;
 } else return FALSE;
}
// -----------------------------------------------------------------------
//  ������� ��������� ���������� � ������ � ����
// -----------------------------------------------------------------------
__declspec(dllexport) BOOL WINAPI WE140SAVE_MODULE_DESCR(LPVOID lptr, MODULE_DESCRIPTION_E140 *wmd)
{
 return (BOOL)((ILE140*)lptr)->SAVE_MODULE_DESCRIPTION(wmd);
}

//------------------------------------------------------------------------
// ���������� ������� ������ ����������
//------------------------------------------------------------------------
__declspec(dllexport) DWORD WINAPI LV_CreateChannelE140(DWORD channel, DWORD gain, DWORD diff_mode)
{
return (channel & 0x1F) | ((gain & 3) << 6) | ((diff_mode & 1) << 5);
}


// -----------------------------------------------------------------------
//  ��������� ���������� ����������� ����� � ����
// -----------------------------------------------------------------------
// Nch - ����� �������� ������� (�� 1 �� 128)
// Channels - ������ � �������� �������
// Rate ������� ������ ������� ��� � ��
// AdcDelay - ����� �������������� ��� (������������ ��������), � ��
// InputMode, SynchroAdType, SynchroAdMode, SynchroAdChanne ��������� ������������� (�������� ��������)
// SynchroAdPorog ����� ������������ � �������
// SetRate ������� �������������� ������� ������ ������� ��� � ��
// SetAdcDelay ������� ������������� ������������ �������� (��)
__declspec(dllexport) BOOL WINAPI LV_SET_ADC_SYCHRO_READ_PARAMETERS(DWORD Nch, DWORD *Channels,
	float Rate, float AdcDelay, DWORD InputMode, DWORD SynchroAdType, DWORD SynchroAdMode, DWORD SynchroAdChannel,
   float SynchroAdPorog, float *SetRate, float *SetAdcDelay)
{
BOOL result;
double f;
int i;
double adc_ms, rate_ms, inter_kadr_ms;

if(!Nch) return false;

// ������� ������� ��������� ������ ���
if(!pModule->GET_ADC_PARS(&ap)) return false;

// ��������� �������� ��������� ���
ap.InputMode = InputMode;
ap.SynchroAdType=SynchroAdType;
ap.SynchroAdMode=SynchroAdMode;
ap.SynchroAdChannel=SynchroAdChannel;
ap.SynchroAdPorog=SynchroAdPorog*8000./ADC_INPUT_RANGES_E140[(SynchroAdChannel >> 6) & 3];

ap.ChannelsQuantity = Nch; 		// ���-�� �������� ������
for(i = 0x0; i < ap.ChannelsQuantity; i++) ap.ControlTable[i] = (WORD)Channels[i];

if(AdcDelay < 0.01) ap.AdcRate=100.;
  else ap.AdcRate = 1./AdcDelay;					// ������� ������ ��� � ���

// ��������� ��� � ��
if(Rate > 100000) Rate=100000;
adc_ms=1./ap.AdcRate;
rate_ms=1000./Rate;
inter_kadr_ms=rate_ms-(Nch-1)*adc_ms;
if(inter_kadr_ms < 0) inter_kadr_ms=0;
ap.InterKadrDelay = inter_kadr_ms;			// ����������� �������� � ��

// ��������� ��������� ��������� ������ ��� � ������
if(!pModule->SET_ADC_PARS(&ap)) return false;

if(!pModule->GET_ADC_PARS(&ap)) return false;

*SetAdcDelay=1./ap.AdcRate;
*SetRate=ap.KadrRate*1000.;

return true;
}

DWORD WINAPI ServiceReadThread(PVOID /*Context*/);
// ������� ������ ��������� � ��������
void ShowThreadErrorMessage(void);

// ������������� ������ ����� ������
HANDLE hReadThread;
DWORD ReadTid;


// ���-�� ���������� �������� (������� 32) ��� �. ReadData()
DWORD DataStep = 32*1024;	// ����� �������� ������ �� 64 ��

// ����� ������
SHORT *AdcBuffer, *GlobalPtr;

// ������ ���������� ������ ������ ����� ������
bool IsReadThreadComplete;
// ����� ������ ��� ���������� ����� ������
WORD ReadThreadErrorNumber;

DWORD DataReadN, DataReadAll;;

// �������� �������-���������
DWORD Counter = 0x0, OldCounter = 0xFFFFFFFF;


int GlobalPacketSize;
SHORT *RealTimePtr;
int RealTimeReadIndex;
DWORD RealBufferSize;
static int StopEvent;
static int OverflowFlag;
static int UserIndex;

// BufferSize - ������ ������ � ���-������� ������, � ������� ����� ���������� ������ �� ������ (����� ���� ���� 1 ��, ���� �� ��� �������)
// PacketSize - ������ ������ ������� � �����, ���������� � ������� ����� ������ ����� ��������� �������� �� PacketSize ����
// ������ ������������ � FIFO �������� RealBufferSize ������� ��  GlobalPacketSize
// ������� ��������� ��������� ������ - RealTimeReadIndex
// ������� ��������� ��������� ����������  UserIndex
__declspec(dllexport) BOOL WINAPI LV_START_REAL_TIME_SAMPLING(DWORD BufferSize, DWORD PacketSize)
{
if(!ap.ChannelsQuantity ) return false;
if(BufferSize < 1024  || (BufferSize & 1)) return false;
if(PacketSize > BufferSize/2) PacketSize=BufferSize/2;
if(PacketSize < 32) PacketSize=32;
if(PacketSize % 32) { PacketSize=(PacketSize/32); PacketSize=(PacketSize+1)*32; }

// ������� ������ ���������� ������ ����� ������
IsReadThreadComplete = false;

// ���� ������ �� �������� ��� ����� ������
RealTimeReadIndex=0;
StopEvent=0;
GlobalPacketSize=PacketSize;
RealBufferSize=BufferSize;
AdcBuffer = NULL;
UserIndex=0;

// ���� �� ������ ����� ����� ������
hReadThread = NULL;

// ������� ���� ������ ������ ����� ������
ReadThreadErrorNumber = 0x0;

// ������� ������ ��� �����
AdcBuffer = new SHORT[2*PacketSize];
if(!AdcBuffer) return false;

RealTimePtr = new SHORT[RealBufferSize];
if(!RealTimePtr) { free(AdcBuffer); return false; }

// ������ � ��������� ����� ����� ������
hReadThread = CreateThread(0, 0x2000, ServiceRealTImeReadThread, 0, 0, &ReadTid);
if(!hReadThread) { free(AdcBuffer); return false; }

if(ReadThreadErrorNumber) return false;

return true;
}

__declspec(dllexport) BOOL WINAPI LV_STOP_REAL_TIME_SAMPLING(void)
{
StopEvent=1;

for(;!IsReadThreadComplete;)
	{
      Sleep(20);
	}

// ��� ��������� ������ ������ ����� ������
WaitForSingleObject(hReadThread, INFINITE);

free(AdcBuffer); free(RealTimePtr);
AdcBuffer=NULL;
RealTimePtr=NULL;

if(ReadThreadErrorNumber) return false;

return true;
}

static int cnt=0;

// Data ������ � ������� ����� ���������� ������
// NPoints ������� �������� �������������
// WaitFlag=1 -> ����� ����� ���� �� �������� NPoints ������ ����� ������ ����� ���� (�� �� ������ NPoints)
// ReadPoints -> ������� ������ ������� �������� � Data
// ReadyToReadPoints -> ������� ������ ���� ������� � �����������
// overflow -> ���� ������������ ������
// TimeOut	������� ��, �� ���������� �������� ������� �������� ���������� ���� ���� ������ �� ���� ������� (��� ���� ���������� ������� ������)
__declspec(dllexport) BOOL WINAPI LV_GET_REAL_TIME_DATA(SHORT *Data, DWORD NPoints, DWORD WaitFlag, DWORD *ReadPoints, DWORD *ReadyToReadPoints,
			DWORD *overflow, DWORD TimeOut)
{
DWORD n;
DWORD n_ready, n_to_send;
DWORD time_waited;

*ReadPoints=0;
time_waited=0;

// ��������� ������� ����� ������ ����������
for(;;)
	{
	if(UserIndex <= RealTimeReadIndex) n_ready=RealTimeReadIndex-UserIndex;
	  else n_ready=RealTimeReadIndex+(RealBufferSize-UserIndex);

   *ReadyToReadPoints=n_ready;

   if(n_ready >= NPoints) { n_to_send=NPoints; break; }

   if(!WaitFlag) { n_to_send=n_ready; break; }

   if(ReadThreadErrorNumber) { *overflow=OverflowFlag; return false; }

   Sleep(10);
   time_waited += 10;

   if(time_waited > TimeOut)
   	{
      *overflow=OverflowFlag;
      return false;
      }
   }


if(!n_to_send) { *ReadyToReadPoints=n_ready; *overflow=OverflowFlag; return true; }

// ������ ������������ � FIFO �������� RealBufferSize ������� ��  GlobalPacketSize
// ������� ��������� ��������� ������ - RealTimeReadIndex
// ������� ��������� ��������� ����������  UserIndex
if((UserIndex+n_to_send) <= RealBufferSize)
         	{
            memcpy(Data, &RealTimePtr[UserIndex], 2*n_to_send);
            UserIndex += n_to_send;
            if(UserIndex >= RealBufferSize) UserIndex=0;
            }
           else
           	{
            // 1. ������� ����� �� ����� ������
            n=RealBufferSize-UserIndex;
            memcpy(Data, &RealTimePtr[UserIndex], 2*n);

            UserIndex=0;

            // ��������� �����
            memcpy(&Data[n], &RealTimePtr[UserIndex], 2*(n_to_send-n));
            UserIndex += (n_to_send-n);
            }

*ReadyToReadPoints=n_ready-n_to_send;
*ReadPoints=n_to_send;
*overflow=OverflowFlag;

return true;
}



//------------------------------------------------------------------------
// �����, � ������� �������������� ���� ������
//------------------------------------------------------------------------
DWORD WINAPI ServiceRealTImeReadThread(PVOID)
{
	WORD RequestNumber;
	DWORD i;
	// ������������� ������� �� ���� �������
	HANDLE ReadEvent[2];
	// ������ OVERLAPPED �������� �� ���� ���������
	OVERLAPPED ReadOv[2];
	IO_REQUEST_LUSBAPI IoReq[2];
   DWORD n;DWORD OldPointer;

	// ��������� ������ ��� � ������������ ������� USB-����� ������ ������
	if(!pModule->STOP_ADC()) { ReadThreadErrorNumber = 0x1; IsReadThreadComplete = true; return 0x0; }

	// ��������� ����������� ��� ����� ������ ���������
	for(i = 0x0; i < 0x2; i++)
	{
		// ������ �������
		ReadEvent[i] = CreateEvent(NULL, FALSE , FALSE, NULL);
		// ������������� ��������� ���� OVERLAPPED
		ZeroMemory(&ReadOv[i], sizeof(OVERLAPPED)); ReadOv[i].hEvent = ReadEvent[i];
		// ��������� ��������� IoReq
		IoReq[i].Buffer = AdcBuffer + i*GlobalPacketSize;
		IoReq[i].NumberOfWordsToPass = GlobalPacketSize;
		IoReq[i].NumberOfWordsPassed = 0x0;
		IoReq[i].Overlapped = &ReadOv[i];
		IoReq[i].TimeOut = 1000;
	}

	// ������ ��������������� ������ �� ���� ������
	RequestNumber = 0x0;
	if(!pModule->ReadData(&IoReq[RequestNumber])) { CloseHandle(ReadEvent[0]); CloseHandle(ReadEvent[1]); ReadThreadErrorNumber = 0x2; IsReadThreadComplete = true; return 0x0; }

	// �������� ���
	if(pModule->START_ADC())
	{
		// ���� ����� ������
		for(;;)
		{
			// ������� ������ �� ��������� ������ ������
			RequestNumber ^= 0x1;
			if(!pModule->ReadData(&IoReq[RequestNumber])) { ReadThreadErrorNumber = 0x2; break; }

			// ��� ���������� �������� ����� ���������� ������ ������
         for(;;)
         	{
            if(StopEvent) break;
				if(WaitForSingleObject(ReadEvent[RequestNumber^0x1], IoReq[RequestNumber^0x1].TimeOut) == WAIT_TIMEOUT) continue;
            	else break;
            }
         if(StopEvent) break;

			// ������ ������������ � FIFO �������� RealBufferSize ������� ��  GlobalPacketSize
			// ������� ��������� ��������� ������ - RealTimeReadIndex
			// ������� ��������� ��������� ����������  UserIndex
         OldPointer=RealTimeReadIndex;
         if((RealTimeReadIndex+GlobalPacketSize) <= RealBufferSize)
         	{
            memcpy(&RealTimePtr[RealTimeReadIndex], IoReq[RequestNumber^0x1].Buffer, 2*GlobalPacketSize);
            RealTimeReadIndex += GlobalPacketSize;

	         // �������� �� ������������
   	      if(UserIndex > OldPointer && UserIndex <= RealTimeReadIndex)   OverflowFlag=1;

            if(RealTimeReadIndex == RealBufferSize) RealTimeReadIndex=0;
            }
           else
           	{
            // 1. ������� ����� �� ����� ������
            n=RealBufferSize-RealTimeReadIndex;
            memcpy(&RealTimePtr[RealTimeReadIndex], IoReq[RequestNumber^0x1].Buffer, 2*n);

	         // �������� �� ������������
            RealTimeReadIndex += n;
   	      if(UserIndex > OldPointer && UserIndex <= RealTimeReadIndex)   OverflowFlag=1;

            RealTimeReadIndex=0;

            // ��������� �����
            memcpy(&RealTimePtr[RealTimeReadIndex], &IoReq[RequestNumber^0x1].Buffer[n], 2*(GlobalPacketSize-n));
            RealTimeReadIndex += (GlobalPacketSize-n);
            }


			if(ReadThreadErrorNumber) break;
				else Sleep(20);
		}

	}
	else { ReadThreadErrorNumber = 0x6; }

	// ��������� ������ ���
	if(!pModule->STOP_ADC()) ReadThreadErrorNumber = 0x1;
	// ������ �������� ������������� ����������� ������ �� ���� ������
	if(!CancelIo(pModule->GetModuleHandle())) { ReadThreadErrorNumber = 0x7; }
	// ��������� ��� �������������� �������
	for(i = 0x0; i < 0x2; i++) CloseHandle(ReadEvent[i]);
	// ��������� ��������
	Sleep(20);
	// ��������� ������ ���������� ������ ������ ����� ������
	IsReadThreadComplete = true;
	// ������ ����� �������� �������� �� ������
	return 0x0;
}



__declspec(dllexport) BOOL WINAPI WEProcessOnePoint(SHORT src, double *dest, DWORD channel, DWORD calibr, DWORD volt)
{
    return (BOOL)ProcessOnePoint(src, dest, channel, calibr, volt);
}


__declspec(dllexport) BOOL WINAPI WEDAC_SAMPLE_VOLT(double DacData, DWORD calibr, DWORD dac_number)
{
short val;

val=DacData*2048./5.00;
if(calibr == TRUE) val=(val+E140DescriptionStruct.Dac.OffsetCalibration[dac_number])*E140DescriptionStruct.Dac.ScaleCalibration[dac_number];

return (BOOL)pModule->DAC_SAMPLE(&val, dac_number);
}

// -----------------------------------------------------------------------------
// �������������� ����� ��� � ������ � ������ ����������
// -----------------------------------------------------------------------------
__declspec(dllexport) BOOL WINAPI WEProcessArray(SHORT *src, double *dest, DWORD size, DWORD calibr, DWORD volt)
{
	double K_offset[MAX_CONTROL_TABLE_LENGTH_E140], K_scale[MAX_CONTROL_TABLE_LENGTH_E140];
	DWORD i, j, nch, gain_index;
	double ScaleGain[4]={10.0/8000., 2.5/8000., 0.625/8000., 0.15625/8000.};

	nch=ap.ChannelsQuantity;
   if(!nch) { return FALSE; }

   // ���������� ������������
   for(i=0; i < nch; i++)
   	{
      gain_index=(ap.ControlTable[i] >> 6) & 3;
      if(calibr == FALSE)
      	{
         // ��� ����������
         if(volt == FALSE)
         	{
            // ���� ���, ��� ����������
            K_offset[i]=0;
            K_scale[i]=1.0;
            }
           else
           	{
            // ������, ��� ����������
            K_offset[i]=0;
            K_scale[i]=ScaleGain[gain_index];
            }
         }
        else
      	{
         // � �����������
         if(volt == FALSE)
         	{
            // ���� ���, � �����������
            K_offset[i]=E140DescriptionStruct.Adc.OffsetCalibration[gain_index];
            K_scale[i]=E140DescriptionStruct.Adc.ScaleCalibration[gain_index];
            }
           else
           	{
            // ������, � �����������
            K_offset[i]=E140DescriptionStruct.Adc.OffsetCalibration[gain_index];
            K_scale[i]=E140DescriptionStruct.Adc.ScaleCalibration[gain_index]*ScaleGain[gain_index];
            }
         }
      }

   for(i=j=0; i < size; i++)
   	{
      *dest++=((*src++)+K_offset[j])*K_scale[j];
      j++;
      if(j >= nch) j=0;
      }

	// ��� ������ :)))))
	return TRUE;
}

