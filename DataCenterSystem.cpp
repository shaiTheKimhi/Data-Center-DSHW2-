

#include <new>
#include "DataCenterSystem.h"



DataCenterSystem::DataCenterSystem() {
    this->dataCentersArray = NULL;
    this->allServersTraffic = NULL;
    this->dataCenterUnionFindByID = NULL;
    this->serversHashMap = NULL;
    this->elementsNum = 0;
}

DataCenterSystem::~DataCenterSystem() {
    if (this->dataCentersArray) {
        for (int i = 0; i < this->elementsNum; i++)
                delete this->dataCentersArray[i];
        delete[](this->dataCentersArray);
        this->dataCentersArray = NULL;
    }
    if (this->allServersTraffic) {
        delete(this->allServersTraffic);
        this->allServersTraffic = NULL;
    }
    if (this->dataCenterUnionFindByID) {
        delete (this->dataCenterUnionFindByID);
        this->dataCenterUnionFindByID = NULL;
    }
    if (this->serversHashMap) {
        delete (this->serversHashMap);
        this->serversHashMap = NULL;
    }
}
void DataCenterSystem::Init(int n) {
    try {
        this->dataCentersArray =  new DataCenter*[n];
        for (int i = 1; i <= n ; i++) {
            DataCenter *dc = new DataCenter(i);
            dataCentersArray[i-1] = dc;
        }
        this->allServersTraffic = NULL;
        this->dataCenterUnionFindByID = new UnionFind(n);
        this->serversHashMap = new HashMap<shared_ptr<Server>>();
        this->elementsNum = n;
    }
    catch(std::bad_alloc &ba){
        this->dataCentersArray = NULL;
        this->allServersTraffic = NULL;
        this->dataCenterUnionFindByID = NULL;
        this->serversHashMap = NULL;
    }
}

void DataCenterSystem::Quit(void** DS) {
    DataCenterSystem *ds = (DataCenterSystem *) *DS;
    if (ds != NULL) {
        delete(ds);
    }
    *DS = NULL;
}

StatusType DataCenterSystem:: MergeDataCenters(int dataCenter1,  int dataCenter2) {
    if (dataCenter1 <= 0 || dataCenter1 > this->elementsNum || dataCenter2 <= 0 || dataCenter2 > this->elementsNum) {
        return INVALID_INPUT;
    }
    try {
        int dataCenter1Set = this->dataCenterUnionFindByID->find(dataCenter1-1);
        int dataCenter2Set = this->dataCenterUnionFindByID->find(dataCenter2-1);
        DataCenter* toMerge1 = this->dataCentersArray[dataCenter1Set];
        DataCenter* toMerge2 = this->dataCentersArray[dataCenter2Set];
        
        RankTree<ServerNodeKey,int>* rankTree1 = toMerge1->DCsServersTraffic;
        RankTree<ServerNodeKey,int>* rankTree2 = toMerge2->DCsServersTraffic;
        int newTrafficCount = 0;
        if (rankTree1 != NULL) {
            newTrafficCount += rankTree1->getSize();
        }
        if (rankTree2 != NULL) {
            newTrafficCount += rankTree2->getSize();
        }
        //getting merged servers tree:
        if (newTrafficCount > 0) {
            if (rankTree1 != NULL && rankTree2 != NULL) {
                RankTree<ServerNodeKey,int>* temp = rankTree1->merge(rankTree2, rankTree1->getSize(), rankTree2->getSize());
                delete rankTree1;
		        delete rankTree2;
                this->dataCentersArray[dataCenter1Set]->DCsServersTraffic = NULL;
                this->dataCentersArray[dataCenter2Set]->DCsServersTraffic = NULL;
		        rankTree1 = temp;
	        }
            else if (rankTree2 != NULL && rankTree1 == NULL) {
                rankTree1 = rankTree2;
            }
        }
        int newServerCount = toMerge1->serversCounter + toMerge2->serversCounter;
        //merge IDs in unionFind
        int mergedID = this->dataCenterUnionFindByID->unionFunc(dataCenter1Set, dataCenter2Set);
        //updating info after union
        this->dataCentersArray[mergedID]->DCsServersTraffic = rankTree1;
        this->dataCentersArray[mergedID]->serversCounter = newServerCount;
        if (mergedID != dataCenter1Set) {
            delete this->dataCentersArray[dataCenter1Set];
            this->dataCentersArray[dataCenter1Set] = NULL;
        } else if (mergedID != dataCenter2Set) {
            delete this->dataCentersArray[dataCenter2Set];
            this->dataCentersArray[dataCenter2Set] = NULL;
        }
        return SUCCESS;
    }
    catch (std::bad_alloc &ba) {
        return ALLOCATION_ERROR;
    }
}

StatusType DataCenterSystem::AddServer(int dataCenterID, int serverID) {
    if (dataCenterID <= 0 || dataCenterID > this->elementsNum || serverID <= 0) {
        return INVALID_INPUT;
    }
    if (isServerExist(serverID)) {
        return FAILURE;
    }
    try {
        int currDCGroup = this->dataCenterUnionFindByID->find(dataCenterID-1);
        Server *newServer = new Server(serverID,currDCGroup+1);
        shared_ptr<Server> ns(newServer);
        this->serversHashMap->insert(serverID,ns);
        //delete newServer;
        this->dataCentersArray[currDCGroup]->serversCounter++;
        return SUCCESS;
    }
    catch (std::bad_alloc &ba) {
        return ALLOCATION_ERROR;
    }
}

StatusType DataCenterSystem::RemoveServer(int serverID) {
    if (serverID <= 0) {
        return INVALID_INPUT;
    }
    if (!(isServerExist(serverID))) {
        return FAILURE;
    }
    try {
        shared_ptr<Server> serverToRemove = *(this->serversHashMap->find(serverID));
        int DCWithServerToRemoveID = this->dataCenterUnionFindByID->find((serverToRemove->DCFatherID)-1);
        DataCenter* DCsServerToRemove = this->dataCentersArray[DCWithServerToRemoveID];
        int trafficToRemove = serverToRemove->traffic;
        this->serversHashMap->deleteNode(serverID);

        if (trafficToRemove != -1) {
            ServerNodeKey toRemove = ServerNodeKey(serverID,trafficToRemove);
            this->allServersTraffic->deleteKey(toRemove);

            DCsServerToRemove->DCsServersTraffic->deleteKey(toRemove);
            DCsServerToRemove->serversCounter--;
        }
        return SUCCESS;
    }
    catch (std::bad_alloc &ba) {
        return ALLOCATION_ERROR;
    }
}

StatusType DataCenterSystem::SetTraffic(int serverID, int traffic) {
    if (traffic < 0 || serverID <= 0)
        return INVALID_INPUT;
    if (!(isServerExist(serverID)))
        return FAILURE;
    try {
            shared_ptr<Server> addTrafficServer = *(this->serversHashMap->find(serverID));
            int oldTraffic = addTrafficServer->traffic;
            addTrafficServer->traffic = traffic;
            int fatherDCID = this->dataCenterUnionFindByID->find((addTrafficServer->DCFatherID)-1);
            DataCenter *fatherDC = this->dataCentersArray[fatherDCID];

            //check if there was no traffic for this server before, if so add new node in right place to both trees
            //else remove old node and new one
            if (oldTraffic == -1) {
                ServerNodeKey* newNode = new ServerNodeKey(serverID,traffic);
                if (this->allServersTraffic == NULL) {
                    this->allServersTraffic = new RankTree<ServerNodeKey,int>();
                }
                this->allServersTraffic->insert(*newNode,traffic);
                int newData = this->allServersTraffic->findAVLNode(*newNode)->isLeftSonExist() ? this->allServersTraffic->findAVLNode(*newNode)->getLeftSonData() : 0;
                newData += this->allServersTraffic->findAVLNode(*newNode)->isRightSonExist() ? this->allServersTraffic->findAVLNode(*newNode)->getRightSonData(): 0;
                newData += traffic;
                this->allServersTraffic->changeData(*newNode,newData);

                if (fatherDC->DCsServersTraffic == NULL) {
                    fatherDC->DCsServersTraffic = new RankTree<ServerNodeKey,int>();
                }

                fatherDC->DCsServersTraffic->insert(*newNode,traffic);
                int newDataDC = fatherDC->DCsServersTraffic->findAVLNode(*newNode)->isLeftSonExist() ? fatherDC->DCsServersTraffic->findAVLNode(*newNode)->getLeftSonData(): 0;
                newDataDC += fatherDC->DCsServersTraffic->findAVLNode(*newNode)->isRightSonExist() ? fatherDC->DCsServersTraffic->findAVLNode(*newNode)->getRightSonData(): 0;
                newDataDC += traffic;
                fatherDC->DCsServersTraffic->changeData(*newNode,newDataDC);
                delete newNode;
            } else {
                ServerNodeKey oldNode = ServerNodeKey(serverID,oldTraffic);
                this->allServersTraffic->deleteKey(oldNode);
                fatherDC->DCsServersTraffic->deleteKey(oldNode);

                ServerNodeKey* newNode = new ServerNodeKey(serverID,traffic);

                this->allServersTraffic->insert(*newNode,traffic);
                int newData = this->allServersTraffic->findAVLNode(*newNode)->isLeftSonExist() ? this->allServersTraffic->findAVLNode(*newNode)->getLeftSonData() : 0;
                newData += this->allServersTraffic->findAVLNode(*newNode)->isRightSonExist() ? this->allServersTraffic->findAVLNode(*newNode)->getRightSonData() : 0;
                newData += traffic;
                this->allServersTraffic->changeData(*newNode,newData);

                fatherDC->DCsServersTraffic->insert(*newNode,traffic);
                int newDataDC = fatherDC->DCsServersTraffic->findAVLNode(*newNode)->isLeftSonExist() ?fatherDC->DCsServersTraffic->findAVLNode(*newNode)->getLeftSonData(): 0;
                newDataDC += fatherDC->DCsServersTraffic->findAVLNode(*newNode)->isRightSonExist()? fatherDC->DCsServersTraffic->findAVLNode(*newNode)->getRightSonData(): 0;
                newDataDC += traffic;
                fatherDC->DCsServersTraffic->changeData(*newNode,newDataDC);
                delete newNode;
            }
            return SUCCESS;
    }
    catch (std::bad_alloc &ba) {
        return ALLOCATION_ERROR;
    }
}

StatusType DataCenterSystem::SumHighestTrafficServers(int dataCenterID, int k, int* traffic) {
    if (dataCenterID < 0 || dataCenterID > this->elementsNum || k < 0 || traffic == NULL ) {
        return INVALID_INPUT;
    }
    try {
        if (dataCenterID > 0) {
            int fatherId = this->dataCenterUnionFindByID->find(dataCenterID-1);
            DataCenter* DC = this->dataCentersArray[fatherId];
            int serversWithTrafficAmount = DC->DCsServersTraffic->getSize();
            int minIndex = (k > serversWithTrafficAmount) ? 0 : (serversWithTrafficAmount-k);
            *traffic = DC->DCsServersTraffic->getDataByMinIndex(minIndex);
            return SUCCESS;
        }
        else {
            int serversWithTrafficAmount = this->allServersTraffic->getSize();
            int minIndex = (k > serversWithTrafficAmount) ? 0 : (serversWithTrafficAmount-k);
            *traffic = this->allServersTraffic->getDataByMinIndex(minIndex);
            return SUCCESS;
        }
    }
    catch (std::bad_alloc &ba) {
        return ALLOCATION_ERROR;
    }
}

bool DataCenterSystem::isServerExist(int serverID) {
    return this->serversHashMap->isExist(serverID);
}
