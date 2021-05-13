/*
* Name: Amyn Jiwani
* Assignment Name: PWM Car Driving Simulator Assignment
* Date: May 5, 2020
* Assignment Description: This program uses a fairly basic and common solution with curses to create an interactive driving program with PWM (Pulse Width Modulation- used to simulate the real experience)
*/

// Project Libraries needed to run the code
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>                  
#include <time.h>
#include <ctype.h>
#include <ncurses.h>

// Global variable
int percentspeed = 0; //percentage of motor power used (also controls the speed of the motor)
int percentage();

// The main function to intiate the program
int main() {	
	printf("Welcome to the Car Simulation program\n");
	printf("Input 'c' to get a list of possible commands/inputs");
	pthread_t motor;										// defines a thread to allow the running of the motor
	pthread_create(&motor, NULL, motorcontrol, NULL);	// creates the thread and replaces each value in the thread with the values of the motorcontrol pointer function above; in addition to being the function that defines the thread
	UserCommands(); 	//function to take take user inputs as commands
}

//Functions
void control(){		//controls the percentage of duty cycle through curses (key input) 
	int character;
	int carvalue = 0; //Helps activate the conditionals used to exit certain loops
  	initscr();		// Intiates curses		
	cbreak();		// Disables line buffering
	refresh();		// Used to refresh screen whenever needed
	keypad(stdscr, TRUE);	// Used to allow user input of special keys (like F1)	
	noecho();		// Prevents echo while gettinng user input
	
	while(1) {
		printf("Try using the arrow keys to increase or decrease the speed of the (pretend) car.\n");
		printf("You can change gear by using numbers '1', '2', '3' '4' or '5'. Just make sure not to overheat!\n");
		printf("Press ENTER to exit.");
		character = getch(); 	//used in order to read user input
		
		switch(character) { //uses the 'switch', a classic and common method for when the program has to choose between multiple user inputs or 'cases'. Executes a different set of code according to the user input by making the case a loop that has to be broken out of each time the button is pressed
			case KEY_DOWN:				//Decrease speed by 2.5 percent each time
				if(percentspeed > 0) {
					erase();			//clears the terminal
					printf("Speed has decreased!");
					percentspeed -= 2.5;
					printw("The car is now running at %i\n", percentspeed);
					refresh(); 			//refreshes the screen
					break;
				}
				//Otherwise, it displays a corresponding message
				else {		
					erase();	//clears the terminal
					printf("Well, looks like you've reached 0 percent speed!");
					refresh();	//refreshes the screen
					break;	
				}
			case KEY_UP:			//Increase speed by 2.5 percent each time
				if(percentspeed < 100) {				
					erase();
					printf("Speed has increased!");
					percent += 2.5; //Increases the speed
					printw("The car is now running at %i\n", percentspeed);
					refresh();
					break;
				}
				else if(percentspeed > 100) {//overheat contingency code (if the percent exceeds 100, it immediatly displays message and shuts down)			
					erase();
					printf("Overheat...shutting down");
					refresh();
					break;
				}
				else {
					erase();
					printf("Well, looks like you've reached 100 percent speed!\n");
					refresh();
					break;	
				}
			
			case 48:    			//48: Key 1; If pressed, leads to 0% Power- this stops the car 
				erase();
				percentspeed = 0; //makes the speed 0
				printf("Percent: %i", percentspeed);
				refresh();
				break;	
				
			case 49:    		    //49: Key 2; If pressed, leads to 25% speed (known as Gear 1)
				erase();
				percentspeed = 25;
				printf("Percent:  %i", percentspeed);
				refresh();
				break;
				
			case 50:    			//50: Key 3; If pressed, leads to 50% speed (known as Gear 2)
				erase();
				percentspeed = 50;
				printf("Percent:  %i\n", percentspeed);
				refresh();
				break;	
				
			case 51:    			//51: Key 4; If pressed, leads to 75% speed (known as Gear 3)
				erase();
				percentspeed = 75;
				printf("Percent:  %i\n", percentspeed);
				refresh();
				break;	
			
			case 52:    			//52: Key 5; If pressed, bypasses rest of the code to lead to 100% speed (known as the Final Gear)
				erase();
				percentspeed = 100;
				printf("Percent:  %i\n", percentspeed);
				refresh();
				break;		
		
			case 10:			 //49: Key ENTER; If pressed, exits the motor control
				carvalue = 1;
				break;	
			
			default:		//If user presses an invalid key, it will print the current percentage.
				erase();
				printf("Percent:  %i\n", percentspeed);
				refresh();
				break;	
		}
		percentage(percentspeed);
		if(carvalue != 0) // Breaks out of the loop if the carvalue isn't 0 (ensures that it breaks out)
			break;		
	}
	erase();			//This clears the terminal
	endwin();			//End curses mode
}

//declares pointer function- used for threads with no data type association
void *motorcontrol(void *ptr) {		

  	struct timespec t, t2; //defines list of variables to be easily manipulated
  	t.tv_sec  = 0; //seconds
  	t.tv_nsec = percentage(percentspeed); //percent speed in microseconds, keeps it at 20000us- 50 Hertz (as specified)
  	while(1) {
		//INSERT outb HERE
		nanosleep(&t , &t2); //sleep for 0.02 seconds
	}
}

int percentage(int percentspeed) { //defines the function 
  	int timeon = ((percentspeed*20000)/100); //20000: us-50 Hz
  	int timeoff = 20000 - timeon;  //formula to canculate time off
	initscr();
  	printf("Time of motor being on: %i microseconds\n", timeon); //prints how long the motor has been on for
  	printf("Time of motor being off: %i microseconds\n", timeoff);
	endwin(); //restores the curses terminal
  	return timeoff;
}

void UserCommands() {
	while(1) {
		char userInput;
		scanf("%c", &userInput); // Checks for user input/characters
		if (userInput != '\n') { //if the user inputted any of the below cases; uses switch function to run said case it
			switch(tolower(userInput)) { // Again, use of Switch function to appropriate function
				case 'c':
					printf("Press 'p' to change the percentage of the duty cycle\n");
					printf("Press 'z' to exit the program\n");
					break;
				
				case 'p': // Lets the user change percentage of the duty cycles
					printf("Input an int value to assign it as the new percentage of the duty cycle\n");
					int percentInput = 0;
					scanf("%i", &percentInput);
						if(percentInput >= 0 && percentInput <= 100) {
              				system("Clearing...");
							printf("Changing the duty cycle to %i percent\n", percentInput);
							printf("Input 'c' to get a list of possible inputs\n"); //Gives user option to check all possibilities
							percent = percentInput;
							control();
              				break; //breaks out of the case
						} 
						else {
							system("Clearing...");
							printf("Invalid input\n");
							printf("Input 'c' to get a list of possible inputs\n"); //Gives user option to check all possibilities
						}
				
				case 'z':	// Lets the user exit the program
					printf("Exiting the program...\n");
					exit(0);
					
				default: 	//Sets error message as default
					system("Clearing...");
					printf("Invalid input\n");
					printf("Input 'c' to get a list of possible inputs\n"); //Gives user option to check all possibilities
				break; 		//breaks out of the case
			}
			
		}
	}
}
// The main function to intiate the program
int main() {	
	printf("Welcome to the Car Simulation program\n");
	printf("Input 'c' to get a list of possible commands/inputs");
	pthread_t motor;										// defines a thread to allow the running of the motor
	pthread_create(&motor, NULL, motorcontrol, NULL);	// creates the thread and replaces each value in the thread with the values of the motorcontrol pointer function above; in addition to being the function that defines the thread
	UserCommands(); 	//function to take take user inputs as commands
}

