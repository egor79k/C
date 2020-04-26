
#define CPrint(X, Y, STR)	printf("\e[%d;%dH%c", Y, X, STR);

#define SetPlayer	CPrint (SCREEN_PX, SCREEN_PY, player);	\
					fflush (stdout);

#define RemovePlayer	CPrint (SCREEN_PX, SCREEN_PY, ' ');


#define DEBUG 0
//#define DEBUG_PROF 0

#ifdef DEBUG
	#define ON_DEBUG(code) code
#else
	#define ON_DEBUG(code) ;
#endif

#ifdef DEBUG_PROF
	#define ON_DEBUG_PROF(code) code
#else
	#define ON_DEBUG_PROF(code) ;
#endif

typedef unsigned long long ull_t;
typedef unsigned short us_t;
typedef unsigned char uc_t;
typedef unsigned int ui_t;
