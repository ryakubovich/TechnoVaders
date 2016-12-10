CMake_minimum_required(VERSION 2.8)

add_definitions(--std=c++11)

# Set up your project name.
project(TechnoVaders)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

# Set up the main source folder.
set(SOURCE_ROOT src)
FILE(GLOB_RECURSE HEAD_LIST "src/*.hpp")
include_directories(3party/jsoncpp/include)

# Scan source folder to find all sources and put the result to SRC_LIST.
aux_source_directory(${SOURCE_ROOT} SRC_LIST)
qt5_wrap_cpp(QT_WRAPPED_SRC ${SRC_LIST})
aux_source_directory(3party/jsoncpp/src/lib_json JSONCPP_SRC)

# Create executable by SRC_LIST.
add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEAD_LIST} ${QT_WRAPPED_SRC} ${JSONCPP_SRC})

# Qt modules
qt5_use_modules(${PROJECT_NAME} Widgets OpenGL)

# Add subdirectory with Google Test Library and JSONCpp
add_subdirectory(3party/googletest)
add_subdirectory(3party/jsoncpp)

# Enable unit testing.
enable_testing()
# Set up testing project name.
set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)
# Add include directories for testing project.
include_directories(3party/googletest/googletest/include ${SOURCE_ROOT})
include_directories(3party/jsoncpp/include)
# Set up testing project.
set(TESTS_SOURCE_ROOT tests)
aux_source_directory(${TESTS_SOURCE_ROOT} TEST_SRC_FILES)
aux_source_directory(3party/jsoncpp/src/lib_json JSONCPP_SRC)
set(TEST_SRC_FILES ${SRC_LIST} ${TEST_SRC_FILES})
list(REMOVE_ITEM TEST_SRC_FILES src/main.cpp)
qt5_wrap_cpp(QT_WRAPPED_TEST_SRC ${TEST_SRC_FILES})
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES} ${JSONCPP_SRC} ${QT_WRAPPED_TEST_SRC})
# Link gtest and gtest_main libraries.
target_link_libraries(${PROJECT_TEST_NAME} gtest gtest_main)
qt5_use_modules(${PROJECT_TEST_NAME} Widgets OpenGL)
# Finish tests setting up.
add_test(test ${PROJECT_TEST_NAME})
set(CMAKE_BUILD_TYPE Debug)

macro(copy_files GLOBPAT DESTINATION)
  file(GLOB COPY_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${GLOBPAT})
  foreach(FILENAME ${COPY_FILES})
    set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}")
    set(DST "${DESTINATION}/${FILENAME}")
    add_custom_command(TARGET ${PROJECT_TEST_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SRC} ${DST})
  endforeach(FILENAME)
endmacro(copy_files)

set(DATA_FILES_LIST data/*.png data/settings.json data/log.txt data/levels/* data/background.jpg)
copy_files("${DATA_FILES_LIST}" ${CMAKE_CURRENT_BINARY_DIR})
