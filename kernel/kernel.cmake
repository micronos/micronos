# include_directories(
#     ${MONOOS_SOURCE_DIR}/ 
#     ${CMAKE_CURRENT_BINARY_DIR}/include
#     ${CMAKE_CURRENT_BINARY_DIR}/include/internal
# )

list(APPEND SOURCE
    # boot.S
    kernel.c
)
