idf_component_register(SRCS "command_send.c" "node_manager.c" "cpu_usage.c" "receive_data.c" "client_post.c"
                            "mesh_main.c"
                    INCLUDE_DIRS "." "include")
target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
