
#define ALIVE 10
#define DEAD 20
#define NOT_SET 30

struct state_msg{
	int id;
	int state;
};

void neighbours_state_init(int n_neighbours, int neighbours_state[]);