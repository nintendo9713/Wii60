#include "mbed.h"

//Declare Button Outputs
DigitalOut ABut(p9);
DigitalOut BBut(p10);
DigitalOut XBut(p11);
DigitalOut YBut(p12);
DigitalOut LBBut(p13);
DigitalOut RBBut(p14);
DigitalOut LSCBut(p15);
DigitalOut RSCBut(p16);
DigitalOut GuideBut(p17);
DigitalOut StartBut(p18);
DigitalOut BackBut(p19);
DigitalOut SyncBut(p20);
DigitalOut DpadUpBut(p21);
DigitalOut DpadDownBut(p22);
DigitalOut DpadLeftBut(p23);
DigitalOut DpadRightBut(p24);

//Declare Chip Select Pin
DigitalOut cs(p8);

//Declare Rumble Input
DigitalIn rumble(p25);

//Declare LEDs
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

//Serial Port Connection
Serial pc(USBTX, USBRX);
//SPI Connection
SPI jst(p5, p6, p7); // mosi, miso, sclk

//Prototype for Joystick & Trigger Values
void digitalPotWrite(int,int); 

int main() {

        led1 = led2 = led3 = led4 = false;
        
        
        //Initialize Buttons
        ABut      = BBut        = XBut        = YBut         = 
        LBBut     = RBBut       = LSCBut      = RSCBut       = 
        GuideBut  = StartBut    = BackBut     = SyncBut      = 
        DpadUpBut = DpadDownBut = DpadLeftBut = DpadRightBut = 0;

        
        //Turn IC off
        cs = 1;

        //Initialize Serial Port 
        pc.baud(115200);
        //Initialize SPI Connection
        jst.format(8,0);
        jst.frequency(100000);        
        
        // Declare Buffer for Serial Input and Count Variable
        int buffer[23];
        int i;
        
        
        while(1)
        {

            //If Serial Port is Readable
            if (pc.readable() == 1){
                
                //Store Serial Data in Buffer
                for (i= 0; i < 23; i++){
                    buffer[i] = (int)pc.getc();
                }

                
/*[0] AccelX   */   /* Unused */
/*[1] AccelY   */   /* Unused */
/*[2] AccelZ   */   /* Unused */
/*[3] nunAccX  */   /* Unused */
/*[4] nunAccY  */   /* Unused */
/*[5] nunAccZ  */   if ((int)buffer[5] < 128)RBBut = true;else{ RBBut = false;}
/*[6] JoyX     */   digitalPotWrite         (0, 255-(int)buffer[6]);
/*[7] JoyY     */   digitalPotWrite             (1, (int)buffer[7]);
/*[8] A        */   ABut        =             (int)(bool) buffer[8];
/*[9] B        */   digitalPotWrite (5, (int)((bool)buffer[9])*250);
/*[10] One     */   StartBut    =             (int)(bool)buffer[10];
/*[11] Two     */   DpadDownBut =             (int)(bool)buffer[11];
/*[12] Up      */   BBut        =             (int)(bool)buffer[12];
/*[13] Down    */   LSCBut      =             (int)(bool)buffer[13];
/*[14] Left    */   YBut        =             (int)(bool)buffer[14];  /* Address | Controller  */
/*[15] Right   */   XBut        =             (int)(bool)buffer[15];  /*      0       LSX      */
/*[16] Plus    */   RSCBut      =             (int)(bool)buffer[16];  /*      1       LSY      */
/*[17] Minus   */   BackBut     =             (int)(bool)buffer[17];  /*      2       LT       */
/*[18] Home    */   GuideBut    =             (int)(bool)buffer[18];  /*      3       RSX      */
/*[19] C       */   digitalPotWrite (2,(int)((bool)buffer[19])*250);  /*      4       RSY      */
/*[20] Z       */   LBBut       =             (int)(bool)buffer[20];  /*      5       RT       */
/*[21] xIR     */   digitalPotWrite             (3,(int)buffer[21]); 
/*[22] yIR     */   digitalPotWrite         (4,255-(int)buffer[22]);                             
                                

                }
                  
                  
                  if ( buffer[6] < 128){
                        led1 = true;}
                  if ( buffer[6] >= 128){
                        led1 = false;  }    
                  if ( buffer[7] < 128){
                        led2 = true;}
                  if ( buffer[7] >= 128){
                        led2 = false;  }                           
                  if ( buffer[21] < 128){
                        led3 = true;}
                  if ( buffer[21] >= 128){
                        led3 = false;  }
                  if ( buffer[22] < 128){
                        led4 = true;}
                  if ( buffer[22] >= 128){
                        led4 = false;  }               
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

