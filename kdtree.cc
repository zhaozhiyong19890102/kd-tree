#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stack>
#include "kdtree.h"

using namespace std;

double distance(double *a, double *b, int dim){
	double d = 0.0;
	for (int i = 0; i < dim; i ++){
		d += (a[i] - b[i]) * (a[i] - b[i]);
	}
	return d;
}

double distance_except_dim(double *a, double *b, int except_dim){
	double d = (a[except_dim] - b[except_dim]) * (a[except_dim] - b[except_dim]);
	return d;
}

void copy2result(double *a, double *result, int dim){
	for (int i = 0; i < dim; i ++){
		result[i] = a[i];
	}
}

// 递归构建kd树，通过节点所在的层数控制选择的维度
int kdtree_insert(kdtree_node * &tree_node, double *data, int layer, int dim){
	// 空树
	if (NULL == tree_node){
		// 申请空间
		tree_node = (kdtree_node *)malloc(sizeof(kdtree_node));
		if (NULL == tree_node) return 1;

		//插入元素
		for (int i = 0; i < dim; i ++){
			(tree_node->data)[i] = data[i];
		}
		tree_node->dim = layer % (dim);
		tree_node->left = NULL;
		tree_node->right = NULL;

		return 0;
	}
	
	// 插入左子树
	if (data[tree_node->dim] <= (tree_node->data)[tree_node->dim]){
		return kdtree_insert(tree_node->left, data, ++layer, dim);
	}
	
	// 插入右子树
	return kdtree_insert(tree_node->right, data, ++layer, dim);
}


void search_nearest(kdtree_node *tree, double *data_search, int dim, double *result){
	// 一直找到叶子节点
	fprintf(stderr, "\nstart searching....\n");
	stack<kdtree_node *> st;
	
	kdtree_node *p = tree;
	
	while (p->left != NULL || p->right != NULL){
		st.push(p);// 将p压栈
		if (data_search[p->dim] <= (p->data)[p->dim]){// 选择左子树
			// 判断左子树是否为空
			if (p->left == NULL) break;
			p = p->left;
		}else{ // 选择右子树
			if (p->right == NULL) break;
			p = p->right;
		}
	}
	
	
	// 现在与栈中的数据进行对比
	double min_distance = distance(data_search, p->data, dim);// 与根结点之间的距离
	fprintf(stderr, "init: %lf\n", min_distance);
	copy2result(p->data, result, dim);
	// 打印最优值
	for (int i = 0; i < dim; i++){
                fprintf(stderr, "%lf\t", result[i]);
        }
        fprintf(stderr, "\n");

	double d = 0;
	while (st.size() > 0){
		kdtree_node *q = st.top();// 找到栈顶元素
		st.pop(); // 出栈
		
		// 判断与父节点之间的距离
		d = distance(data_search, q->data, dim);
		
		if (d <= min_distance){
			min_distance = d;
			copy2result(q->data, result, dim);
		}

		// 判断与分隔面是否相交
		double d_line = distance_except_dim(data_search, q->data, q->dim); // 到平面之间的距离
		if (d_line < min_distance){ // 相交
			// 如果本来在右子树，现在查找左子树
			// 如果本来在左子树，现在查找右子树
			if (data_search[q->dim] > (q->data)[q->dim]){
				// 选择左子树
				if (q->left != NULL) q = q->left;
				else q = NULL;
			}else{
				// 选择右子树
				if (q->right != NULL) q = q->right;
				else q = NULL;
			}
			if (q != NULL){
				while (q->left != NULL || q->right != NULL){
					st.push(q);
					if (data_search[q->dim] <= (q->data)[q->dim]){
						if (q->left == NULL) break;
						q = q->left;
					}else{
						if (q->right == NULL) break;
						q = q->right;
					}
				}
				if (q->left == NULL && q->right == NULL) st.push(q);
			}
		}

	}
}


void kdtree_print(kdtree_node *tree, int dim){
	if (tree != NULL){
		fprintf(stderr, "dim:%d\n", tree->dim);
		for (int i = 0; i < dim; i++){
			fprintf(stderr, "%lf\t", (tree->data)[i]);
		}
		fprintf(stderr, "\n");
		kdtree_print(tree->left, dim);
		kdtree_print(tree->right, dim);
	}
}

void kdtree_print_in(kdtree_node *tree, int dim){
	if (tree != NULL){
		kdtree_print_in(tree->left, dim);
		fprintf(stderr, "dim:%d\n", tree->dim);
		for (int i = 0; i < dim; i++){
			fprintf(stderr, "%lf\t", (tree->data)[i]);
		}
		fprintf(stderr, "\n");
		kdtree_print_in(tree->right, dim);
	}
}
