#include "mesh_light.h"

bool startloopnodenotsendlist[CONFIG_MESH_ROUTE_TABLE_SIZE] = {0};
int commandID = 0;
#define commandMaxSendCount 20; //so lan gui comman nhieu nhat





// void esp_mesh_p2p_tx_main(void *arg)
// {
//     // esp_mesh_get_routing_table((mesh_addr_t *) &route_table,
//     //                             CONFIG_MESH_ROUTE_TABLE_SIZE * 6, &route_table_size);
//     // printf("data send: %s\r\n", data.data);

//     data.data = (uint8_t*)command;
//     data.size = sizeof(command);
//     data.proto = MESH_PROTO_BIN;
//     data.tos = MESH_TOS_P2P;
//     mesh_addr_t sendaddr;
//     esp_err_t err;
//     for (i = 1; i < CONFIG_MESH_ROUTE_TABLE_SIZE; i++) {
//         if(listOfNode[i] == 1)
//         {
//             sendaddr = getAddr(i);
//             err = esp_mesh_send(&sendaddr, &data, MESH_DATA_P2P, NULL, 0);
//         }
//     }
// }
esp_err_t senddata(int nodeid, char* command)//gui data den nodeid
{
    mesh_data_t data;
    data.data = (uint8_t*)command;
    data.size = strlen(command);
    mesh_addr_t addr = getAddr(nodeid);
    if(noderegister(nodeid)== 1)
    {
        return esp_mesh_send(&addr, &data, MESH_DATA_P2P, NULL, 0);
    }
    return ESP_FAIL;
}

bool isListOfNodeAllNull(bool listOfNode[])//kiem tra danh sach listofnode co bang khong het hay khong
{
    for(int i = 0; i<CONFIG_MESH_ROUTE_TABLE_SIZE; i++)
    {
        if(listOfNode[i] == 1)
        return 0;
    }
    return 1;
}

bool sendDataToListOfNodeHandler(bool listOfNode[], char* command)//gui command den anh sach cac node listofNode// bij looix khong send towis tat ca node trong danh sach
{
     esp_err_t err;
     int max = commandMaxSendCount;
    for(int i = 0; i< max; i++)
    {
        // for(int j = 0; j<CONFIG_MESH_ROUTE_TABLE_SIZE;j++)//gui den tat ca cac node trong list
        int j = 0;
        while(j<CONFIG_MESH_ROUTE_TABLE_SIZE)
        {
            if(listOfNode[j] == 1)
            {
                if(senddata(j, command) == ESP_OK)//node nao gui duoc thi listofnode tuong ung = 0
                {
                    // listOfNode[j] = 0;
                }
            }
            j++;
            
        }
        // if(isListOfNodeAllNull(listOfNode) == 1)//kiem tra xem co phai tat ca cac node deu gui
        // {
        //     return 1;
        // }
        vTaskDelay(100);
    }
    return 0;
}

void makeNodeList(bool nodelist[], bool nodenotsendlist[])
{
    for(int i = 0; i< CONFIG_MESH_ROUTE_TABLE_SIZE; i++)
    {
        nodelist[i] = noderegister(i);
        // if(nodenotsendlist[i] == 0)
        // {
        //     nodelist[i] = noderegister(i);
        // }
        // else
        // {
        //     nodelist[i] = 0;
        // }
    }
}
void addToNotSendStartloopList(int nodeid)
{
    startloopnodenotsendlist[nodeid] = 1;
}
void resetNotSendStartLoopList()
{
    for(int i = 0; i< CONFIG_MESH_ROUTE_TABLE_SIZE;i++)
    {
        startloopnodenotsendlist[i] = 0;
    }
}
void customdelay(int n)
{
    for(int z=0; z<n ; z++)
    {
        // xTaskCreate(command2Handler, "command2Handler", 3072, NULL, 5, NULL);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        printf("Delay in %d\r\n", n-z);
    }
}
void sendAllNode(char *command)
{
    for(int j = 0; j< 10;j++)
    {
        mesh_data_t data;
        data.data = (uint8_t*)command;
        data.size = strlen(command);
        mesh_addr_t route_table[CONFIG_MESH_ROUTE_TABLE_SIZE];
        int route_table_size = 0;
        esp_mesh_get_routing_table(&route_table, CONFIG_MESH_ROUTE_TABLE_SIZE, route_table_size);
        for(int i = 0; i< route_table_size;i++)
        {
            if(esp_mesh_send(&route_table[i], &data, MESH_DATA_P2P, NULL, 0) != ESP_OK)
            {
                printf("send error\r\n");
            }
        }
        vTaskDelay(100);
    }
}
void startLoop_command()
{
        char command[10];
        bool nodelist[CONFIG_MESH_ROUTE_TABLE_SIZE] = {0};
        sprintf(command, "StartLoop:5000");
        printf("start delay loop");
        makeNodeList(nodelist,startloopnodenotsendlist );
        // printf("node list: \r\n");
        // for(int i = 0; i<CONFIG_MESH_ROUTE_TABLE_SIZE;i++)
        // {
        //     printf("nodelist[%d]: %d\r\n",i,nodelist[i]);
        // }
        
        printf("send loop\r\n");
        sendDataToListOfNodeHandler(nodelist, command);
        vTaskDelete(NULL);
}
void startLoopCommandHandler(void)//task se xu ly 
{
    bool nodelist[CONFIG_MESH_ROUTE_TABLE_SIZE] = {0};
    resetNotSendStartLoopList();
    int thisNodeID = commandID;
    char command[10];
    commandID++;
    //bat dau giai doan cho
    customdelay(Delay1);
    
    // sendAllNode(command);
    // customdelay(600);
        xTaskCreate(startLoop_command, "startLoop_command", 6024, NULL, 5, NULL);
    for(int i = 0;i<Delay2*SendCommandPerSecond;i++)
    {

        // xTaskCreate(startLoop_command, "startLoop_command", 6024, NULL, 5, NULL);
        if(checkCPU)
        {
            xTaskCreate(command2Handler, "command2Handler", 3072, NULL, 5, NULL);
        }
        if(SendLoop2)
        {
            xTaskCreate(startLoop_command, "startLoop_command", 6024, NULL, 5, NULL);
        }
        vTaskDelay(1000/SendCommandPerSecond/ portTICK_PERIOD_MS);
    }
    printf("start delay buffer");
    sprintf(command, "start_buffer");
    makeNodeList(nodelist,startloopnodenotsendlist );
    sendDataToListOfNodeHandler(nodelist, command);
    customdelay(Delay3);
    esp_mesh_stop();
    vTaskDelete(NULL);
}
esp_err_t sendTurnOffCommand(int nodeid)
{
    char command[30] = "turn_off";
    return senddata(nodeid, command);

}
bool checkIfBigger(char *cpu)
{
    char cpuint[5];
    sprintf(cpuint, "%s", strtok(cpu, "."));
    if(atoi(cpuint) >=MaxCPU)
    {
        // printf("Check if bigger: 1\r\n");
        return 1;
    }
    // printf("check if bigger: 0\r\n");
    return 0;
}
void command2Handler()
{
    char nodecpu[7];
    char command[10] = "turn_off";
        // printf("check commmand2\r\n");
        for(int i = 0; i<CONFIG_MESH_ROUTE_TABLE_SIZE;i++)
        {
            if(noderegister(i)==1)
            {
                getNodeCpu2(nodecpu, i);
                if(checkIfBigger(nodecpu) == 1)
                {
                    printf("command '%s' send to node %d\r\n",command, i);
                    for(int i = 0; i< 20;i++)
                    {
                        senddata(i, command);
                    }
                    startloopnodenotsendlist[i] = 1;
                }
            }
        }
    vTaskDelete(NULL);
}
void command2()
{
    while(1)
    {
        printf("command2 handler\r\n");
        vTaskDelay(1000);
        
    }
    vTaskDelete(NULL);
}
void startLoopCommand(void)
{
    xTaskCreate(startLoopCommandHandler, "startLoopCommandHandler", 9072, NULL, 5, NULL);
}
void command_send_init(void)
{
    startLoopCommand();
    // xTaskCreate(command2, "command2", 3072, NULL, 5, NULL);
    // xTaskCreate(esp_mesh_p2p_tx_main, "MPTX", 3072, NULL, 5, NULL);
    // xTaskCreate(delayLoopBuffer, "delayloop", 3072, NULL, 5, NULL);
}