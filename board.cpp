

#include "board.h"
#include "config.h"
#include "activeObjects.h"



// encoder port
Encoder myEnc(ENCODER_A, ENCODER_B);

// scale
HX711 scale(SCALE_DOUT, SCALE_CLK);

// OLED
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

float alpha = 0.8;
float lastGramms = 0;
float averageWeight = 0;
float averageWeightArray[10];

void postEncoderEvent(int direction) {

    if (direction == 1) {
        // post event
        QActive_postISR((QActive *)&AO_Encoder, ENCODER_INC_SIG, 0);
        //Serial1.println("ENCODER_INC_SIG posted.");
    }
    if (direction == -1) {
        // post event
        QActive_postISR((QActive *)&AO_Encoder, ENCODER_DEC_SIG, 0);
        //Serial1.println("ENCODER_DEC_SIG posted.");
    }
}

ISR(INT4_vect) { 
    // call the static update function
    int direction = Encoder::update(myEnc.interruptArgs[0]);
    if (direction) {
        // post event
        postEncoderEvent(direction);
    }
}

ISR(INT5_vect) { 
    // call the static update function
    int direction = Encoder::update(myEnc.interruptArgs[1]);
    if (direction) {
        // post event
        postEncoderEvent(direction);
    }
}


ISR(INT0_vect) { 
    // post event
    QActive_postISR((QActive *)&AO_Grinder, BUTTON_DOWN_SIG, 0);
    Serial1.println("BUTTON_DOWN_SIG posted.");
}

void BSP_ledOff(void) {
    digitalWrite(BUTTON_LED_PIN, LOW);    // turn the LED off by making the voltage LOW
}

void BSP_ledOn(void) {
    digitalWrite(BUTTON_LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void BSP_println(char* str) {
    Serial1.println(str);
}

void BSP_setupTickTimer(void) {

    // ===== configure 8bit Timer0 =====

    cli();          // disable global interrupt

    TCCR0A = 0;     // set entire TCCR1A register to 0
    TCCR0B = 0;     // set entire TCCR1A register to 0

    TCCR0A |= (1 << WGM01);                 // timer/counter control, CTC mode, no PWM
    TCCR0B |= (1 << CS02 ) | (1 << CS00);   // timer/counter control, 1/1024 prescaler, internal clock source

    TCNT0  = 0;                             // timer/counter 0, initialize couter to 0
    OCR0A  = (F_CPU / BSP_TICKS_PER_SEC / 1024) - 1;  // output compare register, init with value to compare to

    TIMSK0 |= (1 << OCIE0A);         // timer interrupt mask, enable TIMER0 compare A Interrupt
    
    sei();          // enable global interrupts

}
    
ISR (TIMER0_COMPA_vect) {

    QF_tickISR(); /* process time events for rate 0 */
}


void BSP_displayDose(float dose) {

    // clear area
    //display.clearDisplay();
    display.fillRect(0,0,128,22,0);

    display.setFont(&FreeSansBold9pt7b);

    display.setCursor(10,14);

    char buffer[10];

    // convert float to string using dtostrf(FLOAT,WIDTH,PRECSISION,BUFFER);
    dtostrf(dose, 5, FLOAT_PRECISION, buffer);

    display.print("Dose:");
    display.print(buffer);
    display.print("g");
    display.display();
}

void BSP_displayPosition(float position) {

    // clear last number
    //tft.getTextBounds(string, x, y, &y1, &y1, &w, &h);


    display.fillRect(0,24,128,39,0);

    display.drawRoundRect(2, 22, 124, 42, 3, 1);

    display.setFont(&FreeSansBold18pt7b);

    char buffer[10];

    // convert float to string using dtostrf(FLOAT,WIDTH,PRECSISION,BUFFER);
    dtostrf(position, 5, FLOAT_PRECISION, buffer);

    display.setCursor(12,53);

    display.print(buffer);
    display.print("g");
    display.display();
}

void originOfCenterTextInBounds(char* str, uint16_t x0b, uint16_t y0b, uint16_t xb, uint16_t yb, int16_t* x0t, int16_t* y0t){

    display.setFont(&FreeSansBold18pt7b);

    uint16_t w, h;

    display.getTextBounds(str, 0, 0, x0t, y0t, &w, &h);

    Serial1.print("Bounds for string are: ");
    Serial1.print(w);
    Serial1.print(" x ");
    Serial1.println(h);

    *x0t = (xb - w) / 2;
    *y0t = (yb + h) / 2;
}


float BSP_scaleSample(void) {
    // get the weight from the scale
    float gramms = scale.get_units(1)*1000;

    lastGramms = alpha * gramms + (1-alpha) * lastGramms;

    float result = lastGramms;

    if (abs(result) < 0.1){
        result = 0;
    }

    // do averaging
    float sum = 0;
    for (int i = 0; i < 9; i++) {
        averageWeightArray[i+1] = averageWeightArray[i];
        sum += averageWeightArray[i+1];
    }
    averageWeightArray[0] = lastGramms;
    sum += lastGramms;

    averageWeight = sum/10;

    // post an event to signal measurement is done
    QActive_postISR((QActive *)&AO_Scale, SCALE_SAMPLING_DONE_SIG, 0);

    return result;
}

float BSP_scaleAverageWeight(void) {

    return averageWeight;
}

void BSP_scaleTare(void) {

    // reset history
    lastGramms = 0;
    // tare
    scale.tare(10);
    // post an event to signal tare is done
    QActive_postISR((QActive *)&AO_Scale, SCALE_TARE_DONE_SIG, 0);
}


void BSP_init(void) {

    Serial1.begin(115200U); // set the highest stanard baud rate of 115200 bps
    Serial1.println("Starting.");

    cli();

    // button pins
    pinMode(BUTTON_LED_PIN, OUTPUT);    // button LED
    pinMode(BUTTON_SWITCH_PIN, INPUT);     // button direction
    digitalWrite(BUTTON_SWITCH_PIN, HIGH); // button: turn on pullup resistors

    // configure external interrupt 1 (ArduinoPin2, PD1, Encoder)
    EICRB |= (1<<ISC40)|(0<<ISC41); // external int ctrl, any edge detection for interrupt INT4
    EIMSK |= (1<<INT4);             // external int mask, activates interrupt 0

    EICRB |= (1<<ISC50)|(0<<ISC51); // external int ctrl, any edge detection for interrupt INT5
    EIMSK |= (1<<INT5);             // external int mask, activates interrupt 1

    // configure external interrupt 0 (Pin21)
    EICRA |= (0<<ISC00)|(1<<ISC01); // external int ctrl, falling edge detection for interrupt INT0
    EIMSK |= (1<<INT0);             // external int mask, activates interrupt 0

    Serial1.println("Interrupts set up done.");


    Serial1.print("Init and calibrating scale...  ");
    scale.set_scale(SCALE_CALIBRATION_FACTOR); //This value is obtained by using the SparkFun_HX711_Calibration sketch
    scale.tare();  //Assuming there is no weight on the scale at start up, reset the scale to 0
    Serial1.println("Done.");

    Serial1.print("Init and setup display...  ");
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC);

    display.clearDisplay();
    Serial1.println("Done.");

    // text display
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(0,0);
    display.println("Ennel power!");

    //display.setFont(&FreeSansBold18pt7b);
    display.display();

    


    int16_t x0t, y0t;

    originOfCenterTextInBounds("1234", 0, 0, 128, 64, &x0t, &y0t);

    Serial1.print("x0t: ");
    Serial1.println(x0t);
    Serial1.print("y0t: ");
    Serial1.println(y0t);

    sei(); /* set Global Interrupt Enable */

}

