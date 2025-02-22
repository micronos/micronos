enable_language(C ASM)

# Thêm target boot dưới dạng object library
add_library(boot OBJECT boot.S)
set_target_properties(boot PROPERTIES
    COMPILE_FLAGS "-mcpu=cortex-a7 -fpic -ffreestanding"
    LINKER_LANGUAGE "ASM"
    OUTPUT_NAME "boot"
)

# Thêm kernel.c
add_library(kernel OBJECT kernel.c)
set_target_properties(kernel PROPERTIES
    COMPILE_FLAGS "-mcpu=cortex-a7 -fpic -ffreestanding -std=gnu99 -O2 -Wall -Wextra"
    OUTPUT_NAME "kernel"
)

# target_include_directories(MyExecutable PRIVATE ${CMAKE_SOURCE_DIR}/include)


# Linker script
set(LINKER_SCRIPT linker.ld)

# Tạo target kernel.elf từ boot và kernel
add_executable(kernel.elf $<TARGET_OBJECTS:boot> $<TARGET_OBJECTS:kernel>)
set_target_properties(kernel.elf PROPERTIES
    LINK_FLAGS  "-T ${CMAKE_SOURCE_DIR}/boot/arch/arm32/linker.ld -ffreestanding -O2 -nostdlib"
    OUTPUT_NAME "kernel.elf"
)


# 0x18000