

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
StatusType DataCenterSystem::Init(int n) {
    if (n <= 0) {
        return FAILURE;
    }
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
        return SUCCESS;
    }
    catch(std::bad_alloc &ba){
        this->dataCentersArray = NULL;
        this->allServersTraffic = NULL;
        this->dataCenterUnionFindByID = NULL;
        this->serversHashMap = NULL;
        return FAILURE;
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
        int currDCGroup = this->dataCenterUnionFindByID->find(dataCenterID);
        Server *newServer = new Server(serverID,currDCGroup);
        this->serversHashMap->insert(serverID,newServer);
        this->dataCentersArray[currDCGroup-1]->serversCounter++;
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
            if(this->allServersTraffic->findAVLNode(trafficToRemove)){
                RankTree<int,int>* iDsTree = this->allServersTraffic->findAVLNode(trafficToRemove)->getData();
                iDsTree->deleteKey(serverID);
                if (iDsTree->getSize() == 0) {
                    this->allServersTraffic->deleteKey(trafficToRemove);
                }
            }
            if(DCsServerToRemove->DCsServersTraffic) {
                if(DCsServerToRemove->DCsServersTraffic->findAVLNode(trafficToRemove)) {
                    RankTree<int,int>* iDsTree = DCsServerToRemove->DCsServersTraffic->findAVLNode(trafficToRemove)->getData();
                    iDsTree->deleteKey(serverID);
                    if (iDsTree->getSize() == 0) {
                        this->allServersTraffic->deleteKey(trafficToRemove);
                    }
                }
                DCsServerToRemove->serversCounter--;
            }
        }
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

