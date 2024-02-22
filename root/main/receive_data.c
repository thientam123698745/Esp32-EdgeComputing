#include "mesh_light.h"
#define RX_SIZE          (1500)
static uint8_t rx_buf[RX_SIZE] = { 0, };
char ms1[CONFIG_MESH_AP_CONNECTIONS * 10] = "";


bool Post = 1;

const char *MESH_TAG = "ROOT_RX";
int is_running = 0;
int NumOfReiceiveMessage = 0;
char ReiceiveBuffer[ReiceiveBufferSize][ReiceiveMessageSize];
mesh_addr_t ReiceiveAddrBuffer[ReiceiveBufferSize];



void data_analysis(char *ms, mesh_addr_t addr)
{
    char *token;
    int nodeid = 0;
    int cpu_usage = 0;
    token = strtok (ms,":,");
    while (token != NULL)
    {

        nodeid = atoi(token);
        token = strtok (NULL,":,");
        writeNodeCpu(token, nodeid);
        registerNode(addr, nodeid);
        token = strtok (NULL,":,");
    }

}
void MessageProcess()
{
    while(1)
    {
        if(NumOfReiceiveMessage == 0)
        {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        else
        {
            data_analysis(ReiceiveBuffer[NumOfReiceiveMessage-1], ReiceiveAddrBuffer[NumOfReiceiveMessage-1]);
            NumOfReiceiveMessage--;
        }
    }
    vTaskDelete(NULL);
}

void esp_mesh_p2p_rx_main(void *arg)
{
    int recv_count = 0;
    esp_err_t err;
    mesh_addr_t from;
    int send_count = 0;
    mesh_data_t data;
    int flag = 0;
    data.data = rx_buf;
    data.size = RX_SIZE;
    mesh_addr_t MyID;
    is_running = true;
    char feedback[50] = "node: Hello";
    static mesh_addr_t mesh_parent_addr;
    int mesh_layer = 0;
    while (is_running) {
        data.size = RX_SIZE;
        err = esp_mesh_recv(&from, &data, portMAX_DELAY, &flag, NULL, 0);
        sprintf(ms1, "%s", data.data);
        // if(NumOfReiceiveMessage == ReiceiveBufferSize)
        // {
        //     printf("Out of message size\r\n");
        //     continue;
        // }
        // sprintf(ReiceiveBuffer[NumOfReiceiveMessage], "%s", data.data);
        // ReiceiveAddrBuffer[NumOfReiceiveMessage] = from;
        // NumOfReiceiveMessage = NumOfReiceiveMessage + 1;
        // printf(ms1);
        data_analysis(ms1, from);
    }
    vTaskDelete(NULL);
}
void receive_data_init(void)
{
        xTaskCreate(esp_mesh_p2p_rx_main, "MPRX", 3072, NULL, 3, NULL);
        // xTaskCreate(MessageProcess, "MessageProcess", 3072, NULL, 5, NULL);
        
}