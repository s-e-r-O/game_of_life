#include "connection.h"

void array_state_reset(int n, int array_state[]){
	for (int i=0; i < n; i++){
		array_state[i] = NOT_SET;
	}
}