//
// Created by Yaara Karmy on 29/12/2019.
//

#ifndef GENERICS_HASHMAP_H
#define GENERICS_HASHMAP_H

#include<cstdio>

template <class D>
class HashMap;

template <class D>
class HashNode {
    int key;
    D d;
    HashNode *next;

    friend class HashMap<D>;

public:
    HashNode(int key, D d) {
        this->key = key;
        this->d = d;
        this->next = NULL;
    }
};

template <class D>
class HashMap {
    HashNode<D> **map;
    int elementsNum;
    int mapSize;

public:

    HashMap() {
        mapSize = 16;
        elementsNum = 0;
        map = new HashNode<D> *[mapSize];
        for (int i = 0; i < mapSize; i++) {
            map[i] = NULL;
        }
    }
    ~HashMap() {
        for (int i = 0; i < mapSize; i++) {
            HashNode<D> *temp = map[i];
            while (temp != NULL) {
                HashNode<D> *prev = temp;
                temp = temp->next;
                delete prev;
            }
        }
        delete [] map;
    }

    int hashFunc(int key) {
        return (key % mapSize);
    }

    D* find (int key) {
        int val = hashFunc(key);
        HashNode<D> *temp = map[val];
        while(temp != NULL) {
            if (temp->key == key) {
                return (&temp->d);
            }
            temp = temp->next;
        }
        return NULL;
    }

    bool isExist (int key) {
        return (find(key) != NULL);
    }

    void insert(int key, D d) {
        if (mapSize == 2*elementsNum) {
            enlargeMap();
        }
        int val = hashFunc(key);
        HashNode<D> *entryNode = map[val];
        HashNode<D> *prev = NULL;
        (entryNode != NULL) ? addToCell(key,d,prev,entryNode,val): addToEmptyCell(key,d,entryNode,val);
        elementsNum++;
    }

    bool deleteNode(int key) {
        int val = hashFunc(key);
        HashNode<D> *valNode = map[val];
        HashNode<D> *prev = NULL;

        while (valNode != NULL) {
            if (valNode->key == key) {
                if (prev != NULL) {
                    prev->next = valNode->next;
                } else {
                    map[val] = valNode->next;
                }
                delete valNode;
                elementsNum--;
                if (mapSize == 4*elementsNum) {
                    reduceMap();
                }
                return true;
            } else {
                prev = valNode;
                valNode= valNode->next;
            }
        }
        return false;
    }

private:
    void enlargeMap() {
        int oldSize = mapSize;
        mapSize = mapSize*2 + 1;
        HashNode<D>** newMap = new HashNode<D>*[mapSize]();

        for (int i = 0; i < oldSize; ++i) {
            HashNode<D> *n = map[i];
            while(n != NULL) {
                HashNode<D> *temp = n;
                n = n->next;

                int val = hashFunc(temp->key);
                newMap[val] = temp;
            }
        }
        delete [] map;
        map = newMap;
    }

    void reduceMap() {
        int oldSize = mapSize;
        mapSize = oldSize/2 + 1;
        HashNode<D>** newMap = new HashNode<D>*[mapSize]();

        for (int i = 0; i <oldSize; i++) {
            HashNode<D> *n = map[i];
            while(n != NULL) {
                HashNode<D> *temp = n;
                n = n->next;

                int val = hashFunc(temp->key);
                newMap[val] = temp;
            }
        }
        delete [] map;
        map = newMap;
    }

    void addToCell(int key, D d, HashNode<D>* prev, HashNode<D>* entryNode, int val) {

        prev = new HashNode<D>(key,d);
        prev->next = entryNode;
        map[val] = prev;
    }

    void addToEmptyCell(int key, D d,HashNode<D>* entryNode, int val) {
        entryNode = new HashNode<D>(key,d);
        map[val] = entryNode;
    }
};

#endif //GENERICS_HASHMAP_H
