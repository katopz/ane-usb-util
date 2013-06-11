//
//  USBUtil
//  Will list all USB device ID
//
//  Created by Todsaporn Banjerdkit (katopz) on 09/06/2013.
//  Copyright (c) 2012 Todsaporn Banjerdkit. All rights reserved.
//

// c++
#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <Setupapi.h>
#include <devguid.h>
#include "conio.h"
#include "tchar.h"

// ane
#include <iostream>
#include "FlashRuntimeExtensions.h"

#include <string>
#include <stdlib.h>
using namespace std;

#pragma comment (lib, "Setupapi.lib")

extern "C" __declspec(dllexport) void USBUtilInitializer(void** extDataToSet, FREContextInitializer* ctxInitializerToSet, 
														 FREContextFinalizer* ctxFinalizerToSet);
extern "C" __declspec(dllexport) void USBUtilFinalizer(void* extData);

FREObject getUSBInfo(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {

	// init deviceInfoSet
	HDEVINFO deviceInfoSet;
	GUID *guidDev = (GUID*) &GUID_DEVCLASS_USB; 
	deviceInfoSet = SetupDiGetClassDevs(guidDev, NULL, NULL, DIGCF_PRESENT | DIGCF_PROFILE);
	TCHAR buffer [4000];
	DWORD buffersize = 4000;
	int memberIndex = 0;

	TCHAR result [4000];
	wcscpy_s(result , _T(""));

	// loop all deviceInfoData in deviceInfoSet
	while (true) 
	{
		// init deviceInfoData
		SP_DEVINFO_DATA deviceInfoData;
		ZeroMemory(&deviceInfoData, sizeof(SP_DEVINFO_DATA));
		deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

		// eof
		if (SetupDiEnumDeviceInfo(deviceInfoSet, memberIndex, &deviceInfoData) == FALSE)
		{
			if (GetLastError() == ERROR_NO_MORE_ITEMS)
			{
				break;
			}
		}

		// get id
		DWORD nSize = 0 ;
		SetupDiGetDeviceInstanceId (deviceInfoSet, &deviceInfoData, buffer, sizeof(buffer), &nSize);
		buffer [nSize] = '\0';

		// concat result
		wcscat_s(result, buffer);

		// next
		memberIndex++;

		// eof
		if (SetupDiEnumDeviceInfo(deviceInfoSet, memberIndex, &deviceInfoData) == TRUE)
		{
			// separate by new line
			wcscat_s(result, _T("\n"));
		}
	}

	int BUFFER_SIZE = sizeof(result);
	size_t i;
	char *pMBBuffer = (char *)malloc( BUFFER_SIZE );

	// Conversion
	wcstombs_s(&i, pMBBuffer, (size_t)BUFFER_SIZE, result, (size_t)BUFFER_SIZE );

	const uint8_t* msg = (const uint8_t*)pMBBuffer;
	FREObject retObj;
	FRENewObjectFromUTF8(strlen((const char*)msg) + 1, msg, &retObj);

	// Free multibyte character buffer
	if (pMBBuffer)
	{
		free(pMBBuffer);
	}

	// release
	if (deviceInfoSet)
	{
		SetupDiDestroyDeviceInfoList(deviceInfoSet);
	}

	// result
	return retObj;
}

void USBUtilContextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx, 
							   uint32_t* numFunctionsToTest, const FRENamedFunction** functionsToSet) {
								   *numFunctionsToTest = 1;
								   FRENamedFunction* func = (FRENamedFunction*)malloc(sizeof(FRENamedFunction)*1);
								   func[0].name = (const uint8_t*)"getUSBInfo";
								   func[0].functionData = NULL;
								   func[0].function = &getUSBInfo;
								   *functionsToSet = func;
}

void USBUtilContextFinalizer(FREContext ctx) {
	return;
}

__declspec(dllexport) void USBUtilInitializer(void** extDataToSet, FREContextInitializer* ctxInitializerToSet, 
											  FREContextFinalizer* ctxFinalizerToSet) {
												  *extDataToSet = NULL;
												  *ctxInitializerToSet = USBUtilContextInitializer;
												  *ctxFinalizerToSet = USBUtilContextFinalizer;
}

__declspec(dllexport) void USBUtilFinalizer(void* extData) {
	return;
}
