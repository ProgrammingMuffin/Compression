#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Pair PAIR;
typedef struct Node NODE;
typedef struct Leaf LEAF;

struct Node
{
	char byte;
	int freq;
	NODE *left;
	NODE *right;
	NODE *parent;
};

NODE *root;

struct Leaf
{
	char byte;
	int freq; //frequency of this halfbyte
};
s
struct Pair
{
	int i, j;
};

NODE* createNode( );
void initLeaves(LEAF*, int, NODE**);
void Insert(NODE*, NODE*);
int getUniqueLeaves(LEAF*, char*, int);
void sort_buffer(LEAF*, int);
PAIR getMinPair(NODE**, int);

int main( )
{
	LEAF num[8], buff[8];
	NODE *lv[8], *iv[8]; //lv = leaf vertex, iv = intermediate vertex
	char str[9];
	int i, n, size, iv_size;
	root = 0;
	n = 8;
	printf("\n\tEnter String to compress: ");
	scanf("%s", str);
	size = getUniqueLeaves(num, str, n);
	sort_buffer(num, size);
	for(i=0;i<size;i++)
		printf("%c -- %d\t\t", num[i].byte, num[i].freq);
	iv_size = size;
	return 0;
}

/*int Fact(int n)
{
	if(n==0 || n==1)
		return 1;
	else
		return n*Fact(n-1);
}*/

PAIR getMinPair(NODE *v[], int n)
{
	int i, j, min;
	PAIR p;
	i=0;j=0;
	min = v[i]->freq + v[j]->freq;
	for(i=0;i<n;i++)
	{
		for(j=i;j<n;j++)
		{
			if((v[i]->freq + v[j]->freq) < min)
			{
				min = v[i]->freq + v[j]->freq;
			}
		}
	}
	p.i = i;
	p.j = j;
	return p;
}

void sort_buffer(LEAF l[], int size)
{
	int i, j;
	LEAF temp;
	for(i=0;i<size;i++)
	{
		for(j=0;j<(size-i-1);j++)
		{
			if(l[j].freq > l[j+1].freq)
			{
				temp = l[j];
				l[j] = l[j+1];
				l[j+1] = temp;
			}
		}
	}
}

int getUniqueLeaves(LEAF l[], char *str, int n)
{
	int i, j, k;
	char unique[9];
	unique[0] = str[0];
	for(k=0;k<n;k++)
	{
		for(j=0;j<strlen(unique);j++)
		{
			if(str[k] == unique[j])
				break;
		}
		if(j==(strlen(unique)))
		{
			unique[j] = str[k];
		}
	}
	j = strlen(unique);
	for(k=0;k<j;k++)
	{
		l[k].byte = unique[k];
		l[k].freq = 0;
		for(i=0;i<n;i++)
		{
			if(l[k].byte == str[i])
				l[k].freq++;
		}
	}
	return j;
}

NODE* createNode( )
{
	NODE *x;
	x = (NODE *)malloc(sizeof(NODE));
	x->left = 0;
	x->right = 0;
	x->parent = 0;
	return x;
}

void initLeaves(LEAF l[], int size, NODE *v[])
{
	int i;
	for(i=0;i<size;i++)
	{
		v[i] = createNode();
		v[i]->byte = l[i].byte;
		v[i]->freq = l[i].freq;
	}
}

void Insert(NODE *n1, NODE *n2)
{
	NODE *p;
	p = GetNode();
	p->left = 
}
