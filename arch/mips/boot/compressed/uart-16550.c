/*
 * 16550 compatible uart based serial debug support for zboot
 */

#include <linux/types.h>
#include <linux/serial_reg.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18

#include <asm/addrspace.h>

#if defined(CONFIG_MACH_LOONGSON) || defined(CONFIG_MIPS_MALTA)
#define UART_BASE 0x1fd003f8
#define PORT(offset) (CKSEG1ADDR(UART_BASE) + (offset))
#endif

#ifdef CONFIG_AR7
#include <ar7.h>
#define PORT(offset) (CKSEG1ADDR(AR7_REGS_UART0) + (4 * offset))
#endif

#ifdef CONFIG_MACH_JZ4740
<<<<<<< HEAD
<<<<<<< HEAD
#define UART0_BASE  0xB0030000
#define PORT(offset) (UART0_BASE + (4 * offset))
=======
=======
>>>>>>> v3.18
#include <asm/mach-jz4740/base.h>
#define PORT(offset) (CKSEG1ADDR(JZ4740_UART0_BASE_ADDR) + (4 * offset))
#endif

#ifdef CONFIG_CPU_XLR
#define UART0_BASE  0x1EF14000
#define PORT(offset) (CKSEG1ADDR(UART0_BASE) + (4 * offset))
#define IOTYPE unsigned int
#endif

#ifdef CONFIG_CPU_XLP
#define UART0_BASE  0x18030100
#define PORT(offset) (CKSEG1ADDR(UART0_BASE) + (4 * offset))
#define IOTYPE unsigned int
#endif

#ifndef IOTYPE
#define IOTYPE char
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif

#ifndef PORT
#error please define the serial port address for your own machine
#endif

static inline unsigned int serial_in(int offset)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return *((char *)PORT(offset));
=======
	return *((volatile IOTYPE *)PORT(offset)) & 0xFF;
>>>>>>> v3.18
=======
	return *((volatile IOTYPE *)PORT(offset)) & 0xFF;
>>>>>>> v3.18
}

static inline void serial_out(int offset, int value)
{
<<<<<<< HEAD
<<<<<<< HEAD
	*((char *)PORT(offset)) = value;
=======
	*((volatile IOTYPE *)PORT(offset)) = value & 0xFF;
>>>>>>> v3.18
=======
	*((volatile IOTYPE *)PORT(offset)) = value & 0xFF;
>>>>>>> v3.18
}

void putc(char c)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int timeout = 1024;
=======
	int timeout = 1000000;
>>>>>>> v3.18
=======
	int timeout = 1000000;
>>>>>>> v3.18

	while (((serial_in(UART_LSR) & UART_LSR_THRE) == 0) && (timeout-- > 0))
		;

	serial_out(UART_TX, c);
}
