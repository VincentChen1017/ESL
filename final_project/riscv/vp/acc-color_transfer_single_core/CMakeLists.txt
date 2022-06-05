file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(platform-acc-color_transfer_single_core
ethernet.cpp
display.cpp
${HEADERS})

target_include_directories(platform-acc-color_transfer_single_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(riscv-vp-acc-color_transfer_single_core
        main.cpp)

target_link_libraries(riscv-vp-acc-color_transfer_single_core rv32 platform-acc-color_transfer_single_core platform-common gdb-mc ${Boost_LIBRARIES} ${SystemC_LIBRARIES} pthread)

INSTALL(TARGETS riscv-vp RUNTIME DESTINATION bin)
