#include "mesh_light.h"

#define RX_SIZE          (1500)
uint8_t rx_buf[RX_SIZE] = { 0, };
const char *MESH_TAG = "ROOT_RX";
char ms1[CONFIG_MESH_AP_CONNECTIONS * 10] = "";
char node_data[CONFIG_MESH_AP_CONNECTIONS][CONFIG_MESH_ROUTE_TABLE_SIZE * 10] = {0};
bool node_connect[CONFIG_MESH_ROUTE_TABLE_SIZE] = {0};
char msg1[CONFIG_MESH_ROUTE_TABLE_SIZE * 10] = "";
static mesh_addr_t mesh_parent_addr;
int mesh_layer = 0;
bool is_running = 0;
void data_analysis(char *ms)
{
    char *token;
    int nodeid = 0;
    int cpu_usage = 0;
    char ms2[CONFIG_MESH_AP_CONNECTIONS * 10] = "";
    sprintf(ms2, "%s", ms);
    token = strtok (ms,",:");
    nodeid = atoi(token);
    // printf("nodeid: %d\r\n", nodeid);
    // printf("cpu_usage: %d\r\n", cpu_usage);
    // node_data[nodeid] = ms;
    // sprintf(node_data[nodeid], "%s", ms1);
    strcpy(node_data[nodeid], ms2);
    // printf("Data_analasis ms2: %s\r\n", ms1);
    node_connect[nodeid] = 1;
    // printf("nodeid: %d\r\n", nodeid);
    node_connect[nodeid] = 1;
    // printf("node_data[%d]: %s\r\n", nodeid, node_data[nodeid]);
    // printf("node_connect[%d]: %d\r\n", nodeid, node_connect[nodeid]);
    // printf("node_data[%d]: %d\r\n", nodeid, node_data[nodeid]);
    // printf("node_connect[%d]: %d\r\n", nodeid, node_connect[nodeid]);
}
void rxdata_process()
{
    data_analysis(ms1);
    vTaskDelete(NULL);
}
char *data_synthesis(char* msg2)
{
    sprintf(msg2,"");
    for(int i = 1; i< 50+1; i++)
    {
        // printf("In loop node_data[%d]: %s\r\n", i, node_data[i]);
        // printf("In loop msg: %s\r\n", msg);
        // printf("node_connect[%d]: %d", i, node_connect[i]);
        if(node_connect[i] == 1)
        {
            if(strlen(msg2) != 0)
            {
                strcat(msg2, ",");
            }
            strcat(msg2, node_data[i]);
        }
        node_connect[i] = 0;
    }
    // printf("msg: %s\r\n", msg);
    return msg2;
}
// void esp_mesh_p2p_rx_main(void *arg)
// {
//     int recv_count = 0;
//     esp_err_t err;
//     mesh_addr_t from;
//     int send_count = 0;
//     mesh_data_t data;
//     int flag = 0;
//     data.data = rx_buf;
//     data.size = RX_SIZE;
//     extern int loop;
//     extern char data_rx;

//     char ms2[30] = "start_buffer";
//     while (1) {
//         esp_mesh_recv(&from, &data, portMAX_DELAY, &flag, NULL, 0);
//         printf("Data received: %s\r\n", data.data);
//         strcpy(ms1, (char*)data.data);
//         printf("ms1: %s\r\n", ms1);
        

//         if(strncmp("StartLoop", ms1, sizeof(ms1)) == 0)
//         {
//             loop = 1;
//         }
//         else
//         if (strstr(ms1, "start_buffer") != NULL)
//         {
//             loop = 2;
//         }
//         else
//         (
//             xTaskCreate(rxdata_process, "rxdata_process", 3072, NULL, 4, NULL));
//         // printf("loop: %s\r\n",loop);
//         vTaskDelay(1 * 200 / portTICK_PERIOD_MS);
        
//     }
//     vTaskDelete(NULL);
// }
void esp_mesh_p2p_rx_main(void *arg)
{
    int recv_count = 0;
    esp_err_t err;
    mesh_addr_t from;
    int send_count = 0;
    mesh_data_t data;
    extern int loop;
    int flag = 0;
    data.data = rx_buf;
    data.size = RX_SIZE;
    is_running = true;

    while (is_running) {
        data.size = RX_SIZE;
        err = esp_mesh_recv(&from, &data, portMAX_DELAY, &flag, NULL, 0);

        if (err != ESP_OK || !data.size) {
            ESP_LOGE(MESH_TAG, "err:0x%x, size:%d", err, data.size);
            continue;
        }
        
        printf("Receive data2: %s\r\n", data.data);

        if (data.size >= sizeof(send_count)) {
            send_count = (data.data[25] << 24) | (data.data[24] << 16)
                         | (data.data[23] << 8) | data.data[22];
        }
        strcpy(ms1, (char*)data.data);
        // printf("ms1: %s\r\n", ms1);
        if(strncmp("StartLoop", ms1, 9) == 0)
        {
            loop = 1;
        }
        else
        if (strstr(ms1, "start_buffer") != NULL)
        {
            loop = 2;
        }
        else
        if (strncmp(ms1, "turn_off", sizeof(ms1)) != NULL)
        {
            esp_restart();
        }
        else
        (
            xTaskCreate(rxdata_process, "rxdata_process", 3072, NULL, 4, NULL));
        // recv_count++;
        /* process light control */
        // mesh_light_process(&from, data.data, data.size);
        esp_mesh_get_parent_bssid(&mesh_parent_addr);
        mesh_layer = esp_mesh_get_layer();
        if (!(recv_count % 1)) {
            ESP_LOGW(MESH_TAG,
                     "[#RX:%d/%d][L:%d] parent:"MACSTR", receive from "MACSTR", size:%d, heap:%d, flag:%d[err:0x%x, proto:%d, tos:%d]",
                     recv_count, send_count, mesh_layer,
                     MAC2STR(mesh_parent_addr.addr), MAC2STR(from.addr),
                     data.size, esp_get_minimum_free_heap_size(), flag, err, data.proto,
                     data.tos);
        }
    }
    vTaskDelete(NULL);
}
void rx_mesh_init(void)
{
    xTaskCreate(esp_mesh_p2p_rx_main, "MPRX", 3072, NULL, 3, NULL);
}