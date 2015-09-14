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
    int buffer[23];
    int i;
    digitalPotWrite(0, 127); // 0 is right 255 is left
    digitalPotWrite(1, 127); //0 is back 255 is forward
    digitalPotWrite(2, 0);
    digitalPotWrite(3, 127); // 0 is right 255 is left
    digitalPotWrite(4, 127); // 0 is down 255 is up
    digitalPotWrite(5, 0);
    
    lcd.printf("GTA 5 - Wiimote\nJesse Ardonne");
    
    while(1){
                if (pc.readable()){

                    for (i= 0; i < 23; i++){
                        buffer[i] = (int)pc.getc();
                    }
                
                    //GTA 5 Layout

/*[6] JoyX     */   digitalPotWrite         (0, 255-buffer[6]);
/*[7] JoyY     */   digitalPotWrite              (1,buffer[7]);
/*[21] xIR     */   digitalPotWrite             (3,buffer[21]); 
/*[22] yIR     */   digitalPotWrite         (4,255-buffer[22]);     
/*[9] B        */   digitalPotWrite         (5, buffer[9]*250); //B fires Right Trigger
/*[20] Z  */        digitalPotWrite        (2, buffer[20]*250); //Z fires Left Trigger
                        
                        


                    buffer[8]  ?   x.write_bit(1,15)   :   x.write_bit(0,15);//A --> A
                    (buffer[5] < 128) ?   x.write_bit(1,14)   :   x.write_bit(0,14);//NunchukZ --> B 
                    buffer[17] ?   x.write_bit(1,12)   :   x.write_bit(0,12);//Minus --> Y
                    buffer[16] ?   x.write_bit(1, 8)   :   x.write_bit(0, 8);// Plus --> RSC
                    
                    // Need more buttons, so if c is held down, change what buttons do
                    if (buffer[19]){ //if C button pressed, enable D-pad
                        buffer[15] ?   x.write_bit(0, 7)   :   x.write_bit(0, 7);// right
                        buffer[14] ?   x.write_bit(1, 6)   :   x.write_bit(0, 6);// left
                        buffer[13] ?   x.write_bit(1, 5)   :   x.write_bit(0, 5);// down
                        buffer[12] ?   x.write_bit(1, 4)   :   x.write_bit(0, 4);// up
                        x.write_bit(0,13);
                        x.write_bit(0,10);
                        x.write_bit(0, 9);
                        x.write_bit(0,11);
                    }
                    
                    else{
                        buffer[13] ?   x.write_bit(1,13)   :   x.write_bit(0,13);// down --> X  
                        buffer[15] ?   x.write_bit(1,10)   :   x.write_bit(0,10);// right --> RB
                        buffer[12] ?   x.write_bit(1, 9)   :   x.write_bit(0, 9);// up --> LSC
                        buffer[14] ?   x.write_bit(1,11)   :   x.write_bit(0,11);// left --> LB
                        x.write_bit(0,7);
                        x.write_bit(0,6);
                        x.write_bit(0,5);
                        x.write_bit(0,4);
                    }
                    
                    //buffer[] ?   x.write_bit(1, 3)   :   x.write_bit(0, 3); Sync Button not Mapped
                    buffer[10] ?   x.write_bit(1, 2)   :   x.write_bit(0, 2); // One - Back
                    buffer[11] ?   x.write_bit(1, 1)   :   x.write_bit(0, 1); // Two - Start
                    buffer[18] ?   x.write_bit(1, 0)   :   x.write_bit(0, 0); // Home - Guide

                    

                                       

                }            
        }
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

            buff[0] = xAccel 
            buff[1] = yAccel
            buff[2] = zAccel
            buff[3] = xNun 
            buff[4] = yNun
            buff[5] = zNun
            buff[6] = xJoy 
            buff[7] = yJoy 
			buff[8] = ABut 
			buff[9] = BBut 
			buff[10] = OneBut 
			buff[11] = TwoBut 
			buff[12] = UpBut 
			buff[13] = DownBut 
			buff[14] = LeftBut 
			buff[15] = RightBut 
			buff[16] = PlusBut 
			buff[17] = MinusBut 
			buff[18] = HomeBut 
			buff[19] = CBut 
			buff[20] = ZBut 
            buff[21] = xIR 
            buff[22] = yIR 
            
   
****BIT*****
15 - A
14 - B
13 - X
12 - Y
11 - LB
10 - RB
 9 - LSC
 8 - RSC
 7 - >
 6 - <
 5 - v
 4 - ^
 3 - Sync
 2 - Back 
 1 - Start
 0 - Guide

****SPI_ADDRESS***
0 - LSX
1 - LSY
2 - RSX
3 - RSY
4 - RT
5 - LT         
  
  
  
  Halo : Reach
  
					/*[6] JoyX     *  digitalPotWrite         (0, 255-buffer[6]);
					/*[7] JoyY        digitalPotWrite             (1,buffer[7]);
					/*[21] xIR        digitalPotWrite             (3,buffer[21]); 
					/*[22] yIR        digitalPotWrite         (4,255-buffer[22]);     
								
					/*[9] B           digitalPotWrite (5, buffer[9]*250);
					/*[5] nunAccZ     if (buffer[5] < 128)
											digitalPotWrite (2,250);
										else
											digitalPotWrite (2,  0);
											
											


                    buffer[8]  ?   x.write_bit(1,15)   :   x.write_bit(0,15);//A
                    buffer[12] ?   x.write_bit(1,14)   :   x.write_bit(0,14);//B
                    buffer[14] ?   x.write_bit(1,13)   :   x.write_bit(0,13);//X
                    buffer[15] ?   x.write_bit(1,12)   :   x.write_bit(0,12);//Y
                    
                    buffer[20] ?   x.write_bit(1,11)   :   x.write_bit(0,11);//LB
                    (buffer[2] < 128) ?   x.write_bit(1,10)   :   x.write_bit(0,10);//RB
                    buffer[13] ?   x.write_bit(1, 9)   :   x.write_bit(0, 9);
                    buffer[17] ?   x.write_bit(1, 8)   :   x.write_bit(0, 8);
                    /* Dpad Buttons Not Mapped
                    buffer[] ?   x.write_bit(1, 7)   :   x.write_bit(0, 7);
                    buffer[] ?   x.write_bit(1, 6)   :   x.write_bit(0, 6);
                    buffer[] ?   x.write_bit(1, 5)   :   x.write_bit(0, 5);
                    buffer[] ?   x.write_bit(1, 4)   :   x.write_bit(0, 4);
                    
                    //buffer[] ?   x.write_bit(1, 3)   :   x.write_bit(0, 3); Sync Button not Mapped
                    buffer[16] ?   x.write_bit(1, 2)   :   x.write_bit(0, 2);
                    buffer[10] ?   x.write_bit(1, 1)   :   x.write_bit(0, 1);
                    buffer[18] ?   x.write_bit(1, 0)   :   x.write_bit(0, 0);
           
*/