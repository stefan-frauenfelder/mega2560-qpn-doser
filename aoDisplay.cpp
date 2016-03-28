/*****************************************************************************
* Model: mega2560-qpn-doser.qm
* File:  ./aoDisplay.cpp
*
* This code has been generated by QM tool (see state-machine.com/qm).
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*****************************************************************************/
/*${.::aoDisplay.cpp} ......................................................*/

//#include "qfn_port.h"
#include "qpn.h"
#include "board.h"
#include "activeObjects.h"

/*Q_DEFINE_THIS_FILE*/


/* Scale class declaration -----------------------------------------------*/
/*${components::aoDisplay} .................................................*/
typedef struct aoDisplay {
/* protected: */
    QActive super;
} aoDisplay;

/* protected: */
static QState aoDisplay_initial(aoDisplay * const me);
static QState aoDisplay_displayWeight(aoDisplay * const me);
static QState aoDisplay_displayResult(aoDisplay * const me);
static QState aoDisplay_displayIdle(aoDisplay * const me);


/* Global objects ----------------------------------------------------------*/
aoDisplay AO_Display;     /* the single instance of the Encoder active object */

/* Scale class definition ------------------------------------------------*/
/*${components::aoDisplay_constructor} .....................................*/
void aoDisplay_constructor(void) {
    QActive_ctor(&AO_Display.super, Q_STATE_CAST(&aoDisplay_initial));
}
/*${components::aoDisplay} .................................................*/
/*${components::aoDisplay::SM} .............................................*/
static QState aoDisplay_initial(aoDisplay * const me) {
    /* ${components::aoDisplay::SM::initial} */
    return Q_TRAN(&aoDisplay_displayIdle);
}
/*${components::aoDisplay::SM::displayWeight} ..............................*/
static QState aoDisplay_displayWeight(aoDisplay * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* ${components::aoDisplay::SM::displayWeight} */
        case Q_ENTRY_SIG: {
            BSP_displayPosition(Weight);
            status_ = Q_HANDLED();
            break;
        }
        /* ${components::aoDisplay::SM::displayWeight::DISPLAY_RESULT} */
        case DISPLAY_RESULT_SIG: {
            status_ = Q_TRAN(&aoDisplay_displayResult);
            break;
        }
        /* ${components::aoDisplay::SM::displayWeight::DISPLAY_IDLE} */
        case DISPLAY_IDLE_SIG: {
            status_ = Q_TRAN(&aoDisplay_displayIdle);
            break;
        }
        /* ${components::aoDisplay::SM::displayWeight::REFRESH} */
        case REFRESH_SIG: {
            status_ = Q_TRAN(&aoDisplay_displayWeight);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*${components::aoDisplay::SM::displayResult} ..............................*/
static QState aoDisplay_displayResult(aoDisplay * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* ${components::aoDisplay::SM::displayResult} */
        case Q_ENTRY_SIG: {
            BSP_displayPosition(BSP_scaleAverageWeight());
            status_ = Q_HANDLED();
            break;
        }
        /* ${components::aoDisplay::SM::displayResult::DISPLAY_IDLE} */
        case DISPLAY_IDLE_SIG: {
            status_ = Q_TRAN(&aoDisplay_displayIdle);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*${components::aoDisplay::SM::displayIdle} ................................*/
static QState aoDisplay_displayIdle(aoDisplay * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* ${components::aoDisplay::SM::displayIdle} */
        case Q_ENTRY_SIG: {
            BSP_displayPosition(0.0);
            status_ = Q_HANDLED();
            break;
        }
        /* ${components::aoDisplay::SM::displayIdle::DISPLAY_WEIGHT} */
        case DISPLAY_WEIGHT_SIG: {
            status_ = Q_TRAN(&aoDisplay_displayWeight);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

