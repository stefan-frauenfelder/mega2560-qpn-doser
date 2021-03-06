/*****************************************************************************
* Model: mega2560-qpn-doser.qm
* File:  ./activeObjects.h
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
/*${.::activeObjects.h} ....................................................*/
#ifndef activeobjects_h
#define activeobjects_h

//#include "qfn_port.h"
#include "qpn.h"
#include "board.h"
#include "activeObjects.h"

#include "config.h"               // instance specific configuration

#include "signals.h"

// active objects
extern struct aoScale     AO_Scale;
extern struct aoGrinder   AO_Grinder;
extern struct aoEncoder   AO_Encoder;
extern struct aoDisplay   AO_Display;
extern struct aoWeight    AO_Weight;

extern float TargetDose;

extern float Weight;


/*${components::aoScale_constructor} .......................................*/
void aoScale_constructor(void);

/*${components::aoGrinder_constructor} .....................................*/
void aoGrinder_constructor(void);

/*${components::aoEncoder_constructor} .....................................*/
void aoEncoder_constructor(void);

/*${components::aoDisplay_constructor} .....................................*/
void aoDisplay_constructor(void);

/*${components::aoWeight_constructor} ......................................*/
void aoWeight_constructor(void);


#endif /* activeobjects_h */
