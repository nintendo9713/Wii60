#using <System.dll>

using namespace System;
using namespace System::IO::Ports;

#define USE_CONSOLE

#include <cstdlib>
#include <iostream>
#include <allegro.h>


//#define TRUE 1
#define SENSITIVITY 1
#define MOUSE_SPEED 2


/*
  Wiimote - Slow Down by holding a Button (focus)
          WiimotionPlus -- 
  Move - woo! :\
  MiceKeyboard!
*/
/*
class PC360
{
			//array<Byte>^ controls = gcnew array<Byte>(21);
			Byte controls[21];

	public:
	
};
    */


BITMAP *buffer;

void composeOutput();
int getMouseSpeed(char c);
void resetMouse();
void getControls();


			Byte controls[22];
			int *controlPtr;
			int xAim, yAim;
			int *xAimPtr = &xAim;
			int *yAimPtr = &yAim;
			char mousePo[256];

int main(void)
{
    allegro_init();
    
    install_keyboard();
    install_mouse();

	
	SerialPort^ sp = gcnew SerialPort("COM3", 115200);
        //sp->PortName = SetPortName(sp->"COM3");
        //sp->BaudRate = SetPortBaudRate(sp->115200);
        sp->Parity = Parity::None;
        sp->DataBits = 8;
        sp->StopBits = StopBits::One;

	
		sp->Open();
       
    buffer = create_bitmap(640,480);
    clear_to_color(buffer, makecol(255, 255, 255));
    
    if (set_gfx_mode(GFX_SAFE, 640, 480, 0, 0) != 0) {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Unable to set any graphic mode\n%s\n", allegro_error);
        return 1;
    }
	
	int x;

	array<Byte>^ controls2 = gcnew array<Byte>(22);
	//controlPtr = &controls[0];

    set_mouse_speed(MOUSE_SPEED,MOUSE_SPEED);

    while(!key[KEY_1]){
                       
        get_mouse_mickeys(xAimPtr,yAimPtr);
		getControls();
		resetMouse();
        composeOutput();

        acquire_screen();
        blit(buffer,screen,0,0,0,0,640,480);
        release_screen();
		  
		for (x = 0; x < 22; x++){
				controls2[x] = controls[x];
				//if (x%4 == 0)
					//printf("\n");
				//printf("%3d ",controls2[x]);
		}
			//printf("\n\n");
			
			sp->Write(controls2, 0, 22);

		rest(50);

        clear_bitmap(buffer);
        clear_to_color(buffer, makecol(255,255,255));
		


 
        
    }
    
	sp->Close();

    return 1;
}
END_OF_MAIN()

void composeOutput(){

        
        sprintf_s(mousePo, "W/S: %3d A/D: %3d RSX: %3d RSY: %3d",
                         controls[0],  controls[1], controls[2], controls[3]);
        textout_centre_ex(buffer, font, mousePo, SCREEN_W/2, SCREEN_H/6, makecol(0,0,0),-1);
        
        sprintf_s(mousePo, "R/T: %3d L/T: %3d",
                         controls[4],  controls[5]);
        textout_centre_ex(buffer, font, mousePo, SCREEN_W/2, SCREEN_H/4, makecol(0,0,0),-1); 
        
        sprintf_s(mousePo, "  A: %d   B: %d   X: %d  Y: %d",
                         controls[6],  controls[7],  controls[8],  controls[9]);
        textout_centre_ex(buffer, font, mousePo, SCREEN_W/2, SCREEN_H/3, makecol(0,0,0),-1); 
        
        sprintf_s(mousePo, " LB: %d  RB: %d LSC: %d RSC: %d", 
                         controls[10], controls[11], controls[12], controls[13]);
        textout_centre_ex(buffer, font, mousePo, SCREEN_W/2, SCREEN_H/2, makecol(0,0,0),-1); 
        
        sprintf_s(mousePo, "GUI: %d STR: %d BCK: %d SYN: %d", 
                         controls[14], controls[15], controls[16], controls[17]);
        textout_centre_ex(buffer, font, mousePo, SCREEN_W/2, SCREEN_H/3*2, makecol(0,0,0),-1); 
        
        sprintf_s(mousePo, " Up: %d DWN: %d LFT: %d RGT: %d", 
                         controls[18], controls[19], controls[20], controls[21]);
        textout_centre_ex(buffer, font, mousePo, SCREEN_W/2, SCREEN_H/6*5, makecol(0,0,0),-1); 
         
          
     
}

void resetMouse(){

     position_mouse(SCREEN_W/2, SCREEN_H/2);
     //get_mouse_mickeys(xAimPtr,yAimPtr);
     
}


int getMouseSpeed(char c){
    
    
    //xAim *= 5;
    //yAim *= 5;
    
    if(*xAimPtr > 127)
            *xAimPtr = 128;
    if(*xAimPtr < -128)
            *xAimPtr = -127;
    
    if(yAim > 127)
            yAim = 128;
    if(yAim < -128)
            yAim = -127;
            

   
    if (c == 'x'){
       return (*xAimPtr+127);
    }
    if (c == 'y'){
       return (yAim+127);
    }

}

void getControls(){
     

          if (key[KEY_W]){
             controls[0] = 255;
             }
          else if (key[KEY_S]){
             controls[0] = 0;
             }
          else controls[0] = 127;

          if (key[KEY_D]){
             controls[1] = 255;
             }
          else if (key[KEY_A]){
             controls[1] = 0;
             }
          else controls[1] = 127;
          
          controls[2]  = getMouseSpeed('x');
          controls[3]  = getMouseSpeed('y');          
          controls[4]  = (mouse_b & 1) * 255;
          controls[5]  = (mouse_b & 2) * 127;
          controls[6]  = (key[KEY_SPACE] & 1);
          controls[7]  = (key[KEY_G] & 1);
          controls[8]  = (key[KEY_R] || key[KEY_E]);
          controls[9]  = (key[KEY_Q] & 1);
          controls[10] = (key[KEY_LSHIFT] & 1);
          controls[11] = (key[KEY_F] & 1);
          controls[12] = (key[KEY_LCONTROL] & 1);
          controls[13] = ((mouse_b & 4)*1/4 || key[KEY_Z]);
          controls[14] = (key[KEY_TILDE] & 1);
          controls[15] = (key[KEY_ESC] & 1);
          controls[16] = (key[KEY_TAB] & 1);
          controls[17] = (key[KEY_RCONTROL] & 1);
          controls[18] = (key[KEY_UP] & 1);
          controls[19] = (key[KEY_DOWN] & 1);
          controls[20] = (key[KEY_LEFT] & 1);
          controls[21] = (key[KEY_RIGHT] & 1);
          
          
}

/*
  controls[0] WS -> Left Stick X
  controls[1] AD -> Left Stick Y
  controls[2] mX -> Right Stick X
  controls[3] mY -> Right Stick Y
  
  controls[4] RightClick -> RightTrigger
  controls[5] LeftClick -> Left Trigger  
  
  controls[6] Space -> A
  controls[7] G -> B
  controls[8] R -> X
  controls[9] Q -> Y
  
  controls[10] SHIFT -> LB
  controls[11] F -> RB
  controls[12] CTRL -> Left Stick Click  
  controls[13] MiddleClick -> RightStickClick
  
  controls[14] ~ -> Guide
  controls[15] ESC -> Start
  controls[16] TAB -> Back
  controls[17] 0 -> Sync


  controls[18] ^ - ^
  controls[19] v - v
  controls[20] < - <
  controls[21] > - >
  
*/
