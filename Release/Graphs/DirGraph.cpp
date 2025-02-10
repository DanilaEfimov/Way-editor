#include "DirGraph.h"
#include "General.h"
#include "Parser.h"
#include "Error.h"
#include <bitset>

static byte setBit(uint pos = 0) {
    byte res = 0b00000001;
    pos %= 8;
    res <<= pos;	// res *= 2^pos
    return res;
}

static byte setBit(uint pos, byte& value) {
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

static byte resetBit(uint pos, byte& value){
    byte res = 0b00000001;
    pos %= 8;
    res <<= pos;	// res *= 2^pos
    res = ~res;
    value &= res;
    return value;
}

static void copyBits(size_t size, size_t newSize, byte* from, byte* to){    // size in bytes
    size_t min = size < newSize ? size : newSize;
    for(size_t i = 0; i < min; i++){
        to[i] = from[i];
    }
}

static bool** toMatrix(ushort V, byte* down, byte* up, uint size = 0){
    static bool** mat;
    size = size ? size : V; // in operator+(std::stack<uint>) we need make mat one row more
    mat = new bool*[size];
    for(size_t i = 0; i < size; i++){
        mat[i] = new bool[size]{0};
    }
    for(size_t i = 0; i < V; i++) {
        mat[i][i] = false;
        uint skippedBits = (i + 1) * (i + 2) / 2;
        for(size_t j = i + 1; j < V; j++){
            uint matBits = i * V + j;
            uint bit = matBits - skippedBits;
            uint byte = bit / 8;
            bool connected =  getBit(bit,  up[byte]);
            mat[i][j]  = connected;
        }
    }
    for(size_t i = 0; i < V; i++) {
        mat[i][i] = false;
        uint skippedBits = (i + 1) * (i + 2) / 2;
        for(size_t j = i + 1; j < V; j++){
            uint matBits = i * V + j;
            uint bit = matBits - skippedBits;
            uint byte = bit / 8;
            bool connected =  getBit(bit,  down[byte]);
            mat[j][i]  = connected;
        }
    }
    return mat;
}

void DirGraph::clean() noexcept {
    this->V = 0;
    this->E = 0;
    delete[] this->upConnectivityMat;
    delete[] this->downConnectivityMat;
    this->upConnectivityMat = nullptr;
    this->downConnectivityMat = nullptr;
}

void DirGraph::resizeUp(size_t newSize,  size_t oldSize) noexcept {
    static byte* newUpConnectivityMat;
    newUpConnectivityMat = new byte[newSize];
    copyBits(oldSize, newSize, this->upConnectivityMat, newUpConnectivityMat);
    delete[] this->upConnectivityMat;
    this->upConnectivityMat = new byte[newSize];
    copyBits(newSize, newSize, newUpConnectivityMat, this->upConnectivityMat);
    delete[] newUpConnectivityMat;
}

DirGraph::DirGraph(uint _V, byte **mat) : Graph(_V) {
    this->E = 0;
    if(mat == nullptr || _V == 0){                 // empty graph
        this->V = 0;
        this->upConnectivityMat = nullptr;
        this->downConnectivityMat = nullptr;
        return;
    }
    size_t size = (_V*(_V-1)/2 + 7)/8;
    this->upConnectivityMat = new byte[size ? size : 1]{0};         // half of connectivity matrix upper diagonal
    this->downConnectivityMat = new byte[size ? size : 1]{0};       // half of connectivity matrix lower diagonal
    for(size_t i = 0; i < _V; i++){                                     // upper main diagonal
        for(size_t j = i + 1; j < _V; j++){                             // here j < i
            uint offset = (i+1)*(i+2)/2;
            uint matBits = i*_V + j;
            uint bit = matBits - offset;
            uint byte = bit / 8;
            if(!mat[i][j])
                this->upConnectivityMat[byte] &= ~setBit(bit);
            else
                this->upConnectivityMat[byte] |= setBit(bit);
            this->E++;
        }
    }
    for(size_t j = 0; j < _V; j++){                                     // lower main diagonal
        for(size_t i = j + 1; i < _V; i++){                             // here j > i
            uint offset = (j+1)*(j+2)/2;
            uint matBits = j*_V + i;
            uint bit = matBits - offset;
            uint byte = bit / 8;
            if(!mat[i][j])
                this->downConnectivityMat[byte] &= ~setBit(bit);
            else
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

std::string& DirGraph::show() const {
    static std::string conectList;    // connectivity lists
    conectList = Parser::sType(this->getType()) + "\n";
    for(size_t i = 1; i <= this->V; i++){
        conectList += std::to_string(i);
        conectList += ": ";
        for(size_t j = 1; j <= this->V; j++){
            if(this->isConnected(i, j)){
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

int DirGraph::getDegree(uint _Vertex, bool io) const {          // io: out - false; in - true;
    if (_Vertex > this->V || _Vertex == 0) {					// undefined argument branch
        return -1;
    }
    byte* vec;
    if(io == OUT){vec = this->upConnectivityMat;}
    else {vec = this->downConnectivityMat;}
    uint res = 0;
    size_t base = _Vertex - 2;
    size_t offset = this->V-2;
    size_t address = base;
    if(_Vertex > 1){    // colum bypass condition
        for(size_t i = 0; (i < _Vertex - 1) && offset; i++){    // collum of matrix bypass
            size_t byte = address / 8;
            size_t bit = address % 8;
            res += getBit(bit, vec[byte]);
            address += offset;
            offset--;
        }
    }
    size_t compliment = this->V - _Vertex;
    size_t skipped = _Vertex*(_Vertex + 1) / 2;
    base = this->V*_Vertex - skipped - compliment;
    //  i < compliment  = row bypass condition
    for(size_t i = 0; i < compliment; i++){     // row of matrix bypass
        address = base + i;
        size_t byte = address / 8;
        size_t bit = address % 8;
        res += getBit(bit, vec[byte]);
    }
    return res;
    /*                  ### example ###
    *    _ 1 0 1 0   lets get degree 3 OUT    _ 1 * 1 0
    *    _ _ 0 1 0   and mark that fields     _ _ * 1 0
    *    _ _ _ 0 1   which we have to         _ _ _ * *
    *    _ _ _ _ 0   bypass                   _ _ _ _ 0
    *
    */
}

int DirGraph::getType() const {
    return dirgraph;
}

bool DirGraph::isConnected(uint _out, uint _in) const {                // can't be equals arguments
    bool res = false;
    if (_in > this->V || _out > this->V || _in == _out || _in == 0 || _out == 0) {
        return res;
    }
    bool directionBit = _out < _in;
    if(!directionBit){byte_t temp = _in; _in = _out; _out = temp;}
    uint compliment = this->V - _out;
    uint base = _out * this->V - _out * (_out + 1) / 2 - compliment;	// in bits everywhere!
    uint offset = _in - _out - 1;
    uint address = base + offset;
    uint byte = address / 8;
    uint bit = address % 8;
    res = getBit(bit, directionBit ? this->upConnectivityMat[byte] : this->downConnectivityMat[byte]);
    return res;
    // here we have not difference between _in and _out. We have't swap it for _in < _out (look at UDirGraph)
    // here if _in > _out then we have to search such field of matrix, where i > j
    // it's about this->upConnectivityMat, else i < j -> this->downConnectivityMat
    // look at ternar operator in prelast line
}

uint DirGraph::getEdges() const {
    return this->E;
}

void DirGraph::setEdge(uint _out, uint _in) {                          // can't be equals arguments
    if (_in > this->V || _out > this->V || _in == _out || isConnected(_out, _in)) {
        return;
    }
    bool directionBit = _out < _in;
    if(!directionBit){byte_t temp = _in; _in = _out; _out = temp;}
    uint compliment = this->V - _out;
    uint base = _out * this->V - _out * (_out + 1) / 2 - compliment;	// in bits everywhere!
    uint offset = _in > _out ? _in - _out - 1 : _out - _in - 1;
    uint address = base + offset;
    uint byte = address / 8;
    uint bit = address % 8;
    byte_t pos = 1 << bit;
    if(directionBit){ this->upConnectivityMat[byte] |= pos; }
    else{ this->downConnectivityMat[byte] |= pos; }
    this->E++;
    // here all like in isConnected. Look at DirGraph::isConnected() description
    // only there we turn getBit(...) instead setBit(...)
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
        for(size_t j = 1; j <= minV; j++){
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

DirGraph& DirGraph::operator+(std::stack<uint> &_Right) {
    if(_Right.empty()){ return *this; }
    bool** mat = toMatrix(this->V, this->downConnectivityMat,this->upConnectivityMat, this->V + 1);
    while(!_Right.empty()){
        uint _Vertex = _Right.top();
        if(_Vertex != 0 && (_Vertex <= this->V)){
            mat[this->V][_Vertex-1] = true; // setEdge
            this->E++;
        }
        else if(_Vertex == 0){ break; }
        else {Error(_INVALID_ARGUMENT_);}
        _Right.pop();
    }
    size_t newSize = this->V ? (this->V * (this->V + 1) / 2 + 7) / 8 : 1;
    size_t oldSize = this->V ? (this->V * (this->V - 1) / 2 + 7) / 8 : 1;
    this->resizeUp(newSize, oldSize);
    this->V++;
    delete[] this->downConnectivityMat;
    this->downConnectivityMat = new byte[newSize]{0};
    for (size_t i = 0; i < this->V; i++) {
        uint skippedBits;
        uint matBits;
        for (size_t j = 0; j < this->V; j++) {
            if(i == j){ continue; }
            if(i > j) {
                matBits = j * this->V + i;
                skippedBits = (j + 1) * (j + 2) / 2;
            }
            else {
                matBits = i * this->V + j;
                skippedBits = (i + 1) * (i + 2) / 2;
            }
            uint bit = matBits - skippedBits;
            uint byte = bit / 8;
            if (mat[i][j]) {
                if(i > j) this->downConnectivityMat[byte] |= setBit(bit);
                else this->upConnectivityMat[byte] |= setBit(bit);
            }
            else {
                if(i > j) this->downConnectivityMat[byte] &= ~setBit(bit);
                else this->upConnectivityMat[byte] &= ~setBit(bit);
            }
        }
    }
    for(size_t i = 0; i < this->V; i++){
        delete[] mat[i];
    }
    delete[] mat;
    return *this;
    // arguments in stack - vertexes, which will be accessable from new vertex
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

DirGraph& DirGraph::operator-(uint _Vertex) {   // NOT FIXED
    if (_Vertex > this->V || _Vertex == 0) {
        return *this;
    }
    if(this->V == 1){   // empty graph branch
        this->clean();
        return *this;
    }
    if(_Vertex > this->V || _Vertex == 0 || this->V == 0){return *this;}
    this->E -= this->getDegree(_Vertex, IN) + this->getDegree(_Vertex, OUT);
    uint size = (this->V*(this->V - 1)/2 + 7) / 8; // used bits in connectivity vector
    uint newSize = this->V - 1 ? ((this->V-1)*(this->V - 2)/2 + 7) / 8 : 1;
    uint replaced = 0;
    byte* newUp = new byte[newSize ? newSize : 1]{false};
    byte* newDown = new byte[newSize ? newSize : 1]{false};
    for(size_t i = 0; i < this->V; i++){
        if(i + 1 == _Vertex){continue;}
        for(size_t j = 0; j < this->V; j++){
            if(j+1 == _Vertex){continue;}
            if(i == j){continue;}
            uint mat;
            uint skipped;
            if(i > j){
                mat = j * this->V + i + 1;
                skipped = (j+1)*(j+2)/2;
            }
            else{
                mat = i * this->V + j + 1;
                skipped = (i+1)*(i+2)/2;
            }
            uint sequence = mat - skipped;
            uint byte = sequence / 8;
            uint bit = sequence % 8;
            if(i > j){
                if(getBit(bit, this->downConnectivityMat[byte])){setBit(replaced % 8, newDown[replaced / 8]);}
                else{resetBit(replaced % 8, newDown[replaced / 8]);}
            }
            else {
                if(getBit(bit, this->upConnectivityMat[byte])){setBit(replaced % 8, newUp[replaced / 8]);}
                else{resetBit(replaced % 8, newUp[replaced / 8]);}
            }
            replaced++;
        }
    }
    this->V -= 1;
    delete[] this->upConnectivityMat;
    this->upConnectivityMat = newUp;
    delete[] this->downConnectivityMat;
    this->downConnectivityMat = newDown;
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

int DirGraph::operator()(uint _Vertex) const {
    return this->getDegree(_Vertex, OUT);
    // it hard to choose IN | OUT
    // but I think that usefull is OUT for bypass algorithms
}
