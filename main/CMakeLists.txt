cmake_minimum_required(VERSION 3.16)   # Yêu cầu phiên bản CMake tối thiểu

idf_component_register(SRCS "main.c"          
                       INCLUDE_DIRS "."
                       REQUIRES sim_4g uart_driver)

