#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <sys/time.h>
#include "esp_sntp.h"
#include "esp_log.h"
#include "main.h"
#include "ntp_task.h"

int ntp_time_in_sync = 0;
static const char *ntp_task_log_tag = "ntp_task";
TaskHandle_t ntp_task_handle = NULL;

void start_ntp_task()
{
    ESP_LOGI(ntp_task_log_tag, "Starting NTP-Task");
    // start NTP-Task
    BaseType_t xReturned = xTaskCreate(ntp_task,"ntp_task", 4048, (void*)0, tskIDLE_PRIORITY, &ntp_task_handle );
    
    if( xReturned != pdPASS ) {
        ESP_LOGI(ntp_task_log_tag, "Unable to spawn NTP-TASK.");
    } else {
        ESP_LOGI(ntp_task_log_tag, "NTP-Task successfully created.");
    }
}


void ntp_task( void* param )
{
    
    ESP_LOGI(ntp_task_log_tag, "Entering task-loop.");

    for(;;)
    {
        obtain_time();

        if( ntp_time_in_sync == 1 ) {
            char strftime_buf[64];
            // Set timezone to Eastern Standard Time and print local time
            ESP_LOGI(ntp_task_log_tag, "Setting timezone to: %s", NTP_TIMEZONE);
            setenv("TZ", NTP_TIMEZONE, 1);
            tzset();
        

            // Output current system time
            time_t now = 0;
            struct tm timeinfo = { 0 };
            time(&now);
            localtime_r(&now, &timeinfo);
            strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
            ESP_LOGI(ntp_task_log_tag, "The current date/time in Germany is: %s", strftime_buf);

            //WAIT 24 Hours
            int delay_counter = 0;
            while (delay_counter != 23) {
                vTaskDelay((1000*3600) / portTICK_RATE_MS);
                delay_counter++;
            }

        } 
        else {
            vTaskDelay(pdMS_TO_TICKS(50));
        }

        
    }
}


void obtain_time(void)
{
    // ESP_ERROR_CHECK( esp_event_loop_create_default() );

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */

    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 20;
    sntp_sync_status_t ret;
    do {
        ESP_LOGI(ntp_task_log_tag, "Waiting for NTP-Sync to be received... (%d/%d)", retry, retry_count);
        
        ret = sntp_get_sync_status();
        if( ret == SNTP_SYNC_STATUS_RESET ) {
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
        else if (ret == SNTP_SYNC_STATUS_COMPLETED) {
            ntp_time_in_sync = 1;
        }
               
    } while( ret == SNTP_SYNC_STATUS_RESET && ++retry < retry_count );
    
    time(&now);
    localtime_r(&now, &timeinfo);
}


void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(ntp_task_log_tag, "Notification of a time synchronization event");
}

void initialize_sntp(void)
{
    ESP_LOGI(ntp_task_log_tag, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
#ifdef CONFIG_SNTP_TIME_SYNC_METHOD_SMOOTH
    sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
#endif
    sntp_init();
}