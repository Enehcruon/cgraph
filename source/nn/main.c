
#include <stdio.h>
#include <inttypes.h>
#include <string.h> 
#include <memory.h>
#include <stdlib.h>

#include "cg_api.h"
#include "cg_enums.h"
#include "cg_types.h"
#include "../lua_api/vendor/lua/lua.h"

struct CGNode* sigmoid_node(struct CGNode* x){
	return cg_newBinOp(CGBOT_DIV, cg_newDoubleNode(1.0), cg_newBinOp(CGBOT_ADD, cg_newDoubleNode(1.0), cg_newUnOp(CGUOT_EXP, cg_newUnOp(CGUOT_MINUS, x))));
}

struct CGNode* softmax_node(struct CGNode* x){
	return cg_newBinOp(CGBOT_DIV, cg_newUnOp(CGUOT_EXP, x), cg_newAxisBoundOp(CGABOT_SUM,cg_newUnOp(CGUOT_EXP, x), 1));
}

double* raw_copy(double* src, uint64_t len){
	double* dest = calloc(len, sizeof(double));
	memcpy(dest, src, len*sizeof(double));
	return dest;
}

int main(int argc, char* argv[]){
	
	double x_val[] = {0.2, 0.3, 0.5, 0.8, 0.12, 0.34, 0.75, 0.08};
	double x_val2[] ={50.1,0.5,111.4,0};
	double x_val3[] ={5.1,3.5,1.4,0.4};
	double T1_val[] = {.21, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1};
	double b1_val[] = {0.1, 0.1, 0.1, 0.1, 0.1};
	double T2_val[] =  {0.3, 0.4, 0.12, 0.14, 0.1,0.3, 0.4, 0.12, 0.14, 0.1,0.3, 0.4, 0.12, 0.14, 0.1};
	double b2_val[] = {0.0, 0.0, 0.0};
	//double T3_val[] =  {0.1,0.4,0.8,0.3,0.7,0.2,0.5,0.2,0.9 };
	//double b3_val[] = {1.0, 1.0, 1.0};
	double y_val[] = {1, 0};
	
	
	
	struct CGNode* x = cg_newVariable("x");
	struct CGNode* y = cg_newVariable("y");
	struct CGNode* T_1 = cg_newVariable("T_1");
	struct CGNode* b_1 = cg_newVariable("b_1");
	struct CGNode* T_2 = cg_newVariable("T_2");
	struct CGNode* b_2 = cg_newVariable("b_2");
	//struct CGNode* T_3 = cg_newVariable("T_3");
	//struct CGNode* b_3 = cg_newVariable("b_3");
	
	struct CGNode* L1 = sigmoid_node(cg_newBinOp(CGBOT_ADD, cg_newBinOp(CGBOT_DOT, x, T_1), b_1));
	struct CGNode* L2 = sigmoid_node(cg_newBinOp(CGBOT_ADD, cg_newBinOp(CGBOT_DOT, L1, T_2), b_2));
	struct CGNode* H  = cg_newCrossEntropyLoss(softmax_node(L2), y, 3);
	//struct CGNode* H  = cg_newCrossEntropyLoss(softmax_node(cg_newBinOp(CGBOT_ADD, cg_newBinOp(CGBOT_DOT, L2, T_3), b_3)), y, 3);
	
	//struct CGNode* H  = cg_newCrossEntropyLoss(x, y, 3);
	
	struct CGraph* graph = cg_newGraph("nn", H);
	
	cg_setVar(graph, "x", cg_newMatrixNode(2, 4, x_val));
	cg_setVar(graph, "y", cg_newVectorNode(2, y_val));
	cg_setVar(graph, "T_1", cg_newMatrixNode(4, 5, T1_val));
	cg_setVar(graph, "b_1", cg_newVectorNode(5, b1_val));
	cg_setVar(graph, "T_2", cg_newMatrixNode(5, 3, T2_val));
	cg_setVar(graph, "b_2", cg_newVectorNode(3, b2_val));
	//cg_setVar(graph, "T_3", cg_newMatrixNode(3, 3, T3_val));
	//cg_setVar(graph, "b_3", cg_newVectorNode(3, b3_val));
	
	struct CGResultNode* res = cg_evalGraph(graph);
	
	printf("Result type: %d\n", cg_getResultType(res));
	
	switch(cg_getResultType(res)){
		case CGVT_DOUBLE:
		{
			CGDouble* d = cg_getResultDoubleVal(res);
			printf("%f\n",  d->value);
			break;
		}
		
		case CGVT_VECTOR:
		{
			CGVector* vec = cg_getResultVectorVal(res);
			uint64_t i = 0;
			printf("(");
			for(; i < vec->len; i++){
				printf("%f, ", vec->data[i]);
			}
			printf(")\n");
			break;
		}
		
		case CGVT_MATRIX:
		{
			CGMatrix* m = cg_getResultMatrixVal(res);
			uint64_t i = 0;
			uint64_t j = 0;
			printf("(");
			for(; i < m->rows; i++){
				printf("\n\t");
				for(j = 0; j < m->cols; j++){
					printf("%f, ", m->data[i*m->cols+j]);
				}
			}
			printf(")\n");
			break;
		}
	}

	
	cg_autoDiffGraph(graph);
/*	
	struct CGNode* dx = cg_getVarDiff(graph, "x");
	
	cg_printNodeValue(dx);
	
	
	
	
	
	
	
	
	
	
	
	
	cg_setVar(graph, "x", cg_newMatrixNode(1, 4, x_val2));
	res = cg_evalGraph(graph);
	switch(cg_getResultType(res)){
		case CGVT_DOUBLE:
		{
			CGDouble* d = cg_getResultDoubleVal(res);
			printf("%f\n",  d->value);
			break;
		}
		
		case CGVT_VECTOR:
		{
			CGVector* vec = cg_getResultVectorVal(res);
			uint64_t i = 0;
			printf("(");
			for(; i < vec->len; i++){
				printf("%f, ", vec->data[i]);
			}
			printf(")\n");
			break;
		}
		
		case CGVT_MATRIX:
		{
			CGMatrix* m = cg_getResultMatrixVal(res);
			uint64_t i = 0;
			uint64_t j = 0;
			printf("(");
			for(; i < m->rows; i++){
				printf("\n\t");
				for(j = 0; j < m->cols; j++){
					printf("%f, ", m->data[i*m->cols+j]);
				}
			}
			printf(")\n");
			break;
		}
	}
	
	
	cg_autoDiffGraph(graph);
	
	dx = cg_getVarDiff(graph, "x");
	
	cg_printNodeValue(dx);
	
	cg_freeGraph(graph);
	free(graph);
	*/
	return 0;
}
