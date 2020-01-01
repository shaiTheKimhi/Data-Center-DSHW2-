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

    public:
        D getData(){
            return data;
        }
    };

    AVLNode* root; //Any tree is a Node as a root
    int size; //how many nodes
    
    void insert(K key, D data, AVLNode* parent);
    void deleteNode(AVLNode* toDelete);
    void reduceRankPath(K key);
    void changeNode(AVLNode* parent, AVLNode* oldNode, AVLNode* newNode);
    void reBalance(AVLNode* node, bool isDelete = false);

public:
    RankTree() : root(NULL), size(0) {}
	
	/*AVLNode* createShell(int m, AVLNode* parent = NULL)
	{
		if(!m) return NULL;
		AVLNode* node = new AVLNode(0,0,parent);
		node->leftSon = createShell(m/2, node);
		node->rightSon = createShell(m/2, node);
		return node;
	}
	void pour(int** arr, AVLNode* node, int m, int* i)
	{
		pour(arr, node->left, m, i);
		if(!node) return;
		if(i >= m) return;
		node->key = arr[i][0];
		node->data = arr[i][1];
		*(i) += 1;
		pour(arr, node->right, m, i);
		
	}
	RankTree(int** arr, int m):root(NULL),size(m)
	{
		int i = 0;
		AVLNode* node = createShell(m);
		pour(arr, node, m, &i);
		root = node;
		//check if anything needs to be added
	}*/
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
        int n, nl, nrl, nr, nrr; 
        if(!node->rightSon->leftSon) nrl = 0;
        else nrl = node->rightSon->leftSon->rank;
        if(!node->rightSon->rightSon) nrr = 0;
        else nrr = node->rightSon->rightSon->rank;
        if(!node->leftSon) nl = 0;
        else nl = node->leftSon->rank;
        
        n = 1 + nl + nrl;
        nr = 1 + nrr + n;
        node->rank = n;//1 + node->leftSon->rank + node->rightSon->leftSon->rank;
        node->rightSon->rank =nr; //1 + node->rightSon->rightSon->rank + node->rank;

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
        int n, nl, nlr, nr, nll; 
        if(!node->leftSon->rightSon) nlr = 0;
        else nlr = node->leftSon->rightSon->rank;
        if(!node->leftSon->leftSon) nll = 0;
        else nll = node->leftSon->leftSon->rank;
        if(!node->rightSon) nr = 0;
        else nr = node->rightSon->rank;
        
        n = 1 + nr + nlr;
        nl = 1 + nll + n;
        
        node->rank = n;//1 + node->rightSon->rank + node->leftSon->rightSon->rank;
        node->leftSon->rank = nl;//1 + node->leftSon->leftSon->rank + node->rank;

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
    D getData(K key) const;
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
        parent->data = data;
        return;
    }
    parent->rank += 1;
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
    deleteNode(current);
    size--;
    return true;
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
        toDelete->data = ancestor->data;
        deleteNode(ancestor);
    } else { // node is a leaf or remained a leaf
        AVLNode* parent = toDelete->parent;
        if (!parent) {
            root = NULL;
        }
        else if (parent->leftSon == toDelete) {
            parent->leftSon = NULL;
        }
        else {
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
D RankTree<K, D>::getData(K key) const {
    AVLNode* nodeToGetData = findAVLNode(key);
    if (nodeToGetData) {
        return nodeToGetData->data;
    } else return NULL;
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



// template<class K, class D>
// AVLNode* createShell(int n,K key, D data, AVLNode* parent)
// {
//     if(n <= 0) return NULL;
//     AVLNode* node = AVLNode* node = new AVLNode(key,data, parent);
//     node->leftSon = createShell(n - 1,key,data,node);
//     node->rightSon = createShell(n - 2,key,data,node);
// }

// template<class K, class D>
// AVLNode* createShell(int n,K key, D data)
// {
//     if(n <= 0) return NULL;
//     AVLNode* node = new AVLNode(key,data);
//     node->leftSon = createShell(n - 1,key,data,node);
//     node->rightSon = createShell(n - 2,key,data,node);
//     return node;
// }

// template<class K, class D>
// RankTree<K, D>* RankTree<K,D>::merge(RankTree<K,D> t1, RankTree<K,D> t2, int m1, int m2)
// {
//     if(!m1 && (!m2)) NULL;
//     AVLNode* shell = createShell(m1+m2, t.root->key,t.root->data);

// }


#endif //RankTree_RankTree_H