#define DEBUG 0
#define DEFAULT __FILE__, __LINE__, __FUNCTION__

#ifdef DEBUG
	#define ON_DEBUG(code) code
#else
	#define ON_DEBUG(code) ;
#endif

const int MAX_PNG_NAME = 20;
const int RESIZE_VALUE = 5;
const int HYSTERESIS   = 2;


struct list
{
	int *data = {};
	int *next = {};
	int *prev = {};
	int head = 0;
	int tail = 0;
	int size = 0;
	int free_elem = 1;
	char *name = "";
	bool sort = 1;
};


bool ListConstruct (list *lst, char *name, int length);
bool ListResize (list *lst, int resize_val);
bool ListDestruct (list *lst);
bool ListFree (list *lst);

int InsertAfter (list *lst, int prev_num, int value);
int InsertBefore (list *lst, int next_num, int value);
bool Delete (list *lst, int num);
int FindByValue (list *lst, int value);
int FindByOrder (list *lst, int order_num);
bool SortList (list *lst);

void Dump (list *lst, const char *reason = "Dump", const char *file = "Not pointed file", const int line = -1, const char *function = "Not pointed function");
void GraphDump (list *lst, char png_file[MAX_PNG_NAME], const char mode = 'l');
int ListOk (list *lst);
void CheckList (list *lst);