
const int MAX_FILE_NAME = 50;
char WORLD_FILE_NAME[MAX_FILE_NAME] = "new_world.g";

const int WORLD_NAME_SIZE = 20;
char *WORLD_NAME = 0;

const int INIT_SIZE = 6 * sizeof (us_t) + WORLD_NAME_SIZE;

const us_t VERSION = 0;

us_t GLOBAL_LENGTH = 171;	// World size
us_t GLOBAL_HEIGHT = 44;
us_t SCREEN_LENGTH = COLS;	// Screen size
us_t SCREEN_HEIGHT = LINES;

us_t HORIZONT = 0;

us_t CHUNK_X = 0;
us_t CHUNK_Y = 0;

us_t GLOBAL_PX = 1;				//Player position
us_t GLOBAL_PY = 36;
us_t SCREEN_PX = 1;				//Player position
us_t SCREEN_PY = 36;
					// ^ M[*][]
uc_t **MATRIX = 0;	// |             Gobal world matrix
					// +---> M[][*]
ull_t COUNTER = 0;