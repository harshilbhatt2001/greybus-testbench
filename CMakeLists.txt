cmake_minimum_required(VERSION 3.22)
project(greybus_testbench C)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-Wall")

set(HEADER_FILES debug.h controller.h greybus_protocols.h greybus_manifest.h greybus-testbench.h controllers/uart_controller.h)

add_library(libgreybus_testbench connection.c controller.c interface.c manifest.c svc.c control.c controllers/uart_controller.c ${HEADER_FILES})
target_include_directories(libgreybus_testbench PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(greybus_testbench main.c ${HEADER_FILES})
target_include_directories(greybus_testbench PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(greybus_testbench libgreybus_testbench)