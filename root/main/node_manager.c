#include "mesh_light.h"
mesh_addr_t node_addr[CONFIG_MESH_ROUTE_TABLE_SIZE];
bool node_register[CONFIG_MESH_ROUTE_TABLE_SIZE] = {0};
char node_cpu[CONFIG_MESH_ROUTE_TABLE_SIZE][10] = {0};
bool node_haveMessage[CONFIG_MESH_ROUTE_TABLE_SIZE] = {0};
int numberOfCpuMessage = 0;

void registerNode(mesh_addr_t addr, int nodeid)
{
    // if(node_register[nodeid] == 0)
    // {
        node_addr[nodeid] = addr;
        node_register[nodeid] = 1;
    // }
    // else
    // {
    //     if()
    // }
}

mesh_addr_t getAddr(int nodeid)
{
    return node_addr[nodeid];
}

void printListNode(void)
{
    // printf("List of node registered: ");
    // for(int i = 0; i<CONFIG_MESH_ROUTE_TABLE_SIZE; i++)
    // {
    //     if(node_haveMessage[i] == 1)
    //     {
    //         printf("%d ",i);
    //     }
    // }
    // printf("\r\n");
}

void writeNodeCpu(char *cpu, int nodeid)
{
    sprintf(node_cpu[nodeid], "%s", cpu);
    node_haveMessage[nodeid] = 1;
    numberOfCpuMessage++;
}

void getNodeCpu(char *cpu, int nodeid)
{
    sprintf(cpu, "%s", node_cpu[nodeid]);
    node_haveMessage[nodeid] = 0;
    numberOfCpuMessage--;
}

void getNodeCpu2(char *cpu, int nodeid)
{
    sprintf(cpu, "%s", node_cpu[nodeid]);
}

bool didGetNodeMessage(int nodeid)
{
    return node_haveMessage[nodeid];
}

int getNumberOfNode(void)
{
    return numberOfCpuMessage;
}

bool noderegister(int i)
{
    return node_register[i];
}