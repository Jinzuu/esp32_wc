
/* The examples use WiFi configuration that you can set via project configuration menu
   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define WIFI_SSID      "MKLanGast"
#define WIFI_PASS      "MartaKaiLanGast"
#define WIFI_MAXIMUM_RETRY  5



/***
 * LED-Strip configuration
 */
#define LED_STRIP_LED_COUNT 4
#define CONFIG_EXAMPLE_RMT_TX_GPIO 18
#define RMT_TX_CHANNEL RMT_CHANNEL_0
#define EXAMPLE_CHASE_SPEED_MS (100)

/***
 * NTP Time Configuration
 */
#define NTP_TIMEZONE "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00"