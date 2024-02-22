#include "mesh_light.h"

static const char *MESH_TAG = "ROOT_TX";
int i;
esp_err_t err;
int send_count = 0;
mesh_addr_t route_table[CONFIG_MESH_ROUTE_TABLE_SIZE];
int route_table_size = 0;
mesh_data_t data;
char ms[1024] = "Node: hello";
uint8_t Address[6];
uint8_t tx_buf[1024] = {
    0,
};

extern char RunTime[1024];
float cpu = 0;
bool HaveSend = 0;
mesh_addr_t route_table[CONFIG_MESH_ROUTE_TABLE_SIZE];

void esp_mesh_p2p_tx_main()
{
    int i;
    esp_err_t err;
    int send_count = 0;
    char msg1[CONFIG_MESH_ROUTE_TABLE_SIZE * 10] = "";
    mesh_data_t data;
    char  ms[200] = "{\
    \"nodeid\" : \"99\",\
    \"cpu_usage\" : \"99\",\
    \"temp\" : \"99\"\
    }";
    data.data = (uint8_t *)ms;
    data.size = sizeof(ms);
    data.proto = MESH_PROTO_BIN;
    data.tos = MESH_TOS_P2P;
    // extern char msg1[CONFIG_MESH_ROUTE_TABLE_SIZE * 10] = "";
    mesh_addr_t ParentsAddr;
    // while (1) {
        /* non-root do nothing but print */
        cpu = power();
        sprintf(ms,"%d:%.2f", NodeID, cpu);
        data_synthesis(msg1);
        if(strlen(msg1) != 0)
        {
            strcat(ms,",");
            strcat(ms,msg1);
        }
        int MyLayer = esp_mesh_get_layer();
        esp_mesh_send(NULL, &data, MESH_DATA_P2P , NULL, 0);
        printf("Send data: %s\r\n", data.data);
        HaveSend = 1;
}
void DelaySendData(void)
{
    while(1)
    {
        if(HaveSend == 0)
        {
            esp_mesh_p2p_tx_main();
        }
        HaveSend = 0;
        // if(esp_mesh_get_routing_table_size()!=0)
        // {
        //     vTaskDelay(1.2 * 1000 / portTICK_PERIOD_MS);
        // }
        vTaskDelay(1 * 100 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}
void tx_mesh_init(void)
{
    // xTaskCreatePinnedToCore(esp_mesh_p2p_tx_main, "MPTX", 8192, NULL, 5, NULL, 0);
    xTaskCreatePinnedToCore(DelaySendData, "DelaySendData", 8192, NULL, 5, NULL, 0);
}