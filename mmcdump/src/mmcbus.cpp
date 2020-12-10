/*
 * Copyright (c) 2020 EKA2L1 Team.
 *
 * This work is licensed under the terms of the MIT license.  
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */
 
#include "mmcbus.h"

CMmcBusChannel::CMmcBusChannel()
	: RBusLogicalChannel() {
}

TVersion CMmcBusChannel::GetVersion() {
	TVersion dver;
	dver.iMajor = 1;
	dver.iMinor = 0;
	dver.iBuild = 1;
	
	return dver;
}
	
void CMmcBusChannel::ConstructL() {
	User::LeaveIfError(DoCreate(KMmcIfDvcName, GetVersion(), NULL, 0, NULL, NULL, EOwnerProcess));
}

TInt CMmcBusChannel::SelectCard(TInt aIndex) {
	return DoControl(EMmcControlOpSelectCard, (TAny*)aIndex);
}

TInt CMmcBusChannel::GetCardInfo(TMmcIfInfo *aInfo) {
	return DoControl(EMmcControlOpCardInfo, aInfo);
}
