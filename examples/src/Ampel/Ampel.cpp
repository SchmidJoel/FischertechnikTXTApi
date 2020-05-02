#include <unistd.h>         // for sleep()

#include "KeLibTxtDl.h"     // TXT Lib
#include "FtShmem.h"        // TXT Transfer Area

int main(void) {
	FISH_X1_TRANSFER *pTArea;
	if (StartTxtDownloadProg() == KELIB_ERROR_NONE) {
		pTArea = GetKeLibTransferAreaMainAddress();
		if (pTArea) {	
			while(true){
				pTArea->ftX1out.duty[0] = 512;			//red
				sleep(3);
				pTArea->ftX1out.duty[1] = 512;			//yellow
				sleep(1);
				pTArea->ftX1out.duty[0] = 0;			//red
				pTArea->ftX1out.duty[1] = 0;			//yellow
				pTArea->ftX1out.duty[2] = 512;			//green
				sleep(3);
				pTArea->ftX1out.duty[2] = 0;			//green
				pTArea->ftX1out.duty[1] = 512;			//yellow
				sleep(1);
				pTArea->ftX1out.duty[1] = 0;			//yellow
			}			
		}
		StopTxtDownloadProg();
	}
	return 0;
}
