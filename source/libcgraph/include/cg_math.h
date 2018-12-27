#ifndef CG_MATH_H
#define CG_MATH_H

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <malloc.h>

#include "cgraph.h"
#include "cg_operation.h"
#include "cg_types.h"
#include "cg_variables.h"
#include "cg_errors.h"
#include "cg_constants.h"
#include "cg_enums.h"
#include "cg_factory.h"

CGNode* sigmoid(CGNode* x);
CGNode* softmax(CGNode* x, uint8_t axis);
CGNode* crossEntropy(CGNode* x, CGNode* y);
CGNode* relu(CGNode* x);

// log(1+ e^x)
CGNode* smoothRelu(CGNode* x);


CGNode* variance(CGNode* X, uint8_t axis);


#endif
