#include "Quadtree.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
QuadTree* buildquadtree(QuadTree* root,QuadTreeNode** matrix,int startheight, int endheight,int startwidth, int endwidth)
{   
    root=malloc(sizeof(QuadTree));
    root->top_left=NULL;
    root->top_right=NULL;
    root->bottom_left=NULL;
    root->bottom_right=NULL;
    root->blue=0;
    root->red=0;
    root->green=0;
    root->startheight=startheight;
    root->startwidth=startwidth;
    root->endwidth=endwidth;
    root->endheight=endheight;
    root->size=(endheight-startheight)*(endwidth-startwidth);
    root->size=sqrt(root->size);
    int i,j,check=1;   
    unsigned char blue,red,green;
    blue=matrix[startheight][startwidth].blue;
    red=matrix[startheight][startwidth].red;
    green=matrix[startheight][startwidth].green;
    for(i=startheight;i<endheight;i++)
        for(j=startwidth;j<endwidth;j++)
            if(matrix[i][j].red!=red || matrix[i][j].blue!=blue || matrix[i][j].green!=green)
                {
                    check=0;
                    break;
                }
    if(check==1)
       {
           root->red=red;
           root->blue=blue;
           root->green=green;
            return root;
       }
    else
    {    
        int mheight=(startheight+endheight)/2;
        int mwidth=(startwidth+endwidth)/2;
        root->top_left=buildquadtree(root->top_left,matrix,startheight,mheight,startwidth,mwidth);
        root->top_right=buildquadtree(root->top_right,matrix,startheight,mheight,mwidth,endwidth);
        root->bottom_right=buildquadtree(root->bottom_right,matrix,mheight,endheight,mwidth,endwidth);
        root->bottom_left=buildquadtree(root->bottom_left,matrix,mheight,endheight,startwidth,mwidth);
    }

}
void freeTree(QuadTree *root)
{
    if(root==NULL)
    return;
    QuadTree *aux=root;
    if(root->top_left!=NULL)
        freeTree(root->top_left);
    if(root->top_right!=NULL)
        freeTree(root->top_right);
    if(root->bottom_left!=NULL)
        freeTree(root->bottom_left);
    if(root->bottom_right!=NULL)
        freeTree(root->bottom_right);
    free(aux);
}
void printTree(QuadTree *root)
{
    if(root==NULL)
    return;
    printf("%d %d %d",root->red,root->green,root->blue);
    if(root->top_left!=NULL);
    {
        printf("\n");
        printTree(root->top_left);
        printTree(root->top_right);
        printTree(root->bottom_right);
        printTree(root->bottom_left);
    }
}
unsigned int getsizeoftree(QuadTree *root)
{
    if(root==NULL)
    return 0;
    else return 1+getsizeoftree(root->top_left)+getsizeoftree(root->top_right)+getsizeoftree(root->bottom_right)+getsizeoftree(root->bottom_left);
}
unsigned int numberofleaves(QuadTree *root)
{
    if(root==NULL)
        return 0;
    else if(root->top_left==NULL)
        return 1;
    else
        return numberofleaves(root->top_left)+numberofleaves(root->top_right)+numberofleaves(root->bottom_left)+numberofleaves(root->bottom_right);
    
}
unsigned char avgred(QuadTree *root,QuadTreeNode** matrix)
{
    int i,j;
    unsigned long long size=(root->endheight-root->startheight)*(root->endwidth-root->startwidth);
    unsigned long long sum=0;
    for(i=root->startheight;i<root->endheight;i++)
        for(j=root->startwidth;j<root->endwidth;j++)
            sum+=matrix[i][j].red;
    return sum/size;
}
unsigned char avgblue(QuadTree *root,QuadTreeNode** matrix)
{
    int i,j;
    unsigned long long size=(root->endheight-root->startheight)*(root->endwidth-root->startwidth);
    unsigned long long sum=0;
    for(i=root->startheight;i<root->endheight;i++)
        for(j=root->startwidth;j<root->endwidth;j++)
            sum+=matrix[i][j].blue;
    return sum/size;
}
unsigned char avggreen(QuadTree *root,QuadTreeNode** matrix)
{
    int i,j;
    unsigned long long size=(root->endheight-root->startheight)*(root->endwidth-root->startwidth);
    unsigned long long sum=0;
    for(i=root->startheight;i<root->endheight;i++)
        for(j=root->startwidth;j<root->endwidth;j++)
            sum+=matrix[i][j].green;
    return sum/size;
}
void compressTree(QuadTree *root, QuadTreeNode** matrix,int factor)
{
    if(root->top_left == NULL)
        return;
    int i,j;
    unsigned long long mean=0,size;
    size=(root->endwidth-root->startwidth);
    unsigned char red=avgred(root,matrix);
    unsigned char blue=avgblue(root,matrix);
    unsigned char green=avggreen(root,matrix);
    for(i=root->startheight;i<root->endheight;i++)
        for(j=root->startwidth;j<root->endwidth;j++)    
            {
                mean+=(red-matrix[i][j].red)*(red-matrix[i][j].red);
                mean+=(green-matrix[i][j].green)*(green-matrix[i][j].green);
                mean+=(blue-matrix[i][j].blue)*(blue-matrix[i][j].blue);
            }
    mean/=3*size*size;
    if(mean<=factor)
        {
            root->blue=avgblue(root,matrix);
            root->red=avgred(root,matrix);
            root->green=avggreen(root,matrix);
            freeTree(root->top_left);
            freeTree(root->top_right);
            freeTree(root->bottom_left);
            freeTree(root->bottom_right);
            root->top_left=NULL;
            root->top_right=NULL;
            root->bottom_right=NULL;
            root->bottom_left=NULL;
        }
    if(root->top_left!=NULL)
    {
        
        compressTree(root->top_left,matrix,factor);
        compressTree(root->top_right,matrix,factor);
        compressTree(root->bottom_left,matrix,factor);
        compressTree(root->bottom_right,matrix,factor);
        root->blue=avgblue(root,matrix);
        root->red=avgred(root,matrix);
        root->green=avggreen(root,matrix);
    }
    
}
static int m=0;
void compress(QuadTree *root, QuadTreeNode *vector)
{
    vector[m].area=(root->endheight-root->startheight)*(root->endwidth-root->startwidth);
    vector[m].red=root->red;
    vector[m].blue=root->blue;
    vector[m].green=root->green;
    if(root->top_left!=NULL)
    {
        int cm=m;
        m++;
        vector[cm].top_left=m;
        compress(root->top_left,vector);
        m++;
        vector[cm].top_right=m;
        compress(root->top_right,vector);
        m++;
        vector[cm].bottom_right=m;
        compress(root->bottom_right,vector);
        m++;
        vector[cm].bottom_left=m;
        compress(root->bottom_left,vector);
    }
    else
    {
        vector[m].top_left=-1;
        vector[m].top_right=-1;
        vector[m].bottom_left=-1;
        vector[m].bottom_right=-1;
    }
    
}
QuadTree* vectortotree(QuadTree *tree, QuadTreeNode *vector,int n)
{
    tree=malloc(sizeof(QuadTree));
    tree->size=sqrt(vector[n].area);
    tree->red=vector[n].red;
    tree->blue=vector[n].blue;
    tree->green=vector[n].green;
    tree->top_left=NULL;
    tree->top_right=NULL;
    tree->bottom_left=NULL;
    tree->bottom_right=NULL;
    if(vector[n].top_left!=-1)
    {
        tree->top_left=vectortotree(tree->top_left,vector,vector[n].top_left);
        tree->top_right=vectortotree(tree->top_right,vector,vector[n].top_right);
        tree->bottom_right=vectortotree(tree->bottom_right,vector,vector[n].bottom_right);
        tree->bottom_left=vectortotree(tree->bottom_left,vector,vector[n].bottom_left);
    }
    
    return tree;
}
void treetomatrix(QuadTree *tree, QuadTreeNode **matrix,int startheight,int endheight, int startwidth, int endwidth)
{
    int i,j;
    for(i=startheight;i<endheight;i++)
        for(j=startwidth;j<endwidth;j++)
            {
                matrix[i][j].red=tree->red;
                matrix[i][j].blue=tree->blue;
                matrix[i][j].green=tree->green;
            }
    if(tree->top_left!=NULL)
    {
        treetomatrix(tree->top_left,matrix,startheight,endheight-tree->size/2,startwidth,endwidth-tree->size/2);
        treetomatrix(tree->top_right,matrix,startheight,endheight-tree->size/2,startwidth+tree->size/2,endwidth);
        treetomatrix(tree->bottom_right,matrix,startheight+tree->size/2,endheight,startwidth+tree->size/2,endwidth);
        treetomatrix(tree->bottom_left,matrix,startheight+tree->size/2,endheight,startwidth,endwidth-tree->size/2);
    }
}
void horizontalmirror(QuadTree *root)
{
    QuadTree *aux;
    aux=root->top_left;
    root->top_left=root->top_right;
    root->top_right=aux;
    aux=root->bottom_left;
    root->bottom_left=root->bottom_right;
    root->bottom_right=aux;
    if(root->top_left!=NULL)
    {
        horizontalmirror(root->top_left);
        horizontalmirror(root->top_right);
        horizontalmirror(root->bottom_left);
        horizontalmirror(root->bottom_right);
    }
}
void verticalmirror(QuadTree *root)
{
    QuadTree *aux;
    aux=root->top_left;
    root->top_left=root->bottom_left;
    root->bottom_left=aux;
    aux=root->top_right;
    root->top_right=root->bottom_right;
    root->bottom_right=aux;
    if(root->top_left!=NULL)
    {
        verticalmirror(root->top_left);
        verticalmirror(root->top_right);
        verticalmirror(root->bottom_left);
        verticalmirror(root->bottom_right);
    }
}