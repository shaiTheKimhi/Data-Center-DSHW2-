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

    int getTraffic() {
        return this->traffic;
    }

    int getID() {
        return this->serverID;
    }

    bool operator< (ServerNodeKey toCmp) {
        if (this->traffic < toCmp.traffic) {
            return true;
        }

        if (this->traffic > toCmp.traffic) {
            return false;
        }

        if(this->traffic == toCmp.traffic){
            return (this->serverID < toCmp.serverID);
        }
        return false;
    }

    bool operator> (ServerNodeKey toCmp) {
        if (this->traffic > toCmp.traffic) {
            return true;
        }

        if (this->traffic < toCmp.traffic) {
            return false;
        }

        if(this->traffic == toCmp.traffic){
            return (this->serverID > toCmp.serverID);
        }
        return false;
    }

    bool operator== (ServerNodeKey toCmp) {
        if (this->traffic == toCmp.traffic) {
            return (this->serverID == toCmp.serverID);
        }
        return false;
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

