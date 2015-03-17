// Declare functions
bool can_i_move(int from_x, int from_y, int to_x, int to_y, char drone_symbol, char state);

#define KRED  "\x1B[31m"   // Home
#define RESET "\033[0m"

/***************************************************************/
/*                                                             */
/*                Declare global variables                     */
/*                                                             */
/***************************************************************/

// Define grid width and height
int grid_width  = 25;
int grid_height = 17;

// Create a multidimensional array simulating a grid (in y,x format)
char grid[17][31];

// Define the HOME BASE location on the grid
int home_x = 12;
int home_y = 8;

// Define how many DROP OFF locations there are
int how_many_drop_offs = 4;

// Define the DROP OFF locations on the grid
int dropoffs[6][2] = { {5,8}, {5,16}, {11,8}, {11,16}};


// Define drone count variables
int drone_count   = 0;
int active_drones = 0;

// Define a character to be used for blank spaces on the grid
char empty_spot_char = ' ';

// Declare a mutex lock
pthread_mutex_t lock;

// Symbols used to associate a color to a drone
char drone_symbols[5] = {'a', 'b', 'c', 'd', 'e'};
int  drone_current_symbol = 0;




/***************************************************************/
/*                                                             */
/*     Retrieve the user input                                 */
/*     - the user will enter how many drones he/she wants      */
/*                                                             */
/***************************************************************/
void getUserInput() {
	
	printf("\nEnter the number of drones you would like: ");
	scanf ("%d", &drone_count);
	printf("\n");

	if (drone_count < 1)
		getUserInput();
	
}




/***************************************************************/
/*                                                             */
/*     Initialize mutex lock                                   */
/*                                                             */
/***************************************************************/
void makeLock() {
	pthread_mutex_init(&lock, NULL);
}




/***************************************************************/
/*                                                             */
/*     Drone thread function                                   */
/*     - this function deals with the entire drone function    */
/*                                                             */
/***************************************************************/
void* drone() {
	
	// Define drone variables
	int rand_int    = rand() % how_many_drop_offs;  // Random number based on number of drop offs
	int final_x     = dropoffs[rand_int][1];        // Assign the X location of the drop off
	int final_y     = dropoffs[rand_int][0];        // Assign the Y location of the drop off
	int current_x   = home_x;						// Current X location (starts at home base)
	int current_y   = home_y;						// Current Y location (starts at home base)
	bool first_run  = true;							// Variable checking if it is the first run
	char state      = '>';                          // > means going, and < means coming back 


	// Assign a drone a symbol (used for coloring on the grid)
	char my_symbol = drone_symbols[ drone_current_symbol ];

	// Increase/reset the color after spawning a drone
	if (drone_current_symbol == 4)
		drone_current_symbol = 0;
	else
		drone_current_symbol = drone_current_symbol+1;
	

	// Start drone
	while(1) {
		
		// Lock the mutex
		//pthread_mutex_lock(&lock);
		
		// Check if the drone is still moving to his drop off location
		if (state == '>') {
			// Check to see if the drone has arrived at his drop off location
			if ( (current_x == final_x) && (current_y == final_y) ) {
				state   = '<';    // Change state to < which means going back
				final_x = home_x; // Set drone X destination back to home
				final_y = home_y; // Set drone Y destination back tohome
			}
		}
		// Check if the drone is still moving back to the home base
		else if (state == '<') {
			// Check to see if the drone has arrived back home
			if ( (current_x == final_x) && (current_y == final_y) ) {
				drone_count   = drone_count - 1;   // Decrease the drone count
				active_drones = active_drones - 1; // Decrease the active drone count
				grid[home_y][home_x] = 'H';        // Set the home base char back to 'H' from 'O'
				
				pthread_exit(NULL);                // Exit thread
			}
		}
		
		
		
		// ***************************************************************************
		//                                                                           *
		// The following code determines the next logical move the drone should make *
		//                                                                           *
		// ***************************************************************************
		
		// *************************************************************************
		// Diaganol movements
		//   - As diaganol movements are the most logical, the next 4 if statements
		//     check to see if the drone can move diagonally WITHOUT COLLISION
		// *************************************************************************
		if ( (current_y > final_y) && (current_x < final_x) && can_i_move(current_x, current_y, current_x+1, current_y-1, my_symbol, state) ) {
			
			// UP+RIGHT 
			current_y = current_y-1;
			current_x = current_x+1;			
		}
		else if ( (current_y > final_y) && (current_x > final_x) && can_i_move(current_x, current_y, current_x-1, current_y-1, my_symbol, state) ) {
			
			// UP+LEFT
			current_y = current_y-1;
			current_x = current_x-1;			
		}
		else if ( (current_y < final_y) && (current_x < final_x) && can_i_move(current_x, current_y, current_x+1, current_y+1, my_symbol, state) ) {
			
			// DOWN+RIGHT
			current_y = current_y+1;
			current_x = current_x+1;
		}
		else if ( (current_y < final_y) && (current_x > final_x) && can_i_move(current_x, current_y, current_x-1, current_y+1, my_symbol, state) ) {
			
			// DOWN+LEFT
			current_y = current_y+1;
			current_x = current_x-1;			
		}
		// *************************************************************************
		// Up/Down/Left/Right movements
		//   - If the above diagonal movement was unsuccessful, we try the next
		//     logical movement, which is Up/Down/Left/Right. We check to see if
		//     the drone can move up/down/left/right WITHOUT COLLISION
		// *************************************************************************
		else if ( (current_x < final_x) && can_i_move(current_x, current_y, current_x+1, current_y, my_symbol, state) ) {
			
			//move right
			current_x = current_x+1;
		}
		else if ( (current_x > final_x) && can_i_move(current_x, current_y, current_x-1, current_y, my_symbol, state) ) {
			
			//move left
			current_x = current_x-1;
		}
		else if ( (current_y < final_y) && can_i_move(current_x, current_y, current_x, current_y+1, my_symbol, state) ) {
			
			//move up
			current_y = current_y+1;		
		}
		else if ( (current_y > final_y) && can_i_move(current_x, current_y, current_x, current_y-1, my_symbol, state) ) {
			
			//move down
			current_y = current_y-1;		
		}
	    // *************************************************************************
	    // The following code essentially just makes a 'safe' move if the above
	    // logical move attempts were not successfull
	    // *************************************************************************
		else {
			
			bool random_move_sucess = false;
			int  random_move_number;
			int  random_attempts = 0;
			
			//printf("HERE NUMBER: %d", random_move_number);
		
			
			while(random_move_sucess == false || random_attempts == 6) {
				random_move_number = rand() % 4;
				
				if (can_i_move(current_x, current_y, current_x, current_y-1, my_symbol, state) && random_move_number == 0) {
					random_move_sucess = true;
					//move down
					current_y = current_y-1;
				}

				else if (can_i_move(current_x, current_y, current_x-1, current_y, my_symbol, state) && random_move_number == 1) {
					random_move_sucess = true;
					//move left
					current_x = current_x - 1;
				}

				else if (can_i_move(current_x, current_y, current_x, current_y+1, my_symbol, state) && random_move_number == 2) {
					random_move_sucess = true;
					//move up
					current_y = current_y+1;
				}

				else if (can_i_move(current_x, current_y, current_x+1, current_y, my_symbol, state) && random_move_number == 3) {
					random_move_sucess = true;
					//move right
					current_x = current_x + 1;
				}
				
				random_attempts = random_attempts+1;
			}
				
		}
	
	
		
		// - Check to see if the drone has left the home base yet. If it has,
		//   it is now considered to be an active drone, thus we increase the
		//   active drone count
		if ( (first_run == true) && ( (current_x != home_x) || (current_y != home_y) ) ) {
			active_drones = active_drones + 1;
			first_run = false;
		}


		// Sleep for 1 second and unlock the mutex
		sleep(5);			
		//pthread_mutex_unlock(&lock);
	}
}




/***************************************************************/
/*                                                             */
/*     Can I move function                                     */
/*     - This function is used with the drone thread to check  */
/*       if a location on the grid is free to move             */
/*     - This function also does the move as well              */
/*                                                             */
/***************************************************************/
bool can_i_move(int from_x, int from_y, int to_x, int to_y, char drone_symbol, char state) 
{
	
	// Ensure we are first definetely safe to move to the next spot, if so continue
	if (grid[to_y][to_x] == '*') {
		printf(KRED "Reroute at %d,%d  |  "RESET, to_y,to_x);
		return false;
	}
	else if ( (grid[to_y][to_x] != 'a') && (grid[to_y][to_x] != 'b') && 
	     (grid[to_y][to_x] != 'c') && (grid[to_y][to_x] != 'd') && 
	     (grid[to_y][to_x] != 'e') && (grid[to_y][to_x] != 'O')) {
		
		
		
		// Do not allow a drone to enter the HOME location unless he has delivered his package
		if ( (to_y == home_y && to_x == home_x) && (state == '>') ){
			return false;
		}
	    
		
		// If we are moving INTO an BLANK SPOT, we are clear to move
		if ( (grid[to_y][to_x] != 'H') && (grid[to_y][to_x] != 'D') ) {
			grid[to_y][to_x] = drone_symbol;
		}
		
		// If we are moving INTO an H or a D, we are clear to move, we change the char to O to claim the spot
		if ( (grid[to_y][to_x] == 'H') || (grid[to_y][to_x] == 'D') ) {
			grid[to_y][to_x] = 'O';
		}
		
		
		
		// If we are moving AWAY from a DROPOFF point, we change the char back to 'D'
		if (grid[from_y][from_x] == 'O') {
			grid[from_y][from_x] = 'D';
		}
		// If we are moving AWAY from empty airspace, we change the char back to the empty spot char
		else if ( (grid[from_y][from_x] != 'H') && (grid[from_y][from_x] != 'D') ) {
			grid[from_y][from_x] = empty_spot_char;
		}
		
		
		// Let the drone know that he moved
		return true;
	} else {
		// Let the drone know he has avoided a collision and to check for another move
		return false;
	}
}
