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

struct Pair
{
	int i, j, sum;
};

NODE* createNode( );
void initLeaves(LEAF*, int, NODE**);
void Insert(NODE*, NODE*);
int getUniqueLeaves(LEAF*, char*, int);
void sort_lbuffer(LEAF*, int);
PAIR getMinPair(NODE**, int);
int pop_prebuffer(LEAF*, PAIR, NODE**, int);

int main( )
{
	LEAF num[8], buff[8];
	NODE *lv[8], *iv[8]; //lv = leaf vertex, iv = intermediate vertex
	char str[9];
	int i, n, size, iv_size, buff_size; //buff_size is for LEAF buff[]. Size is size of LEAF num[]
	root = 0;
	n = 8;
	printf("\n\tEnter String to compress: ");
	scanf("%s", str);
	size = getUniqueLeaves(num, str, n);
	sort_lbuffer(num, size); //lbuffer = leaf buffer (also called prebuffer)
	for(i=0;i<size;i++)
		printf("%c -- %d\t\t", num[i].byte, num[i].freq);
	iv_size = size;
	buff_size = size;
	return 0;
}

/*int Fact(int n)
{
	if(n==0 || n==1)
		return 1;
	else
		return n*Fact(n-1);
}*/

int pop_prebuffer(LEAF buff[], PAIR p, NODE *iv[], int size) //Here size is buff[] size.
{
	LEAF temp[size];
	int i, j;
	j=0;
	for(i=0;i<size;i++)
	{
		if(i != p.i && i != p.j)
		{
			temp[j].byte = buff[i].byte;
			temp[j].freq = buff[i].freq;
			j++;
		}
	}
	temp[j].byte = 0; /*delete if this causes bugs*/
	temp[j].freq = p.sum;
	j++;
	sort_lbuffer(temp, j);
	for(i=0;i<j;i++)
	{
		buff[i].byte = temp[i].byte;
		buff[i].freq = temp[i].freq;
	}
	return j;
}

int refresh_buffer(LEAF buff[], PAIR p, NODE *iv[], int size) //input size is iv_size 
{
	int new_size, i;
	new_size = pop_prebuffer(buff, p, iv, size);
	for(i=0;i<new_size;i++)
	{
		iv[i]->byte = buff[i].byte;
		iv[i]->freq = buff[i].freq;
	}
	return new_size;
}

void huffman_compress(NODE *iv[], LEAF buff[], int iv_size, int buff_size)
{
	PAIR p;
	NODE *n1, *n2;
	int m, n;
	m = iv_size;
	n = buff_size;
	while(m > 1)
	{
		p = getMinPair(iv, m);
		n1 = iv[p.i];
		n2 = iv[p.j];
		m = refresh_buffer(buff, p, iv, m);
		m = update_tree(iv, m, n1, n2, p); /*function to update tree*/
	}
	root = iv[0]; /*complete the binary tree*/
}

int update_tree(NODE *iv[], int iv_size, NODE *n1, NODE *n2, PAIR p)
{
	int new_size;
	new_size = insert_vertex(iv, iv_size, n1, n2, p.sum);
	sort_vbuffer(iv, new_size);
	return new_size;
}

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
	p.sum = min;
	return p;
}

void sort_lbuffer(LEAF l[], int size)
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

void sort_vbuffer(NODE *iv[], int iv_size)
{
        int i, j;
        NODE *temp;
        for(i=0;i<iv_size;i++)
        {
                for(j=0;j<(iv_size-i-1);j++)
                {
                        if(iv[j]->freq > iv[j+1]->freq)
                        {
                                temp = iv[j];
                                iv[j] = iv[j+1];
                                iv[j+1] = temp;
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

void initLeaves(LEAF l[], LEAF buff[], int size, NODE *v[])
{
	int i;
	for(i=0;i<size;i++)
	{
		v[i] = createNode();
		v[i]->byte = l[i].byte;
		v[i]->freq = l[i].freq;
		buff[i].byte = l[i].byte;
		buff[i].freq = l[i].freq;
	}
}

int insert_vertex(NODE *v[], int iv_size, NODE *n1, NODE *n2, int f)
{
	NODE *x;
	x = GetNode();
	x->left = n1;
	x->right = n2;
	x->freq = f;
	n1->parent = x;
	n2->parent = x;
	v[iv_size] = x;
	iv_size++;
	return iv_size;
}
