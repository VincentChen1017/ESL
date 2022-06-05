file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(platform-acc-color_transfer
ethernet.cpp
display.cpp
${HEADERS})

target_include_directories(platform-acc-color_transfer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(riscv-vp-acc-color_transfer
        main.cpp)

target_link_libraries(riscv-vp-acc-color_transfer rv32 platform-acc-color_transfer platform-common gdb-mc ${Boost_LIBRARIES} ${SystemC_LIBRARIES} pthread)

INSTALL(TARGETS riscv-vp RUNTIME DESTINATION bin)
