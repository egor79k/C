//Jumps matching for Frontend and Backend
JMP(jne, EQAL_NUM,     L_EQAL_)
JMP(je,  NO_EQAL_NUM,  L_NO_EQAL_)
JMP(jbe, ABOVE_NUM,    L_ABOVE_)
JMP(jb,  ABOVE_EQ_NUM, L_ABOVE_EQ_)
JMP(jae, BELOW_NUM,    L_BELOW_)
JMP(ja,  BELOW_EQ_NUM, L_BELOW_EQ_)