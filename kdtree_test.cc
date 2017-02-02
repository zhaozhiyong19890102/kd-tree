#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdtree.h"

// 解析特征
int parse_feature(char *p, double *fea, int *dim){
	// 解析特征
	char *q = p;
	int i = 0;
	while ((q = strchr(p, '\t')) != NULL){
		*q = 0;
		fea[i] = atof(p);
		//fprintf(stderr, "atof(p):%lf\n", atof(p));
		p = q + 1;
		//r = r + 1;
		i += 1;
	}

	// 解析最后一个
	fea[i] = atof(p);

	*dim = i + 1;
	//fprintf(stderr, "atof(p):%lf\n", atof(p));

	//fprintf(stderr, "fea:%lf\t%lf\n", fea[0], fea[1]);
}

int main(){
	kdtree_node *tree_node = NULL;
	// 从文件中读入数据
	FILE *fp = fopen("data.txt", "r");
	char feature[MAX_LEN];
	double data[MAX_LEN];
	int data_dim = 0; // 数据的维数

	double data_search[2] = {6.0, 3.0};

	while (fgets(feature, MAX_LEN, fp)){
		fprintf(stderr, "%s", feature);
		parse_feature(feature, data, &data_dim);
		fprintf(stderr, "distance: %lf\n", distance(data, data_search, data_dim));
		// 插入到kd树中
		kdtree_insert(tree_node, data, 0, data_dim);		
	}
	fclose(fp);
	fprintf(stderr, "dim:%d\n", data_dim);
	fprintf(stderr, "insert_ok\n");
	// test
	
	kdtree_print(tree_node, data_dim);
	printf("\n");
	kdtree_print_in(tree_node, data_dim);
	
	
	double result[2];

	search_nearest(tree_node, data_search, data_dim, result);
	
	fprintf(stderr, "\n the final result: ");	
	for (int i = 0; i < data_dim; i++){
		fprintf(stderr, "%lf\t", result[i]);
	}
	fprintf(stderr, "\n");
	return 0;	
}
