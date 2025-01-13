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
    bool res = 0b00000001;
    pos %= 8;
    res <<= pos;    // res *= 2^pos
    res = ~res;     // 0b11110111 e.g.
    value &= res;
    return res;
}

UPseudoGraph::UPseudoGraph(uint V, byte** mat) : UDirGraph(V, mat) {
    this->loops = new byte[(V+7)/8]{0};
    // by default this graph created without loops
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
        for(size_t j = i; j <= this->V; j++){
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
    if(this->isConnected(_Vertex, _Vertex)){res++;}
    return res;
}

bool UPseudoGraph::isConnected(uint _in, uint _out) const {
    if(_in > this->V || _out > this->V || _in == 0 || _out == 0){
        return false;
    }
    if(_in != _out){
        return this->UDirGraph::isConnected(_in, _out);
    }
    else{
        uint v = _in;
        uint byte = (v - 1) / 8;
        uint bit = (v - 1) % 8;
        return getBit(bit, this->loops[byte]);
    }
    return false;
}

void UPseudoGraph::setEdge(uint _in, uint _out) {
    if(_in == 0 || _out == 0 || _in > this->V || _out > this->V){
        return;
    }
    if(_in == _out){
        if(!getBit(_in-1, this->loops[(_in-1)/8])){
            this->E++;
            setBit(_in-1, this->loops[(_in-1)/8]);
        }
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
        this->UDirGraph::setEdge(_in, _out);
    }
}

UDirGraph& UPseudoGraph::operator-(uint _Vertex) {
    if(_Vertex == 0 || _Vertex > this->V){return *this;}
    byte* lastLoops = this->loops;
    this->loops = new byte[(this->V + 6)/8]{0};
    for(size_t i = 0; i < this->V; i++){
        uint bit;
        uint byte;
        if(i + 1 < _Vertex){
            bit = i % 8;
            byte = i / 8;
        }
        if(i + 1 > _Vertex){
            bit = (i-1) % 8;
            byte = (i-1) / 8;
        }
        if(getBit(bit, lastLoops[byte])){
            setBit(bit, this->loops[byte]);
        }
    }
    this->UDirGraph::operator-(_Vertex);
    return *this;
}
