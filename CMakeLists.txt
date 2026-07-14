cmake_minimum_required(VERSION 3.16)
project(hmi_app VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------ find dependencies ------------------
find_package(PkgConfig REQUIRED)

# Helper: try pkg-config with version a, fall back to version b
macro(pkg_find_fallback NAME PKG_A PKG_B)
    pkg_check_modules(${NAME} IMPORTED_TARGET ${PKG_A})
    if(NOT ${NAME}_FOUND)
        pkg_check_modules(${NAME} REQUIRED IMPORTED_TARGET ${PKG_B})
    endif()
endmacro()

pkg_find_fallback(GLIBMM  glibmm-2.68  glibmm-2.4)
pkg_find_fallback(SIGCXX  sigc++-3.0   sigc++-2.0)
pkg_find_fallback(CAIROMM cairomm-1.16 cairomm-1.0)
pkg_find_fallback(PANGOMM pangomm-2.48 pangomm-1.4)
pkg_find_fallback(GTKMM   gtkmm-4.0    gtkmm-3.0)

# ------------------ sources ------------------
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/StatusWidget.cpp
    src/ControlPanel.cpp
    src/GaugeWidget.cpp
    src/LogView.cpp
)

set(HEADERS
    src/MainWindow.h
    src/StatusWidget.h
    src/ControlPanel.h
    src/GaugeWidget.h
    src/LogView.h
)

# ------------------ executable ------------------
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE
    PkgConfig::GTKMM
    PkgConfig::GLIBMM
    PkgConfig::SIGCXX
    PkgConfig::CAIROMM
    PkgConfig::PANGOMM
)

# ------------------ compile options ------------------
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
