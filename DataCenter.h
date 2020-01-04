#include "library2.h"
#include "RankTree.h"

class Server {
public:

    int serverID;
    int traffic;
    int DCFatherID;

    Server(int serverID, int DCFatherID): serverID(serverID),traffic(-1),DCFatherID(DCFatherID){};
};

class ServerNodeKey {
public:
    int serverID;
    int traffic;

    ServerNodeKey(int serverID): serverID(serverID),traffic(-1){};
    bool operator> (ServerNodeKey* toCompare) {
        if (toCompare == NULL){
            return true;
        }
        if (this->traffic > toCompare->traffic) {
            return true;
        }

        if (this->traffic < toCompare->traffic)  {
            return false;
        }

        if (this->traffic == toCompare->traffic) {
            return (this->serverID > toCompare->serverID);
        }
    }

    bool operator< (ServerNodeKey* toCompare) {
        if (toCompare == NULL){
            return false;
        }
        if (this->traffic < toCompare->traffic) {
            return true;
        }

        if (this->traffic > toCompare->traffic)  {
            return false;
        }

        if (this->traffic == toCompare->traffic) {
            return (this->serverID < toCompare->serverID);
        }
    }

    bool operator== (ServerNodeKey* toCompare) {
        if (toCompare == NULL){
            return false;
        }
        if (this->traffic == toCompare->traffic) {
            return (this->serverID == toCompare->serverID);
        }
        else {
            return false;
        }
    }
};

class DataCenter {
public:
    int dataCenterID;
    int serversCounter;
    RankTree<ServerNodeKey*,int>* DCsServersTraffic;
    DataCenter(int dataCenterID);
    ~DataCenter();
};

