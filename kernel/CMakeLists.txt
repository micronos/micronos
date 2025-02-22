
PROJECT(KERNEL)

include(kernel.cmake)

enable_language(ASM)

set(CMAKE_ASM_FLAGS "-mcpu=cortex-a7 -fpic -ffreestanding")
set(CMAKE_C_FLAGS "-mcpu=cortex-a7 -nostdlib -fpic -ffreestanding -std=gnu99 -O2 -Wall -Wextra")

# set(CMAKE_EXE_LINKER_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")

set(KERNEL_SOURCE ${SOURCE})
add_executable(kernel
    ${KERNEL_SOURCE}
)


add_executable(boot boot.S)
# set_target_properties(boot PROPERTIES LINKER_LANGUAGE ASM)
target_link_options(boot PRIVATE
    -T ${CMAKE_SOURCE_DIR}/kernel/linker.ld
)

# target_link_libraries(boot boot.S)

# Thêm linker script
target_link_options(kernel PRIVATE
    -T ${CMAKE_SOURCE_DIR}/kernel/linker.ld
)

# Bật tối ưu hóa code (cho ARM Cortex-M)
# target_compile_options(kernel PRIVATE
#     -mcpu=cortex-a7
#     -fpic -ffreestanding 
#     -std=gnu99
#     -Wall
#     -Wextra
#     -mthumb
#     -Os  # Optimize for size
# )


# Tạo file `kernel.img` để chạy trên QEMU
# add_custom_command(
#     TARGET kernel POST_BUILD
#     COMMAND ${CMAKE_OBJCOPY} -O binary kernel kernel.img
#     COMMENT "Generating kernel.img"
# )