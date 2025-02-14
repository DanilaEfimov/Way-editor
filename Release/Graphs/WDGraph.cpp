#include "WDGraph.h"
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

static byte resetBit(uint pos, byte& value){
    byte res = 0b00000001;
    pos %= 8;
    res <<= pos;    // res *= 2^pos
    res = ~res;     // 0b11110111 e.g.
    value &= res;
    return res;
}

static byte getBit(uint pos, byte value) {
    byte mask = 0b00000001;
    pos %= 8;
    mask <<= pos;	// res *= 2^pos
    byte res = value & mask ? 0b00000001 : 0b00000000;
    return res;
}

static void copyBits(size_t size1, size_t size2, word* arr1, word* arr2){
    size_t min = size1 < size2 ? size1 : size2;
    for(size_t i = 0; i < min; i++){ arr2[i] = arr1[i]; }
}

static bool** toMatrix(ushort V, byte* cv, uint size = 0){
    size = size ? size : V; // in operator+(std::stack<uint>) we need make mat one row more
    bool** mat = new bool*[size];
    for(size_t i = 0; i < size; i++){
        mat[i] = new bool[size]{0};
    }
    for(size_t i = 0; i < V; i++){
        mat[i][i] = false;
        uint skippedBits = (i + 1) * (i + 2) / 2;
        for(size_t j = i+1; j < V; j++){
            uint matBits = i * V + j;
            uint bit = matBits - skippedBits;
            uint byte = bit / 8;
            bool connected = getBit(bit, cv[byte]);
            mat[i][j] = mat[j][i] = connected;
        }
    }
    for(size_t i = 0; i < size; i++){
        mat[i][size-1] = mat[size-1][i] = false;
    }
    return mat;
}

static void setDefaultWeight(word* weights, uint _V, double weight){
    for(size_t i = 0; i < _V; i++){
        weights[i] = weight;
    }
}

WDGraph::WDGraph(uint V, byte** mat, double weight) : DirGraph(V, mat) {
    if(this->V == 0 || this->downConnectivityMat == nullptr || this->upConnectivityMat == nullptr){
        this->vWeights = nullptr;
        return;
    }
    this->vWeights = new word[this->V];
    setDefaultWeight(this->vWeights, this->V, weight);
    this->eWeights = new word[this->V*(this->V-1)];
    this->setNormalEWeights();
}

WDGraph::~WDGraph() {
    delete[] this->eWeights;
    delete[] this->vWeights;
}

void WDGraph::print(std::fstream &_to) const {
    this->DirGraph::print(_to);
    _to << "weights:\n";
    _to << "edges (from-to):\n";
    for(size_t i = 1; i <= this->V; i++){
        size_t skipped = i*(i+1)/2;
        for(size_t j = i + 1; j <= this->V; j++){
            size_t sequence = (i-1)*this->V + (j - 1) - skipped;
            _to << this->eWeights[sequence]
                << " [" << i << ", " << j << "], ";
            if(sequence % OUT_SEED == 0){_to << std::endl;}
        }
    }
    _to << std::endl;
    _to << "edges (to-from):\n";
    size_t shift = this->V*(this->V-1)/2;
    for(size_t i = 1; i <= this->V; i++){
        size_t skipped = i*(i+1)/2;
        for(size_t j = i + 1; j <= this->V; j++){
            size_t sequence = (i-1)*this->V + (j - 1) - skipped;
            _to << this->eWeights[sequence+shift]
                << " [" << j << ", " << i << "], ";
            if(sequence % OUT_SEED == 0){_to << std::endl;}
        }
    }
    _to << std::endl;
    _to << "vertexes:\n";
    for(size_t i = 0; i < this->V; i++){
        _to << this->vWeights[i] << '[' << i+1 << "]; ";
        if((i + 1) % OUT_SEED == 0){_to << std::endl;}
    }
    _to << std::endl;
}

int WDGraph::getType() const {
    return wdgraph;
}

void WDGraph::setNormalVWeights() {
    for(size_t i = 0; i < this->V; i++){
        this->vWeights[i] = static_cast<double>(1.0);
    }
    // like this->setRandomWeights(0);
    // look at setRandomWeights definition
}

void WDGraph::setMedianVWeights() {
    if(this->V == 0){
        return;
    }
    uint* degrees = new uint[this->V];
    uint* degreeSum = new uint[this->V];
    for(size_t i = 0; i < this->V; i++){
        degrees[i] = this->getDegree(i+1, IN) + this->getDegree(i+1, OUT);
        degreeSum[i] = 0;
    }
    for(size_t i = 0; i < this->V; i++){
        for(size_t j = 0; j < this->V; j++){
            if(this->isConnected(i+1, j+1)){
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

void WDGraph::setNormalEWeights() {
    size_t shift = this->V*(this->V-1)/2;
    for(size_t i = 1; i <= this->V; i++){
        size_t skipped = i*(i+1)/2;
        for(size_t j = i+1; j <= this->V; j++) {
            double w = this->isConnected(i, j) ? 1.0 : 0.0;
            size_t sequence = (i-1)*this->V+(j-1) - skipped;
            this->eWeights[sequence] = w;
            w = this->isConnected(j, i) ? 1.0 : 0.0;
            this->eWeights[sequence + shift] = w;
        }
    }
    // 0 means undefined edge
}

void WDGraph::setMedianEWeights() {
    double weight = 0.0;
    uint* degrees = new uint[this->V];
    for(size_t i = 0; i < this->V; i++){
        degrees[i] = this->getDegree(i+1, OUT) + this->getDegree(i+1, IN);
    };
    for(size_t i = 1; i <= this->V; i++){
        for(size_t j = i+1; j <= this->V; j++){
            uint compliment = this->V - i;
            uint skipped = i*(i+1)/2;
            uint base = this->V*i - skipped - compliment;
            uint offset = j - i - 1;
            uint adres = base + offset;
            uint byte = adres / 8;
            uint bit = adres % 8;
            bool conected = getBit(bit, this->upConnectivityMat[byte])
                        || getBit(bit, this->downConnectivityMat[byte]);
            if(conected){
                size_t shift = this->V*(this->V-1)/2;
                size_t skipped = i*(i+1)/2;
                size_t sequence = (i-1)*this->V + (j - 1) - skipped;
                weight = static_cast<double>((degrees[i-1] + degrees[j-1]) / 2.0);
                this->eWeights[sequence] = weight;
                this->eWeights[sequence + shift] = weight;
            }
            // here is i-1 and j-1 shift with one more '-1'
            // because shifts due to array indexation, and last
            // '-1' due to j initialization j = i+1
        }
    }
}

double WDGraph::getWeightV(uint _Vertex) const {
    if(_Vertex > this->V || _Vertex == 0){
        return -INFINITY;
    }
    if(this->vWeights == nullptr) { return -INFINITY; }
    return this->vWeights[_Vertex-1];
}

double WDGraph::getWeightE(uint _in, uint _out) const {
    if(_in > this->V || _out > this->V ||
        _in == 0 || _out == 0 || _in == _out){
        return -INFINITY;
    }
    if(this->eWeights == nullptr){ return -INFINITY; }
    if(!this->isConnected(_in, _out)) { return 0; }
    if(_out > _in){ uint temp = _in; _in = _out; _out = temp; }
    size_t skipped = _out*(_out+1)/2;
    size_t sequence = (_out-1)*this->V+(_in-1)-skipped;
    double w = this->eWeights[sequence];
    return w;
}

void WDGraph::setVWeight(uint _Vertex, double _value)
{

}

void WDGraph::setEWeight(uint _in, uint _out, double _value) {
    if(_in > this->V || _out > this->V ||
        _in == 0 || _out == 0 || _in == _out){
        return;
    }
    if(this->eWeights == nullptr) { return; }
    bool io = _in > _out;
    if(io) {
        size_t temp = _in; _in = _out; _out = temp;
    }
    size_t skipped = _in*(_in+1)/2;
    size_t shift = this->V*(this->V)/2;
    size_t sequence = (_in-1) * this->V + _out - 1 - skipped ;
    this->eWeights[sequence + (io ? shift : 0)] = _value;
}

void WDGraph::setRandomWeights(uint _seedV, uint _seedE, double _begin, double _end) {
    if(_begin > _end){
        return;
    }
    double median = (_end + _begin)/2.0f;
    double delta = (_end - _begin)/2.0f;
    size_t shift = this->V*(this->V-1)/2;
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
            this->eWeights[i*this->V + j + shift] = weight;
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

double WDGraph::getMaxEweight() const {
    double maxW = -INFINITY;
    for(size_t i = 0; i < this->V*(this->V-1); i++){
        if(this->eWeights[i] > maxW){
            maxW = this->eWeights[i];
        }
    }
    return maxW;
}

double WDGraph::getMaxVWeight() const {
    double maxW = -INFINITY;
    for(size_t i = 0; i < this->V; i++){
        if(this->vWeights[i] > maxW){
            maxW = this->vWeights[i];
        }
    }
    return maxW;
}

void WDGraph::setEdge(uint _in, uint _out) {
    this->DirGraph::setEdge(_in, _out);
    this->eWeights[(_out-1) * this->V + (_in-1) - 1] = static_cast<double>(1.0);
}

void WDGraph::eraseEdge(uint _in, uint _out) {
    if(_in > this->V || _out > this->V ||
        _in == 0 || _out == 0 || _in == _out){
        return;
    }
    if(this->isConnected(_in, _out)){
        size_t sequence = this->V*(_out-1) + (_in - 1);
        this->eWeights[sequence] = 0.0;
    }
    this->DirGraph::eraseEdge(_in, _out);
}

std::stack<uint> &WDGraph::Dejcstra(uint _in, uint _out) const {
    static std::stack<uint> _Dejcstra = std::stack<uint>{};
    return _Dejcstra;
}

WDGraph &WDGraph::operator-(uint _Vertex) {
    if(_Vertex > this->V || _Vertex == 0 || this->V == 0){
        return *this;
    }
    // update this->vWeights
    size_t newSizeV = this->V - 1 ? this->V : 1;
    word* newVW = new word[newSizeV];
    size_t compliment = this->V - _Vertex;
    for(size_t i = 0; i < compliment; i++){
        newVW[i + _Vertex - 1] = newVW[i + _Vertex];
    }
    copyBits(this->V, this->V-1, this->vWeights, newVW);
    delete[] this->vWeights;
    this->vWeights = newVW;
    // update this->eWeights
    size_t newSizeE = (this->V-1)*(this->V-2)/2 ? (this->V-1)*(this->V-2)/2 : 1;
    word* newEW = new word[(newSizeE + 7) / 8];
    uint replaced = 0;
    for(size_t i = 0; i < this->V; i++){
        if(i + 1 == _Vertex){continue;}
        for(size_t j = i+1; j < this->V; j++){
            if(j+1 == _Vertex){continue;}
            newEW[replaced] = this->eWeights[this->V*i+j - 1] - 1;
            replaced++;
        }
    }
    this->DirGraph::operator-(_Vertex);
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

WDGraph &WDGraph::operator+(std::stack<uint> &_Right) {
    if(_Right.empty()){return *this;}
    this->DirGraph::operator+(_Right);
    size_t lastSizeE = this->V*(this->V-1)/2 ? this->V*(this->V-1)/2 : 1;
    size_t newSizeE = this->V*(this->V+1)/2 ? this->V*(this->V+1)/2 : 1;
    word* newEW = new word[(newSizeE+7)/8]{0.0};
    copyBits(newSizeE, lastSizeE, this->eWeights, newEW);
    delete[] this->eWeights;
    this->eWeights = newEW;
    return *this;
}
