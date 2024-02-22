#include "mesh_light.h"

bool Is_post_msg_not_null = 0;
char post_msg[CONFIG_MESH_ROUTE_TABLE_SIZE * 15] = "{";


esp_err_t client_post_handler(esp_http_client_event_handle_t evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        printf("HTTP_EVENT_ON_DATA: %.*s\n", evt->data_len, (char *)evt->data);
        break;

    default:
        break;
    }
    return ESP_OK;
}

void client_post_function(char* mes)
{
    esp_http_client_config_t config_post = {
        .url = "http://192.168.137.1:3001/api/createRecord11",
        .method = HTTP_METHOD_POST,
        .event_handler = client_post_handler};
        
    esp_http_client_handle_t client_post = esp_http_client_init(&config_post);

    char *post_data = mes;

    esp_http_client_set_post_field(client_post, post_data, strlen(post_data));
    esp_http_client_set_header(client_post, "Content-Type", "application/json");

    esp_http_client_perform(client_post);
    esp_http_client_cleanup(client_post);
}
void client_delete_data_function()
{
    esp_http_client_config_t config_get = {
        .url = "http://192.168.137.1:3001/api/deletealldata",
        .method = HTTP_METHOD_POST,
        .event_handler = client_post_handler};
        
    esp_http_client_handle_t client_get = esp_http_client_init(&config_get);
    char *post_data = "";

    esp_http_client_set_post_field(client_get, post_data, sizeof(post_data));
    esp_http_client_set_header(client_get, "Content-Type", "application/json");

    esp_http_client_perform(client_get);
    esp_http_client_cleanup(client_get);
}
void msg_create(void)
{
    sprintf(post_msg,"{");
    for(int i = 1; i<CONFIG_MESH_ROUTE_TABLE_SIZE;i++)
    {
        char sub_msg[5];
        if(Is_post_msg_not_null == 1)
        {
            
            if(didGetNodeMessage(i) == 1)
            {
                strcat(post_msg,",\"node");
                sprintf(sub_msg, "%d",i);
                strcat(post_msg,sub_msg);
                strcat(post_msg,"\":");
                getNodeCpu(sub_msg, i);
                strcat(post_msg,sub_msg);
            }
        }
        else
        if (didGetNodeMessage(i) == 1)//kiểm tra xem node có kết nối hay không 
        {
            strcat(post_msg,"\"node");
            sprintf(sub_msg, "%d",i);
            strcat(post_msg,sub_msg);
            strcat(post_msg,"\":");
            getNodeCpu(sub_msg, i);
            strcat(post_msg,sub_msg);
            Is_post_msg_not_null = 1;
        }
    }
    strcat(post_msg,"}");
}
void client_post_data()
{
    // printf("Endter client post\r\n");
    // printf("Is_post_msg_not_null: %d\r\n", Is_post_msg_not_null);
    msg_create();
    printListNode();
    if(Is_post_msg_not_null == 1)
    {
        // printf("Number of node connected: %d\r\n", NoN);
        printf("post_msg: %s\r\n",post_msg);
        client_post_function(post_msg);
        Is_post_msg_not_null = 0;
        
    }
}

void DelayPost()
{
    while(1)
    {
        vTaskDelay(1000 * DelayPostInSecond/ portTICK_PERIOD_MS);
        client_post_data();
    }
}

void client_post_init(void)
{
    client_delete_data_function();
    xTaskCreate(DelayPost, "DelayPost", 3072, NULL, 5, NULL);
    return;
}
    