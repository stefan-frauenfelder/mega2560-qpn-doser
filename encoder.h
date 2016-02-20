/* Encoder Library, for measuring quadrature encoded signals
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 * Copyright (c) 2011,2013 PJRC.COM, LLC - Paul Stoffregen <paul@pjrc.com>
 *
 * Version 1.2 - fix -2 bug in C-only code
 * Version 1.1 - expand to support boards with up to 60 interrupts
 * Version 1.0 - initial release
 */


#ifndef encoder_h_
#define encoder_h_

#include "Arduino.h"  // always include in your sketch
#include "pins_arduino.h"
#include "direct_pin_read.h"

#define ENCODER_ARGLIST_SIZE 2

// All the data needed by interrupts is consolidated into this ugly struct
// to facilitate assembly language optimizing of the speed critical update.
// The assembly code uses auto-incrementing addressing modes, so the struct
// must remain in exactly this order.
typedef struct {
    volatile IO_REG_TYPE * pin1_register;
    volatile IO_REG_TYPE * pin2_register;
    IO_REG_TYPE            pin1_bitmask;
    IO_REG_TYPE            pin2_bitmask;
    uint8_t                state;
    int32_t                position;
    int32_t                oldPosition;
} Encoder_internal_state_t;


class Encoder
{
	public:
	    Encoder(uint8_t pin1, uint8_t pin2);
	    inline int32_t read();
	    inline void write(int32_t p);

	    static Encoder_internal_state_t * interruptArgs[ENCODER_ARGLIST_SIZE];

	private:
	    Encoder_internal_state_t encoder;
	    uint8_t interrupts_in_use;


	public:
	    static int8_t update(Encoder_internal_state_t *arg);

	    static uint8_t attach_interrupt(uint8_t pin, Encoder_internal_state_t *state);
};

#endif
