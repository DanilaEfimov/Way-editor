#include "UPseudoGraph.h"
#include "General.h"
#include "Parser.h"
#include <bitset>

static byte setBit(uint pos = 0) {
    byte res = 0b00000001;
    pos %= 8;
    res <<= pos;	// res *= 2^pos
    return res;
}

static byte setBit(uint pos, byte& value) {
    byte mask = 0b00000001;
    pos %= 8;
    mask <<= pos;	// res *= 2^pos
    value |= mask;
    return value;
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
    res <<= pos;    // res *= 2^pos
    res = ~res;     // 0b11110111 e.g.
    value &= res;
    return res;
}

static void copyBits(size_t size, size_t newSize, byte* from, byte* to){    // size in bytes
    size_t min = size < newSize ? size : newSize;
    for(size_t i = 0; i < min; i++){
        to[i] = from[i];
    }
}

UPseudoGraph::UPseudoGraph(uint V, byte** mat) : UDirGraph(V, mat) {
    this->loops = new byte[(V+7)/8]{0};
    for(size_t i = 0; i < V; i++){
        size_t byte = i / 8;
        size_t bit = i % 8;
        if(mat[i][i]) {
            setBit(bit, this->loops[byte]);
        }
    }
}

UPseudoGraph::~UPseudoGraph() {
    delete[] this->loops;
}

int UPseudoGraph::getType() const {
    return upseudograph;
}

void UPseudoGraph::print(std::fstream &_to) const {
    if (!_to.is_open()) {
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
            byte_t value = this->connectivityVector[base];
            _to << (int)getBit(offset, value);
        }
    }
    for(size_t i = 0; i < this->V; i++){
        uint byte = i / 8;
        uint bit = i % 8;
        _to << getBit(bit, this->loops[byte]);
        // once difference between UDirGraph::print is this
        // in the end prints V bits from main diagonal
        // connectivity matrix
    }
    uint size = STATIC_MEMORY + ((this->V*(this->V-1)/2+7)/8)*8;// in bits everywhere!
    _to << "\n" << size << std::endl;
}

std::string& UPseudoGraph::show() const {
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
    // once difference between UDirGraph::show is
    // init at inside loop: size_t j = i; (was i+1)
}

int UPseudoGraph::getDegree(uint _Vertex) const {
    if(_Vertex == 0 || _Vertex > this->V){
        return -1;
    }
    int res = this->UDirGraph::getDegree(_Vertex);
    return res;
}

bool UPseudoGraph::isConnected(uint _in, uint _out) const {
    if(_in > this->V || _out > this->V || _in*_out == 0){
        return false;
    }
    if(_in != _out){
        return this->UDirGraph::isConnected(_in, _out);
    }
    else{
        uint byte = (_in - 1) / 8;
        uint bit = (_in - 1) % 8;
        return getBit(bit, this->loops[byte]);
    }
    return false;
}

std::stack<uint> &UPseudoGraph::getLoops() const {
    static std::stack<uint> allLoops;
    allLoops = std::stack<uint>{};
    for(size_t i = 0; i < this->V; i++){
        if(this->isConnected(i+1, i+1)){
            allLoops.push(i+1);
        }
    }
    return allLoops;
}

void UPseudoGraph::setEdge(uint _in, uint _out) {
    if(_in == 0 || _out == 0 || _in > this->V || _out > this->V){
        return;
    }
    if(_in == _out){
        this->E++;
        setBit(_in-1, this->loops[(_in-1)/8]);
    }
    else{
        this->UDirGraph::setEdge(_in, _out);
    }
}

void UPseudoGraph::eraseEdge(uint _in, uint _out) {
    if(_in == 0 || _out == 0 || _in > this->V || _out > this->V){
        return;
    }
    if(_in == _out){
        if(getBit(_in-1, this->loops[(_in-1)/8])){
            this->E--;
            resetBit(_in-1, this->loops[(_in-1)/8]);
        }
    }
    else{
        this->UDirGraph::eraseEdge(_in, _out);
    }
}

UPseudoGraph &UPseudoGraph::operator+(std::stack<uint> &_Right) {
    size_t lastSize = (this->V + 7)/8;
    size_t newSize = (this->V + 8)/8;
    if(newSize > lastSize){
        byte* newLoops = new byte[newSize]{0};
        copyBits(lastSize, newSize, this->loops, newLoops);
        delete[] this->loops;
        this->loops = new byte[newSize]{0};
        copyBits(lastSize, newSize, newLoops, this->loops);
        delete[] newLoops;
    }
    this->UDirGraph::operator+(_Right);
    return *this;
}

UPseudoGraph& UPseudoGraph::operator-(uint _Vertex) {
    if(_Vertex == 0 || _Vertex > this->V){return *this;}
    size_t newSize = this->V ? (this->V + 7)/8 : 1;
    size_t oldSize = this->V > 1 ? (this->V + 6)/8 : 1;
    byte* lastLoops = this->loops;
    if(oldSize < newSize){
        lastLoops = new byte[newSize];
        copyBits(oldSize, newSize, this->loops, lastLoops);
        delete[] this->loops;
        this->loops = new byte[newSize]{0};
    }
    size_t bit;
    size_t byte;
    size_t skip = 0;
    for(size_t i = 0; i < this->V; i++){
        bit = i % 8;
        byte = (i+skip) / 8;
        if(i + 1 == _Vertex) {skip = 1; continue;}
        if(getBit(bit, lastLoops[byte])){
            setBit(bit - skip, this->loops[byte]);
        }
        else {
            resetBit(bit - skip, this->loops[byte]);
        }
    }
    bit = (_Vertex - 1) % 8;    // if deleted vertex has loop..
    byte = (_Vertex - 1) / 8;
    resetBit(bit, this->loops[byte]);
    if(oldSize < newSize) { delete[] lastLoops; }
    this->UDirGraph::operator-(_Vertex);
    return *this;
}
