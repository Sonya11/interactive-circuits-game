/*
 * lcd.c
 *
 *  Created on: Nov 25, 2017
 *   
 */

#include "lcd.h"

void lcd_setup(Graphics_Context * context_address){
    Crystalfontz128x128_Init(); //initialize LCD (including SPI)
    Crystalfontz128x128_SetOrientation(0); //set screen to up orientation

    //initialize LCD info aka graphics context
    Graphics_initContext(context_address, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(context_address, GRAPHICS_COLOR_GOLD);
    Graphics_setBackgroundColor(context_address, GRAPHICS_COLOR_BLACK);
    GrContextFontSet(context_address, &g_sFontCm48b); //FONT_EX_MARKER g_sFontCmtt48 g_sFontCmss48b
    Graphics_clearDisplay(context_address);
}
