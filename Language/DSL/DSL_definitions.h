//Terminals
const int L_START_NUM = 6960;
const int L_END_SIGN = '!';
//const int L_END_NUM = 3400;
const int L_ARGS_START_NUM = 1536;
const int L_ARGS_END_SIGN = ':';
const int L_VAR_SPLITTER = ',';
const int L_OPER_SPLITTER = ',';


const char L_START_[] = "Primum";
//const char L_END_[] = "Finis";
const char L_ARGS_START_[] = "bona"; // also used as L_OPEN_: '('

//OPERATORS
const int PLUS_NUM     = '+';
const int MINUS_NUM    = '-';
const int MUL_NUM      = '*';
const int DIV_NUM      = '/';
const int POW_NUM      = '^';
const int EQAL_NUM     = 183;
const int NO_EQAL_NUM  = 155;
const int ABOVE_NUM    = 62;
const int ABOVE_EQ_NUM = 184;
const int BELOW_NUM    = 60;
const int BELOW_EQ_NUM = 182;
const int ASSN_NUM     = '=';
const int IF_NUM       = 309;
const int WHILE_NUM    = 3227;
const int OPER_NUM     = 303;
const int DEF_NUM      = 678;
const int VAR_NUM      = 736;
const int VARLIST_NUM  = 14128;
const int CALL_NUM     = 1557;
const int RETURN_NUM   = 7028;
const int PRINT_NUM    = 3464;
const int READ_NUM     = 1472;
const int D_NUM        = 'D';
const int P_NUM        = 'P';
const int B_NUM        = 'B';
const int C_NUM        = 'C';


const char PLUS_[]     = "+";
const char MINUS_[]    = "-";
const char MUL_[]      = "*";
const char DIV_[]      = "/";
const char POW_[]      = "^";
const char EQAL_[]     = "==";
const char NO_EQAL_[]  = "!=";
const char ABOVE_[]    = ">";
const char ABOVE_EQ_[] = ">=";
const char BELOW_[]    = "<";
const char BELOW_EQ_[] = "<=";
const char ASSN_[]     = "=";
const char IF_[]       = "if";
const char WHILE_[]    = "while";
const char OPER_[]     = "Op";
const char DEF_[]      = "Def";
const char VAR_[]      = "Var";
const char VARLIST_[]  = "VarList";
const char CALL_[]     = "Call";
const char RETURN_[]   = "Return";	//FUNC
const char PRINT_[]    = "Print";	//FUNC
const char READ_[]     = "Read";	//FUNC
const char D_[]        = "D";
const char P_[]        = "P";		//FUNC
const char B_[]        = "B";		//FUNC
const char C_[]        = "C";



const int L_IF_NUM       = 325;		// si
const int L_ELSE_NUM     = 1509;	//alia
const int L_WHILE_NUM    = 3154;	// donec
const int L_DEF_NUM      = 26862;	// Definire
const int L_VAR_NUM      = 112568;	// Variabilis
const int L_RETURN_NUM   = 14132;	// reditum
const int L_PRINT_NUM    = 6729;	// scribo
const int L_READ_NUM     = 1610;	// lego

const int L_EQAL_NUM     = 795;		//est
const int L_NO_EQAL_NUM  = 52961;	//inaequale
const int L_ABOVE_NUM    = 1716;	//plus
const int L_ABOVE_EQ_NUM = 14436;	//plusest
const int L_BELOW_NUM    = 3535;	//minus
const int L_BELOW_EQ_NUM = 28975;	//minusest

const int L_ASSN_NUM     = 53451;	// assignare
const int L_PLUS_NUM     = 1505;	// adde
const int L_MINUS_NUM    = 107077;	// subtrahere
const int L_MUL_NUM      = 56928;	// productum
const int L_DIV_NUM      = 6794;	// partem
const int L_POW_NUM      = 29007;	// quatenus
const int L_CLOSE_NUM    = 3425;	// simul (L_OPEN_ is L_ARGS_="bona")


const char L_IF_[]       = "si";
const char L_ELSE_[]     = "alia";
const char L_WHILE_[]    = "donec";
const char L_DEF_[]      = "Definire";
const char L_VAR_[]      = "Variabilis";
const char L_RETURN_[]   = "reditum";
const char L_PRINT_[]    = "scribo";
const char L_READ_[]     = "lego";

const char L_EQAL_[]     = "est";
const char L_NO_EQAL_[]  = "inaequale";
const char L_ABOVE_[]    = "plus";
const char L_ABOVE_EQ_[] = "plusest";
const char L_BELOW_[]    = "minus";
const char L_BELOW_EQ_[] = "minusest";

const char L_ASSN_[]     = "assignare";
const char L_PLUS_[]     = "adde";
const char L_MINUS_[]    = "subtrahere";
const char L_MUL_[]      = "productum";
const char L_DIV_[]      = "partem";
const char L_POW_[]      = "quatenus";
const char L_CLOSE_[]    = "simul";



//FUNCTIONS
const int SIN_NUM = 765; //'s' + 2 * 'i' + 4 * 'n';
const int COS_NUM = 781; //'c' + 2 * 'o' + 4 * 's';
const int TG_NUM  = 322; //'t' + 2 * 'g';
const int CTG_NUM = 743; //'c' + 2 * 't' + 4 * 'g';
const int ARCSIN_NUM = 6841; //'a' + 2 * 'r' + 4 * 'c' + 8 * 's' + 16 * 'i' + 32 * 'n';
const int ARCCOS_NUM = 6969; //'a' + 2 * 'r' + 4 * 'c' + 8 * 'c' + 16 * 'o' + 32 * 's';
const int ARCTG_NUM  = 3297; // 'a' + 2 * 'r' + 4 * 'c' + 8 * 't' + 16 * 'g';
const int ARCCTG_NUM = 6665; //'a' + 2 * 'r' + 4 * 'c' + 8 * 'c' + 16 * 't' + 32 * 'g';

const int LN_NUM  = 328; //'l' + 2 * 'n';
const int LG_NUM  = 314; // 'l' + 2 * 'g';
const int SQRT_NUM= 1725; //'s' + 2 * 'q' + 4 * 'r' + 8 * 't';


const char SIN_[]    = "sin";
const char COS_[]    = "cos";
const char TG_[]     = "tg";
const char CTG_[]    = "ctg";
const char ARCSIN_[] = "arcsin";
const char ARCCOS_[] = "arccos";
const char ARCTG_[]  = "arctg";
const char ARCCTG_[] = "arcctg";

const char LN_[]     = "ln";
const char LG_[]     = "lg";
const char SQRT_[]   = "sqrt";

const char A_SQRT_[]   = "ROOT";
const char A_[]   = "<Empty>";


//const char phrases[9][45] = {"It's obvious that:\n", "With missing part of transformations:\n", "You can deduce this formula yourself:\n", "By simple transformations we obtain:\n", "", "Anyone with half a brain knows that:\n", "You can find this formula in Wikipedia:\n", "I won't check it of course:\n", "I live this formula as an exercize:\n"};


//#define dL Differentiate (node->left, output)
//#define dR Differentiate (node->right, output)

#define cL Copy (node->left)
#define cR Copy (node->right)


#define NUM(value) CreateNode (CONSTANT, value)

#define VAR(value) CreateNode (VARIABLE, value)

#define FUNC(value, right) CreateNode (FUNCTION, value, NULL, right)

#define OPER(value, left, right) CreateNode (OPERATOR, value, left, right)


#define PLUS(left, right) CreateNode (OPERATOR, '+', left, right)

#define MINUS(left, right) CreateNode (OPERATOR, '-', left, right)

#define MUL(left, right) CreateNode (OPERATOR, '*', left, right)

#define DIV(left, right) CreateNode (OPERATOR, '/', left, right)

#define POW(left, right) CreateNode (OPERATOR, '^', left, right)


#define SIN(right) CreateNode (FUNCTION, SIN_NUM, NULL, right)

#define COS(right) CreateNode (FUNCTION, COS_NUM, NULL, right)

#define LN(right) CreateNode (FUNCTION, LN_NUM, NULL, right)

#define SQRT(right) CreateNode (FUNCTION, SQRT_NUM, NULL, right)


//#define IF(left, right) CreateNode (OPERATOR, IF_NUM, left, right)

//#define WHILE(left, right) CreateNode (OPERATOR, WHILE_NUM, left, right)

//#define ASSN(left, right) CreateNode (OPERATOR, ASSN_NUM, left, right)

//#define OPER(left, right) CreateNode (OPERATOR, OPER_NUM, left, right)

#define D(right) CreateNode (OPERATOR, D_NUM, NULL, right)