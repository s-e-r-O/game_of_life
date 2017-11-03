#include "utils.h"

void neighbours_state_init(int n_neighbours, int neighbours_state[]){
	for (int i=0; i < n_neighbours; i++){
		neighbours_state[i] = NOT_SET;
	}
}