#include <windows.h>
#include <setupapi.h>
#include <cfgmgr32.h>
#include <tchar.h>
#include <string>
#include <iostream>
using namespace std;

#pragma comment (lib, "setupapi.lib")


bool FindKinect ()
{
    unsigned i;
    DWORD dwSize, dwPropertyRegDataType;
    CONFIGRET status;
    HDEVINFO hDevInfo;
    SP_DEVINFO_DATA DeviceInfoData;
    const static LPCTSTR arPrefix[3] = {TEXT("VID_"), TEXT("PID_"), TEXT("MI_")};
    TCHAR szDeviceInstanceID [MAX_DEVICE_ID_LEN];
    TCHAR szDesc[1024];

    // List all connected USB devices
    hDevInfo = SetupDiGetClassDevs (NULL, TEXT("USB"), NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT);
    if (hDevInfo == INVALID_HANDLE_VALUE)
        return false;

    // Find the ones that are driverless
	bool bIsFind_ = false;
    for (i = 0; ; i++)  {
        DeviceInfoData.cbSize = sizeof (DeviceInfoData);
        if (!SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData))
            break;

        status = CM_Get_Device_ID(DeviceInfoData.DevInst, szDeviceInstanceID , MAX_PATH, 0);
        if (status != CR_SUCCESS)
            continue;

        if (SetupDiGetDeviceRegistryProperty (hDevInfo, &DeviceInfoData, SPDRP_DEVICEDESC,
                                              &dwPropertyRegDataType, (BYTE*)szDesc,
                                              sizeof(szDesc),   // The size, in bytes
                                              &dwSize))
		{
			wstring ws(szDesc);
			string str(ws.begin(), ws.end());
			if(str.find("Kinect") != string::npos)
			{
				bIsFind_ = true;
				break;
			}
		}            
    }

    return bIsFind_;
}
