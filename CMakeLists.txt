cmake_minimum_required(VERSION 3.15)
project(ArithmeticCoding)

if(MSVC)
    add_compile_options(/utf-8)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(wxWidgets_ROOT_DIR $ENV{WXWIN})
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# Core sources
set(CORE_SOURCES
    src/core/ArithmeticCoder.cpp
    src/core/Symbol.h
)

# UI sources
set(UI_SOURCES
    src/ui/MainFrame.cpp
    src/ui/MainFrame.h
)

# Main application source
set(MAIN_SOURCES
    src/main.cpp
)

set(ALL_SOURCES ${CORE_SOURCES} ${UI_SOURCES} ${MAIN_SOURCES})

add_executable(Arithmetic WIN32 ${ALL_SOURCES})

target_include_directories(Arithmetic PRIVATE src)
target_link_libraries(Arithmetic ${wxWidgets_LIBRARIES})
target_compile_definitions(Arithmetic PRIVATE UNICODE _UNICODE)