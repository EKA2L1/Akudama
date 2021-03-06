/*
 * Copyright (c) 2020 EKA2L1 Team.
 *
 * This work is licensed under the terms of the MIT license.  
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <e32base.h>
#include <e32std.h>
#include <f32file.h>

_LIT(KAccuracyOutputPath, "C:\\AccuracyCheck.txt");
_LIT(KMmcFailOpenAccuracyFileStr, "Fail opening file to write tests!");

_LIT8(KTestFailFmtStr, "Test %d failed with code %d");

typedef void (*TestFuncType)(RFile &);

void DoLocalCodeCheckL(RFile &aFile) {
	const TUint32 KTestCodeChunkSize = 0x1000;
	
	RChunk testChunk;
	User::LeaveIfError(testChunk.CreateLocalCode(KTestCodeChunkSize, KTestCodeChunkSize, EOwnerProcess));
	
	_LIT8(KLocalCodeChunkOutLineStr, "Base address of local code chunk is 0x%08x\n");
	
	TBuf8<128> outLine;
	outLine.Format(KLocalCodeChunkOutLineStr, (TUint32)testChunk.Base());
	
	aFile.Write(outLine);
	testChunk.Close();
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
	
	RFile infoFile;
	if (infoFile.Replace(fs, KAccuracyOutputPath, EFileShareAny|EFileWrite) != KErrNone) {
		notifier.InfoPrint(KMmcFailOpenAccuracyFileStr);
	} else {
		TestFuncType listTests[] = { DoLocalCodeCheckL };

		for (TUint32 i = 0; i < sizeof(listTests) / sizeof(TestFuncType); i++) {
			TRAPD(nerr, listTests[i](infoFile));
			
			if (nerr != KErrNone) {
				TBuf8<128> errLine;
				errLine.Format(KTestFailFmtStr, i, nerr);
				
				infoFile.Write(errLine);
			}
		}
	}
	
	infoFile.Close();
	notifier.Close();
	fs.Close();
	
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}

