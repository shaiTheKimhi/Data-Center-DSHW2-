#include "library2.h"
#include "RankTree.h"

class Server {
public:

    int serverID;
    int traffic;
    int DCFatherID;

    Server(int serverID, int DCFatherID): serverID(serverID),traffic(-1),DCFatherID(DCFatherID){};
};

class DataCenter {
public:
    int dataCenterID;
    int serversCounter;
    RankTree<int, RankTree<int,int>*>* DCsServersTraffic;
    DataCenter(int dataCenterID);
    ~DataCenter();
};

