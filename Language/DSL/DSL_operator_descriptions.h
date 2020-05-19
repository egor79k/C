//Used in Differentiate, Simplify
OPERATOR(PLUS_, PLUS(dL, dR),                                          NUM(num_1 + num_2), )

OPERATOR(MINUS_, MINUS(dL, dR),                                         NUM(num_1 - num_2), )

OPERATOR(MUL_, PLUS(MUL(dL, cR), MUL(cL, dR)),                        NUM(num_1 * num_2), )

OPERATOR(DIV_, DIV(MINUS(MUL(dL, cR), MUL(cL, dR)), POW(cR, NUM(2))), NUM(num_1 / num_2), )

OPERATOR(POW_, PLUS(MUL(cR, MUL(POW(cL, MINUS(cR, NUM(1))), dL)), MUL(POW(cL, cR), MUL(dR, LN(cL)))), NUM(pow (num_1, num_2)), )

OPERATOR(EQAL_, , NULL, )

OPERATOR(NO_EQAL_, , NULL, )

OPERATOR(ABOVE_, , NULL, \\)

OPERATOR(ABOVE_EQ_, , NULL, \\)

OPERATOR(BELOW_, , NULL, \\)

OPERATOR(BELOW_EQ_, , NULL, \\)

OPERATOR(ASSN_, , NULL, )

OPERATOR(IF_, , NULL, )

OPERATOR(WHILE_, , NULL, )

OPERATOR(OPER_, , NULL, )

OPERATOR(DEF_, , NULL, )

OPERATOR(VAR_, , NULL, )

OPERATOR(VARLIST_, , NULL, )

OPERATOR(CALL_, , NULL, )

OPERATOR(D_, , NULL, )

OPERATOR(B_, , NULL, )

OPERATOR(C_, , NULL, )
//MUL(dL, MUL(cR, POW(cL, MINUS(cR, NUM(1)))))