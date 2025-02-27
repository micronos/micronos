enable_language(C ASM)

# Thêm target boot dưới dạng object library
add_library(asm OBJECT src/asm.S)
set_target_properties(asm PROPERTIES
    COMPILE_FLAGS "-mcpu=cortex-a7 -fpic -ffreestanding"
    LINKER_LANGUAGE "ASM"
    OUTPUT_NAME "asm"
)

# Thêm main.c
list(APPEND BOOT_SOURCE
    src/atag.c
    src/framebuffer.c
    src/gpu.c
    src/kerio.c
    src/mailbox.c
    src/main.c
    src/mem.c
    src/uart.c
    src/common/stdlib.c
)
add_library(boot OBJECT ${BOOT_SOURCE})
set_target_properties(boot PROPERTIES
    COMPILE_FLAGS "-mcpu=cortex-a7 -fpic -ffreestanding -std=gnu99 -O2 -Wall -Wextra"
    OUTPUT_NAME "boot"
)

# Chỉ định thư mục chứa header
target_include_directories(boot PRIVATE include)

# target_include_directories(MyExecutable PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Linker script
# set(LINKER_SCRIPT linker.ld)

# Tạo target kernel.elf từ boot và kernel
add_executable(boot.elf $<TARGET_OBJECTS:asm> $<TARGET_OBJECTS:boot>)
set_target_properties(boot.elf PROPERTIES
    LINK_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld -ffreestanding -O2 -nostdlib"
    OUTPUT_NAME "boot.elf"
)

# Chuyển ELF thành BIN
# add_custom_command(TARGET boot.elf POST_BUILD
# COMMAND ${CMAKE_OBJCOPY} --change-start=0x8000 -O binary boot.elf boot.bin
# COMMENT "Converting ELF to BIN"
# )

# Chuyển ELF thành BIN
set(BOOT_BIN "boot.bin")
set(ROOT_BUILD_DIR "${CMAKE_BINARY_DIR}")

add_custom_command(TARGET boot.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary boot.elf ${BOOT_BIN}
    COMMAND ${CMAKE_COMMAND} -E copy ${BOOT_BIN} ${ROOT_BUILD_DIR}/
    COMMENT "Converting ELF to IMG"
)


# 0x18000