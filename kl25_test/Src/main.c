/**
 * Display uptime, soil moisture, humidity and temperature in LCD.
 * Send the data to database/web server.
 */
#include <includes.h>


int main(void)
{
    vSystemInit();
    vCreateTasks();
    vCreateQueues();
    vTaskStartScheduler();
    
    while (1)
    {
        ; /* Should not get here! */
    }
}
