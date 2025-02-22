cmake_minimum_required(VERSION 3.16)
project(MICRONOS)

add_subdirectory(boot)

# Thêm tệp thực thi
# add_executable(kernel kernel/kernel.c)

# Liên kết thư viện (nếu có)
# target_link_libraries(MyApp PRIVATE some_library)

# Tùy chỉnh cờ biên dịch
# target_compile_options(kernel -Wall -Wextra -O2)

set(CMAKE_VERBOSE_MAKEFILE ON)
message(STATUS "C_FLAGS: ${CMAKE_C_FLAGS}")
message(STATUS "CXX_FLAGS: ${CMAKE_CXX_FLAGS}")