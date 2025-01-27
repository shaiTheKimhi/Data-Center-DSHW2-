#include <exception>
#include <new>
#include "library2.h"
#include "DataCenterSystem.h"
using std::exception;

void *Init(int n) {
    if (n<= 0) {
        return NULL;
    }
    try {
        DataCenterSystem *DS = new DataCenterSystem();
        DS->Init(n);
        return (void*)DS;
    }
    catch(std::bad_alloc &ba) {
        return NULL;
    }
}

StatusType MergeDataCenters(void *DS, int dataCenter1,  int dataCenter2) {
    if (DS == NULL) {
        return INVALID_INPUT;
    }
    return ((DataCenterSystem *)DS)->MergeDataCenters(dataCenter1,dataCenter2);
}

StatusType AddServer(void *DS, int dataCenterID, int serverID) {
    if (DS == NULL) {
        return INVALID_INPUT;
    }
    return ((DataCenterSystem *)DS)->AddServer(dataCenterID,serverID);
}

StatusType RemoveServer(void *DS, int serverID) {
    if (DS == NULL) {
        return INVALID_INPUT;
    }
    return ((DataCenterSystem *)DS)->RemoveServer(serverID);
}

StatusType SetTraffic(void *DS, int serverID, int traffic) {
    if (DS == NULL) {
        return INVALID_INPUT;
    }
    return ((DataCenterSystem *)DS)->SetTraffic(serverID,traffic);
}

StatusType SumHighestTrafficServers(void *DS, int dataCenterID, int k, int* traffic) {
    if (DS == NULL) {
        return INVALID_INPUT;
    }
    return ((DataCenterSystem *)DS)->SumHighestTrafficServers(dataCenterID,k,traffic);
}

void Quit(void **DS) {
    return ((DataCenterSystem *)DS)->Quit(DS);
}