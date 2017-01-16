#include <stdio.h>
#include "platform.h"
#include "sdcard_inc.h"

void main(void) {
	if(!R_SdCardSlotInit()) {
		printf("ERROR: No SDCard found on the slot. The program will continue when the SDCard is present ...\n");
		while(!R_SdCardSlotInit());
	}

	printf("SD Card found!\n");

	R_MmcDiskInit();
	R_SdCardDiskMount(SDCARD_LUN);

	DIR dir;
	FIL file;
	FILINFO finfo;
	FRESULT res;

	res = f_opendir(&dir, "1:");
	while(res == FR_OK) {
		 while ((f_readdir(&dir, &finfo) == FR_OK) && finfo.fname[0])  {
			 if (finfo.fattrib & AM_DIR) { /* entry is a directory */
				 printf("\nDirectory: %s", finfo.fname);
			 } else { /* Entry is a file. */
				 printf("\nFile: %s", finfo.fname);
			 }
		 }
	}

	for(;;);
}
