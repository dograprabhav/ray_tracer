
cmake_minimum_required ( VERSION 3.1.0...3.27.0 )

project ( RTWeekend LANGUAGES CXX )

# Set to C++11
set ( CMAKE_CXX_STANDARD          11 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )
set ( CMAKE_CXX_EXTENSIONS        OFF )

# Source
set ( v1 
    src/v1/main.cpp
)

set ( v2
    src/v2/main.cpp
    src/v2/color.h
    src/v2/ray.h
    src/v2/vec3.h
)

set ( v3
    src/v3/main.cpp
    src/v3/color.h
    src/v3/ray.h
    src/v3/vec3.h
)

set ( v4
    src/v4/main.cpp
    src/v4/interval.h
    src/v4/color.h
    src/v4/hittable.h
    src/v4/hittable_list.h
    src/v4/ray.h
    src/v4/commons.h
    src/v4/sphere.h
    src/v4/vec3.h
)

set ( v5
    src/v5/main.cpp
    src/v5/interval.h
    src/v5/color.h
    src/v5/hittable.h
    src/v5/hittable_list.h
    src/v5/material.h
    src/v5/ray.h
    src/v5/commons.h
    src/v5/sphere.h
    src/v5/vec3.h
)

set ( v6
    src/v6_final/main.cpp
    src/v6_final/interval.h
    src/v6_final/color.h
    src/v6_final/hittable.h
    src/v6_final/hittable_list.h
    src/v6_final/material.h
    src/v6_final/ray.h
    src/v6_final/commons.h
    src/v6_final/sphere.h
    src/v6_final/vec3.h
)

include_directories(src)

add_executable(v1 ${EXTERNAL} ${v1})
add_executable(v2 ${EXTERNAL} ${v2})
add_executable(v3 ${EXTERNAL} ${v3})
add_executable(v4 ${EXTERNAL} ${v4})
add_executable(v5 ${EXTERNAL} ${v5})
add_executable(v6 ${EXTERNAL} ${v6})