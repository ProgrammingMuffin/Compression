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

struct Leaf
{
	char byte;
	int freq; //frequency of this halfbyte
};

struct Pair
{
	int i, j, sum;
};

NODE *root;

NODE* createNode( );
void initLeaves(LEAF*, LEAF*, int, NODE**);
int insert_vertex(NODE**, int, NODE*, NODE*, int);
int getUniqueLeaves(LEAF*, char*, int);
void sort_lbuffer(LEAF*, int);
void sort_vbuffer(NODE**, int);
void getMinPair(PAIR*, NODE**, int);
int pop_prebuffer(LEAF*, PAIR, NODE**, int);
int refresh_buffer(LEAF*, PAIR, NODE**, int);
void huffman_compress(NODE**, LEAF*, int, int);
int update_tree(NODE**, int, NODE*, NODE*, PAIR);


int main( )
{
	LEAF num[32], buff[32];
	NODE *lv[32], *iv[32]; //lv = leaf vertex, iv = intermediate vertex
	char str[9];
	int i, n, size, iv_size, buff_size; //buff_size is for LEAF buff[]. Size is size of LEAF num[]
	root = 0;
	n = 8; // just check this part once.
	printf("\n\tEnter String to compress: ");
	scanf("%s", str);
	size = getUniqueLeaves(num, str, n);
	sort_lbuffer(num, size); //lbuffer = leaf buffer (also called prebuffer)
	for(i=0;i<size;i++)
		printf("%c -- %d\t\t", num[i].byte, num[i].freq);
	iv_size = size;
	buff_size = size;
	initLeaves(num, buff, buff_size, iv);
	huffman_compress(iv, buff, iv_size, buff_size);
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
			printf("\n\tJ loop is: %d\n", j);
			temp[j].byte = buff[i].byte;
			temp[j].freq = buff[i].freq;
			j++;
		}
	}
	//temp[j].byte = 0; /*delete if this causes bugs*/
	temp[j].freq = p.sum;
	j++;
	sort_lbuffer(temp, j);
	//printf("\n\tPop Prebuffer J value is: %d\n", j);
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
	p.i=0;
	p.j=1;
	NODE *n1, *n2;
	int m, n;
	m = iv_size;
	n = buff_size;
	printf("\n\tm value is: %d\n", m);
	while(m > 1)
	{
		//printf("\n\tInside Huffman Compress Loop\n");
		getMinPair(&p, iv, m);
		printf("\n\tPair in huffman function is: %d, %d = %d\n", p.i, p.j, p.sum);
		n1 = iv[p.i];
		n2 = iv[p.j];
		//printf("\n\tn1 = %d\t\tn2 = %d\n", n1->freq, n2->freq);
		m = refresh_buffer(buff, p, iv, m);
		m = update_tree(iv, m, n1, n2, p); /*function to update tree*/
	}
	root = iv[0]; /*complete the binary tree*/
	printf("\n\tTree properly constructed\n");
}

int update_tree(NODE *iv[], int iv_size, NODE *n1, NODE *n2, PAIR p)
{
	int new_size;
	new_size = insert_vertex(iv, iv_size, n1, n2, p.sum);
	sort_vbuffer(iv, new_size);
	return new_size;
}

void getMinPair(PAIR *p, NODE *v[], int n)
{
	int min;
	min = (v[0]->freq + v[1]->freq);
	p->sum = min;
	p->i = 0;
	p->j = 1;
	//printf("\n\tv[0]->freq + v[1]->freq = %d + %d = %d\n", v[0]->freq, v[1]->freq, lol);
	/*for(i=0;i<n;i++)
	{
		for(j=(i+1);j<n;j++)
		{
			if(lol > (v[i]->freq + v[j]->freq))
			{
				lol = (v[i]->freq + v[j]->freq);
				p->i = i;
				p->j = j;
				p->sum = lol;
				printf("\n\tMin value is: %d, %d = %d\n", i, j, lol);
			}
			printf("\n\tOutside min pair for loop, min value is: %d\n", p->sum);
		}
	}*/
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
	int i, j, k, uni_size;
	char unique[9]; // unique[9] changed to unique[n+1], just in case.
	unique[0] = str[0];
	uni_size = 1;
	printf("\n\tValue of n is: %d\n", n);
	for(k=0;k<n && str[k]!=0;k++)
	{
		for(j=0;j<uni_size;j++)
		{
			if(str[k] == unique[j])
				break;
		}
		if(j==uni_size)
		{
			unique[j] = str[k];
			uni_size++;
		}
	}
	unique[uni_size] = 0;
	//uni_size = strlen(unique);
	for(k=0;k<uni_size;k++)
	{
		l[k].byte = unique[k];
		l[k].freq = 0;
		for(i=0;i<n;i++)
		{
			if(l[k].byte == str[i])
				l[k].freq++;
		}
	}
	printf("\n\t J value is %d @ getUniqueLeaves( );\n", uni_size);
	return uni_size;
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
	x = createNode();
	x->left = n1;
	x->right = n2;
	x->freq = f;
	n1->parent = x;
	n2->parent = x;
	v[iv_size] = x;
	iv_size--;
	return iv_size;
}
