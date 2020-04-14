#define DEBUG 0
#define DEFAULT __FILE__, __LINE__, __FUNCTION__

#ifdef DEBUG
	#define ON_DEBUG(code) code
#else
	#define ON_DEBUG(code) ;
#endif


class List
{
private:
	const int MAX_PNG_NAME = 20;
	const int RESIZE_VALUE = 5;
	const int HYSTERESIS   = 2;

	char **data = {};
	int *next = {};
	int *prev = {};
	int head = 0;
	int tail = 0;
	int size = 0;
	int free_elem = 1;
	bool sort = 1;

public:
	List(int length);
	~List();
	//bool ListConstruct (int length);
	bool ListResize (int resize_val);
	//bool ListDestruct (List *lst);
	//bool ListFree (List *lst);

	int InsertAfter (int prev_num, int value);
	//int InsertBefore (int next_num, int value);
	//bool Delete (int num);
	int FindByValue (int value);
	bool CmpStr (char *str_1, char *str_2);
	//int FindByOrder (int order_num);
	//bool SortList (List *lst);

	//void Dump (const char *reason = "Dump", const char *file = "Not pointed file", const int line = -1, const char *function = "Not pointed function");
	//void GraphDump (char png_file[MAX_PNG_NAME], const char mode = 'l');
	int ListOk ();
	void CheckList ();
};