#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "mesh_light.h"

float GetRunTimeStats( void )
{
TaskStatus_t *pxTaskStatusArray;
 volatile UBaseType_t uxArraySize, x;
 uint32_t ulTotalRunTime;
float result = 0;
int NUM = 0;
float percent = 0;
     // Make sure the write buffer does not contain a string.

     // Take a snapshot of the number of tasks in case it changes while this
     // function is executing.
     uxArraySize = uxTaskGetNumberOfTasks();

     // Allocate a TaskStatus_t structure for each task.  An array could be
     // allocated statically at compile time.
     pxTaskStatusArray = pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

     if( pxTaskStatusArray != NULL )
     {
         // Generate raw status information about each task.
         uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, &ulTotalRunTime );
         // For percentage calculations.
         ulTotalRunTime /= 100UL;

         // Avoid divide by zero errors.
         if( ulTotalRunTime > 0 )
         {
             // For each populated position in the pxTaskStatusArray array,
             // format the raw data as human readable ASCII data
             for( x = 0; x < uxArraySize; x++ )
             {
                 // What percentage of the total run time has the task used?
                 // This will always be rounded down to the nearest integer.
                 // ulTotalRunTimeDiv100 has already been divided by 100.
                 percent = (float) pxTaskStatusArray[ x ].ulRunTimeCounter / ulTotalRunTime;
                if(strncmp("IDLE", pxTaskStatusArray[ x ].pcTaskName, sizeof(pxTaskStatusArray[ x ].pcTaskName)) != 0)
                {
                    result += percent;
                }
                //  pcWriteBuffer += strlen( ( char * ) pcWriteBuffer );
             }
         }
        result = (int) (result * 100);
        result = (float)result / 100;  
         // The array is no longer needed, free the memory it consumes.
         vPortFree( pxTaskStatusArray );
     }
    return result;
}

float power(void)
{
    return GetRunTimeStats();
}