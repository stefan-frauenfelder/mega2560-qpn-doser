/*****************************************************************************
* Model: Doser.qm
* File:  ./grinder.cpp
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
/*${.::grinder.cpp} ........................................................*/

//#include "qfn_port.h"
#include "qpn.h"
#include "board.h"
#include "activeObjects.h"

/*Q_DEFINE_THIS_FILE*/


/* Scale class declaration -----------------------------------------------*/
/*${components::Grinder} ...................................................*/
typedef struct Grinder {
/* protected: */
    QActive super;

/* public: */
    float dose;
} Grinder;

/* protected: */
static QState Grinder_initial(Grinder * const me);
static QState Grinder_on(Grinder * const me);
static QState Grinder_idle(Grinder * const me);
static QState Grinder_grinding(Grinder * const me);


/* Global objects ----------------------------------------------------------*/
Grinder AO_Grinder;     /* the single instance of the Grinder active object */

/* Scale class definition ------------------------------------------------*/
/*${components::Grinder_constructor} .......................................*/
void Grinder_constructor(void) {
    QActive_ctor(&AO_Grinder.super, Q_STATE_CAST(&Grinder_initial));
}
/*${components::Grinder} ...................................................*/
/*${components::Grinder::SM} ...............................................*/
static QState Grinder_initial(Grinder * const me) {
    /* ${components::Grinder::SM::initial} */
    return Q_TRAN(&Grinder_on);
}
/*${components::Grinder::SM::on} ...........................................*/
static QState Grinder_on(Grinder * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* ${components::Grinder::SM::on::initial} */
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&Grinder_idle);
            break;
        }
        /* ${components::Grinder::SM::on::ENCODER_INC} */
        case ENCODER_INC_SIG: {
            TargetDose += GRINDER_DOSE_STEP;
            BSP_displayDose(TargetDose);
            status_ = Q_HANDLED();
            break;
        }
        /* ${components::Grinder::SM::on::ENCODER_DEC} */
        case ENCODER_DEC_SIG: {
            TargetDose -= GRINDER_DOSE_STEP;
            BSP_displayDose(TargetDose);
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*${components::Grinder::SM::on::idle} .....................................*/
static QState Grinder_idle(Grinder * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* ${components::Grinder::SM::on::idle} */
        case Q_ENTRY_SIG: {
            TargetDose = 7;
            status_ = Q_HANDLED();
            break;
        }
        /* ${components::Grinder::SM::on::idle::Q_TIMEOUT} */
        case Q_TIMEOUT_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&Grinder_on);
            break;
        }
    }
    return status_;
}
/*${components::Grinder::SM::on::grinding} .................................*/
static QState Grinder_grinding(Grinder * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        default: {
            status_ = Q_SUPER(&Grinder_on);
            break;
        }
    }
    return status_;
}
