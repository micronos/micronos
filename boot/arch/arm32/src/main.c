#include <stddef.h>
#include <stdint.h>
#include <common/stdlib.h>

#include <uart.h>
#include <mem.h>
#include <atag.h>

#include <kerio.h>
#include <gpu.h>




// uint32_t framebuffer_address = *(volatile uint32_t *)0x40040000;  // Đọc địa chỉ từ Mailbox


void bootstrap(uint32_t r0, uint32_t r1, uint32_t atags)
{
    // char buf[256];
    // Declare as unused
    (void) r0;
    (void) r1;
    (void) atags;

    uart_init();
    // puts("Initializing Memory Module\n");
    mem_init((atag_t *)atags);
    uart_puts("Initializing Memory Module\n");

    // gpu_init();

    // puts("Hello, kernel World!\n");

    while (1) {
        // gets(buf,256);
        // puts(buf);
        // putc('\n');
    }
}
