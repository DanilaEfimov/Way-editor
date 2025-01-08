#include "DirGraph.h"
#include "General.h"
#include <bitset>

static byte setBit(uint pos = 0) {
    byte res = 0b00000001;
    pos %= 8;
    res <<= pos;	// res *= 2^pos
    return res;
}

static byte getBit(uint pos, byte value) {
    byte mask = 0b00000001;
    pos %= 8;
    mask <<= pos;	// res *= 2^pos
    byte res = value & mask ? 0b00000001 : 0b00000000;
    return res;
}

static byte resetBit(uint pos, byte& value){
    byte res = 0b00000001;
    pos %= 8;
    res <<= pos;	// res *= 2^pos
    res = ~res;
    value &= res;
    return value;
}

static bool** toMatrix(ushort V, byte* cv, uint size = 0){
    size = size ? size : V; // in operator+(std::stack<uint>) we need make mat one row more
    bool** mat = new bool*[size];
    for(size_t i = 0; i < size; i++){
        mat[i] = new bool[size];
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
    return mat;
}

DirGraph::DirGraph(uint _V, byte **mat) : Graph(_V) {
    this->E = 0;
    if(mat == nullptr){                 // empty graph
        this->V = 0;
        this->upConnectivityMat = nullptr;
        this->downConnectivityMat = nullptr;
        return;
    }
    this->upConnectivityMat = new byte[(_V*(_V-1)/2 + 7)/8]{0};         // half of connectivity matrix upper diagonal
    this->downConnectivityMat = new byte[(_V*(_V-1)/2 + 7)/8]{0};       // half of connectivity matrix lower diagonal
    for(size_t i = 0; i < _V; i++){                                     // upper main diagonal
        for(size_t j = i + 1; j < _V; j++){                             // here j < i
            if(!mat[i][j]){
                continue;
            }
            uint offset = (i+1)*(i+2)/2;
            uint matBits = i*_V + j;
            uint bit = matBits - offset;
            uint byte = bit / 8;
            this->upConnectivityMat[byte] |= setBit(bit);
            this->E++;
        }
    }
    for(size_t j = 0; j < _V; j++){                                     // lower main diagonal
        for(size_t i = j + 1; i < _V; i++){                             // here j > i
            if(!mat[i][j]){                                             // transposed matrix
                continue;
            }
            uint offset = (j+1)*(j+2)/2;
            uint matBits = j*_V + i;
            uint bit = matBits - offset;
            uint byte = bit / 8;
            this->downConnectivityMat[byte] |= setBit(bit);
            this->E++;
        }
    }
}

DirGraph::~DirGraph() {
    delete[] this->upConnectivityMat;
    delete[] this->downConnectivityMat;
}

void DirGraph::print(std::fstream &_to) const {
    if(!_to.is_open()){
        return;
    }
    _to << "\nbit descriptor:\n";
    std::bitset<32> edges(this->E);
    std::bitset<16> vertexes(this->V);
    _to << vertexes << ' ' << edges << ' ';
    uint bytes = ((this->V * this->V + 1) / 2 + 7) / 8;
    uint bits = this->V * (this->V - 1) / 2;
    for (size_t base = 0; base < bytes; base++) {
        for (size_t offset = 0;
             offset < 8 && ((base * 8 + offset) < bits);
             offset++) {
            byte_t value = this->upConnectivityMat[base];
            _to << (int)getBit(offset, value);
        }
    }
    _to << ' ';
    for (size_t base = 0; base < bytes; base++) {
        for (size_t offset = 0;
             offset < 8 && ((base * 8 + offset) < bits);
             offset++) {
            byte_t value = this->downConnectivityMat[base];
            _to << (int)getBit(offset, value);
        }
    }
    uint size = STATIC_MEMORY + ((this->V*(this->V-1)+7)/8)*8;  // in bits everywhere!
    _to << "\n" << size << std::endl;
}

std::string DirGraph::show() const {
    std::string conectList = "\n";    // connectivity lists
    for(size_t i = 1; i <= this->V; i++){
        conectList += std::to_string(i);
        conectList += ": ";
        for(size_t j = 1; j <= this->V; j++){
            if(this->isConnected(i, j) || isConnected(j, i)){
                conectList += std::to_string(j);
                conectList += ", ";
            }
        }
        conectList += "\n";
    }
    conectList += "\n";
    return conectList;
    /*
    *   this function will be rarely used, so
    *   we can don't worry for performance.
    *   it will be called when user updated graph
    */
}

int DirGraph::getDegree(uint _Vertex, bool io) const {          // io: in - false; out - true;
    if(_Vertex > this->V || _Vertex == 0){
        return -1;                                              // invalid argument
    }
    int degree = 0;
    for (size_t v = 1; v < _Vertex; v++) {						// check all vertexes littler than _Vertex
        uint offset = _Vertex - v - 1;							// how many bits we have to shift to find [v][_Vertex] field
        uint compliment = this->V - v;
        uint base = v * this->V - v * (v + 1) / 2 - compliment;	// it's like begin of segment, but for matrix row
        uint address = base + offset;
        uint byte = address / 8;
        byte_t isConnect = getBit(offset, io ? this->upConnectivityMat[byte] : this->downConnectivityMat[byte]);
        degree += isConnect;									// getBit() returns 1 or 0
    }															// here checked all vertexes littler than _Vertex
    // let's check others, what bigger than _Vertex
    uint _compliment = this->V - _Vertex;						// how many bits contains about _Vertex's connectivity
    uint _base = _Vertex * this->V - _Vertex * (_Vertex + 1) / 2 - _compliment;
    for (size_t i = 0; i < _compliment; i++) {
        uint byte = (_base + i) / 8;
        byte_t field = io ? this->upConnectivityMat[byte] : this->downConnectivityMat[byte];
        degree += getBit(i, field);								// look at definition of 'getBit()': there i %= 8 too
    }
    return degree;
}

int DirGraph::getType() const {
    return dirgraph;
}

bool DirGraph::isConnected(uint _in, uint _out) const {                // can't be equals arguments
    bool res = false;
    if (_in > this->V || _out > this->V || _in == _out || _in == 0 || _out == 0) {
        return res;
    }
    bool directionBit = _out > _in;
    if(directionBit){byte_t temp = _in; _in = _out; _out = temp;}
    uint compliment = this->V - _out;
    uint base = _out * this->V - _out * (_out + 1) / 2 - compliment;	// in bits everywhere!
    uint offset = _in - _out - 1;
    uint address = base + offset;
    uint byte = address / 8;
    uint bit = address % 8;
    res = getBit(bit, !directionBit ? this->upConnectivityMat[byte] : this->downConnectivityMat[byte]);
    return res;
    // here we have not difference between _in and _out. We have't swap it for _in < _out (look at UDirGraph)
    // here if _in > _out then we have to search such field of matrix, where i > j
    // it's about this->upConnectivityMat, else i < j -> this->downConnectivityMat
    // look at ternar operator in prelast line
}

uint DirGraph::getEdges() const {
    return this->E;
}

void DirGraph::setEdge(uint _in, uint _out) {                          // can't be equals arguments
    if (_in > this->V || _out > this->V || _in == _out) {
        return;
    }
    bool directionBit = _out > _in;
    if(directionBit){byte_t temp = _in; _in = _out; _out = temp;}
    uint compliment = this->V - _out;
    uint base = _out * this->V - _out * (_out + 1) / 2 - compliment;	// in bits everywhere!
    uint offset = _in > _out ? _in - _out - 1 : _out - _in - 1;
    uint address = base + offset;
    uint byte = address / 8;
    uint bit = address % 8;
    byte_t pos = 1 << bit;
    bool connected = getBit(bit, !directionBit ? this->upConnectivityMat[byte] : this->downConnectivityMat[byte]);
    if(connected){ return; }
    else if(!directionBit){
        this->upConnectivityMat[byte] |= pos;
    }
    else{
        this->downConnectivityMat[byte] |= pos;
    }
    this->E++;
    // here all like in isConnected. Look at DirGraph::isConnected() description
    // only there we return getBit(...) instead setBit(...)
}

void DirGraph::eraseEdge(uint _in, uint _out) {
    if (_in > this->V || _out > this->V || _in == _out || _in == 0 || _out == 0) {
        return;
    }
    bool directionBit = _out > _in;
    if(directionBit){byte_t temp = _in; _in = _out; _out = temp;}
    uint compliment = this->V - _out;
    uint base = _out * this->V - _out * (_out + 1) / 2 - compliment;	// in bits everywhere!
    uint offset = _in - _out - 1;
    uint address = base + offset;
    uint byte = address / 8;
    uint bit = address % 8;
    bool conected = getBit(bit, !directionBit ? this->upConnectivityMat[byte] : this->downConnectivityMat[byte]);
    if(conected){
        byte_t& value = !directionBit ? this->upConnectivityMat[byte] : this->downConnectivityMat[byte];
        value = resetBit(bit, value);
        this->E--;
    }
    // operation of deleting edge can be realized as:
    // value -= 1 << bit;
}

std::stack<uint>& DirGraph::BFS(uint _root) const {
    static std::stack<uint> _BFS = std::stack<uint>{};
    return _BFS;
}

std::stack<uint>& DirGraph::DFS(uint _root) const {
    static std::stack<uint> _DFS = std::stack<uint>{};
    return _DFS;
}

std::stack<uint>& DirGraph::EulerCycle(uint _begin) const {
    static std::stack<uint> _EulerCycle = std::stack<uint>{};
    return _EulerCycle;
}

DirGraph& DirGraph::operator+=(const DirGraph &_Right) {    // not fixed (only same size)
    /*
    * this operator performs adding of graphs
    * it means that same edges will be added at
    *  _Left (this) Graph.
    */
    ushort minV = this->V < _Right.getV() ? this->V : _Right.getV();
    for(size_t i = 1; i <= minV; i++){
        for(size_t j = i+1; j <= minV; j++){
            if(_Right.isConnected(j,i)){
                this->setEdge(j,i);
            }
            if(_Right.isConnected(i,j)){
                this->setEdge(i,j);
            }
        }
    }
    return *this;
}

DirGraph& DirGraph::operator+(std::stack<uint> &_Right)
{
    return *this;
}

DirGraph& DirGraph::operator-=(const DirGraph &_Right) {    // not fixed (only same size)
    /*
    * this operator performs adding of graphs
    * it means that same edges will be added at
    *  _Left (this) Graph.
    */
    ushort minV = this->V < _Right.getV() ? this->V : _Right.getV();
    for(size_t i = 1; i <= minV; i++){
        for(size_t j = i+1; j <= minV; j++){
            if(!_Right.isConnected(j,i)){
                this->eraseEdge(j,i);
            }
            if(!_Right.isConnected(i,j)){
                this->eraseEdge(i,j);
            }
        }
    }
    return *this;
}

DirGraph& DirGraph::operator-(uint _Vertex) {
    if (_Vertex > this->V || _Vertex == 0) {
        return *this;
    }
    if(this->V == 1){
        this->V--;
        this->E = 0;
        delete[] this->upConnectivityMat;
        delete[] this->downConnectivityMat;
        this->upConnectivityMat = nullptr;
        this->downConnectivityMat = nullptr;
        return *this;
        // empty graph branch
    }
    this->E = 0;
    bool** mat = toMatrix(this->V, this->upConnectivityMat);
    this->upConnectivityMat = new byte[((this->V - 1) * (this->V - 2) / 2 + 7) / 8]{false};
    this->downConnectivityMat = new byte[((this->V - 1) * (this->V - 2) / 2 + 7) / 8]{false};
    uint compliment = this->V - _Vertex;
    for(size_t i = 0; i < this->V; i++){
        if(i + 1 == _Vertex){ continue; }
        for(size_t j = i+1; j < this->V; j++){
            if(j + 1 == _Vertex){ continue; }
            uint matBits = i*this->V + j;
            uint skipped = (i+1)*(i+2)/2;
            uint bit = matBits - skipped;
            if(i+1 >= _Vertex && j+1 >= _Vertex){bit-=this->V-1;}
            else if(j+1 >= _Vertex) {bit -= i+1;}
            else if(i+1 >= _Vertex) {bit -= compliment;}
            uint byte = bit / 8;
            if(mat[i][j]){
                this->upConnectivityMat[byte] |= setBit(bit);
                this->E++;
            }
            if(mat[j][i]){
                this->downConnectivityMat[byte] |= setBit(bit);
                this->E++;
            }
        }
    }
    for(size_t i = 0; i < this->V; i++){
        delete[] mat[i];
    }
    delete[] mat;
    this->V--;
    return *this;
}

int DirGraph::operator()(uint _Vertex) const {
    return this->getDegree(_Vertex, OUT);
    // it hard to choose IN | OUT
    // but I think that usefull is OUT for bypass algorithms
}
