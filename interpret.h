#ifndef INTERPRET_H
#define INTERPRET_H

#include "ilist.h"
#include <stdbool.h>
#include "buildin.h"
#include "ial.h"
#include "stack_frame.h"

extern symData* lastReturnedValue;
void interpret(ilist *L);
void getOperands(symData ** op1, symData ** op2, symData ** op3);

#endif // INTERPRET_H
