#include<iostream>
#include "AVLTree.h"

int main(void)
{
    AVLTree<int,int> t;
    for(int i = 0; i < 5; i++)
        t.insert(i,0);
    //t.getRank(2);

    return 0;
}