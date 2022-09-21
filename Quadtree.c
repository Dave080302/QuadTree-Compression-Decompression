#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Quadtree.h"
int m=0;
int main(int argc, char* argv[])
{
    QuadTreeNode** auxiliar;
    int i,j,parameter=-1;
    char parametermirror;
    unsigned char mode=0;
    char inputfile[100],outputfile[100];
    if(strcmp(argv[1],"-c")==0)
        {
            parameter=atoi(argv[2]);
            strcpy(inputfile,argv[3]);
            strcpy(outputfile,argv[4]);
            mode='c';
        }
    else if(strcmp(argv[1],"-m")==0)
    {
        mode='m';
        parametermirror=argv[2][0];
        parameter=atoi(argv[3]);
        strcpy(inputfile,argv[4]);
        strcpy(outputfile,argv[5]);
    }
    else if(strcmp(argv[1],"-d")==0)
    {
        mode='d';
        strcpy(inputfile,argv[2]);
        strcpy(outputfile,argv[3]);
    }
    if(mode=='m' || mode=='c'){
    FILE *f=fopen(inputfile, "rb");
    char buffer[4];
    int width=0,height=0;
    fgets(buffer,3,f);
    fscanf(f,"%d %d\n",&width,&height);
    unsigned char maximum;
    fscanf(f,"%hhu\n",&maximum);
    auxiliar=malloc(height*sizeof(QuadTreeNode*));
    for(i=0;i<height;i++)
        auxiliar[i]=malloc(width*sizeof(QuadTreeNode));
    for(i=0;i<height;i++)
        for(j=0;j<width;j++)
        {
            fread(&auxiliar[i][j].red,sizeof(unsigned char),1,f);
            fread(&auxiliar[i][j].green,sizeof(unsigned char),1,f);
            fread(&auxiliar[i][j].blue,sizeof(unsigned char),1,f);
        }
    fclose(f);
    QuadTree* root;
    root=buildquadtree(root,auxiliar,0,height,0,width);
    if(mode=='c')
        {
            QuadTreeNode* vector;
            compressTree(root,auxiliar,parameter);
            unsigned int n=getsizeoftree(root);
            vector=malloc(n*sizeof(QuadTreeNode));
            compress(root,vector);
            unsigned int colors=numberofleaves(root);
            FILE *w=fopen(outputfile,"wb");
            fwrite(&colors,sizeof(unsigned int),1,w);
            fwrite(&n,sizeof(unsigned int),1,w);
            fwrite(vector,sizeof(QuadTreeNode),n,w);
            fclose(w);
            free(vector);
        }
    else if(mode=='m')
    {
        compressTree(root,auxiliar,parameter);
        if(parametermirror=='v')
            verticalmirror(root);
        else 
            horizontalmirror(root);
        treetomatrix(root,auxiliar,0,height,0,width);
        FILE *w=fopen(outputfile,"wb");
        char type[4]="P6";
        unsigned char max=255;
        fprintf(w,"%s\n","P6");
        fprintf(w,"%d %d\n",height,width);
        fprintf(w,"%hhu\n",max);
        for(i=0;i<height;i++)
            for(j=0;j<width;j++)
                {
                    fwrite(&auxiliar[i][j].red,sizeof(unsigned char),1,w);
                    fwrite(&auxiliar[i][j].green,sizeof(unsigned char),1,w);
                    fwrite(&auxiliar[i][j].blue,sizeof(unsigned char),1,w);
                }
        fclose(w);

    }
    freeTree(root);
    for(i=0;i<height;i++)
        free(auxiliar[i]);
    free(auxiliar);
    }
    else 
    {
        QuadTreeNode *vector,**matrix;
        FILE *f=fopen(inputfile,"rb");
        int leaves,n;
        fread(&leaves,sizeof(int),1,f);
        fread(&n,sizeof(int),1,f);
        vector=malloc(n*sizeof(QuadTreeNode));
        fread(vector,sizeof(QuadTreeNode),n,f);
        QuadTree *tree;
        unsigned int size=vector[0].area;
        size=sqrt(size);
        matrix=malloc(size*sizeof(QuadTreeNode*));
        for(i=0;i<size;i++)
            matrix[i]=malloc(size*sizeof(QuadTree));
        tree=vectortotree(tree,vector,0);
        treetomatrix(tree,matrix,0,size,0,size);
        fclose(f);
        FILE *w=fopen(outputfile,"wb");
        char type[4]="P6";
        unsigned char max=255;
        fprintf(w,"%s\n","P6");
        fprintf(w,"%d %d\n",size,size);
        fprintf(w,"%hhu\n",max);
        for(i=0;i<size;i++)
            for(j=0;j<size;j++)
                {
                    fwrite(&matrix[i][j].red,sizeof(unsigned char),1,w);
                    fwrite(&matrix[i][j].green,sizeof(unsigned char),1,w);
                    fwrite(&matrix[i][j].blue,sizeof(unsigned char),1,w);
                }
        fclose(w);
        free(vector);
        freeTree(tree);
        for(i=0;i<size;i++)
            free(matrix[i]);
        free(matrix);
    }
    
}