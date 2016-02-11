
#ifndef config_h
#define config_h

// voltages
#define AVCCMV      3042
#define VBATTMAXMV  3000
#define VBATTMINMV  1800


#define BSP_TICKS_PER_SEC  100U


// button with led
#define BUTTON_LED_PIN 5
#define BUTTON_SWITCH_PIN 7

// encoder port
#define ENCODER_CTL DDRD //encoder port control
#define ENCODER_WR PORTD //encoder port write
#define ENCODER_RD PIND //encoder port read
#define ENCODER_A 2
#define ENCODER_B 3

// scale
#define SCALE_DOUT  4
#define SCALE_CLK   6
#define SCALE_CALIBRATION_FACTOR -444190.0 //This value is obtained using the SparkFun_HX711_Calibration sketch

// OLED
//#define SPI_MOSI 16
//#define SPI_MISO 14
//#define SPI_CLK  15
#define OLED_DC     8
#define OLED_CS    10
#define OLED_RESET  9

// sensor general
const int ONESEC_TOUT               =  1 * BSP_TICKS_PER_SEC;   // one second
const int TWOSECS_TOUT              =  2 * BSP_TICKS_PER_SEC;   // two seconds
const int THREESECS_TOUT            =  3 * BSP_TICKS_PER_SEC;   // three seconds

// scale
const int SCALE_MEASUREMENT_RATE    =  BSP_TICKS_PER_SEC / 20;

// grinder
const float GRINDER_DOSE_STEP       =  0.1;


#endif   /* config_h */
