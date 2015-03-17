/***************************************************************/
/*                                                             */
/*  Define color variables                                     */
/*                                                             */
/***************************************************************/
#define KNRM  "\x1B[0m"    // Drone
#define KMAG  "\x1B[35m"   // Drone
#define KCYN  "\x1B[36m"   // Drone
#define KYEL  "\x1B[33m"   // Drone
#define KBLU  "\x1B[34m"   // Border / Drone
#define KRED  "\x1B[31m"   // Home
#define KGRN  "\x1B[32m"   // Dropoff
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"


// A variable used to ensure the map is printed a final time.
bool final_print = false;


/***************************************************************/
/*                                                             */
/*  Grid print thread function                                 */
/*  - This is a thread that prints the grid                    */
/*                                                             */
/***************************************************************/
void* grid_print_thread() {

	// Initially place an 'H' at the predefined HOME BASE location
	grid[home_y][home_x] = 'H';
	
	
	
	//********************/
	//     Obstacles     //
	//********************/	
	//   y  x    home= 8,12
	grid[8][10] = '*';
	grid[8][14] = '*';
	
	grid[6][12]  = '*';
	grid[10][12] = '*';
	
	grid[6][10]  = '*';
	grid[6][14]  = '*';
	grid[10][10] = '*';
	grid[10][14] = '*';
	
	
	
	// Place a 'D' at the predefined DROP OFF locations
	int i;
	for (i=0; i < how_many_drop_offs; i++) {
		grid[dropoffs[i][0]][dropoffs[i][1]] = 'D';
	}
	
	
	// Start print grid thread
	while(1) {
		
		// Print the home location
		grid[home_y][home_x] = 'H';
		
		
		// Print the total drone count and the active drone count
		printf("\n\n             Drone Count: %d | Active Drones: "KRED"%d\n"RESET, drone_count, active_drones);
	
		
		// Print the top border of the grid
		printf(KBLU "     ***************************************************\n"RESET); 	
		
		
		//**********************************************************
		// Run through the multidimensional array and print the grid
		//**********************************************************
		int i;
		for(i=0; i < grid_height; i++) {
		
			int j;
			for (j=0; j < grid_width; j++) {
				if(j == 0) {
					printf("     "KBLU"*"RESET"%c ", grid[i][j]);
				} else if (j == grid_width-1) {
					printf("%c"KBLU"*"RESET, grid[i][j]);
				} else {
					
					// DRONE colors
					if      (grid[i][j] == 'a') printf(KCYN"+ "RESET);
					else if (grid[i][j] == 'b') printf(KMAG"+ "RESET);
					else if (grid[i][j] == 'c') printf(KBLU"+ "RESET);
					else if (grid[i][j] == 'd') printf(KNRM"+ "RESET);
					else if (grid[i][j] == 'e') printf(KYEL"+ "RESET);
					// DROPOFF color
					else if (grid[i][j] == 'D') printf(KGRN"%c "RESET, grid[i][j]);
					// Delivered color
					else if (grid[i][j] == 'O') printf(KNRM"%c "RESET, grid[i][j]);
					// Home color
					else if (grid[i][j] == 'H') printf(KRED"%c "RESET, grid[i][j]);
					// Anything else
					else printf("%c ", grid[i][j]);
					
					
				}
			}	
			printf("\n");
		}

		// Print the bottom border of the grid
		printf(KBLU "     ***************************************************\n"RESET);
		
		//print a key to show the 'H' 'D' and '+' mean
		printf("  " KRED "H" RESET " = Home  |  " KGRN "D" RESET " = Drop Off  |  " KCYN "+" RESET " = Drone  |  * = Obstacle      \n\n");
    


		// If all the drones have returned home, kill this thread.
		if (final_print == true)
			pthread_exit(NULL);
		
		// If all the drones have returned home, allow this thread to run once more.
		if (drone_count == 0)
			final_print = true;
		
		
		
		// Sleep for 1 second and unlock the mutex
		sleep(1);
		
	}

}


/***************************************************************/
/*                                                             */
/*  Grid generate                                              */
/*  - This function fills in blank spots in the entire array   */
/*                                                             */
/***************************************************************/
void grid_generate() {
	int i;
	for(i=0; i < grid_height; i++) {
		
		int j;
		for (j=0; j < grid_width; j++) {
			grid[i][j] = empty_spot_char;
		}
	} 	
	
}

