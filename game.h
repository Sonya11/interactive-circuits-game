/*
 * game.h
 *
 *  Created on: Nov 30, 2017
 *      
 */

#ifndef GAME_H_
#define GAME_H_


/***************************************
 * The function below is used to display
 * "Game Lost" to the LCD display in the
 *         case of a lost game!
 **************************************/

void lose_game(void);

/***************************************
 * The function below is used to display
 * "Game Win!" to the LCD, wait for more
 * user input, and begin a countdown for
 *              the next game!
 ***************************************/

void win_game(void);

/*********************************************
 * The below function is the most important
 * of the entire game, it runs everything
 * from the LCD countdown timer to activating
 *          the win/lose conditions!
 *********************************************/

void play_game(void);

/****************************************************************
 * Wait for a button press to initiate the beginning of the game!
 ***************************************************************/

void wait_to_begin(void);

/***********************************************
 * Countdown Delay that gives the player time
 *          to catch their bearings
 ***********************************************/
void game_delay(void);

#endif /* GAME_H_ */
