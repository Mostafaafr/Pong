/*
 * Created: 9/21/2021 21:21:21 AM
 * Author : Mostafa Afr :)
 */ 

#include <avr/io.h>
#include "ST7735.h"
#include "LCD_GFX.h"
#include <util/delay.h>
#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
#include "serialprint.h" //my library from last lab

char String[25];


int player1_x0 = 0;
int player1_y0 = 0;
int player1_x1 = 15;
int player1_y1 = 50;
	
int player2_x0 = 144;
int player2_y0 = 77;
int player2_x1 = 159;
int player2_y1 = 127;
int score1 = 0;
int score2 = 0;
int paddle_direction = -1;

int round_counter1 = 0;
int round_counter2 = 0;
int circlex0 = 79;
int circley0 = 63;
int circleradius = 6;
int drawCounter = 0;



void drawScoreboard() {
	
	LCD_drawLine(65, 0, 65, 16, WHITE);
	LCD_drawLine(65, 16, 94, 16, WHITE);
	LCD_drawLine(94, 16, 94, 0, WHITE);
	
	char score_str[20];
	sprintf(score_str, "%d", score1);
	LCD_drawString(70, 3, score_str, WHITE, BLACK);
	sprintf(score_str, "%d", score2);
	LCD_drawString(85, 3, score_str, WHITE, BLACK);
}

void Initialize()
{
	lcd_init();
	// timer 1
	DDRD |= (1 << DDD3);
	DDRD |= (1 << DDD5);
	DDRD |= (1 << DDD2);
	TCCR1B &= ~(1 << WGM13);    // Set timer 1 to normal mode
	TCCR1B &= ~(1 << WGM12);
	TCCR1A &= ~(1 << WGM11);
	TCCR1A &= ~(1 << WGM10);
	
	ADMUX = 0;
	PRR &= ~(1 << PRADC);    
	ADMUX &= ~(1 << REFS1);    
	ADMUX |= (1 << REFS0);

	ADCSRA |= (1 << ADPS1);    //Prescale by 128 
	ADCSRA |= (1 << ADPS2);
	ADCSRA |= (1 << ADPS0);

	ADCSRB &= ~(1 << ADTS1);
	ADCSRA |= (1 << ADATE);
	ADCSRB &= ~(1 << ADTS2);
	ADCSRB &= ~(1 << ADTS0);

	ADMUX &= ~(1 << MUX1);      // Select channel A0
	ADMUX &= ~(1 << MUX2);
	ADMUX &= ~(1 << MUX3);
	ADMUX &= ~(1 << MUX0);
	
	ADCSRA |= (1 << ADIE);      
	DIDR0 |= (1 << ADC0D);      
	ADCSRA |= (1 << ADEN);      
	ADCSRA |= (1 << ADSC);      
	serial_start(BAUD_PRESCALER);
}

int main(void)
{
	Initialize();
	LCD_setScreen(0X0000);
	LCD_drawCircle(circlex0, circley0, circleradius, 0xFFFF);	
	LCD_drawBlock( player1_x0,  player1_y0,  player1_x1,  player1_y1, 0xFFFF); //paddle 1
	LCD_drawBlock( player2_x0,  player2_y0,  player2_x1,  player2_y1, 0xFFFF); //paddle 2
	drawScoreboard();
	int ball_speed = (rand() % 5) + 1; // randomly choose ball speed
int num_directions = 8; // set the number of directions
int ball_direction_x, ball_direction_y;
int rand_dir = rand() % num_directions; // generate a random integer between 0 and 7

switch (rand_dir) { // create a set of cases so the ball has 8 ways to go at the start of the game
	case 0:
	ball_direction_x = 1;
	ball_direction_y = 0;
	break;
	case 1:
	ball_direction_x = 1;
	ball_direction_y = 1;
	break;
	case 2:
	ball_direction_x = 0;
	ball_direction_y = 1;
	break;
	case 3:
	ball_direction_x = -1;
	ball_direction_y = 1;
	break;
	case 4:
	ball_direction_x = -1;
	ball_direction_y = 0;
	break;
	case 5:
	ball_direction_x = -1;
	ball_direction_y = -1;
	break;
	case 6:
	ball_direction_x = 0;
	ball_direction_y = -1;
	break;
	case 7:
	ball_direction_x = 1;
	ball_direction_y = -1;
	break;
}
	

    while (1)
    {
    // Clear the previous paddle
    LCD_drawBlock(player2_x0, player2_y0, player2_x1, player2_y1, 0x0000);
 
    // Update the paddle position
    player2_y0 += paddle_direction; // move left by one pixel
    player2_y1 += paddle_direction;

    // Check if the paddle has reached the top or bottom edge of the screen
    if (player2_y0 <= 0 || player2_y1 >= 127) {
	    //If the paddle has reached the top or bottom edge, change the direction
	    paddle_direction *= -1;
		
		if (ADC < 470) {
			player1_y0 -= 2; // move up by two pixels
			player1_y1 -= 2;
		}

		//Move player 1 paddle down if joystick_pos_y is above 495
		if (ADC > 495) {
			sprintf(String, "%d\n", ADC);
			print(String);
			player1_y0 += 2; // move down by two pixels
			player1_y1 += 2;
		}
		
		// Clear the previous paddle
		LCD_drawBlock(player1_x0, player1_y0, player1_x1, player1_y1, 0x0000);
		
		// Draw the updated paddle
		LCD_drawBlock(player1_x0, player1_y0, player1_x1, player1_y1, 0xFFFF);

    }

    // Draw the updated paddle
    LCD_drawBlock(player2_x0, player2_y0, player2_x1, player2_y1, 0xFFFF);
    _delay_ms(50);
	
	while (1)
	{
		// Clear the previous paddle
		LCD_drawBlock(player1_x0, player1_y0, player1_x1, player1_y1, 0x0000);
		
		if (ADC < 430) {
			player1_y0 -= 2; // move up by two pixels
			player1_y1 -= 2;
			} else if (ADC> 530) {
			player1_y0 += 2; // move down by two pixels
			player1_y1 += 2;
		}
		
		// Check if the paddle has reached the top or bottom edge of the screen
		if (player1_y0 <= 0) {
			player1_y0 = 0;
			player1_y1 = 50;
			} else if (player1_y1 >= 127) {
			player1_y0 = 77;
			player1_y1 = 127;
		}
		
		// Draw the updated paddle
		LCD_drawBlock(player1_x0, player1_y0, player1_x1, player1_y1, 0xFFFF);
		
		// Clear the previous paddle
		LCD_drawBlock(player2_x0, player2_y0, player2_x1, player2_y1, 0x0000);
		
		// Update the paddle position
		player2_y0 += paddle_direction; // move left by one pixel
		player2_y1 += paddle_direction;

		// Check if the paddle has reached the top or bottom edge of the screen
		if (player2_y0 <= 0 || player2_y1 >= 127) {
			// If the paddle has reached the top or bottom edge, change the direction
			paddle_direction *= -1;
		}

		// Draw the updated paddle
		LCD_drawBlock(player2_x0, player2_y0, player2_x1, player2_y1, 0xFFFF);
		
		     		  // Calculate the new position of the ball
		     		  int new_ball_x = circlex0 + ball_direction_x * ball_speed;
		     		  int new_ball_y = circley0 + ball_direction_y * ball_speed;

		     		  // Check if the ball has hit the left or right boundary of the screen
		     		  if (new_ball_y - circleradius <= 0 || new_ball_y + circleradius >= 127) {
			     		  ball_direction_y *= -1; // Reverse the y direction
						   PORTD |= (1 << PORTD3);
						   _delay_ms(500);
						   PORTD &= ~(1 << PORTD3);
		     		  }

		     		  // Check for paddle collisions (This part of the code could definitely be improved)
		     		  if (new_ball_x - circleradius <= player1_x1 && new_ball_y >= player1_y0 && new_ball_y <= player1_y1) {
			     		  ball_direction_x *= -1;
			     		  } else if (new_ball_x + circleradius >= player2_x0 && new_ball_y >= player2_y0 && new_ball_y <= player2_y1) {
			     		  ball_direction_x *= -1;
						   PORTD |= (1 << PORTD3);
						   _delay_ms(500);
						   PORTD &= ~(1 << PORTD3);
		     		  }

		     		  // Check for scoreboard collision
		     		  if (new_ball_x >= 65 && new_ball_x <= 95 && new_ball_y >= 0 && new_ball_y <= 17) {
			     		  ball_direction_y *= -1;
						   PORTD |= (1 << PORTD3);
						   _delay_ms(500);
						   PORTD &= ~(1 << PORTD3);
		     		  }

		     		  // Check if the ball has hit the top or bottom boundary of the screen
		     		  if (new_ball_x - circleradius <= 0 || new_ball_x + circleradius >= 159) {
			     		  // Update the score and reset the ball position if it hits the top or bottom boundary
			     		  if (new_ball_x - circleradius <= 0) {
				     		  score1 += 1;
							  PORTD |= (1 << PORTD3); //turn on buzzer
							  _delay_ms(500);
							  PORTD &= ~(1 << PORTD3);
							PORTD |= (1 << PORTD5); //turn on LED
							_delay_ms(500);
							PORTD &= ~(1 << PORTD5);
				     		  new_ball_x = 79;
				     		  new_ball_y = 63;
							LCD_setScreen(BLACK);
							LCD_drawString(55, 50, "Player 1 Scored!", WHITE, BLACK); //give players some time to react before ball starts moving again
							   _delay_ms(1500);
							   LCD_setScreen(BLACK);
				     		  } else if (new_ball_x + circleradius >= 159) {
				     		  score2 += 1;
							PORTD |= (1 << PORTD3); //turn on buzzer
							_delay_ms(500);
							PORTD &= ~(1 << PORTD3); 
							PORTD |= (1 << PORTD2); //turn on LED
							_delay_ms(500);
							PORTD &= ~(1 << PORTD2);
				     		  new_ball_x = 79;
				     		  new_ball_y = 63;
							LCD_setScreen(BLACK);
							LCD_drawString(55, 50, "Player 2 Scored!", WHITE, BLACK); //give players some time to react before ball starts moving again
							  _delay_ms(1500);
							  LCD_setScreen(BLACK);
			     		  }

			     		  // Redraw the scoreboard
			     		  drawScoreboard();

			     		  // Reset the ball direction
			     		  ball_direction_x *= -1;
		     		  }

		     		  // Clear the previous position of the ball
		     		  LCD_drawCircle(circlex0, circley0, circleradius, 0x0000);

		     		  // Update the position of the ball
		     		  circlex0 = new_ball_x;
		     		  circley0 = new_ball_y;

		     		  // Draw the updated position of the ball
		     		  LCD_drawCircle(circlex0, circley0, circleradius, 0xFFFF);

		     		  // Add a delay to slow down the movement
		     		  _delay_ms(50);
					   
					   if (score1 == 5 && round_counter1 == 0) {
						   // Display "Round 2" on the screen
						   
						   LCD_setScreen(BLACK);
						   LCD_drawString(55, 50, "Round 2", WHITE, BLACK);
						   _delay_ms(20000);
						   LCD_setScreen(BLACK); 
						   round_counter1++;

						   // Reset scores and ball position
						   score1 = 0;
						   score2 = 0;
						   circlex0 = 79;
						   circley0 = 63;
						   ball_direction_x = (rand() % 5) ? 1 : -1;
						   ball_direction_y = (rand() % 5) ? 1 : -1;}
						   
						   if (score1 == 5 && round_counter1 == 1) {
						   // Display "Round 2" on the screen
						   
						   LCD_setScreen(BLACK);
						   LCD_drawString(55, 50, "Player 2 Wins!", WHITE, BLACK); //I accidentally switched the name on my score variables. It has no impact on the actual game, it just makes my code confusing
						   _delay_ms(100000);
						    LCD_setScreen(BLACK); 
					   }
					   
						//All this below is pretty much just round logic to make sure the round system works correctly
					   if (score2 == 5 && round_counter2 == 0 && round_counter1 == 0) {
						   // Display "Round 2" on the screen
						   
						   LCD_setScreen(BLACK);
						   LCD_drawString(55, 50, "Round 2", WHITE, BLACK);
						   _delay_ms(20000);
						   LCD_setScreen(BLACK);
							round_counter2++;

						   // Reset scores and ball position
						   score1 = 0;
						   score2 = 0;
						   circlex0 = 79;
						   circley0 = 63;
						   ball_direction_x = (rand() % 5) ? 1 : -1;
						   ball_direction_y = (rand() % 5) ? 1 : -1;

						   // Redraw the scoreboard
						   drawScoreboard();
					   }
					   
					   if (score2 == 5 && round_counter2 == 1) {
						   
						   LCD_setScreen(BLACK);
						   LCD_drawString(35, 50, "Player 1 Wins!", WHITE, BLACK);
						   _delay_ms(100000);
						   LCD_setScreen(BLACK);
					   }

						if (score2 == 5 && round_counter2 == 0 && round_counter1 == 1) {
	
							LCD_setScreen(BLACK);
							LCD_drawString(55, 50, "Round 3!", WHITE, BLACK);
							_delay_ms(15000);
							LCD_setScreen(BLACK);
							round_counter2++;

							// Reset scores and ball position
							score1 = 0;
							score2 = 0;
							circlex0 = 79;
							circley0 = 63;
							ball_direction_x = (rand() % 5) ? 1 : -1;
							ball_direction_y = (rand() % 5) ? 1 : -1;

							// Redraw the scoreboard
							drawScoreboard();
						}
						if (score1 == 5 && round_counter1 == 0 && round_counter2 == 1) {
							// Display "Round 3" on the screen
													
							LCD_setScreen(BLACK);
							LCD_drawString(55, 50, "Round 3!", WHITE, BLACK);
							_delay_ms(15000);
							LCD_setScreen(BLACK);
							round_counter1++;

							// Reset scores and ball position
							score1 = 0;
							score2 = 0;
							circlex0 = 79;
							circley0 = 63;
							ball_direction_x = (rand() % 5) ? 1 : -1;
							ball_direction_y = (rand() % 5) ? 1 : -1;

							// Redraw the scoreboard
							drawScoreboard();
						}
// 					   	if (score1 == 4 || score2 == 4 && drawCounter == 0) { // The finish him that I wanted to implement that lagged the game out
// 						   	LCD_drawString(45, 50, "FINISH HIM!", WHITE, BLACK);
// 							drawCounter++;
// 					   	}

	     		    }

	
					   }
}

	
	// 	while (1) //This was how I tested for ADC values
	// 	{
	// 			sprintf(String, "%d\n", ADC);
	// 			print(String);
	// 			_delay_ms(50);
	// 	}
