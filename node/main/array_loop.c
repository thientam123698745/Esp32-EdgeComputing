#include "string.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mesh_light.h"

#include "esp_log.h"
static const char *TAG = "loop_buffer";

#define COL 30
#define ROW 10
#define K 5000
#define k2 150
char data_rx[30] = "Hello";
TaskHandle_t array_handle;
TaskHandle_t buffer_handle;
typedef struct
{
    int third_array[ROW][COL];
} holder_t;
int loop = 0;

void array_loop(void *p)
{
    printf("Enter array_loop\r\n");
    holder_t holder;
    int first_array[ROW][COL], second_array[ROW][COL];
    // if (loop == 2)
    //     {
    //         // loop = 0;
    //         // break;
    //     }
    // ESP_LOGI(TAG, "Start loop");
    
    // for (int i = 0; i < ROW; i++)
    //     for (int j = 0; j < COL; j++)
    //     {
    //         first_array[i][j] = i;
    //         second_array[i][j] = i;
    //         holder.third_array[i][j] = i;
    //     }
    // for (unsigned long i = 0; i < COL; i++)
    // {
    //     for (unsigned long j = 0; j < ROW; j++)
    //     {
    //         for (unsigned long k = 0; k < K; k++)
    //         {
    //             holder.third_array[j][i] = holder.third_array[j][i] + first_array[k][i] + second_array[k][i];
    //         }
    //     }
    // }
    // printf("End loop\r\n");
    int loopcount = 0;
        while(true)   
        {
            for (unsigned long i = 0; i < ROW; i++)
                for (unsigned long j = 0; j < COL; j++)
                {
                    first_array[i][j] = i;
                    second_array[i][j] = i;
                    holder.third_array[i][j] = i;
                }
            for (unsigned long i = 0; i < COL; i++)
            {
                for (unsigned long j = 0; j < ROW; j++)
                {
                    for (unsigned long k = 0; k < k2; k++)
                    {
                        holder.third_array[j][i] = holder.third_array[j][i] + first_array[k][i] + second_array[k][i];
                    }
                }
            }
            printf("Loop: %d\r\n", loop);
                if(loop == 1)
                {   
                    // for(int i = 0; i<100;i++)
                    while(true)
                    {
                        if (loop == 2)
                            {
                                // loop = 0;
                                break;
                            }
                        ESP_LOGI(TAG, "Start loop");
                        
                        for (int i = 0; i < ROW; i++)
                            for (int j = 0; j < COL; j++)
                            {
                                first_array[i][j] = i;
                                second_array[i][j] = i;
                                holder.third_array[i][j] = i;
                            }
                        for (unsigned long i = 0; i < COL; i++)
                        {
                            for (unsigned long j = 0; j < ROW; j++)
                            {
                                for (unsigned long k = 0; k < K; k++)
                                {
                                    holder.third_array[j][i] = holder.third_array[j][i] + first_array[k][i] + second_array[k][i];
                                }
                            }
                        }
                        printf("loop count:%d\r\n");
                        // loopcount++;
                        // if(loopcount > 1000)
                        // {
                        //     break;
                        // }
                    vTaskDelay(iDelay/10 / portTICK_PERIOD_MS);
                    }
                    loop = 3;
                }
                else if(loop == 2)
                {
                    char buffer[3];
                    strcpy(buffer, data_rx);
                    vTaskDelete(NULL);
                }
                else 
                {
                    // printf("array loop delay 5s\n");
                        vTaskDelay(iDelay / portTICK_PERIOD_MS);
                }
                //  vTaskDelay(5 / portTICK_PERIOD_MS);
        }
    vTaskDelete(NULL);
}
void buffer(void *p)
{
    
}
void array_loop_init(void)
{
    xTaskCreatePinnedToCore(array_loop, "array_loop", 8192, NULL, 7, &array_handle, 0);
    // xTaskCreatePinnedToCore(buffer, "buffer", 8192, NULL, 1, &buffer_handle, 0);
    // vTaskSuspend(array_handle);
    // vTaskSuspend(buffer_handle);
}