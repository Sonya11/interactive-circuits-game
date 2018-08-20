/*
 * game.c
 *
 *  Created on: Nov 30, 2017
 *      Author: ryan
 */

#include "game.h"
#include "msp.h"
#include "bluetooth.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h> ///home/sonya/ti/simplelink_msp432p4_sdk_1_50_00_12/source   <ti/devices/msp432p4xx/driverlib/driverlib.h> <driverlib/MSP432P4xx/driverlib.h>
#include <ti/grlib/grlib.h>
#include <LcdDriver/Crystalfontz128x128_ST7735.h>

extern uint8_t game_state;
extern uint8_t timer_flag;

// Variables not used in interrupts//
// Lookup table to display countdown values (w/o sprintf)
static const char * countdown_table[] = {"5:00","4:59","4:58","4:57","4:56","4:55","4:54","4:53","4:52","4:51",
                                         "4:50","4:49","4:48","4:47","4:46","4:45","4:44","4:43","4:42","4:41",
                                         "4:40","4:39","4:38","4:37","4:36","4:35","4:34","4:33","4:32","4:31",
                                         "4:30","4:29","4:28","4:27","4:26","4:25","4:24","4:23","4:22","4:21",
                                         "4:20","4:19","4:18","4:17","4:16","4:15","4:14","4:13","4:12","4:11",
                                         "4:10","4:09","4:08","4:07","4:06","4:05","4:04","4:03","4:02","4:01",
                                         "4:00","3:59","3:58","3:57","3:56","3:55","3:54","3:53","3:52","3:51",
                                         "3:50","3:49","3:48","3:47","3:46","3:45","3:44","3:43","3:42","3:41",
                                         "3:40","3:39","3:38","3:37","3:36","3:35","3:34","3:33","3:32","3:31",
                                         "3:30","3:29","3:28","3:27","3:26","3:25","3:24","3:23","3:22","3:21",
                                         "3:20","3:19","3:18","3:17","3:16","3:15","3:14","3:13","3:12","3:11",
                                         "3:10","3:09","3:08","3:07","3:06","3:05","3:04","3:03","3:02","3:01",
                                         "3:00","2:59","2:58","2:57","2:56","2:55","2:54","2:53","2:52","2:51",
                                         "2:50","2:49","2:48","2:47","2:46","2:45","2:44","2:43","2:42","2:41",
                                         "2:40","2:39","2:38","2:37","2:36","2:35","2:34","2:33","2:32","2:31",
                                         "2:30","2:29","2:28","2:27","2:26","2:25","2:24","2:23","2:22","2:21",
                                         "2:20","2:19","2:18","2:17","2:16","2:15","2:14","2:13","2:12","2:11",
                                         "2:10","2:09","2:08","2:07","2:06","2:05","2:04","2:03","2:02","2:01",
                                         "2:00","1:59","1:58","1:57","1:56","1:55","1:54","1:53","1:52","1:51",
                                         "1:50","1:49","1:48","1:47","1:46","1:45","1:44","1:43","1:42","1:41",
                                         "1:40","1:39","1:38","1:37","1:36","1:35","1:34","1:33","1:32","1:31",
                                         "1:30","1:29","1:28","1:27","1:26","1:25","1:24","1:23","1:22","1:21",
                                         "1:20","1:19","1:18","1:17","1:16","1:15","1:14","1:13","1:12","1:11",
                                         "1:10","1:09","1:08","1:07","1:06","1:05","1:04","1:03","1:02","1:01",
                                         "1:00","0:59","0:58","0:57","0:56","0:55","0:54","0:53","0:52","0:51",
                                         "0:50","0:49","0:48","0:47","0:46","0:45","0:44","0:43","0:42","0:41",
                                         "0:40","0:39","0:38","0:37","0:36","0:35","0:34","0:33","0:32","0:31",
                                         "0:30","0:29","0:28","0:27","0:26","0:25","0:24","0:23","0:22","0:21",
                                         "0:20","0:19","0:18","0:17","0:16","0:15","0:14","0:13","0:12","0:11",
                                         "0:010","0:09","0:08","0:07","0:06","0:05","0:04","0:03","0:02","0:01","0:00"};

static const uint16_t table_size = sizeof(countdown_table)/sizeof(countdown_table[0]);
static uint16_t table_idx = 0;
extern Graphics_Context lcd_context;
uint8_t duty_cyc= 0;

// Variables to be used by interrupts //
volatile float volt_val = 0;
volatile uint16_t pot_resist = 0;
volatile float pot_cur = 0;

void lose_game(void)
{
    UART_send("Game Lost\n", 10);
    Graphics_clearDisplay(&lcd_context);
    Graphics_drawStringCentered(&lcd_context,(int8_t *)"Game", AUTO_STRING_LENGTH, 64, 40, OPAQUE_TEXT);
    Graphics_drawStringCentered(&lcd_context,(int8_t *)"Over!", AUTO_STRING_LENGTH, 64, 80, OPAQUE_TEXT);
}

void game_delay(void)
{
    UART_send("Beginning Next Game\n", 20);
    table_idx = 297;              // Get ready to do a 3 second countdown
    if(table_idx <= table_size)
    {
        if(timer_flag == 1)
        {
            timer_flag = 0;
            //DISPLAY COUNTDOWN VALUES//
            // If the player fails to complete in the allotted time, we lose
            if(table_idx == table_size)
            {
                Graphics_clearDisplay(&lcd_context);
                Graphics_drawStringCentered(&lcd_context,(int8_t *)"Begin", AUTO_STRING_LENGTH, 64, 60, OPAQUE_TEXT);
            }
            else
            {
                Graphics_clearDisplay(&lcd_context);
                Graphics_drawStringCentered(&lcd_context,(int8_t *)countdown_table[table_idx], AUTO_STRING_LENGTH, 64, 64, OPAQUE_TEXT);
            }
            table_idx++;
        }
    }
    table_idx = 0;
}

void win_game(void)
{
    UART_send("Game Win\n", 9);
    Graphics_clearDisplay(&lcd_context);
    Graphics_drawStringCentered(&lcd_context,(int8_t *)"Game", AUTO_STRING_LENGTH, 64, 40, OPAQUE_TEXT);
    Graphics_drawStringCentered(&lcd_context,(int8_t *)"Win!", AUTO_STRING_LENGTH, 64, 80, OPAQUE_TEXT);
}

void wait_to_begin(void)
{
    if(!game_state)
        Graphics_drawStringCentered(&lcd_context,(int8_t *)"Begin?", AUTO_STRING_LENGTH, 64, 60, OPAQUE_TEXT);
}

void play_game(void)
{

    if(table_idx <= table_size)
    {
        volt_val = ADC14->MEM[0]*0.00020142; // Change the ADC value into a voltage value
        pot_cur = volt_val/9680;
        pot_resist = (3.3 - volt_val)/pot_cur; // calculate the potentiometer resistance for the third game

        // BELOW ARE THE GAMES AND THEIR WIN CONDITIONS //

        // FIRST GAME //
        // For the Potentiometer Game, the player needs to match the chosen resistance! //
        // In the third game we want to calculate the resistance of the potentiometer //
        // Player must set the potentiometer to 1k Ohm Resistance //
        if((pot_resist <= 4230 && pot_resist >= 4170) && game_state == 1)
        {
            win_game();
            game_state = 0;
            game_delay();
        }
        // SECOND GAME //
        // If the player correctly re-evaluates currents, they win!
        else if(volt_val <= 3.27 && volt_val >= 3.23 && (game_state == 2))
        {
            win_game();
            game_state = 0;
            game_delay();
        }
        // THIRD GAME //
        else if(volt_val <= 3.1 && volt_val >= 3.0 && (game_state == 3))
        {
            win_game();
            game_state = 0;
            game_delay();
        }
        // FOURTH GAME //
        // In the fourth game, we calculate an averaged circuit for the PWM //
        // Player must create a 75 percent duty cycle wave that averages to 2.5V
        else if((volt_val >= 2.4 && volt_val <= 2.6) && game_state == 4)
        {
            win_game();
            game_state = 0;
            game_delay();
        }


        if(game_state)
        {
            if(timer_flag == 1)
            {
                timer_flag = 0;

                //DISPLAY COUNTDOWN VALUES//
                // If the player fails to complete in the allotted time, we lose
                if(table_idx == table_size)
                {
                    lose_game();
                      game_state = 0;
                }
                else
                {
                    Graphics_clearDisplay(&lcd_context);
                    Graphics_drawStringCentered(&lcd_context,(int8_t *)countdown_table[table_idx], AUTO_STRING_LENGTH, 64, 64, OPAQUE_TEXT);
                }

                table_idx++;
              }
        }
    }
}
