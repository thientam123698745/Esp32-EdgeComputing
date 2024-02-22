/* Mesh Internal Communication Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#ifndef __MESH_LIGHT_H__
#define __MESH_LIGHT_H__

#include "esp_err.h"
#include "nvs_flash.h"
#include "esp_http_client.h"
#include <string.h>
#include "esp_wifi.h"
#include "esp_mac.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_mesh.h"
#include "esp_mesh_internal.h"
#include "cJSON.h"

/*******************************************************
 *                Constants
 *******************************************************/

#define  MESH_TOKEN_ID       (0x0)
#define  MESH_TOKEN_VALUE    (0xbeef)
#define  MESH_CONTROL_CMD    (0x2)
#define NodeID 0
#define DelayPostInSecond 10
#define ReiceiveBufferSize 40
#define ReiceiveMessageSize 60
#define iDelay 100
#define SendLoop 1
#define SendLoop2 0
#define sendLoopDelay 5
#define checkCPU 0
#define SendCommandPerSecond 1
#define Delay1 300
#define Delay2 300
#define Delay3 300
#define MaxCPU 30
/*******************************************************
 *                Type Definitions
 *******************************************************/

/*******************************************************
 *                Structures
 *******************************************************/
typedef struct {
    uint8_t cmd;
    bool on;
    uint8_t token_id;
    uint16_t token_value;
} mesh_light_ctl_t;

/*******************************************************
 *                Variables Declarations
 *******************************************************/

/*******************************************************
 *                Function Definitions
 *******************************************************/
void mesh_connected_indicator(int layer);
void mesh_disconnected_indicator(void);
void receive_data_init(void);
void array_loop(void *p);
//node_manager
void registerNode(mesh_addr_t addr, int nodeid);
mesh_addr_t getAddr(int nodeid);
void printListNode(void);
void writeNodeCpu(char *cpu, int nodeid);
void getNodeCpu(char *cpu, int nodeid);
bool didGetNodeMessage(int nodeid);
int getNumberOfNode(void);
bool noderegister(int i);
void getNodeCpu2(char *cpu, int nodeid);

//command_send
void command_send_init(void);
void command2Handler();

//client_post
void client_post_init(void);

#endif /* __MESH_LIGHT_H__ */
