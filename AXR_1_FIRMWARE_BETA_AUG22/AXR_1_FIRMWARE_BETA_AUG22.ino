//
// AXR_1_FIRMWARE_BETA_AUG22
//
// NEW FIRMWARE FOR SAMD21/ATMega328P/ATMega2560 CHIPSET
//
// Author 		Thomas Sevilla
// 				Thomas Sevilla
//
// Date			8/22/16 7:25 AM
// Version		1.0
//
// Copyright	© Thomas Sevilla, 2016
// Licence		NOAA/AOML
//
//


#include "Wire.h"
#include "Adafruit_SI5351.h"
#include <SPI.h>
#include <stdint.h>
#include "LTC_2400.h"

#define CS 8
#define gndRly 10
#define sigRly 12

//*_*_*_*_*_*_*_*_*_*_*Global Variables*_*_*_*_*_*__*_*_*_*_*_*_*_*_*_*_

uint16_t eoc_timeout = 50; //70 is 15 Samples/Sec if only one conversion...50 works witht the clock

double vref=5.0489;
int stop=1;
int read=999;
int HD_EN=0;
int ClockSel = 0;


Adafruit_SI5351 clockGen = Adafruit_SI5351();

//*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*__*_*_*_*_*_*_*_*_*_*_

void set_Ext_Clock(){
    
    if(ClockSel==0){
        
        //*_*_*_*_*_*_*_*_*_*_*For 10Hz*_*_*_*_*_*__*_*_*_*_*_*_*_*_*_*_
        
        clockGen.begin();
        clockGen.setupPLL(SI5351_PLL_A, 27, 15373, 15625);
        clockGen.setupMultisynth(0, SI5351_PLL_A, 4, 854, 1);
        clockGen.setupRdiv(0, SI5351_R_DIV_4);
        clockGen.enableOutputs(true);
        
        SerialUSB.println("Clock is Set to 10Hz");
        
    }else if (ClockSel==1){
        
        //*_*_*_*_*_*_*_*_*_*_*For 15Hz*_*_*_*_*_*__*_*_*_*_*_*_*_*_*_*_
        
        clockGen.begin();
        clockGen.setupPLL(SI5351_PLL_A, 27, 15117, 15625);
        clockGen.setupMultisynth(0, SI5351_PLL_A, 4, 1134, 1);
        clockGen.setupRdiv(0, SI5351_R_DIV_2);
        clockGen.enableOutputs(true);
        SerialUSB.println("Clock is Set to 15Hz");
        
    }else if(ClockSel!=0 || ClockSel!=1){
        SerialUSB.println("Variables Corrupted, Could not read Clock Setup");
    }
}

void Calibration(){
    
    //Print out what it will do.
    //Switch relay to get the 5V.
    //Swtich relay to get 0V.
    //Run the Cal function from the .h file
    //save the the Cal.
}

void clean(){
    SerialUSB.write(27);       // ESC command
    SerialUSB.print("[2J");    // clear screen command
    SerialUSB.write(27);
    SerialUSB.print("[H");     // cursor to home command
}

void print_Main(){
    clean();
    SerialUSB.println("CONFIG");
    SerialUSB.println("==== Configuration Menu ====");
    SerialUSB.println("[1] Set Clocking Mode (10s/Sec or 15 s/Sec)");
    SerialUSB.println("[2] Enable HD-Sampling Mode");
    SerialUSB.println("[3] Calibrate");
    SerialUSB.println("[Q] QUIT");
}

void clockMode(){
    read=999;
    if (SerialUSB.available() > 0){
        read = SerialUSB.read();
    }
    switch (read) {
        case '1':
            ClockSel=0;
            set_Ext_Clock();
            delay(1000);
            read='C';
            break;
            
        case '2':
            ClockSel=1;
            set_Ext_Clock();
            delay(1000);
            read='C';
            break;
            
        case '0':
            read='C';
            break;
            
        default:
            clockMode();
            break;
            
    }
}

void sampleMode(){
    
    //    SerialUSB.println("==== HD Sample Menu ====");
    //    SerialUSB.println("[1] HD Sample ON");
    //    SerialUSB.println("[2] HD Sample OFF");
    //    SerialUSB.println("[0] Go Back");
    
    read=999;
   
    if (SerialUSB.available() > 0){
        read = SerialUSB.read();
    }
    switch (read) {
        case '1':
            HD_EN=1;
            SerialUSB.println("HD ON");
            delay(1000);
            read='C';
            break;
            
        case '2':
            HD_EN=0;
            SerialUSB.println("HD OFF");
            delay(1000);
            read='C';
            break;
            
        case '0':
            read='C';
            break;
            
        default:
            HD_EN=0;
            sampleMode();
            break;
            
    }
}

void calMode(){
    SerialUSB.println("CALRUN");
    read=999;
    if (SerialUSB.available() > 0){
        read = SerialUSB.read();
    }
    switch (read) {
        case '1':
            SerialUSB.println("THIS FEATURE HAS NOT BEEN IMPLEMENTED YET");
            //Calibration();
            delay(1000);
            read='C';
            break;
            
        case '0':
            read='C';
            break;
            
        default:
            calMode();
            break;
            
    }
}



void config(){
    read=999;
    if (SerialUSB.available() > 0){
        read = SerialUSB.read();
    }
    
    switch (read) {
            
        case '1':
            clean();
            SerialUSB.println("CLOCK");
            SerialUSB.println("==== Clock Menu ====");
            SerialUSB.println("[1] 10 s/Sec");
            SerialUSB.println("[2] 15 s/Sec");
            SerialUSB.println("[0] Go Back");
            clockMode();
            break;
            
        case '2':
            clean();
             SerialUSB.println("HD");
            SerialUSB.println("==== HD Sample Menu ====");
            SerialUSB.println("[1] HD Sample ON");
            SerialUSB.println("[2] HD Sample OFF");
            SerialUSB.println("[0] Go Back");
            sampleMode();
            break;
        case '3':
            clean();
            SerialUSB.println("CAL");
            SerialUSB.println("==== Calibration Menu ====");
            SerialUSB.println("[1] Calibrate");
            SerialUSB.println("[0] Go Back");
            calMode();
            break;
        case 'Q':
            read='Q';
            break;
            
        default:
            config();
            break;
            
    }
    
}
void move_Relays (0) {

}
void set_intialConditions(){
    pinMode(gndRly,OUTPUT);
    pinMode(sigRly,OUTPUT);
    
    digitalWrite(gndRly,LOW); //Signal should not be shorted to GND
    digitalWrite(sigRly,HIGH); //Signal should be connect to Diff. Amp.
}

void set_spi(){
    pinMode(SCK, OUTPUT);
    pinMode(CS, OUTPUT);
    pinMode(MISO, INPUT);
    
    
    
    digitalWrite(SCK, LOW);
    digitalWrite(CS, HIGH);
    
    digitalWrite(SCK, HIGH);
    
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
}

double CalcRes(double VoltIn) {
    double Resistance = 0;
    Resistance = VoltIn / 0.0001; //0.0001 Is the current used. 100uA.
    return Resistance;
}

double meassure_volt(int chipSel){
    double voltage = 0.0;
    int32_t adc_code = 0;
    
    LTC24XX_EOC_timeout(chipSel, eoc_timeout);                 // First flush power-on conversion that probably happened before
    LTC24XX_SPI_32bit_data(chipSel, &adc_code);                // the DAC output was set
    
    LTC24XX_EOC_timeout(chipSel, eoc_timeout);                 // Wait for end of conversion
    LTC24XX_SPI_32bit_data(chipSel, &adc_code);                // Read the ADC
    voltage = LTC24XX_SE_code_to_voltage(adc_code, vref); // Convert ADC code to voltage
    
    return voltage;
}

void check_probe(int chip){
    
   
    
}

void alive(){
SerialUSB.prinln("AXR-1 ONLINE");
}

void setup(){
    pinMode(CS,OUTPUT);
    set_spi();
    set_intialConditions();
   SerialUSB.begin(115200);
   SerialUSB.prinln("AXR-1 ONLINE");
    delay(1000);
    clean();
    
    while(1){
        if (SerialUSB.available() > 0){
            read = SerialUSB.read();
        }
        while(read == 'C'){
            clean();
            print_Main();
            config();
        }
        if (read == 'Q')// Q- Quits.
            break;
        if(read=='D'){
            alive();
        }
    }
    set_Ext_Clock();
    delay(1000);
    clean();
    read='S';
}

void loop(){
    while(1){
        if (SerialUSB.available() > 0){
            read = SerialUSB.read();
        }
        if(read=='R'){
            stop=0; //Starts the meassurment
            clean;
        }
        if(read=='S'){
            stop=1; //Stops the meassuremnt
            clean();
        }
        if(read=='X'){
            SerialUSB.println("Reset");
            delay(1000);
            NVIC_SystemReset();// Resets the uC -> only for SAMD21
        }
        if(read=='P'){
            check_probe(CS);
        }
        if(read=='D'){
            alive();
        }
        if(stop==0){
            if(HD_EN==0){
                SerialUSB.println("RUN");
            }
            if(HD_EN==1){
                SerialUSB.println("HD_RUN");
            }
        }
        if(stop==1){
            SerialUSB.println("STOPPED");
        }
    }
}