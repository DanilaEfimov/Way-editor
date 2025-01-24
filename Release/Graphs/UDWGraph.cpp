#include "UDWGraph.h"
#include "General.h"
#include <cmath>

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
    this->eWeights = new word[this->V*(this->V-1)/2];
    this->setNormalEWeights();
}

UDWGraph::~UDWGraph() {
    delete[] this->eWeights;
    delete[] this->vWeights;
}

void UDWGraph::print(std::fstream &_to) const { // NOT FIXED!!!
    this->UDirGraph::print(_to);
    _to << "weights:\n";
    _to << "edges:\n";
    for(size_t i = 0; i < this->V*(this->V-1)/2; i++){
        _to << this->eWeights[i] << '[' << (i+1)/this->V + 1<< ", " << (i+1)/this->V+(i+1)%this->V + 1 << "]; ";
        if((i + 1) % OUT_SEED == 0){_to << std::endl;}
    }
    if(this->V*(this->V+1)/2 % OUT_SEED != 0){_to << std::endl;}
    _to << "vertexes:\n";
    for(size_t i = 0; i < this->V; i++){
        _to << this->vWeights[i] << '[' << i+1 << "]; ";
        if((i + 1) % OUT_SEED == 0){_to << std::endl;}
    }
    if(this->V % OUT_SEED != 0){_to << std::endl;}
}

void UDWGraph::setEdge(uint _in, uint _out) {
    this->UDirGraph::setEdge(_in, _out);
    this->eWeights[(_out-1) * this->V + (_in-1) - 1] = static_cast<double>(1.0);
}

int UDWGraph::getType() const {
    return udwgraph;
}

void UDWGraph::setNormalVWeights() {
    for(size_t i = 0; i < this->V; i++){
        this->vWeights[i] = static_cast<double>(1.0);
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
                degreeSum[j] += degrees[i];
            }
        }
    }
    for(size_t i = 0; i < this->V; i++){
        this->vWeights[i] = degrees[i] ? static_cast<double>(degreeSum[i] / degrees[i]) : 0.0f;
    }
    delete[] degreeSum;
    delete[] degrees;
}

void UDWGraph::setNormalEWeights() {
    for(size_t i = 0; i < this->V*(this->V-1)/2; i++){
        this->eWeights[i] = this->isConnected((i+1)/this->V, (i+1)%this->V)
                                ? static_cast<double>(1.0)
                                : static_cast<double>(0.0);
    }
    // 0 means undefined edge
}

void UDWGraph::setMedianEWeights() {
    double weight = 0.0;
    uint* degrees = new uint[this->V];
    for(size_t i = 1; i <= this->V; i++){degrees[i-1] = this->getDegree(i);};
    for(size_t i = 1; i <= this->V; i++){
        for(size_t j = i+1; j <= this->V; j++){
            uint compliment = this->V - i;
            uint skipped = i*(i+1)/2;
            uint base = this->V*i - skipped - compliment;
            uint offset = j - i - 1;
            uint adres = base + offset;
            uint byte = adres / 8;
            uint bit = adres % 8;
            bool conected = getBit(bit, this->connectivityVector[byte]);
            if(conected){
                weight = static_cast<double>((degrees[i-1] + degrees[j-1]) / 2.0);
                this->eWeights[(i-1)*this->V + (j - 1) - 1] = weight;
            }
            // here is i-1 and j-1 shift with one more '-1'
            // because shifts due to array indexation, and last
            // '-1' due to j initialization j = i+1
        }
    }
}

double UDWGraph::getWeightV(uint _Vertex) const {
    if(_Vertex > this->V || _Vertex == 0){
        return -INFINITY;
    }
    if(this->vWeights == nullptr) { return -INFINITY; }
    return this->vWeights[_Vertex-1];
}

double UDWGraph::getWeightE(uint _in, uint _out) const {
    if(_in > this->V || _out > this->V ||
        _in == 0 || _out == 0 || _in == _out){
        return -INFINITY;
    }
    if(_out > _in){ uint temp = _in; _in = _out; _out = temp; }
    if(this->eWeights == nullptr){ return -INFINITY; }
    return this->eWeights[(_in-1)*this->V + (_out - 1)];
}

void UDWGraph::setVWeight(uint _Vertex, double _value) {
    if(_Vertex == 0 || _Vertex > this->V){
        return;
    }
    if(this->vWeights == nullptr) { return; }
    this->vWeights[_Vertex] = _value;
}

void UDWGraph::setEWeight(uint _in, uint _out, double _value) {
    if(_in > this->V || _out > this->V ||
        _in == 0 || _out == 0 || _in == _out){
        return;
    }
    if(this->eWeights == nullptr) { return; }
    this->eWeights[this->V*(_out-1) + (_in - 1)] = _value;
}

void UDWGraph::setRandomWeights(uint _seedV, uint _seedE, double _begin, double _end) {
    if(_begin > _end){
        return;
    }
    double median = (_end + _begin)/2.0f;
    double delta = (_end - _begin)/2.0f;
    for(size_t i = 0; i < this->V; i++){
        double weight = median;
        double k = sin(2.0f * atan(_seedV));
        weight += k * delta;
        this->vWeights[i] = weight;
        k = sin(2.0f * atan(_seedE));
        weight = median;
        weight += k * delta;
        for(size_t j = 0; j < this->V - i - 1; j++){
            this->eWeights[i*this->V + j] = weight;
        }
    }
    /*
    * this random weights performd median between _begin & _end
    * plus some value what turns median between _begin & _end
    * by sinusoid sin(2.0f * atan(_seed))
    * [_seed1 = 0, _seed2 = 0, _begin = 0, _end = 100]
    * setRandomWeights(0,0,0,1) non equals to setNorma@E/V@lWeights()
    */
}

double UDWGraph::getMaxEweight() const {
    double maxW = -INFINITY;
    for(size_t i = 0; i < this->V*(this->V-1)/2; i++){
        if(this->eWeights[i] > maxW){
            maxW = this->eWeights[i];
        }
    }
    return maxW;
}

double UDWGraph::getMaxVWeight() const {
    double maxW = -INFINITY;
    for(size_t i = 0; i < this->V; i++){
        if(this->vWeights[i] > maxW){
            maxW = this->vWeights[i];
        }
    }
    return maxW;
}

void UDWGraph::eraseEdge(uint _in, uint _out) {
    if(_in > this->V || _out > this->V ||
        _in == 0 || _out == 0 || _in == _out){
        return;
    }
    if(this->isConnected(_in, _out)){
        this->eWeights[this->V*(_out-1) + (_in - 1)] = 0.0;
    }
    this->UDirGraph::eraseEdge(_in, _out);
}

std::stack<uint>& UDWGraph::Dejcstra(uint _in, uint _out) const {
    static std::stack<uint> _Dejcstra = std::stack<uint>{};
    return _Dejcstra;
}

UDirGraph& UDWGraph::operator-(uint _Vertex) {
    if(_Vertex > this->V || _Vertex == 0){
        return *this;
    }
    this->UDirGraph::operator-(_Vertex);
    /*here will be deleting and resizing weights*/
    double* lastEWeights = this->eWeights;
    this->eWeights = new double[(this->V-1)*(this->V - 2)/2];
    uint compliment = this->V - _Vertex;
    for(size_t i = 0; i < this->V; i++){
        if(i + 1 == _Vertex){continue;}
        uint skipped = (i + 1) * (i + 2) / 2;
        if(i + 1 >= _Vertex){skipped += compliment;}
        for(size_t j = i + 1; j < this->V - i; j++){
            if(j + 1 == _Vertex){continue;}
            if(j + 1 >= _Vertex){skipped += i + 1;}
            uint matBits = i * this->V + j;
            uint base = matBits - skipped;

        }
    }
    return *this;
    /*
    *   for example G - 2: (deleting of second vertex)
    *   matrix of some UDirGraph:
    *   0 1 0 0 1   or  * 1 0 0 1
    *   1 0 0 1 0       * * 0 1 0
    *   0 0 0 0 0       * * * 0 0
    *   0 1 0 0 1       * * * * 1
    *   1 0 0 1 0       * * * * *
    *   how must looks returned graph mat:
    *   * * 0 0 1   there isn't all i = 1 || j = 1 places
    *   * * * * *   (vertexes id: 0, 1, 2 ... second have id = 1)
    *   * * * 0 0
    *   * * * * 1
    *   * * * * *
    *   so every vertex in mat deals with this->V - 1 places
    *   it's used in conditions after 'bit' calculations
    */
}

int UDWGraph::operator()(uint _Vertex) const {
    return this->getDegree(_Vertex);
}
