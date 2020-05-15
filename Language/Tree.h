
//#define DEBUG 0

#define CHECK_TYPE if (type != OPERATOR && type != CONSTANT && type != VARIABLE && type != FUNCTION) return NULL;

#ifdef DEBUG
	#define ON_DEBUG(code) code
#else
	#define ON_DEBUG(code) ;
#endif


typedef double elem_t;


struct tree
{
	elem_t data;
	struct tree *left;
	struct tree *right;
	struct tree *parent;
	int level;
	char type;
};


const int MAX_FILE_NAME = 35;

const char OPERATOR = 'O';
const char CONSTANT = 'C';
const char VARIABLE = 'V';
const char FUNCTION = 'F';


tree *TreeConstruct (char type, elem_t value);

tree *CreateNode (char type, elem_t value, tree *left = NULL, tree *right = NULL);
tree *InsertLeft (tree *parent, char type, elem_t value);
tree *InsertRight (tree *parent, char type, elem_t value);
void DeleteNode (tree *node);
void Delete (tree *node);
void CopyNode (tree *node, tree *node_new);
tree *Copy (tree *root);
tree *FindNode (tree *root, elem_t value);

void PrintNode (tree *node, FILE *output);
void PrintTree (tree *root, char file_name[MAX_FILE_NAME]);
//char *Get_Buff_From_File (char file_name[MAX_FILE_NAME]);
void WriteLabels (tree *node, FILE *out);
void WriteConnections (tree *node, FILE *out);
void Dump (tree *root, char png_file[MAX_FILE_NAME], const char mode = 'l');

int TreeOk (tree *node);
void CheckTree (tree *root);


//Tree reading functions
tree *ReadTree (char *file_name);
tree *GetG ();	//
tree *GetE ();	// Get + -
tree *GetT ();	// Get * /
tree *GetD ();	// Get ^
tree *GetP ();	// Get ( )
tree *GetF ();	// Get function
tree *GetN ();	// Get num
tree *GetV ();	// Get variable
