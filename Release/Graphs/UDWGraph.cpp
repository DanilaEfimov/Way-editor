#include "UDWGraph.h"
#include<cmath>

static void setDefaultWeight(word* weights, uint _V, double weight){
    for(size_t i = 0; i < _V; i++){
        weights[i] = weight;
    }
}

UDWGraph::UDWGraph(uint _V, byte** mat, double weight) : UDirGraph(_V, mat){
    if(this->V == 0 || this->connectivityVector == nullptr){
        this->weights = nullptr;
        return;
    }
    this->weights = new word[this->V];
    setDefaultWeight(this->weights, this->V, weight);
}

UDWGraph::~UDWGraph()
{

}

void UDWGraph::print(std::fstream &_to) const
{

}

int UDWGraph::getDegree(uint _Vertex) const
{
    return 0;
}

bool UDWGraph::isConnected(uint _in, uint _out) const
{
    return true;
}

void UDWGraph::setEdge(uint _in, uint _out)
{

}

void UDWGraph::setNormalWeights() {
    for(size_t i = 0; i < this->V; i++){
        this->weights[i] = 1.0f;
    }
    // like this->setRandomWeights(0);
    // look at setRandomWeights definition
}

void UDWGraph::setMedianWeights() {
    if(this->V == 0){
        return;
    }
    uint* degrees = new uint[this->V];
    uint* degreeSum = new uint[this->V];
    for(size_t i = 0; i < this->V; i++){
        degrees[i] = this->getDegree(i+1);
        degreeSum = 0;
    }
    for(size_t i = 0; i < this->V; i++){
        for(size_t j = i + 1; j < this->V; j++){
            if(this->isConnected(i, j)){
                degreeSum[i] += degrees[j];
                degreeSum[j] += degrees[j];
            }
        }
    }
    for(size_t i = 0; i < this->V; i++){
        this->weights[i] = degrees[i] ? double(degreeSum[i] / degrees[i]) : 0.0f;
    }
    delete[] degreeSum;
    delete[] degrees;
}

void UDWGraph::setWeight(uint _Vertex, double _value) {
    if(_Vertex == 0 || _Vertex > this->V){
        return;
    }
    this->weights[_Vertex] = _value;
}

void UDWGraph::setRandomWeights(uint _seed1, uint _seed2, double _begin, double _end) {
    if(_begin > _end){
        return;
    }
    for(size_t i = 0; i < this->V; i++){
        double weight = (_end + _begin)/2.0f;
        double k = sin(_seed1 + _seed2);
        weight += k * (_end - weight)/2.0f;
        this->weights[i] = weight;
    }
    /*
    * this random weights performd median between _begin & _end
    * plus some value what turns median between _begin & _end
    * by sinusoid sin(_seed1 + _seed2) [_seed2 = 0, _begin = 0, _end = 100]
    */
}

std::stack<uint>& UDWGraph::Dejcstra(uint _in, uint _out) const {
    static std::stack<uint> _Dejcstra = std::stack<uint>{};
    return _Dejcstra;
}

Graph& UDWGraph::operator+(const Graph &_Right)
{
    return *this;
}

Graph& UDWGraph::operator+(std::stack<uint>& _Right){
    return *this;
}

Graph& UDWGraph::operator-(uint _Vertex){
    return *this;
}

Graph &UDWGraph::operator-(const Graph &_Right)
{
    return *this;
}

int UDWGraph::operator()(uint _Vertex) const
{
    return this->getDegree(_Vertex);
}
