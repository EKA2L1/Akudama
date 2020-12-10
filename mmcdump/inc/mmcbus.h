/*
 * Copyright (c) 2020 EKA2L1 Team.
 *
 * This work is licensed under the terms of the MIT license.  
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */
 
#ifndef __MMCBUS_H__
#define __MMCBUS_H__

#include <e32std.h>

_LIT(KMmcIfDvcName, "MmcIf");

enum TMmcMediaType {
	EMmcROM,
	EMmcFlash,
	EMmcIO,
	EMmcOther,
	EMmcNotSupported
};

struct TMmcIfInfo {
	TUint32 iMmcID[4];
	TUint32 iMmcType;
};

enum TMmcControlOp {
	EMmcControlOpSelectCard = 4,
	EMmcControlOpCardInfo = 6
};

class CMmcBusChannel: public RBusLogicalChannel {
public:
	CMmcBusChannel();
	
	void ConstructL();
	
	TInt SelectCard(TInt aIndex);
	TInt GetCardInfo(TMmcIfInfo *aInfo);
	
	TVersion GetVersion();
};

#endif  // __MMCBUS_H__

