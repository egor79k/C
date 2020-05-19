//Used in Differentiate, PrintNode, WriteLabels, GetF, LaTeX_Tree
FUNCTION(SIN_,    MUL(dR, COS(cR)), A_)

FUNCTION(COS_,    MUL(dR, MUL(NUM(-1), SIN(cR))), A_)

FUNCTION(TG_,     MUL(dR, DIV(NUM(1), POW(COS(cR), NUM(2)))), A_)

FUNCTION(CTG_,    MUL(dR, DIV(NUM(-1), POW(SIN(cR), NUM(2)))), A_)

FUNCTION(ARCSIN_, MUL(dR, DIV(NUM(1), SQRT(MINUS(NUM(1), POW(cR, NUM(2)))))), A_)

FUNCTION(ARCCOS_, MUL(dR, DIV(NUM(-1), SQRT(MINUS(NUM(1), POW(cR, NUM(2)))))), A_)

FUNCTION(ARCTG_,  MUL(dR, DIV(NUM(1), PLUS(NUM(1), POW(cR, NUM(2))))), A_)

FUNCTION(ARCCTG_, MUL(dR, DIV(NUM(-1), PLUS(NUM(1), POW(cR, NUM(2))))), A_)


FUNCTION(LN_,     MUL(dR, DIV(NUM(1), cR)), A_)

FUNCTION(LG_,     MUL(dR, DIV(NUM(1), MUL(cR, LN(NUM(10))))), A_)

FUNCTION(SQRT_,   MUL(dR, DIV(NUM(1), MUL(NUM(2), SQRT(cR)))), A_SQRT_)


FUNCTION(P_,      , A_)

FUNCTION(RETURN_, , A_)

FUNCTION(PRINT_,  , A_)

FUNCTION(READ_,   , A_)