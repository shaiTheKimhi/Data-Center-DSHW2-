#include<iostream>
#include "RankTree.h"



void pourToArray(AVLNode<int,int>* node, int** arr, int index = 0)
{
    arr[index] = new int[2];
    arr[index][0] = node->key;
    arr[index][1] = node->data;
    if(node->leftSon) pourToArray(node->leftSon, arr, index + 1);
    if(node->rightSon) pourToArray(node->rightSon, arr, index + 2);
}
int** toArray(RankTree<int,int> tree, int m)
{
    int** arr = new int*[m];
    pourToArray(tree->getRoot(), arr);
    return arr;
}
int** mergeArray(int** arr1, int** arr2, int m1, int m2)
{
    int** arr = new int*[m1+m2];
    int i1 = 0, i2 = 0, i = 0;
    while(i1 < m1 && i2 < m2)
    {
        if(arr1[i1][0] > arr2[i2][0]) arr[i++] = arr1[i1++];
        else arr[i++] = arr2[i2++];
    }
    for(i1;i1<m1;i1++) arr[i++] = arr1[i1];
    for(i2;i2<m2;i2++) arr[i++] = arr2[i2];
}
RankTree<int,int>* merge(RankTree<int,int> t1, RankTree<int,int> t2, int m1, int m2)
{
    int** arr = mergeArray(toArray(t1,m1),toArray(t2,m2),m1,m2);
}


int main()
{
    return 0;
}