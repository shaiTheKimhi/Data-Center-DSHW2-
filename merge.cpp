#include<iostream>
#include<cstdlib>
#include<ctime>
#include "RankTree.h"



using namespace std;
int main()
{
    srand(time(NULL));
    int arr[100] = { 0 };
    RankTree<int, int>* t = new RankTree<int,int>();
    RankTree<int, int>* t2 = new RankTree<int,int>();
    for (int i = 0; i < 4; i++) { t->insert(i, 0); t2->insert(4 + i,0); }
    t = t->merge(t2, t->getSize(), t2->getSize());
    for (int i = 0; i < 8; i++)cout << t->getRank(i) << endl;
    system("PAUSE");
    return 0;
}