
#include "encoder.h"

#define ENCODER_ARGLIST_SIZE 2


Encoder::Encoder(uint8_t pin1, uint8_t pin2) {

    // setup the pins
    pinMode(pin1, INPUT);
    digitalWrite(pin1, HIGH);
    pinMode(pin2, INPUT);
    digitalWrite(pin2, HIGH);


    encoder.pin1_register = PIN_TO_BASEREG(pin1);
    encoder.pin1_bitmask = PIN_TO_BITMASK(pin1);
    encoder.pin2_register = PIN_TO_BASEREG(pin2);
    encoder.pin2_bitmask = PIN_TO_BITMASK(pin2);
    encoder.position = 0;

    // allow time for a passive R-C filter to charge through the pullup resistors, before reading the initial state
    delayMicroseconds(2000);

    uint8_t s = 0;
    if (DIRECT_PIN_READ(encoder.pin1_register, encoder.pin1_bitmask)) s |= 1;
    if (DIRECT_PIN_READ(encoder.pin2_register, encoder.pin2_bitmask)) s |= 2;
    encoder.state = s;

    interruptArgs[0] = &encoder;
    interruptArgs[1] = &encoder;

    interrupts_in_use =  2;
    //interrupts_in_use += attach_interrupt(pin2, &encoder);
}

Encoder_internal_state_t * Encoder::interruptArgs[ENCODER_ARGLIST_SIZE];


inline int32_t Encoder::read() {
    if (interrupts_in_use < 2) {
        noInterrupts();
        update(&encoder);
    } else {
        noInterrupts();
    }
    int32_t ret = encoder.position;
    interrupts();
    return ret;
}

inline void Encoder::write(int32_t p) {
    noInterrupts();
    encoder.position = p;
    interrupts();
}


int8_t Encoder::update(Encoder_internal_state_t *arg) {

    uint8_t p1val = DIRECT_PIN_READ(arg->pin1_register, arg->pin1_bitmask);
    uint8_t p2val = DIRECT_PIN_READ(arg->pin2_register, arg->pin2_bitmask);
    uint8_t state = arg->state & 3;
    if (p1val) state |= 4;
    if (p2val) state |= 8;
    arg->state = (state >> 2);
    //long oldPosition = arg->position;
    switch (state) {
        case 1: case 7: case 8: case 14:
            arg->position++;
            break;
        case 2: case 4: case 11: case 13:
            arg->position--;
            break;
        case 3: case 12:
            arg->position += 2;
            break;
        case 6: case 9:
            arg->position -= 2;
            break;
    }
    // did position change?
    if (arg->oldPosition != arg->position) {
        // is it at detent?
        if (arg->position % 4 == 0) {
            // increase?
            if (arg->oldPosition < arg->position) {
                // update
                arg->oldPosition = arg->position;
                return 1;
            }
            // decrese!
            else {
                // update
                arg->oldPosition = arg->position;
                return -1;
            }
        }
    }
    return 0;
}

/*
uint8_t Encoder::attach_interrupt(uint8_t pin, Encoder_internal_state_t *state) {
    
    interruptArgs[0] = state;
    interruptArgs[1] = state;

    return 1;
}

*/





//                           _______         _______       
//               Pin1 ______|       |_______|       |______ Pin1
// negative <---         _______         _______         __      --> positive
//               Pin2 __|       |_______|       |_______|   Pin2

        //  new new old old
        //  pin2    pin1    pin2    pin1    Result
        //  ----    ----    ----    ----    ------
        //  0   0   0   0   no movement
        //  0   0   0   1   +1
        //  0   0   1   0   -1
        //  0   0   1   1   +2  (assume pin1 edges only)
        //  0   1   0   0   -1
        //  0   1   0   1   no movement
        //  0   1   1   0   -2  (assume pin1 edges only)
        //  0   1   1   1   +1
        //  1   0   0   0   +1
        //  1   0   0   1   -2  (assume pin1 edges only)
        //  1   0   1   0   no movement
        //  1   0   1   1   -1
        //  1   1   0   0   +2  (assume pin1 edges only)
        //  1   1   0   1   -1
        //  1   1   1   0   +1
        //  1   1   1   1   no movement
