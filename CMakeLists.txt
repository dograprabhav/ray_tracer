
cmake_minimum_required ( VERSION 3.1.0...3.27.0 )

project ( RTWeekend LANGUAGES CXX )

# Set to C++11
set ( CMAKE_CXX_STANDARD          11 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )
set ( CMAKE_CXX_EXTENSIONS        OFF )

# Source
set ( v1 
    src/v1_simple_image/main.cpp
)

include_directories(src)


add_executable(v1 ${EXTERNAL} ${v1})
add_executable(v2 ${EXTERNAL} ${v2})