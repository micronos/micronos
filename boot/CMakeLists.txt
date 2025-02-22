
if(${ARCH} STREQUAL "arm32")
    add_subdirectory(arch/arm32)
elseif(${ARCH} STREQUAL "riscv32")
    add_subdirectory(arch/riscv32)
endif()
