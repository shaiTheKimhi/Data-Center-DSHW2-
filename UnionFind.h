//
// Created by Yaara Karmy on 28/12/2019.
//

#ifndef GENERICS_UNIONFIND_H
#define GENERICS_UNIONFIND_H

class UnionFind {

    int setCounter;
    int *setSize;
    int *parents;
    int *sets;

public:
    //creates empty unionFind with n singletons
    UnionFind(int elementsNum) {
        setSize = new int[elementsNum];
        parents = new int[elementsNum];
        sets = new int[elementsNum];
        setCounter = elementsNum;

        for (int i = 0; i < elementsNum; i++) {
            makeSet(i);
        }
    }
    ~UnionFind() {
        delete(this->setSize);
        this->setSize = nullptr;
        delete(this->parents);
        this->parents = nullptr;
        delete(this->sets);
        this->sets = nullptr;
    }

    int find(int i) {
        if (i < 0) {
            return -1;
        }

        int root = i;
        while (root != parents[root]) {
            root = parents[root];
        }
        while(i != root) {
            int newParent = parents[i];
            parents[i] = root;
            i = newParent;
            sets[i] = sets[root];
        }
        return root;
    }

    int unionFunc(int p,int q) {
        if (isSameSet(p,q)) {
            return -1;
        }
        int i = find(p);
        int j = find(q);
        setSize[i] < setSize[j] ? mergeSize(i,j) :  mergeSize(j,i);
        parents[j] = i;
        setCounter--;
        sets[j] = p;
        return find(j);
    }
private:
    //creates singleTon with element i
    int makeSet(int i) {
        sets[i] = i;
        parents[i] = i;
        setSize[i] = 1;
        return sets[i];
    }
    //checks if two elements belong to the same set
    bool isSameSet(int p, int q) {
        return find(p) == find(q);
    }

    //merge set i to larger set j
    void mergeSize(int i, int j) {
        setSize[j] += setSize[i];
        setSize[i] = 0;
    }
};
#endif //GENERICS_UNIONFIND_H
