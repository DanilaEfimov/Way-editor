#include "graph.h"

#include<fstream>
#include<sstream>
#include<QMessageBox>

uint Graph::count = 0;

Graph::Graph() { // one vertex graph
    this->ID = count;
    this->count++;
    this->V = 1;
    this->E = 0;
    this->vertexes = {};
    this->connectivityList = { std::pair<uint, std::set<uint>>(0,this->vertexes) };
    this->connectivityMat = new bool*[1];
    this->connectivityMat[0] = 0;
}

Graph::Graph(std::string path) {
    std::ifstream file(path);
    if(!file.is_open()){
        QMessageBox fail;
        fail.setWindowTitle("Failed to open file");
        fail.setText("Please, check Your path and file");
        return;
    }
    this->ID = count;
    this->count++;
    std::string temp;
    std::getline(file, temp);
    std::stringstream vertexCount(temp);

    vertexCount >> this->V;
    this->connectivityMat = new bool*[V];
    for(uint i = 0; i < V; i++){
        this->connectivityMat[i] = new bool[V] {false};
    }
    this->E = 0;


    for(uint i = 0; i < this->V && !file.eof(); i++){
        this->vertexes.insert(i);
        std::getline(file, temp);
        std::stringstream list(temp);
        uint vert;
        while(!list.eof()){
            list >> vert;
            this->connectivityList[i].insert(vert);
            this->connectivityMat[i][vert] = true;
            this->E++;
        }
    }
    this->E /= 2;

    for(uint i = 0; i < this->V; i++){
        for(uint j = i; j < this->V; j++){
            if(this->connectivityMat[i][j] != this->connectivityMat[j][i]){
                QMessageBox fail;
                fail.setWindowTitle("Not undirected graph");
                fail.setText("Please, check Your file");
                return;
            }
        }
    }

    if(this->vertexes.size() != V){
        QMessageBox fail;
        fail.setWindowTitle("Something went wrong");
        fail.setText("Please, check Your path and file");
        return;
    }
}

Graph::~Graph() {
    for(uint i = 0; i < this->V; i++){
        delete[] this->connectivityMat[i];
    }
    delete[] this->connectivityMat;
}

float Graph::connectivity() {
    float matSum = 0;
    for(uint i = 0; i < this->V; i++){
        for(uint j = i; j < this->V; j++){
            matSum += this->connectivityMat[i][j] ? 1 : 0;
        }
    }
    return matSum / (this->V * this->V / 2);
}

// Accessors
uint* Graph::getID() {
    return &this->ID;
}

uint* Graph::getV(){
    return &this->V;
}

uint* Graph::getE(){
    return &this->E;
}

std::set<uint> *Graph::getVertexes() {
    return &this->vertexes;
}

std::map<id, std::set<uint> > *Graph::getConnectivityList() {
    return &this->connectivityList;
}

bool ***Graph::getConnectivityMat() {
    return &this->connectivityMat;
}
