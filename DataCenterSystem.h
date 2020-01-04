#include <exception>
#include"DataCenter.h"
#include "HashMap.h"
#include "UnionFind.h"

using std::exception;

class DataCenterSystem{
public:

    DataCenter** dataCentersArray;
    RankTree<ServerNodeKey,int>* allServersTraffic;
    UnionFind* dataCenterUnionFindByID;
    HashMap<Server*>* serversHashMap;
    int elementsNum;

    DataCenterSystem();
    ~DataCenterSystem();
    StatusType Init(int n);//initializes the dataCenterSystem

    StatusType MergeDataCenters(int dataCenter1,  int dataCenter2);

    StatusType AddServer(int dataCenterID, int serverID);

    StatusType RemoveServer(int serverID);

    StatusType SetTraffic(int serverID, int traffic);

    StatusType SumHighestTrafficServers(int dataCenterID, int k, int* traffic);

    void Quit(void **DS);
private:
    bool isServerExist(int serverID);
};