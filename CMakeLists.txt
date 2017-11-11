cmake_minimum_required (VERSION 3.1)
project(boost_testing)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)

# This is needed for compatibility with Boost 1.64
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

### Set output directories on a per-configuration base
# Single configuration
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
# Multi configuration
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
  set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/bin )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

if (MSVC)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if(UNIX AND NOT APPLE)
find_package(LibRt REQUIRED)
endif()
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS regex system filesystem date_time serialization REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

add_executable(base64               "${CMAKE_SOURCE_DIR}/src/base64.cpp")
add_executable(parser_json          "${CMAKE_SOURCE_DIR}/src/parser_json.cpp")
add_executable(parser_xml           "${CMAKE_SOURCE_DIR}/src/parser_xml.cpp")
add_executable(serialization_binary "${CMAKE_SOURCE_DIR}/src/serialization_binary.cpp" "${CMAKE_SOURCE_DIR}/src/serialization_classes.hpp")
add_executable(serialization_text   "${CMAKE_SOURCE_DIR}/src/serialization_text.cpp" "${CMAKE_SOURCE_DIR}/src/serialization_classes.hpp")
add_executable(udp_simple_cli       "${CMAKE_SOURCE_DIR}/src/udp_simple_cli.cpp" "${CMAKE_SOURCE_DIR}/src/udp_lib.hpp"  "${CMAKE_SOURCE_DIR}/src/md5.cpp"  "${CMAKE_SOURCE_DIR}/src/md5.h")
add_executable(udp_simple_ser       "${CMAKE_SOURCE_DIR}/src/udp_simple_ser.cpp" "${CMAKE_SOURCE_DIR}/src/udp_lib.hpp"  "${CMAKE_SOURCE_DIR}/src/md5.cpp"  "${CMAKE_SOURCE_DIR}/src/md5.h")

target_link_libraries(base64               ${Boost_LIBRARIES} Threads::Threads)
target_link_libraries(parser_json          ${Boost_LIBRARIES} Threads::Threads)
target_link_libraries(parser_xml           ${Boost_LIBRARIES} Threads::Threads)
target_link_libraries(serialization_binary ${Boost_LIBRARIES} Threads::Threads)
target_link_libraries(serialization_text   ${Boost_LIBRARIES} Threads::Threads)
target_link_libraries(udp_simple_cli       ${Boost_LIBRARIES} Threads::Threads)
target_link_libraries(udp_simple_ser       ${Boost_LIBRARIES} Threads::Threads)

if(UNIX AND NOT APPLE)
target_link_libraries(base64               LIBRT::LIBRT )
target_link_libraries(parser_json          LIBRT::LIBRT )
target_link_libraries(parser_xml           LIBRT::LIBRT )
target_link_libraries(serialization_binary LIBRT::LIBRT )
target_link_libraries(serialization_text   LIBRT::LIBRT )
target_link_libraries(udp_simple_cli       LIBRT::LIBRT )
target_link_libraries(udp_simple_ser       LIBRT::LIBRT )
endif()