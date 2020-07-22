cmake_minimum_required(VERSION 3.10)
project(easy_get)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pthread -Wall -Werror")

include_directories(src tests src/network src/utils)

set(sources ${sources})
set(sources ${sources} src/parse_input.cpp src/parse_input.h src/options_info.h)
set(sources ${sources} src/utils/common.h src/utils/iterator_range.h src/utils/paginator.h)
set(sources ${sources} src/utils/profile.h tests/test_runner.h)
set(sources ${sources} src/network/url.cpp src/network/url.h)
set(sources ${sources} src/network/socket.cpp src/network/socket.h)
set(sources ${sources} src/network/tcp_connection.cpp src/network/tcp_connection.h)
set(sources ${sources} src/network/http_client.cpp src/network/http_client.h src/network/http_parameters.h)
add_executable(easy_get src/main.cpp ${sources})

add_executable(test_url tests/test_url.cpp tests/test_runner.h ${sources})
add_executable(test_parse_input tests/test_parse_input.cpp tests/test_runner.h ${sources})
add_executable(test_http_queries tests/test_http_queries.cpp tests/test_runner.h ${sources})
