#ifndef NTP_TASK_H
#define NTP_TASK_H

void time_sync_notification_cb(struct timeval *tv);
void initialize_sntp(void);
void ntp_task( void* param );
void obtain_time(void);
void start_ntp_task(void);

extern int ntp_time_in_sync;


#endif // NTP_TASK_H
