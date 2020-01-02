#include<cstdlib>
#include"DataCenter.h"

DataCenter::DataCenter(int dataCenterID) {
this->dataCenterID = dataCenterID;
this->serversCounter = 0;
this->DCsServersTraffic = NULL;
}

DataCenter::~DataCenter() {
    delete(this->DCsServersTraffic);
    this->DCsServersTraffic = NULL;
}