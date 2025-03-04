#include <stddef.h>
#include <stdint.h>
#include <uart.h>
#include <common/stdlib.h>

// Memory-Mapped I/O output
void mmio_write(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t *)reg = data;
}

// Memory-Mapped I/O input
uint32_t mmio_read(uint32_t reg)
{
    return *(volatile uint32_t *)reg;
}

// Loop <delay> times in a way that the compiler won't optimize away
void delay(int32_t count)
{
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
                 : "=r"(count) : [count] "0"(count) : "cc");
}

void uart_init()
{
    uart_control_t control;
    // Disable UART0.
    bzero(&control, 4);
    mmio_write(UART0_CR, control.as_int);

    // Setup the GPIO pin 14 && 15.
    // Disable pull up/down for all GPIO pins & delay for 150 cycles.
    mmio_write(GPPUD, 0x00000000);
    delay(150);

    // Disable pull up/down for pin 14,15 & delay for 150 cycles.
    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);

    // Write 0 to GPPUDCLK0 to make it take effect.
    mmio_write(GPPUDCLK0, 0x00000000);

    // Clear pending interrupts.
    mmio_write(UART0_ICR, 0x7FF);

    // Set integer & fractional part of baud rate.
    // Divider = UART_CLOCK/(16 * Baud)
    // Fraction part register = (Fractional part * 64) + 0.5
    // UART_CLOCK = 3000000; Baud = 115200.

    // Divider = 3000000 / (16 * 115200) = 1.627 = ~1.
    mmio_write(UART0_IBRD, 1);
    // Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
    mmio_write(UART0_FBRD, 40);

    // Enable FIFO & 8 bit data transmissio (1 stop bit, no parity).
    mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

    // Mask all interrupts.
    mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
                               (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

    // Enable UART0, receive & transfer part of UART.
    control.uart_enabled = 1;
    control.transmit_enabled = 1;
    control.receive_enabled = 1;
    mmio_write(UART0_CR, control.as_int);
}

uart_flags_t read_flags(void)
{
    uart_flags_t flags;
    flags.as_int = mmio_read(UART0_FR);
    return flags;
}

void uart_putc(unsigned char c)
{
    uart_flags_t flags;
    // Wait for UART to become ready to transmit.

    do
    {
        flags = read_flags();
    } while (flags.transmit_queue_full);
    mmio_write(UART0_DR, c);
}

unsigned char uart_getc()
{
    // Wait for UART to have received something.
    uart_flags_t flags;
    do
    {
        flags = read_flags();
    } while (flags.recieve_queue_empty);
    return mmio_read(UART0_DR);
}

//// =======================//

char getc_debug(void) {
    return uart_getc();
}

void putc_debug(char c) {
    uart_putc(c);
}

void uart_puts(const char * str) {
    int i;
    for (i = 0; str[i] != '\0'; i ++)
    putc_debug(str[i]);
}

void uart_gets(char * buf, int buflen) {
    int i;
    char c;
    // Leave a spot for null char in buffer
    for (i = 0; (c = getc_debug()) != '\r' && buflen > 1; i++, buflen--) {
        putc_debug(c);
        buf[i] = c;
    }

    putc_debug('\n');
    if (c == '\n') {
        buf[i] = '\0';
    }
    else
        buf[buflen-1] = '\0';
}

