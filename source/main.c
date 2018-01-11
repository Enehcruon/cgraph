/*
 * 
 * 
 */

#include <stdlib.h>
#include <stdio.h>

#include "ccgraph.h"
#include "cg_operation.h"
#include "cg_types.h"
#include "cg_variables.h"
#include "cg_factory.h"
#include "cg_constants.h"

#include "memory.h"

#define PROFILER_DEFINE
#include "profiler.h"

void runMult_MV(){
	printf("Running M.v example\n");
	double value1[] = {
		3, 1, 3,
		1, 5, 9, 
		2, 6, 5,
		1, 1, 1
	};
	
	double value2 []= {
		-1.0, -1.0, 1.0,
	};
	
	CGNode* lhsNode = makeMatrixConstantNode(4, 3, value1);
	CGNode* rhsNode = makeVectorConstantNode(3, value2);

	CGNode* node = makeBinaryOpNode(CGBOT_MULT, lhsNode, rhsNode);
	
	CGResultNode* result = computeCGNode(node);
	CGVector* Y = (CGVector*)result->value;

	printf("result: %s shape: %d\n", getVariableTypeString(result->type), Y->len);
	uint64_t i = 0;
	
	for(;i<Y->len;i++){
		printf("\t%f\n", Y->data[i]);
	}
}

void runMult_MM(){
	printf("Running M.M example\n");
	double value1[] = {
		1, 2, 3,
		4, 5, 6
	};
	
	double value2 []= {
		7, 8,
		9, 10,
		11, 12
	};
	
	CGNode* lhsNode = makeMatrixConstantNode(2, 3, value1);
	CGNode* rhsNode = makeMatrixConstantNode(3, 2, value2);

	CGNode* node = makeBinaryOpNode(CGBOT_MULT, lhsNode, rhsNode);
	
	CGResultNode* result = computeCGNode(node);
	CGMatrix* Y = (CGMatrix*)result->value;

	printf("result: %s shape: (%d, %d)\n", getVariableTypeString(result->type), Y->rows, Y->cols);
	uint64_t i = 0;
	uint64_t j = 0;
	
	for(;i<Y->rows;i++){
		for(j = 0;j<Y->cols;j++){
			printf("\t%f", Y->data[i*Y->cols +j]);
		}
		printf("\n");
	}
}

void runMult_Md(){
	printf("Running M.d example\n");
	double value1[] = {
		1, 2, 3,
		4, 5, 6
	};
	
	double value2 = -0.5;
	
	CGNode* lhsNode = makeMatrixConstantNode(2, 3, value1);
	CGNode* rhsNode = makeDoubleConstantNode(value2);

	CGNode* node = makeBinaryOpNode(CGBOT_MULT, lhsNode, rhsNode);
	
	CGResultNode* result = computeCGNode(node);
	CGMatrix* Y = (CGMatrix*)result->value;

	printf("result: %s shape: (%d, %d)\n", getVariableTypeString(result->type), Y->rows, Y->cols);
	uint64_t i = 0;
	uint64_t j = 0;
	
	for(;i<Y->rows;i++){
		for(j = 0;j<Y->cols;j++){
			printf("\t%f", Y->data[i*Y->cols +j]);
		}
		printf("\n");
	}
}


void runMult_dd(){
	printf("Running d.d example\n");
	double value1 = 3.14;
	
	double value2 = 0.5;
	
	CGNode* lhsNode = makeDoubleConstantNode(value1);
	CGNode* rhsNode = makeDoubleConstantNode(value2);

	CGNode* node = makeBinaryOpNode(CGBOT_MULT, lhsNode, rhsNode);
	
	CGResultNode* result = computeCGNode(node);
	CGDouble* Y = (CGDouble*)result->value;

	fprintf(stdout, "result: %s value: %f\n", getVariableTypeString(result->type), Y->value);
}


void runMult_dV(){
	printf("Running d.v example\n");
	double value1 = 3.14;
	
	double value2 []= {
		-1.0, -1.0, 1.0,
	};
	
	CGNode* lhsNode = makeDoubleConstantNode(value1);
	CGNode* rhsNode = makeVectorConstantNode(3, value2);

	CGNode* node = makeBinaryOpNode(CGBOT_MULT, lhsNode, rhsNode);
	
	CGResultNode* result = computeCGNode(node);
	CGVector* Y = (CGVector*)result->value;

	printf("result: %s shape: %d\n", getVariableTypeString(result->type), Y->len);
	uint64_t i = 0;
	
	for(;i<Y->len;i++){
		printf("\t%f\n", Y->data[i]);
	}
}

void runMult_MvM(){
	printf("Running M.(M.v) example\n");
	double value1[] = {
		3, 1, 3,
		1, 5, 9, 
		2, 6, 5,
		1, 1, 1
	};
	
	double value2[]= {
		-1.0, -1.0, 1.0,
	};
	double value3[] = {
		1, 2, 3,
		4, 5, 6
	};
	
	
	CGNode* lhsNode1 = makeMatrixConstantNode(4, 3, value1);
	CGNode* rhsNode1 = makeVectorConstantNode(3, value2);

	CGNode* rhsNode2 = makeBinaryOpNode(CGBOT_MULT, lhsNode1, rhsNode1);
	CGNode* lhsNode2 = makeMatrixConstantNode(2, 3, value3);
	
	CGNode* node = makeBinaryOpNode(CGBOT_MULT, lhsNode2, rhsNode2);
	
	CGResultNode* result = computeCGNode(node);
	CGVector* Y = (CGVector*)result->value;

	printf("result: %s shape: %d\n", getVariableTypeString(result->type), Y->len);
	uint64_t i = 0;
	
	for(;i<Y->len;i++){
		printf("\t%f\n", Y->data[i]);
	}
}

/*
 * the following function will fail on purpose
 */
void runDiv_MM(){
	printf("Running M/M example which WILL FAIL on purpose\n");
	double value1[] = {
		3, 1, 3,
		1, 5, 9, 
		2, 6, 5,
		1, 1, 1
	};
	
	double value2[]= {
		-1.0, -1.0, 1.0,
	};
	
	CGNode* lhsNode = makeMatrixConstantNode(4, 3, value1);
	CGNode* rhsNode = makeVectorConstantNode(3, value2);

	CGNode* node = makeBinaryOpNode(CGBOT_DIV, lhsNode, rhsNode);
	
	CGResultNode* result = computeCGNode(node);
	CGVector* Y = (CGVector*)result->value;

	printf("result: %s shape: %d\n", getVariableTypeString(result->type), Y->len);
	uint64_t i = 0;
	
	for(;i<Y->len;i++){
		printf("\t%f\n", Y->data[i]);
	}
}


void runDiv_Vd(){
	printf("Running V/d example\n");
	double value1[] = {
		3,
		1, 
		5
	};
	
	double value2 = 2;
	
	
	CGNode* lhsNode = makeVectorConstantNode(3, value1);
	CGNode* rhsNode = makeDoubleConstantNode(value2);
	CGNode* node = makeBinaryOpNode(CGBOT_DIV, lhsNode, rhsNode);
	
	CGResultNode* result = computeCGNode(node);
	CGVector* Y = (CGVector*)result->value;

	printf("result: %s shape: %d\n", getVariableTypeString(result->type), Y->len);
	uint64_t i = 0;
	
	for(;i<Y->len;i++){
		printf("\t%f\n", Y->data[i]);
	}
}

void runDiv_Md(){
	/*
	 * TODO: check if d == 0 then throw error
	 */
	printf("Running M/d example\n");
	double value1[] = {
		3, 1, 3,
		1, 5, 9, 
		2, 6, 5,
		1, 1, 1
	};
	
	double value2 = 0.5;
	
	CGNode* lhsNode = makeMatrixConstantNode(4, 3, value1);
	CGNode* rhsNode = makeDoubleConstantNode(value2);

	CGNode* node = makeBinaryOpNode(CGBOT_DIV, lhsNode, rhsNode);
	
	CGResultNode* result = computeCGNode(node);
	CGMatrix* Y = (CGMatrix*)result->value;

	printf("result: %s shape: %d.%d\n", getVariableTypeString(result->type), Y->rows, Y->cols);
	uint64_t i = 0;
	uint64_t j = 0;
	
	
	for(;i<Y->rows;i++){
		for(j = 0;j<Y->cols;j++){
			printf("\t%f", Y->data[i*Y->cols +j]);
		}
		printf("\n");
	}
}

void runAdd_dd(){
	/*
	 * TODO: check if d == 0 then throw error
	 */
	printf("Running d+d example\n");
	double value1 = 3;
	
	double value2 = 2;
	
	
	CGNode* lhsNode = makeDoubleConstantNode(value1);
	CGNode* rhsNode = makeDoubleConstantNode(value2);
	CGNode* node = makeBinaryOpNode(CGBOT_ADD, lhsNode, rhsNode);
	
	CGResultNode* result = computeCGNode(node);
	CGDouble* Y = (CGDouble*)result->value;

	printf("result: %s value: %f\n", getVariableTypeString(result->type), Y->value);
}

void runAdd_Vd(){
	printf("Running V+d example\n");
	double value1[] = {
		3,
		1, 
		5
	};
	
	double value2 = 2;
	
	
	CGNode* lhsNode = makeVectorConstantNode(3, value1);
	CGNode* rhsNode = makeDoubleConstantNode(value2);
	CGNode* node = makeBinaryOpNode(CGBOT_ADD, lhsNode, rhsNode);
	
	CGResultNode* result = computeCGNode(node);
	CGVector* Y = (CGVector*)result->value;

	printf("result: %s shape: %d\n", getVariableTypeString(result->type), Y->len);
	uint64_t i = 0;
	
	for(;i<Y->len;i++){
		printf("\t%f\n", Y->data[i]);
	}
}

void runAdd_Md(){
	/*
	 * TODO: check if d == 0 then throw error
	 */
	printf("Running M+d example\n");
	double value1[] = {
		3, 1, 3,
		1, 5, 9, 
		2, 6, 5,
		1, 1, 1
	};
	
	double value2 = 0.5;
	
	CGNode* lhsNode = makeMatrixConstantNode(4, 3, value1);
	CGNode* rhsNode = makeDoubleConstantNode(value2);

	CGNode* node = makeBinaryOpNode(CGBOT_ADD, lhsNode, rhsNode);
	
	CGResultNode* result = computeCGNode(node);
	CGMatrix* Y = (CGMatrix*)result->value;

	printf("result: %s shape: %d.%d\n", getVariableTypeString(result->type), Y->rows, Y->cols);
	uint64_t i = 0;
	uint64_t j = 0;
	
	
	for(;i<Y->rows;i++){
		for(j = 0;j<Y->cols;j++){
			printf("\t%f", Y->data[i*Y->cols +j]);
		}
		printf("\n");
	}
}


void runAdd_VV(){
	printf("Running V+V example\n");
	double value1[] = {
		3,
		1, 
		5
	};
	
	double value2[] = {
		-3,
		-1, 
		1
	};
	
	
	CGNode* lhsNode = makeVectorConstantNode(3, value1);
	CGNode* rhsNode = makeVectorConstantNode(3, value2);
	CGNode* node = makeBinaryOpNode(CGBOT_ADD, lhsNode, rhsNode);
	
	CGResultNode* result = computeCGNode(node);
	CGVector* Y = (CGVector*)result->value;

	printf("result: %s shape: %d\n", getVariableTypeString(result->type), Y->len);
	uint64_t i = 0;
	
	for(;i<Y->len;i++){
		printf("\t%f\n", Y->data[i]);
	}
}

void runAdd_MM(){
	/*
	 * TODO: check if d == 0 then throw error
	 */
	printf("Running M+M example\n");
	double value1[] = {
		3, 1, 3, 1,
		1, 5, 9, 1,
		2, 6, 5, 1,
		1, 1, 1, 0,
	};
	
	double value2[] = {
		-2, -1, -3, -1,
		-1, -4, -9, -1,
		-2, -6, -4, -1,
		-1, -1, -1, 1,
	};
	
	
	CGNode* lhsNode = makeMatrixConstantNode(4, 4, value1);
	CGNode* rhsNode = makeMatrixConstantNode(4, 4, value2);

	CGNode* node = makeBinaryOpNode(CGBOT_ADD, lhsNode, rhsNode);
	
	CGResultNode* result = computeCGNode(node);
	CGMatrix* Y = (CGMatrix*)result->value;

	printf("result: %s shape: %d.%d\n", getVariableTypeString(result->type), Y->rows, Y->cols);
	uint64_t i = 0;
	uint64_t j = 0;
	
	
	for(;i<Y->rows;i++){
		for(j = 0;j<Y->cols;j++){
			printf("\t%f", Y->data[i*Y->cols +j]);
		}
		printf("\n");
	}
}


int main(int argc, char *argv[]) {
	//profiler_initialize();
	
	printf("Initializing\n");
	
	runMult_dd();
	runMult_dV();
	runMult_MV();
	runMult_MM();
	runMult_MvM();
	runMult_Md();
	
	// This will fail on purpose.
	// runDiv_MM();
	
	runDiv_Md();
	runDiv_Vd();
	
	runAdd_dd();
	runAdd_Vd();
	runAdd_Md();
	runAdd_VV();
	runAdd_MM();
	
	//profiler_dump_file("stats.txt");
}
