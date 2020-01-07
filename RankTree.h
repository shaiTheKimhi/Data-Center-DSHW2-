//
// Created by Yaara Karmy on 01/12/2019.
//

#ifndef RankTree_RankTree_H
#define RankTree_RankTree_H

#include <cmath>

template <class K, class D>
class RankTree {
    class AVLNode {
        int rank;
        K key;
        D data;
        int height;
        AVLNode* parent;
        AVLNode* leftSon;
        AVLNode* rightSon;

        AVLNode(const K& key, const D& data, AVLNode* p = NULL) : key(key), data(data),
                                                                  height(0),parent(p), leftSon(NULL), rightSon(NULL) {rank = 1;}

        ~AVLNode() {
            if (leftSon != NULL) {
                delete leftSon;
            }
            if (rightSon != NULL) {
                delete rightSon;
            }
        }

        void updateHeight() {
            int left = leftSon ? leftSon->height : -1;
            int right = rightSon ? rightSon->height : -1;
            height = (left > right ? left : right) + 1;
        }

        bool isLeaf() {
            return (!leftSon && !rightSon);
        }

        bool haveTwoSons() {
            return (leftSon && rightSon);
        }

        int getBalanceFactor() {
            return (leftSon ? leftSon->height : -1) - (rightSon ? rightSon->height : -1);
        }

        friend class RankTree;
        class Container
        {
            K first;
            D second;
            Container(K key, D data) { first = key; second = data; }
        };
        void pourToArray(AVLNode** arr, int m, int* index)
        {
            if ((*index) >= m) return;
            if(this->leftSon)
                this->leftSon->pourToArray(arr, m, index);
            arr[(*index)] = this;
            (*index) += 1;
            if(this->rightSon)
                this->rightSon->pourToArray(arr, m, index);
        }
        AVLNode** nodeToArray(int m)
        {
            int i = 0;
            AVLNode** arr = new AVLNode*[m];
            this->pourToArray(arr, m, &i);
            return arr;
        }
        static AVLNode** mergeArray(AVLNode** arr1, AVLNode** arr2, int m1, int m2)
        {
            AVLNode** arr = new AVLNode*[m1 + m2];
            int i1 = 0, i2 = 0, i = 0;
            while (i1 < m1 && i2 < m2)
            {
                if (arr1[i1]->key < arr2[i2]->key) arr[i++] = arr1[i1++];
                else arr[i++] = arr2[i2++];
            }
            for (i1; i1 < m1; i1++)
            {
                arr[i++] = arr1[i1];
            }
            for (i2; i2 < m2; i2++) {
                arr[i++] = arr2[i2];
            }
            return arr;
        }
        static AVLNode* createShell(int m, K key, D data)
        {
            if (m <= 0) return NULL;
            AVLNode* node = new AVLNode(key, data);
            node->rank = m;
            m -= 1;
            if (m % 2 != 0) {
                node->leftSon = createShell(m / 2 + 1, key, data);
            }
            else {
                node->leftSon = createShell(m / 2, key, data);
            }
            node->rightSon = createShell(m / 2, key, data);
            return node;
        }
        void pourNode(AVLNode** arr, int m, int* i)
        {
            if ((*i) >= m) return;
            if(this->leftSon) {
                this->leftSon->pourNode(arr, m, i);
            }
            this->key = arr[(*i)]->key;
            this->data = (arr[(*i)]->key)();
            //this->data = arr[(*i)]->data;

            //this->rank = m;
            (*i) += 1;
            if(this->rightSon) {
                this->rightSon->pourNode(arr, m, i);
            }
        }
        AVLNode* mergeNodes(AVLNode* other, int m1, int m2)
        {
            int i = 0;
            if (!m1 && !m2) return NULL;
            else if (!m2) return this;
            else if (!m1) return other;
            AVLNode** arr1 = this->nodeToArray(m1);
            AVLNode** arr2 = other->nodeToArray(m2);
            AVLNode** arr = mergeArray(arr1, arr2, m1, m2);
            AVLNode* node = createShell(m1+m2, this->key, other->data);
            node->pourNode(arr, m1 + m2, &i);
            delete arr1;
            delete arr2;
            delete arr;
            return node;
        }
    public:
        D getData(){
            return data;
        }

        D getLeftSonData() {
            return leftSon->data;
        }

        bool isLeftSonExist() {
            return (this->leftSon != NULL);
        }

        bool isRightSonExist() {
            return (this->rightSon != NULL);
        }

        D getRightSonData() {
            return rightSon->data;
        }
    };

    AVLNode* root; //Any tree is a Node as a root
    int size; //how many nodes

    void insert(K key, D data, AVLNode* parent);
    void deleteNode(AVLNode* toDelete);
    void reduceRankPath(K key);
    void reduceDataPath(K key);
    void changeNode(AVLNode* parent, AVLNode* oldNode, AVLNode* newNode);
    void reBalance(AVLNode* node, bool isDelete = false);
    //void pourToArray(AVLNode* node, int** arr, int m);

    //Container** treeToArray(int m);





public:
    RankTree() : root(NULL), size(0) {}
    RankTree(AVLNode* node, int m) :root(node), size(m) {}
    ~RankTree() {
        if (root != NULL) {
            delete root;
        }
    }
    bool isEmpty();

    void insert(K key, D data);
    bool deleteKey(K key);
    bool isExist(K key) const;
    bool isExist(K key);

    void changeData(K key, D new_data);
    void destroyData();
    void destroyData(AVLNode* node);

    int getRank(AVLNode*);
    int getRank(K);
    RankTree* merge(RankTree* other, int m1, int m2);

    int getDataByMinIndex(int minIndex) {
        int  firstInRange = false;
        int dataCounter = 0;
        AVLNode* current = root;
        while (current) {
            if ((findNodeIndex(current->key) - 1) == minIndex) {
                if (!firstInRange) {
                    dataCounter += current->getData();
                }
                if (current -> leftSon) {
                    dataCounter -= current->leftSon->getData();
                }
                return dataCounter;
            }
            if (findNodeIndex(current->key) > minIndex) {
                if (firstInRange == false) {
                    firstInRange = true;
                    dataCounter += current->getData();
                }
                current = current->leftSon;
            } else {
                if (firstInRange == true) {
                    if (current->rightSon) {
                        dataCounter -= current->getData() - current->rightSon->key();
                    }
                }
                current = current->rightSon;
            }
        }
        return dataCounter;

    }

    AVLNode* getRoot()
    {
        return this->root;
    }
//    static RankTree<K, D>* merge(RankTree<K,D>, RankTree<K,D>, int, int);


    bool isBalanced() const {
        if (!root) return true;
        int leftSonHeight = getHeight(root->leftSon);
        int rightSonHeight = getHeight(root->rightSon);
        return (abs(leftSonHeight-rightSonHeight) <= 1 && isBalanced(root->rightSon) && isBalanced(root->rightSon));
    }

    //rotation
    void updateParentsAfterRotation(AVLNode* node, AVLNode* sonNode, AVLNode* directedSon) {
        if (directedSon) {
            directedSon->parent = node;
        }
        sonNode->parent = node->parent;
        node->parent = sonNode;
    }
    //rotating left
    void rotateRR(AVLNode* node) {
        //update ranks of temp(parent) and node (right son):
        int n, nl, nrl, nr, nrr,data_l,data_rl,data_r,data_rr,newData;
        if(!node->rightSon->leftSon) {
            nrl = 0;
            data_rl = 0;
        }
        else {
            nrl = node->rightSon->leftSon->rank;
            data_rl = node->rightSon->leftSon->getData();
        }
        if(!node->rightSon->rightSon) {
            nrr = 0;
            data_rr = 0;
        }
        else {
            nrr = node->rightSon->rightSon->rank;
            data_rr = node->rightSon->rightSon->getData();
        }
        if(!node->leftSon){
            nl = 0;
            data_l = (node->key)();
        }
        else {
            nl = node->leftSon->rank;
            data_l = node->leftSon->getData() + (node->key)();
        }

        n = 1 + nl + nrl;
        newData = data_l +  data_rl;
        nr = 1 + nrr + n;
        data_r =  data_rr + data_l + data_rl;
        node->rank = n;//1 + node->leftSon->rank + node->rightSon->leftSon->rank;
        node->data = newData;
        node->rightSon->rank = nr; //1 + node->rightSon->rightSon->rank + node->rank;
        node->rightSon->data = data_r + (node->rightSon->key)();

        AVLNode* temp = node->rightSon;
        node->rightSon = temp->leftSon;
        temp->leftSon = node;

        updateParentsAfterRotation(node,temp,node->rightSon);
        changeNode(temp->parent, node, temp);
        node->updateHeight();
        temp->updateHeight();
    }

    //rotating right
    void rotateLL(AVLNode* node) {
        //update ranks of temp(parent) and node (right son):
        int n, nl, nlr, nr, nll, newData, data_l,data_lr,data_r,data_ll;
        if(!node->leftSon->rightSon) {
            nlr = 0;
            data_lr = 0;
        }
        else {
            nlr = node->leftSon->rightSon->rank;
            data_lr = node->leftSon->rightSon->getData();
        }
        if(!node->leftSon->leftSon) {
            nll = 0;
            data_ll = 0;
        }
        else {
            nll = node->leftSon->leftSon->rank;
            data_ll = node->leftSon->leftSon->getData();
        }
        if(!node->rightSon) {
            nr = 0;
            data_r = (node->key)();
        }
        else {
            nr = node->rightSon->rank;
            data_r = node->rightSon->getData() + (node->key)();
        }

        n = 1 + nr + nlr;
        newData = data_r + data_lr;
        nl = 1 + nll + n;
        data_l = data_ll + data_r + data_lr;

        node->rank = n;//1 + node->rightSon->rank + node->leftSon->rightSon->rank;
        node->data = newData;
        node->leftSon->rank = nl;//1 + node->leftSon->leftSon->rank + node->rank;
        node->leftSon->data = data_l + (node->leftSon->key)();

        AVLNode* temp = node->leftSon;
        node->leftSon = temp->rightSon;
        temp->rightSon = node;

        updateParentsAfterRotation(node,temp,node->leftSon);
        changeNode(temp->parent, node, temp);
        node->updateHeight();
        temp->updateHeight();
    }

    //rotate left then right
    void rotateLR(AVLNode* node) {
        rotateRR(node->leftSon);
        rotateLL(node);

    }

    //rotate right then left
    void rotateRL(AVLNode* node) {
        rotateLL(node->rightSon);
        rotateRR(node);

    }

    //getters
    D getData(K key);
    int getSize() const {
        return size;
    }
    int getHeight() const {
        return root->height;
    }

    K* inOrderKeyArray() const {
        K* inOrderKeys;
        inOrderKeys = (K*)malloc(size*sizeof(*inOrderKeys));
        if (inOrderKeys) {
            AVLNode* nodes[size];
            int i = 0;
            inOrderNodeArray(root, nodes, &i);
            for (int j = 0; j < size; ++j) {
                inOrderKeys[j] = nodes[j]->key;
            }
            return inOrderKeys;
        }
        return NULL;
    }

    D* inOrderDataArray() {
        D* inOrderData;
        inOrderData =  (D*)malloc(size*sizeof(*inOrderData));
        if (inOrderData) {
            AVLNode* nodes[size];
            int i = 0;
            inOrderNodeArray(root, nodes, &i);
            for (int j = 0; j < size; ++j) {
                inOrderData[j] = nodes[j]->data;
            }
            return inOrderData;
        }
        return NULL;
    }

    void inOrderNodeArray(AVLNode* node, AVLNode* arr[], int* i) const {
        if (node) {
            inOrderNodeArray(node->leftSon, arr, i);
            arr[(*i)] = node;
            (*i)++;
            inOrderNodeArray(node->rightSon, arr, i);
        }
    }

    K* postOrderKeyArray() const {
        K* postOrderKeys;
        postOrderKeys = (K*)malloc(size*sizeof(*postOrderKeys));
        if (postOrderKeys) {
            AVLNode* nodes[size];
            int i=0;
            postOrderNodeArray(root, nodes, &i);
            for (int j = 0; j < size; ++j) {
                postOrderKeys[j] = nodes[j]->key;
            }
            return postOrderKeys;
        }
        return NULL;
    }
    void postOrderNodeArray(AVLNode* node, AVLNode* arr[], int* i) const {
        if (node) {
            postOrderNodeArray(node->leftSon, arr, i);
            postOrderNodeArray(node->rightSon, arr, i);
            arr[(*i)] = node;
            (*i)++;
        }
    }

    K* preOrderKeyArray() const {
        K* preOrderKeys;
        preOrderKeys = (K*)malloc(size*sizeof(*preOrderKeys));
        if (preOrderKeys) {
            AVLNode* nodes[size];
            int i=0;
            preOrderNodeArray(root, nodes, &i);
            for (int j = 0; j < size; ++j) {
                preOrderKeys[j] = nodes[j]->key;
            }
            return preOrderKeys;
        }
        return NULL;
    }
    void preOrderNodeArray(AVLNode* node, AVLNode* arr[], int* i) const {
        if (node) {
            arr[(*i)] = node;
            (*i)++;
            preOrderNodeArray(node->leftSon, arr, i);
            preOrderNodeArray(node->rightSon, arr, i);
        }
    }

    AVLNode* findAVLNode (K key) {
        AVLNode* current = root;
        while (current) {
            if (current->key == key) return current;
            if (current->key > key) {
                current = current->leftSon;
            } else {
                current = current->rightSon;
            }
        }
        return NULL;
    }

    int findNodeIndex(K key){
        AVLNode* current = root;
        int indexCounter = 0;
        while (current) {
            if (current->key == key) {
                indexCounter++;
                if (current->leftSon) {
                    indexCounter += current->leftSon->rank;
                }
                return indexCounter;
            }
            if (current->key > key) {
                current = current->leftSon;
            }
            if (current->key < key) {
                indexCounter++;
                if (current->leftSon) {
                    indexCounter += current->leftSon->rank;
                }
                current = current->rightSon;
            }
        }
        return indexCounter;
    }

    void updateData(AVLNode* current) {
        if (current->leftSon) {
            updateData(current->leftSon);
            current->data += current->leftSon->getData();
            current->leftSon->parent = current;
        }
        if (current->rightSon) {
            updateData(current->rightSon);
            current->data += current->rightSon->getData();
            current->rightSon->parent = current;
        }
        else if (!current->rightSon && !current->leftSon) {
            current->data = current->key();
        }
    }
};
template<class K, class D>
int RankTree<K, D>::getRank(AVLNode* node)
{
    return node->rank;
}

template<class K, class D>
int RankTree<K, D>::getRank(K key)
{
    AVLNode* node = findAVLNode(key);
    if (!node) return -1;
    return getRank(node);
}

template<class K, class D>
void RankTree<K, D>::reduceRankPath(K key)
{
    AVLNode* current = root;
    while (current) {
        current->rank -= 1;
        if (current->key == key) return;
        if (current->key > key) {
            current = current->leftSon;
        } else {
            current = current->rightSon;
        }
    }
}

template<class K, class D>
void RankTree<K, D>::insert(K key, D data) {
    if (root == NULL) {
        root = new AVLNode(key, data);

        size++;
    } else {
        insert(key, data, root);
    }
}

template<class K, class D>
void RankTree<K, D>::destroyData() {
    if (root) {
        destroyData(root);
    }
}
template<class K, class D>
void RankTree<K, D>::destroyData(AVLNode* node) {
    if (node) {
        destroyData(node->leftSon);
        destroyData(node->rightSon);
        delete node->data;
    }
}

//here insert should not be used for update
template<class K, class D>
void RankTree<K, D>::insert(K key, D data, AVLNode* parent) {
    if (key == parent->key) {
        parent->data += data;
        return;
    }
    parent->rank += 1;
    parent->data += data;
    if (parent->key > key) {
        if (parent->leftSon) insert(key, data, parent->leftSon);
        else {
            parent->leftSon = new AVLNode(key, data, parent);
            size++;
        }
    } else {
        if (parent->rightSon) insert(key, data, parent->rightSon);
        else {
            parent->rightSon = new AVLNode(key, data, parent);
            size++;
        }
    }
    reBalance(parent, false);
}

// Removing nodes
template<class K, class D>
bool RankTree<K, D>::deleteKey(K key) {
    AVLNode* current = findAVLNode(key);
    if (!current) return false;
    reduceRankPath(key);
    reduceDataPath(key);
    deleteNode(current);
    size--;
    return true;
}

template<class K, class D>
void RankTree<K,D>::reduceDataPath(K key) {
    AVLNode* current = root;
    while (current) {
        current->data -= key();
        if (current->key == key){
            //current->data  = 0;
            return;
        }
        if (current->key > key) {
            current = current->leftSon;
        } else {
            current = current->rightSon;
        }
    }
}
template<class K, class D>
void RankTree<K, D>::deleteNode(AVLNode* toDelete) {
    AVLNode* ancestor = toDelete->rightSon;
    if (toDelete->haveTwoSons()) { // node has Two sons
        while (ancestor->leftSon) {
            ancestor = ancestor->leftSon;
        }
    } else if (toDelete->leftSon) { // node only have left son
        ancestor = toDelete->leftSon;
    }
    if (ancestor) {
        toDelete->key = ancestor->key;
        toDelete->data = ancestor->data + toDelete->data - ancestor->key();
        deleteNode(ancestor);
    } else { // node is a leaf or remained a leaf
        AVLNode* parent = toDelete->parent;
        if (!parent) {
            root = NULL;
        }
        else if (parent->leftSon == toDelete) {
            parent->data -= toDelete->key();
            parent->leftSon = NULL;
        }
        else {
            //parent->data -= toDelete->key();
            parent->rightSon = NULL;
        }
        delete(toDelete);
        toDelete = NULL;
        reBalance(parent,true);
    }
}
template<class K, class D>
bool RankTree<K, D>::isExist(K key) const {
    AVLNode* current = findAVLNode(key);
    return (current != NULL);
}

template<class K, class D>
bool RankTree<K, D>::isExist(K key) {
    AVLNode* current = findAVLNode(key);
    return (current != NULL);
}
template<class K, class D>
D RankTree<K, D>::getData(K key) {
    AVLNode* nodeToGetData = findAVLNode(key);
    if (nodeToGetData) {
        return nodeToGetData->data;
    } else return -1;
}
template<class K, class D>
void RankTree<K, D>::changeData(K key, D new_data){
    AVLNode* nodeToChange = findAVLNode(key);
    if (nodeToChange) {
        nodeToChange->data = new_data;
    }
}

template<class K, class D>
void RankTree<K, D>::reBalance(AVLNode* node, bool isDelete) {
    if (!node) return;
    node->updateHeight();
    int balanceFactor = node->getBalanceFactor();
    if (balanceFactor == 2) {
        if (node->leftSon->getBalanceFactor() >= 0) rotateLL(node);
        else rotateLR(node);
    } else if (balanceFactor == -2) {
        if (node->rightSon->getBalanceFactor() <= 0) rotateRR(node);
        else rotateRL(node);
    }
    if (isDelete) reBalance(node->parent, isDelete);
}

template<class K, class D>
void RankTree<K, D>::changeNode(AVLNode* parent, AVLNode* oldNode, AVLNode* newNode) {
    if (parent == NULL) root = newNode;
    else if (parent->leftSon == oldNode) parent->leftSon = newNode;
    else parent->rightSon = newNode;
}
template<class K, class D>
bool RankTree<K, D>::isEmpty()
{
    return this->root == NULL;
}

template<class K, class D>
RankTree<K, D>* RankTree<K,D>::merge(RankTree<K,D>* other, int m1, int m2)
{
    AVLNode* n1 = this->root;
    AVLNode* n2 = other->root;
    AVLNode* node = n1->mergeNodes(n2, m1, m2);
    updateData(node);
    return new RankTree<K, D>(node, m1 + m2);
    /*int** arr = mergeArray(treeToArray(t1, m1), treeToArray(t2, m2), m1, m2);*/
}
#endif //RankTree_RankTree_H