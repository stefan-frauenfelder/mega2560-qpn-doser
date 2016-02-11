
#ifndef board_h
#define board_h

#include "qpn.h"

#include "Arduino.h"  // always include in your sketch

#include "signals.h"

#include <SPI.h>

#include <SimpleEncoder.h>

#include <stdlib.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>

#include "HX711.h"


void BSP_init(void);
void BSP_ledOn(void);
void BSP_ledOff(void);

void BSP_displayPosition(float position); 
void BSP_displayDose(float dose); 

float BSP_scaleSample(void);


#endif  // board_h
