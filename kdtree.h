#define MAX_LEN 1024

typedef struct KDtree{
	double data[MAX_LEN]; // 数据
	int dim; // 选择的维度
	struct KDtree *left; // 左子树
	struct KDtree *right; // 右子树
}kdtree_node;

double distance(double *a, double *b, int dim);

int kdtree_insert(kdtree_node *&tree_node, double *data, int layer, int dim);

void search_nearest(kdtree_node *tree, double *data_search, int dim, double *result);

void kdtree_print(kdtree_node *tree, int dim);

void kdtree_print_in(kdtree_node *tree, int dim);

