set( CMAKE_VERBOSE_MAKEFILE on )
set(CMAKE_CXX_FLAGS "-s -Os -fno-stack-protector -fno-ident -fno-rtti -fno-exceptions -fomit-frame-pointer")
add_executable(muzik
        muzik.hpp
        main.cpp)
