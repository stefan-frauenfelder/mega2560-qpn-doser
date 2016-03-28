
#ifndef signals_h
#define signals_h

#include "qpn.h"

enum GrinderSignals { // signals for the Grinder application
    SOME_SIG = Q_USER_SIG,
    BUTTON_DOWN_SIG,
    REFRESH_SIG,
    SCALE_TARE_SIG,
    SCALE_TARE_DONE_SIG,
    SCALE_START_SIG,
    SCALE_STOP_SIG,
    SCALE_SAMPLING_DONE_SIG,
    SCALE_FREEZE_SIG,
    SCALE_FREEZE_DONE_SIG,
    ENCODER_INC_SIG,
    ENCODER_DEC_SIG,
    DOSE_REACHED_SIG,
    THRESHOLD_REACHED_SIG,
    NEGATIVE_REACHED_SIG,
    DISPLAY_IDLE_SIG,
    DISPLAY_WEIGHT_SIG,
    DISPLAY_RESULT_SIG
};

#endif  /* signals_h */
