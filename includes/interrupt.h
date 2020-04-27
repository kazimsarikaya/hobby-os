/**
 * @file interrupt.h
 * @brief defines interrupt functions at long mode
 */
#ifndef ___INTERRUPT_H
/*! prevent duplicate header error macro */
#define ___INTERRUPT_H 0

#if ___BITS == 64 || DOXYGEN

/**
 * @brief interrupt error code type
 */
#define interrupt_errcode_t uint64_t

/**
 * @struct interrupt_frame
 * @brief  interrupt frame for interrupt function
 */
typedef struct interrupt_frame {
	uint64_t return_rip; ///< the ip value after interrupt
	uint16_t return_cs; ///< the cs value after intterupt
	uint64_t empty1 : 48; ///< unused value
	uint64_t return_rflags; ///< the rflags after interrupt
	uint64_t return_rsp; ///< the rsp value aka stack after interrupt
	uint16_t return_ss; ///< the ss value aka stach segment after interrupt
	uint64_t empty2 : 48; ///< unused value
} __attribute__((packed)) interrupt_frame_t; ///< struct short hand

/**
 * @brief interrupt table builder functions
 *
 * builds interrupt table at idtr look for @ref descriptor_idt
 */
void interrupt_init();

#endif

#endif
