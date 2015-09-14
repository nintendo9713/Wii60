#include "mbed.h"
#include "MCP23017.h"
#include "TextLCD.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
//Declare Chip Select Pin
DigitalOut cs(p8);


TextLCD lcd(p15, p16, p17, p18, p19, p20); // rs, e, d4-d7
MCP23017 x = MCP23017( p9, p10, 0x42);

SPI jst(p5, p6, p7); // mosi, miso, sclk

//Serial Port Connection
Serial pc(USBTX, USBRX);

//Prototype for Joystick & Trigger Values
void digitalPotWrite(int,int); 


int main() {
    pc.baud(115200);
    
    
    x.config(0,0,0);
    int buffer[22];
    int i;
    digitalPotWrite(0, 127); // 0 is right 255 is left
    digitalPotWrite(1, 127); //0 is back 255 is forward
    digitalPotWrite(2, 0);
    digitalPotWrite(3, 127); // 0 is right 255 is left
    digitalPotWrite(4, 127); // 0 is down 255 is up
    digitalPotWrite(5, 0);
    
    /*
    x.write_bit(1, 15);
    wait(0.02);
    x.write_bit(0, 15);
    */
    lcd.printf("WOO!");
    
    while(1){
                if (pc.readable()){

                    for (i= 0; i < 22; i++){
                        buffer[i] = (int)pc.getc();
                    }
                    
                    
/*[0] LSX    -> A/D    */   digitalPotWrite       (1, buffer[0]);
/*[1] LSY    -> W/S    */   digitalPotWrite   (0, 255-buffer[1]);
/*[2] RSX    -> MoX    */   digitalPotWrite   (3, 255-buffer[2]);
/*[3] RSY    -> MoY    */   digitalPotWrite   (4, 255-buffer[3]);
/*[4] RT     -> RCl    */   digitalPotWrite        (5,buffer[4]);
/*[5] LT     -> LCl    */   digitalPotWrite        (2,buffer[5]);
             
                    
                    buffer[6]   ? x.write_bit(1, 15) : x.write_bit(0, 15);
                    buffer[7]   ? x.write_bit(1, 14) : x.write_bit(0, 14);
                    buffer[8]   ? x.write_bit(1, 13) : x.write_bit(0, 13);
                    buffer[9]   ? x.write_bit(1, 12) : x.write_bit(0, 12);
                    buffer[10]  ? x.write_bit(1, 11) : x.write_bit(0, 11);
                    buffer[11]  ? x.write_bit(1, 10) : x.write_bit(0, 10);
                    buffer[12]  ? x.write_bit(1,  9) : x.write_bit(0,  9);
                    buffer[13]  ? x.write_bit(1,  8) : x.write_bit(0,  8);
                    buffer[14]  ? x.write_bit(1,  0) : x.write_bit(0,  0);
                    buffer[15]  ? x.write_bit(1,  1) : x.write_bit(0,  1);
                    buffer[16]  ? x.write_bit(1,  2) : x.write_bit(0,  2);
                    buffer[17]  ? x.write_bit(1,  3) : x.write_bit(0,  3);
                    buffer[18]  ? x.write_bit(1,  4) : x.write_bit(0,  4);
                    buffer[19]  ? x.write_bit(1,  5) : x.write_bit(0,  5);
                    buffer[20]  ? x.write_bit(1,  6) : x.write_bit(0,  6);
                    buffer[6]   ? x.write_bit(1,  7) : x.write_bit(0,  7);
                    /*
                    if (buffer[6]){
                        x.write_bit(1,15);
                        }
                    else{
                        x.write_bit(0,15);
                        }
                        
                    if (buffer[7]){
                        x.write_bit(1,14);
                        }
                    else{
                        x.write_bit(0,14);
                        }
                       
                    if (buffer[8]){
                        x.write_bit(1,13);
                        }
                    else{
                        x.write_bit(0,13);
                        }
                        
                    if (buffer[9]){
                        x.write_bit(1,12);
                        }
                    else{
                        x.write_bit(0,12);
                        }
                        
                    if (buffer[10]){
                        x.write_bit(1,11);
                        }
                    else{
                        x.write_bit(0,11);
                        }
                    
                    if (buffer[11]){
                        x.write_bit(1,10);
                        }
                    else{
                        x.write_bit(0,10);
                        }
                    
                    if (buffer[12]){
                        x.write_bit(1,9);
                        }
                    else{
                        x.write_bit(0,9);
                        }
                    
                    if (buffer[13]){
                        x.write_bit(1,8);
                        }
                    else{
                        x.write_bit(0,8);
                        }
                    
                    if (buffer[14]){
                        x.write_bit(1,0);
                        }
                    else{
                        x.write_bit(0,0);
                        }
                    
                    if (buffer[15]){
                        x.write_bit(1,1);
                        }
                    else{
                        x.write_bit(0,1);
                        }
                    
                    if (buffer[16]){
                        x.write_bit(1,2);
                        }
                    else{
                        x.write_bit(0,2);
                        }
                    
                    if (buffer[17]){
                        x.write_bit(1,3);
                        }
                    else{
                        x.write_bit(0,3);
                        }
                    
                    if (buffer[18]){
                        x.write_bit(1,4);
                        }
                    else{
                        x.write_bit(0,4);
                        }
                    
                    if (buffer[19]){
                        x.write_bit(1,5);
                        }
                    else{
                        x.write_bit(0,5);
                        }
                    
                    if (buffer[20]){
                        x.write_bit(1,6);
                        }
                    else{
                        x.write_bit(0,6);
                        }
                    
                    if (buffer[21]){
                        x.write_bit(1,7);
                        }
                    else{
                        x.write_bit(0,7);
                        }
                    */
                    
                    
                }
    }
    
    
    /*
    for (q = 0; q < 255; q++){
        digitalPotWrite(0, q);
        wait(0.1);
        
    }*/
    /*
    lcd.printf("D Right");
    x.write_bit(1, 7);
    wait(0.1);
    x.write_bit(0, 7);
    wait(1);
    lcd.cls();
    lcd.printf("D Left");
    x.write_bit(1, 6);
    wait(0.1);
    x.write_bit(0, 6);
    wait(1);
    lcd.cls();
    lcd.printf("Sync!");
    x.write_bit(1, 3);
    wait(1);
    x.write_bit(0, 3);
    wait(2);
    lcd.cls();
    lcd.printf("B");
    x.write_bit(1, 14);
    wait(0.1);
    x.write_bit(0, 14);
    */



    
}

//Function for controlling Joysticks and Trigger Analog Signals
void digitalPotWrite(int address, int value) {
  // Take the CS pin low to select the chip:
  cs=0;
  // Send in the address and value via SPI:
   jst.write(address);
   jst.write(value);
  // Take the CS pin high to de-select the chip:
  cs=1;
}


/*
 0 Guide
 1 Start
 2 Back
 3 Sync
 4 Dup
 5 Ddown
 6 Dleft
 7 Dright
 8 RSC
 9 LSC
10 RB
11 LB
12 Y
13 X
14 B
15 A
*/