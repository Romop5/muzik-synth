set( CMAKE_VERBOSE_MAKEFILE on )
set(CMAKE_CXX_FLAGS "-s -Os -fno-stack-protector -fno-ident -fno-rtti -fno-exceptions -fomit-frame-pointer")

add_library(muzik INTERFACE)
target_include_directories(muzik INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../)

add_executable(muzik-test
        main.cpp)
target_link_libraries(muzik-test PRIVATE muzik)
