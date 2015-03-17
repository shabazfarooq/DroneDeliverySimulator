/***************************************************************/
/*     Author: Shabaz Farooq                                   */
/*                                                             */
/*     Title:  DECOY Drone Simulator                           */
/***************************************************************/


// Include required files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#include "drone.c"
#include "grid.c"


	
int main() {
	
	// Get drone count from user input
	getUserInput();

	// Initialize pthread lock
	makeLock();
	
	// Initialize a multidimensional array to simulate a grid
	grid_generate();


	// Create a thread for the GRID
	pthread_t tgrid;
	pthread_create(&tgrid, NULL, grid_print_thread, NULL);

	
	//Create threads for DRONES
	pthread_t threads[drone_count];
	
	int i;

	for (i=0; i < drone_count; i++) {
		pthread_create(&threads[i], NULL, drone, NULL);
		sleep(1);
	}

	// Wait for the grid to complete before ending
	pthread_join(tgrid, NULL);
	

	return 1;
}
