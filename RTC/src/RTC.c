#include <stdio.h>
#include "platform.h"
#include "r_rtc_rx_if.h"
#include "lcd.h"

#define UPDATE_TIME 0

void rtc_callback(void * args) {
	if(*(rtc_cb_evt_t*)args == RTC_EVT_PERIODIC) {
		/* Read and Display Time: */
		tm_t time;
		if(R_RTC_Read(&time, NULL) == RTC_SUCCESS) {
			char msg1[] = "Time: ";
			char msg2[20];

			sprintf(msg2, "%.2d:%.2d:%.2d",time.tm_hour,time.tm_min, time.tm_sec);
			printf("%s%s\n", msg1,msg2);

			lcd_display(LCD_LINE1, msg1);
			lcd_display(LCD_LINE2, msg2);
		}
	}
}

void main(void) {
	/* Initialize LCD: */
	lcd_initialize();
	lcd_clear();

	/* Initialize RTC: */
	rtc_init_t rtc_init = {
		.p_callback        = rtc_callback,
		.output_freq       = RTC_OUTPUT_OFF,
		.periodic_freq     = RTC_PERIODIC_1_HZ,
		.periodic_priority = 7,
#if UPDATE_TIME
		.set_time          = true
#else
		.set_time          = false
#endif
	};

	tm_t init_time = {
		00,  /* Seconds (0-59) */
		03,  /* Minute (0-59) */
		14,  /* Hour (0-23) */
		16,  /* Day of the month (1-31) */
		0,   /* Month (0-11, 0=January) */
		117, /* Year since 1900 */
		1,   /* Day of the week (0-6, 0=Sunday) */
		16,  /* Day of the year (0-365) */
		0    /* Daylight Savings enabled (>0), disabled (=0), or unknown (<0)*/
	};
	printf("Initializing RTC: %d\n", R_RTC_Open(&rtc_init, &init_time));
	for(;;);
}
