#include <stddef.h>
#include <stdint.h>
#include <common/stdlib.h>

#include <uart.h>
#include <mem.h>
#include <atag.h>

#include <kerio.h>
#include <gpu.h>




#define MAILBOX_BASE        0x3F00B880
#define MAILBOX_READ        ((volatile uint32_t *)(MAILBOX_BASE + 0x00))
#define MAILBOX_STATUS      ((volatile uint32_t *)(MAILBOX_BASE + 0x18))
#define MAILBOX_WRITE       ((volatile uint32_t *)(MAILBOX_BASE + 0x20))
#define MAILBOX_FULL        (1 << 31)
#define MAILBOX_EMPTY       (1 << 30)
#define MAILBOX_CHANNEL_FB  1

void mailbox_write(uint32_t message, uint8_t channel) {
    while (*MAILBOX_STATUS & MAILBOX_FULL);
    *MAILBOX_WRITE = (message & 0xFFFFFFF0) | (channel & 0xF);
}

uint32_t mailbox_reads(uint8_t channel) {
    uint32_t data;
    do {
        while (*MAILBOX_STATUS & MAILBOX_EMPTY);
        data = *MAILBOX_READ;
    } while ((data & 0xF) != channel);
    return data & 0xFFFFFFF0;
}

typedef struct {
    uint32_t width, height, vwidth, vheight, pitch, depth, x_offset, y_offset, address, size;
} framebuffer_t;

framebuffer_t fb __attribute__((aligned(16)));

int framebuffer_inits() {
    fb.width = 800;
    fb.height = 600;
    fb.vwidth = 800;
    fb.vheight = 600;
    fb.depth = 32;
    fb.x_offset = 0;
    fb.y_offset = 0;
    fb.address = 0;

    uint32_t message[8] __attribute__((aligned(16))) = {
        8 * 4, // Tổng kích thước buffer
        0,     // Yêu cầu
        0x40001, // Tag: Allocate framebuffer
        4, 4,   // Buffer size & request size
        0,      // Địa chỉ framebuffer (0 để yêu cầu hệ thống cấp phát)
        0       // Kết thúc
    };
    mailbox_write((uint32_t)message, MAILBOX_CHANNEL_FB);
    mailbox_reads(MAILBOX_CHANNEL_FB);
    fb.address = message[5];

    return (fb.address == 0) ? -1 : 0;
}

void draw_pixel(int x, int y, uint32_t color) {
    volatile uint32_t *framebuffer = (volatile uint32_t *) fb.address;
    framebuffer[y * (fb.pitch / 4) + x] = color;
}

const unsigned char font8x8_basic[128][8] = {
    ['H'] = {0x24, 0x24, 0x24, 0x3C, 0x24, 0x24, 0x24, 0x00},
    ['e'] = {0x00, 0x00, 0x3C, 0x02, 0x1E, 0x22, 0x1E, 0x00},
    ['l'] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00},
    ['o'] = {0x00, 0x00, 0x1E, 0x22, 0x22, 0x22, 0x1E, 0x00},
    ['W'] = {0x00, 0x00, 0x41, 0x41, 0x49, 0x77, 0x41, 0x00},
    ['r'] = {0x00, 0x00, 0x2C, 0x32, 0x20, 0x20, 0x20, 0x00},
    ['d'] = {0x00, 0x00, 0x1C, 0x22, 0x22, 0x22, 0x1E, 0x00},
    ['!'] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x04, 0x00},
};

void draw_char(int x, int y, char c, uint32_t color) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (font8x8_basic[(int)c][row] & (1 << col)) {
                draw_pixel(x + col, y + row, color);
            }
        }
    }
}

void draw_string(int x, int y, const char *s, uint32_t color) {
    while (*s) {
        draw_char(x, y, *s, color);
        x += 8;  // Ký tự 8x8, tăng x lên 8 pixel
        s++;
    }
}

int test_frame() {
    framebuffer_inits();
    draw_string(100, 100, "Hello, World!", 0xFFFFFF);
    while (1);
}




void bootstrap(uint32_t r0, uint32_t r1, uint32_t atags)
{
    // char buf[256];
    // Declare as unused
    (void) r0;
    (void) r1;
    (void) atags;

    uart_init();
    mem_init((atag_t *)atags);
    uart_puts("Initializing Memory Module\n");
    // test_frame();
    gpu_init();

    puts("Hello, kernel World!\n");

    while (1) {
        // gets(buf,256);
        // puts(buf);
        // putc('\n');
    }
}
