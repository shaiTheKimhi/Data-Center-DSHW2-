

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
    delete (this->dataCentersArray);
    this->dataCentersArray = NULL;
    delete(this->allServersTraffic);
    this->allServersTraffic = NULL;
    delete (this->dataCenterUnionFindByID);
    this->dataCenterUnionFindByID = NULL;
    delete (this->serversHashMap);
    this->serversHashMap = NULL;
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
        this->serversHashMap = new HashMap<Server*>();
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
                rankTree1 = rankTree1->merge(rankTree2, rankTree1->getSize(), rankTree2->getSize());
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
            this->dataCentersArray[dataCenter1-1] = this->dataCentersArray[mergedID];
        } else if (mergedID != dataCenter2Set) {
            this->dataCentersArray[dataCenter2-1] = this->dataCentersArray[mergedID];
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
        this->serversHashMap->insert(serverID,newServer);
        this->dataCentersArray[currDCGroup]->serversCounter++;
        return SUCCESS;
    }
    catch (std::bad_alloc &ba) {
        return ALLOCATION_ERROR;
    }
}

StatusType DataCenterSystem::RemoveServer(int serverID) {
    if (serverID < 0) {
        return INVALID_INPUT;
    }
    if (!(isServerExist(serverID))) {
        return FAILURE;
    }
    try {
        Server* serverToRemove = *(this->serversHashMap->find(serverID));
        int DCWithServerToRemoveID = this->dataCenterUnionFindByID->find(serverToRemove->DCFatherID);
        DataCenter* DCsServerToRemove = this->dataCentersArray[DCWithServerToRemoveID-1];
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
    if (traffic < 0 || serverID < 0)
        return INVALID_INPUT;
    if (!(isServerExist(serverID)))
        return FAILURE;
    try {
            Server* addTrafficServer = *(this->serversHashMap->find(serverID));
            int oldTraffic = addTrafficServer->traffic;
            addTrafficServer->traffic = traffic;
            int fatherDCID = this->dataCenterUnionFindByID->find(addTrafficServer->DCFatherID);
            DataCenter *fatherDC = this->dataCentersArray[fatherDCID-1];

            //check if there was no traffic for this server before, if so add new node in right place to both trees
            //else remove old node and new one
            if (traffic == -1) {
                ServerNodeKey* newNode = new ServerNodeKey(serverID,traffic);

                this->allServersTraffic->insert(*newNode,0);
                int newData = this->allServersTraffic->findAVLNode(*newNode)->getLeftSonData();
                newData += this->allServersTraffic->findAVLNode(*newNode)->getRightSonData();
                newData += traffic;
                this->allServersTraffic->changeData(*newNode,newData);

                fatherDC->DCsServersTraffic->insert(*newNode,0);
                int newDataDC = fatherDC->DCsServersTraffic->findAVLNode(*newNode)->getLeftSonData();
                newDataDC += fatherDC->DCsServersTraffic->findAVLNode(*newNode)->getRightSonData();
                newDataDC += traffic;
                fatherDC->DCsServersTraffic->changeData(*newNode,newDataDC);

                //TODO: how to update data all the way to the root?

            } else {
                ServerNodeKey oldNode = ServerNodeKey(serverID,oldTraffic);
                this->allServersTraffic->deleteKey(oldNode);
                fatherDC->DCsServersTraffic->deleteKey(oldNode);

                //TODO: how to update data all the way to the root- for deletion?

                ServerNodeKey* newNode = new ServerNodeKey(serverID,traffic);

                this->allServersTraffic->insert(*newNode,0);
                int newData = this->allServersTraffic->findAVLNode(*newNode)->getLeftSonData();
                newData += this->allServersTraffic->findAVLNode(*newNode)->getRightSonData();
                newData += traffic;
                this->allServersTraffic->changeData(*newNode,newData);

                fatherDC->DCsServersTraffic->insert(*newNode,0);
                int newDataDC = fatherDC->DCsServersTraffic->findAVLNode(*newNode)->getLeftSonData();
                newDataDC += fatherDC->DCsServersTraffic->findAVLNode(*newNode)->getRightSonData();
                newDataDC += traffic;
                fatherDC->DCsServersTraffic->changeData(*newNode,newDataDC);

                //TODO: how to update data all the way to the root?
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
    }
    catch (std::bad_alloc &ba) {
        return ALLOCATION_ERROR;
    }
}

bool DataCenterSystem::isServerExist(int serverID) {
    return this->serversHashMap->isExist(serverID);
}