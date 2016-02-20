

#include "board.h"
#include "config.h"
#include "activeObjects.h"



// encoder port
Encoder myEnc(ENCODER_A, ENCODER_B);

// scale
HX711 scale(SCALE_DOUT, SCALE_CLK);

// OLED
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);


float lastGramms = 0;
float alpha = 0.4;

void postEncoderEvent(int direction) {

    if (direction == 1) {
        // post event
        QActive_postISR((QActive *)&AO_Grinder, ENCODER_INC_SIG, 0);
        Serial1.println("ENCODER_INC_SIG posted.");
    }
    if (direction == -1) {
        // post event
        QActive_postISR((QActive *)&AO_Grinder, ENCODER_DEC_SIG, 0);
        Serial1.println("ENCODER_DEC_SIG posted.");
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

/*
ISR(INT6_vect) { 
    // post event
    //GrinderEvent *pEvt = Q_NEW(GrinderEvent, BUTTON_DOWN_SIG);
    //AO_Grinder->POST(pEvt, 2);
    Serial1.println("BUTTON_DOWN_SIG posted.");
}
*/



void BSP_ledOff(void) {
    digitalWrite(BUTTON_LED_PIN, LOW);    // turn the LED off by making the voltage LOW
}

void BSP_ledOn(void) {
    digitalWrite(BUTTON_LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    //Serial1.println(myEnc.read());
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
    display.fillRect(0,0,128,28,0);

    display.setFont();

    //char buffer[10] = {"123456789"};
    //dtostrf(dose,5,1,buffer);
    //display.getTextBounds(buffer, 0, 0, &x1, &y1, &w, &h);

    display.setCursor(0,10);
    if (dose>0) {
        display.print(" ");
    }
    //if (abs(dose)<1000) {
        //display.print(" ");
        if (abs(dose)<100) {
            display.print(" ");
            if (abs(dose)<10) {
                display.print(" ");
                if (abs(dose)<1) {
                    display.print(" ");
                }
            }
        }
    //}
    display.print(dose,1);
    display.print("g");
    display.display();
}

void BSP_displayPosition(float position) {

    // clear area
    display.fillRect(0,20,128,40,0);

    //display.drawRoundRect(2, 20, 100, 40, 3, 1);

    display.setFont(&FreeSansBold18pt7b);

    display.setCursor(0,50);
    if (position>0) {
        display.print(" ");
    }
    //if (abs(position)<1000) {
        //display.print(" ");
        if (abs(position)<100) {
            display.print(" ");
            if (abs(position)<10) {
                display.print(" ");
                if (abs(position)<1) {
                    display.print(" ");
                }
            }
        }
    //}
    display.print(position,1);
    display.print("g");
    display.display();
}


float BSP_scaleSample(void) {
    // get the weight from the scale
    float gramms = scale.get_units(1)*1000;

    lastGramms = alpha * gramms + (1-alpha) * lastGramms;

    float result = lastGramms;

    if (abs(result) < 0.1){
        result = 0;
    }

    // display weight
    BSP_displayPosition(result);
    // post an event to signal measurement is done
    QActive_postISR((QActive *)&AO_Scale, SCALE_SAMPLING_DONE_SIG, 0);

    return result;
}


void BSP_init(void) {

    Serial1.begin(115200U); // set the highest stanard baud rate of 115200 bps
    Serial1.println("Starting.");

    cli();

    // button pins
    //pinMode(BUTTON_LED_PIN, OUTPUT);    // button LED
    //pinMode(BUTTON_SWITCH_PIN, INPUT);     // button direction
    //digitalWrite(BUTTON_SWITCH_PIN, HIGH); // button: turn on pullup resistors

    // configure external interrupt 1 (ArduinoPin2, PD1, Encoder)
    EICRB |= (1<<ISC40)|(0<<ISC41); // external int ctrl, any edge detection for interrupt INT4
    EIMSK |= (1<<INT4);             // external int mask, activates interrupt 0

    EICRB |= (1<<ISC50)|(0<<ISC51); // external int ctrl, any edge detection for interrupt INT5
    EIMSK |= (1<<INT5);             // external int mask, activates interrupt 1

    // configure external interrupt 6 (Pin7)
    //EICRB |= (0<<ISC60)|(1<<ISC61); // external int ctrl, falling edge detection for interrupt INT6
    //EIMSK |= (1<<INT6);             // external int mask, activates interrupt 6

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

    sei(); /* set Global Interrupt Enable */

}

