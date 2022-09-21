#include <inttypes.h>
typedef struct QuadTreeNode{
    unsigned char blue,green,red;
    uint32_t area;
    int32_t top_left, top_right;
    int32_t bottom_left, bottom_right;
}__attribute__((packed))QuadTreeNode;
typedef struct QuadTree{
    unsigned char blue,green,red;
    int startwidth,startheight,endwidth,endheight;
    unsigned int size;
    struct QuadTree *top_left, *top_right;
    struct QuadTree *bottom_left, *bottom_right;
}QuadTree;
QuadTree* buildquadtree(QuadTree* root,QuadTreeNode** matrix,int startheight, int endheight,int startwidth, int endwidth);
void freeTree(QuadTree *root);
void printTree(QuadTree *root);
unsigned int getsizeoftree(QuadTree *root);
unsigned int numberofleaves(QuadTree *root);
unsigned char avgred(QuadTree *root,QuadTreeNode** matrix);
unsigned char avgblue(QuadTree *root,QuadTreeNode** matrix);
unsigned char avggreen(QuadTree *root,QuadTreeNode** matrix);
void compressTree(QuadTree *root, QuadTreeNode** matrix,int factor);
void compress(QuadTree *root, QuadTreeNode *vector);
QuadTree* vectortotree(QuadTree *tree, QuadTreeNode *vector,int n);
void treetomatrix(QuadTree *tree, QuadTreeNode **matrix,int startheight,int endheight, int startwidth, int endwidth);
void horizontalmirror(QuadTree *root);
void verticalmirror(QuadTree *root);
