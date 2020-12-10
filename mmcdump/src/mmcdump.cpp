/*
 * Copyright (c) 2020 EKA2L1 Team.
 *
 * This work is licensed under the terms of the MIT license.  
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "mmcdump.h"
#include "mmcbus.h"

#include <e32base.h>
#include <e32std.h>
#include <f32file.h>

#include <e32svr.h>

_LIT(KMmcIfLddName, "GD1DRV");
_LIT(KMmcInfoFilePath, "C:\\MmcInfo.txt");

_LIT8(KMmcTypeFormatStr, "MMC type: %S (%d)\n");
_LIT8(KMmcIdFormatStr, "MMC ID: %08x-%08x-%08x-%08x\n");

_LIT(KMmcFailOpenDumpInfoFileStr, "Fail opening MMC info dump file!");
_LIT(KMmcFailDumpInfoFileStr, "Fail dumping MMC info!");

_LIT8(KMmcMediaTypeROMStr, "ROM");
_LIT8(KMmcMediaTypeFlashStr, "Flash");
_LIT8(KMmcMediaTypeIOStr, "IO");
_LIT8(KMmcMediaTypeOtherStr, "Other");
_LIT8(KMmcMediaTypeNotSupportedStr, "Not supported");

const TDesC8 *GetMmcMediaTypeStr(const TInt aType) {
	switch (aType) {
		case EMmcROM:
			return &KMmcMediaTypeROMStr;
			
		case EMmcFlash:
			return &KMmcMediaTypeFlashStr;
			
		case EMmcIO:
			return &KMmcMediaTypeIOStr;
			
		case EMmcOther:
			return &KMmcMediaTypeOtherStr;
			
		case EMmcNotSupported:
			return &KMmcMediaTypeNotSupportedStr;
			
		default:
			break;
	}
	
	return &KMmcMediaTypeNotSupportedStr;
}

void DumpCardInfoL(RFile &aOutputFile) {
	// Load logical device
	User::LoadLogicalDevice(KMmcIfLddName);
	
	CMmcBusChannel mmcDev;
	mmcDev.ConstructL();
	
	User::LeaveIfError(mmcDev.SelectCard(0));
	
	// Get card info
	TMmcIfInfo mmcInfo;
	User::LeaveIfError(mmcDev.GetCardInfo(&mmcInfo));
	
	TBuf8<256> lineToWrite;
	
	lineToWrite.Format(KMmcTypeFormatStr, GetMmcMediaTypeStr(mmcInfo.iMmcType), mmcInfo.iMmcType);
	aOutputFile.Write(lineToWrite);
	
	lineToWrite.Format(KMmcIdFormatStr, mmcInfo.iMmcID[0], mmcInfo.iMmcID[1], mmcInfo.iMmcID[2], mmcInfo.iMmcID[3]);
	aOutputFile.Write(lineToWrite);
	
	User::FreeLogicalDevice(KMmcIfDvcName);
	aOutputFile.Flush();
}

GLDEF_C TInt E32Main()
	{
	// Create cleanup stack
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();

	// Connect to FS service first
	RFs fs;
	fs.Connect(-1);
	
	RNotifier notifier;
	notifier.Connect();
	
	TInt err = KErrNone;
	
	RFile infoFile;
	if (infoFile.Replace(fs, KMmcInfoFilePath, EFileShareAny|EFileWrite) != KErrNone) {
		notifier.InfoPrint(KMmcFailOpenDumpInfoFileStr);
	} else {
		TRAP(err, DumpCardInfoL(infoFile));
		
		if (err != KErrNone) {
			notifier.InfoPrint(KMmcFailDumpInfoFileStr);
		}
	}
	
	infoFile.Close();
	notifier.Close();
	fs.Close();
	
	delete cleanup;
	__UHEAP_MARKEND;
	return err;
	}

