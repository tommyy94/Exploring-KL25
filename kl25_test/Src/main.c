#include <includes.h>


int main(void)
{
    vSystemInit();
    vCreateTasks();
    vCreateQueues();
    vCreateTimers();
    vTaskStartScheduler();
    
    for (;;)
    {
        ; /* Should not get here! */
    }
}
