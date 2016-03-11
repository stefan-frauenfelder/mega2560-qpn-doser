
#ifndef signals_h
#define signals_h

#include "qpn.h"

enum GrinderSignals { // signals for the Grinder application
    SOME_SIG = Q_USER_SIG,
    BUTTON_DOWN_SIG,
    SCALE_TARE_SIG,
    SCALE_TARE_DONE_SIG,
    SCALE_START_SIG,
    SCALE_STOP_SIG,
    SCALE_SAMPLING_DONE_SIG,
    ENCODER_INC_SIG,
    ENCODER_DEC_SIG
};

#endif  /* signals_h */
