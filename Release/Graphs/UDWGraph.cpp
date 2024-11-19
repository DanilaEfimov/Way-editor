#include "UDWGraph.h"
#include<cmath>

static byte setBit(uint pos = 0) {
    byte res = 0b00000001;
    pos %= 8;
    res <<= pos;	// res *= 2^pos
    return res;
}

static byte setBit(uint pos, byte& value) {		// for change bit
    byte res = 0b00000001;
    pos %= 8;
    res <<= pos;	// res *= 2^pos
    value |= res;
    return res;
}

static byte getBit(uint pos, byte value) {
    byte mask = 0b00000001;
    pos %= 8;
    mask <<= pos;	// res *= 2^pos
    byte res = value & mask ? 0b00000001 : 0b00000000;
    return res;
}

static void setDefaultWeight(word* weights, uint _V, double weight){
    for(size_t i = 0; i < _V; i++){
        weights[i] = weight;
    }
}

UDWGraph::UDWGraph(uint _V, byte** mat, double weight) : UDirGraph(_V, mat){
    if(this->V == 0 || this->connectivityVector == nullptr){
        this->vWeights = nullptr;
        return;
    }
    this->vWeights = new word[this->V];
    setDefaultWeight(this->vWeights, this->V, weight);
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
    if (_in > this->V || _out > this->V || _in == _out) {
        return;
    }
    if (_in > _out) {													// _in have to be littlest
        uint temp = _out;
        _out = _in;
        _in = temp;
    }
    uint complimentIN = this->V - _in;
    uint baseIN = _in * this->V - _in * (_in + 1) / 2 - complimentIN;	// in bits everywhere!
    uint offset = _out - _in - 1;
    uint address = baseIN + offset;
    uint byte = (address + 7) / 8;
    uint bit = address % 8;
    setBit(bit, this->connectivityVector[byte]);
}

void UDWGraph::setNormalVWeights() {
    for(size_t i = 0; i < this->V; i++){
        this->vWeights[i] = 1.0f;
    }
    // like this->setRandomWeights(0);
    // look at setRandomWeights definition
}

void UDWGraph::setMedianVWeights() {
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
        this->vWeights[i] = degrees[i] ? double(degreeSum[i] / degrees[i]) : 0.0f;
    }
    delete[] degreeSum;
    delete[] degrees;
}

void UDWGraph::setNormalEWeights()
{

}

void UDWGraph::setMedianEWeights()
{

}

void UDWGraph::setVWeight(uint _Vertex, double _value) {
    if(_Vertex == 0 || _Vertex > this->V){
        return;
    }
    this->vWeights[_Vertex] = _value;
}

void UDWGraph::setRandomWeights(uint _seedV, uint _seedE, double _begin, double _end) {
    if(_begin > _end){
        return;
    }
    for(size_t i = 0; i < this->V; i++){
        double weight = (_end + _begin)/2.0f;
        double k = sin(2.0f * atan(_seedV));
        weight += k * (_end - weight)/2.0f;
        this->vWeights[i] = weight;
    }
    /*
    * this random weights performd median between _begin & _end
    * plus some value what turns median between _begin & _end
    * by sinusoid sin(2.0f * atan(_seed))
    * [_seed1 = 0, _seed2 = 0, _begin = 0, _end = 100]
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
