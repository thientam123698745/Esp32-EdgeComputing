idf_component_register(SRCS "array_loop.c" "cpu_usage.c" "data_receive.c" "data_send.c" "mesh_light.c"
                            "mesh_main.c"
                    INCLUDE_DIRS "." "include")
target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
