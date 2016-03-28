
#ifndef config_h
#define config_h


#define BSP_TICKS_PER_SEC  100U


// button with led
#define BUTTON_LED_PIN 30
#define BUTTON_SWITCH_PIN 21

// encoder port
#define ENCODER_CTL DDRD //encoder port control
#define ENCODER_WR PORTD //encoder port write
#define ENCODER_RD PIND //encoder port read
#define ENCODER_A 2
#define ENCODER_B 3

// scale
#define SCALE_DOUT  22
#define SCALE_CLK   23
#define SCALE_CALIBRATION_FACTOR -444190.0 //This value is obtained using the SparkFun_HX711_Calibration sketch

#define FLOAT_PRECISION 1

// OLED
//#define SPI_MOSI 51
//#define SPI_MISO 50
//#define SPI_CLK  52
#define OLED_DC    49
#define OLED_CS    53
#define OLED_RESET 48

// sensor general
const int ONESEC_TOUT               =  1 * BSP_TICKS_PER_SEC;   // one second
const int TWOSECS_TOUT              =  2 * BSP_TICKS_PER_SEC;   // two seconds
const int THREESECS_TOUT            =  3 * BSP_TICKS_PER_SEC;   // three seconds

// scale
const int   SCALE_MEASUREMENT_RATE    =  BSP_TICKS_PER_SEC / 20;
const float SCALE_NEGATIVE_THRESHOLD = -1.0;
const float SCALE_ZERO_THRESHOLD     =  0.3;
const float SCALE_TARGET_THRESHOLD   = -0.4;

// grinder
const float GRINDER_DOSE_STEP       =  0.1;
const int   GRINDER_SETTLING_TIME   =  BSP_TICKS_PER_SEC / 2;



#endif   /* config_h */
