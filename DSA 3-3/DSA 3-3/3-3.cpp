#define _CRT_SECURE_NO_WARNINGS
#include"temperature.h"
#include<stdlib.h>
#include<iostream>
#define min(x,y) (((x)<(y))?(x):(y))
#define max(x,y) (((x)>(y))?(x):(y))
typedef struct _node {
	int  sign;
	int L;
	int R;
	int  U;
	int  D;
	int  node;
	long long sum;
	struct _node* ch1;
	struct _node* ch2;
}node;
typedef int  (*_cmp)(const void *, const void *);
typedef int  point[3];
_cmp cmp[2];
inline int  cmp_x(const void *, const void *);
inline int  cmp_y(const void *, const void *);
void build(node*, point*,const int );
long long total(const node*, const int , const int , const int , const int ,int *);
int  main() {
	cmp[0] = cmp_x;
	cmp[1] = cmp_y;
	point* station;
	node* root = (node*)malloc(sizeof(node));
	root->sign = 0;
	int  x1, x2, y1, y2;
	int  n = GetNumOfStation();
	int  status;
	station = (point*)malloc(n*sizeof(point));
	const int  N = n;
	long long temp;
	for (;n--;)
		GetStationInfo(n, &station[n][0], &station[n][1], &station[n][2]);
	build(root, station, N);
	for (status = GetQuery(&x1, &y1, &x2, &y2);status;status = GetQuery(&x1, &y1, &x2, &y2)) {
		if (x1 < root->L) x1 = root->L;
		if (x2 > root->R)x2 = root->R;
		if (y1 < root->D) y1 = root->D;
		if (y2 > root->U) y2 = root->U;
		temp = total(root, x1, x2, y2, y1, &n);
		Response(n ? (int)temp / n : 0);
	}
	return 0;
}
inline int  cmp_x(const void* p, const void * q) {
	if (((point*)p)[0] - ((point*)q)[0])
		return ((point*)p)[0] - ((point*)q)[0];
	return ((point*)p)[1] - ((point*)q)[1];
}
inline int  cmp_y(const void* p, const void * q) {
	if (((point*)p)[1] - ((point*)q)[1])
		return ((point*)p)[1] - ((point*)q)[1];
	return ((point*)p)[0] - ((point*)q)[0];
}
void build(node* root, point*list,const int  n) {
	if (!n) return;
	if (n == 1) {
		root->sum = list[0][2];
		root->ch1 = root->ch2 = NULL;
		root->D = root->U = list[0][1];
		root->L = root->R = list[0][0];
		root->node = 1;
		root->sign = -1;
	}
	else {
		qsort(list, n, sizeof(point), cmp[root->sign]);
		root->ch1 = (node*)malloc(sizeof(node));
		root->ch2 = (node*)malloc(sizeof(node));
		if (root->sign&&list[0][1] == list[n - 1][1]) root->sign ^= 1;
		if (!root->sign&&list[0][0] == list[n - 1][0]) root->sign ^= 1;
		root->ch1->sign = root->ch2->sign = root->sign ^ 1;
		build(root->ch1, list, n >> 1);
		build(root->ch2, list + (n >> 1), n - (n >> 1));
		root->D = min(root->ch1->D, root->ch2->D);
		root->U = max(root->ch1->U, root->ch2->U);
		root->L = min(root->ch1->L, root->ch2->L);
		root->R = max(root->ch1->R, root->ch2->R);
		root->node = root->ch1->node + root->ch2->node;
		root->sum = root->ch1->sum + root->ch2->sum;
	}
}
long long total(const node* root, const int  l, const int  r, const int  u, const int  d,int * count) {
	if (root->L == l&&root->R == r&&root->U == u&&root->D == d) {
		*count = root->node;
		return root->sum;
	}
	else if (root->L<=l&&root->R>=r&&root->U >= u&&root->D <= d) {
		if (root->sign >= 0) {
			long long tmp;
			int count1 = 0, count2 = 0;
			if (root->sign) {
				//by y
				if (root->ch1->U >= u) {
					// in ch1(lower)
					tmp= total(root->ch1, l, r, u, d, &count1);
					if (root->ch1->U == root->ch2->D)
						tmp += total(root->ch2, l, r, root->ch1->U, root->ch1->U, &count2);
					*count = count1 + count2;
					return tmp;
				}
				else if (root->D <= d) {
					//in ch2(upper)
					tmp = total(root->ch2, l, r, u, d, &count1);
					if (root->ch1->U == root->ch2->D)
						tmp += total(root->ch1, l, r, root->ch1->U, root->ch1->U, &count2);
					*count = count1 + count2;
					return tmp;
				}
				else {
					long long ave1 = total(root->ch1, l, r, root->ch1->U, d, &count1);
					long long ave2 = total(root->ch2, l, r, u, root->ch2->D, &count2);
					*count = count1 + count2;
					return ave1 + ave2;
				}
			}
			else {
				//by x
				if (root->ch1->R >= r) {
					// in ch1(left)
					tmp = total(root->ch1, l, r, u, d, &count1);
					if (root->ch1->R == root->ch2->L)
						tmp += total(root->ch2, root->ch1->R, root->ch1->R, u, l, &count2);
					*count = count1 + count2;
					return tmp;
				}
				else if (root->ch2->L <= l) {
					//in ch2(right)
					tmp = total(root->ch2, l, r, u, d, &count1);
					if (root->ch1->R == root->ch2->L)
						tmp += total(root->ch1, root->ch1->R, root->ch1->R, u, l, &count2);
					*count = count1 + count2;
					return tmp;
				}
				else {
					int  count1, count2;
					long long ave1 = total(root->ch1, l,root->ch1->R,u,d, &count1);
					long long ave2 = total(root->ch2, root->ch2->L, r, u, d, &count2);
					*count = count1 + count2;
					return ave1 + ave2;
				}
			}
		}
		else {
			*count = 1;
			return root->sum;
		}
	}	
	else {
		*count = 0;
		return 0;
	}
}