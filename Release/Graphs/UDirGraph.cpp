#include "UDirGraph.h"
#include "General.h"
#include "Parser.h"
#include "Error.h"
#include <bitset>
#include <queue>

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
    bool res = 0b00000001;
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

UDirGraph::UDirGraph(uint _V, byte** mat) : Graph(_V){
	this->E = 0;
    if (mat == nullptr) {
		this->V = 0;
        this->connectivityVector = nullptr;     // empty graph branch
        return;
	}
	this->connectivityVector = new byte[(_V * (_V - 1) / 2 + 7) / 8] {false};
												// _V * (_V - 1) / 2 = B is count of necessary bits of matrix, half without diagonal
												// (B + 7) / 8	is count of new bytes for necessary bits
	for (size_t i = 0; i < _V; i++) {
		uint skippedBits = (i + 1) * (i + 2) / 2;
		for (size_t j = i + 1; j < _V; j++) {	// i + 1 is shift for skipping unnecessary fields
			uint matBits = i * _V + j;
			uint bit = matBits - skippedBits;
			uint byte = bit / 8;
			if (mat[i][j]) {
				this->connectivityVector[byte] |= setBit(bit);
				this->E++;
			}
		}
	}
}

UDirGraph::~UDirGraph() {
    if(this->connectivityVector != nullptr){
        delete[] this->connectivityVector;
    }
}

void UDirGraph::print(std::fstream& _to) const {
	if (!_to.is_open()) {
		return;
	}
    _to << Parser::sType(this->getType());
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
    uint size = STATIC_MEMORY + ((this->V*(this->V-1)/2+7)/8)*8;// in bits everywhere!
    _to << "\n" << size << std::endl;
}

std::string& UDirGraph::show() const {
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
    *   this function will be rarely used, so we can
    *   don't worry for performance. O(V^2)
    */
}

int UDirGraph::getDegree(uint _Vertex) const {
	if (_Vertex > this->V || _Vertex == 0) {					// undefined argument branch
		return -1;
	}
	uint res = 0;
	for (size_t v = 1; v < _Vertex; v++) {						// check all vertexes littler than _Vertex
		uint offset = _Vertex - v - 1;							// how many bits we have to shift to find [v][_Vertex] field
		uint compliment = this->V - v;
		uint base = v * this->V - v * (v + 1) / 2 - compliment;	// it's like begin of segment, but for matrix row
		uint address = base + offset;
		uint byte = address / 8;
		byte_t isConnect = getBit(offset, this->connectivityVector[byte]);
		res += isConnect;										// getBit() returns 1 or 0
	}															// here checked all vertexes littler than _Vertex
																// let's check others, what bigger than _Vertex
	uint _compliment = this->V - _Vertex;						// how many bits contains about _Vertex's connectivity
	uint _base = _Vertex * this->V - _Vertex * (_Vertex + 1) / 2 - _compliment;
	for (size_t i = 0; i < _compliment; i++) {
        uint byte = (_base + i) / 8;
		byte_t field = this->connectivityVector[byte];
		res += getBit(i, field);								// look at definition of 'getBit()': there i %= 8 too
	}
	return res;
}

int UDirGraph::getType() const {
    return udirgraph;
}

bool UDirGraph::isConnected(uint _in, uint _out) const {                // can't be equals arguments
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
	res = getBit(bit, this->connectivityVector[byte]);
	return res;
}

uint UDirGraph::getEdges() const {
	return this->E;
}

void UDirGraph::setEdge(uint _in, uint _out) {                          // O(1)
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
    this->connectivityVector[byte] |= 1 << bit;
}

void UDirGraph::eraseEdge(uint _in, uint _out) {
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
    bool conected = getBit(bit, this->connectivityVector[byte]);
    if(conected){
        byte_t& value = this->connectivityVector[byte];
        value = resetBit(bit, value);
        this->E--;
    }
    // here ever littlest of arguments is 'begin' of edge, greatest - 'end'
    // operation of deleting edge can be realized as:
    // value -= 1 << bit;
}

std::stack<uint>& UDirGraph::BFS(uint _root) const {
    static std::stack<uint> _BFS = std::stack<uint>{};
    _BFS = std::stack<uint>{};
    if(_root > this->V || _root == 0){
        return _BFS;    // empty stack is invalid input flag
    }
    bool* scoped = new bool[this->V]{0};     // here contains scoped vertexes
    ushort* distance = new ushort[this->V];
    for(size_t i = 0; i < this->V; i++){
        distance[i] = USHORT_MAX;   // set unscoped flag
    }
    std::queue<uint> Q{};
    Q.push(_root);
    scoped[_root-1] = true;
    distance[_root-1] = 0;
    while(!Q.empty()){
        Q.pop();
        uint power = this->getDegree(_root);
        uint progress = 0;
        for(size_t i = 1;
            i <= this->V && progress != power;
            i++)
        {
            if(!this->isConnected(i, _root) || scoped[i-1]){
                continue;
            }
            else{
                ++progress;
                distance[i-1] = distance[_root-1] + 1;  // track distance
                scoped[i-1] = true;
                Q.push(i);
            }
        }
        if (!Q.empty()){
            _root = Q.front();  // update queue
        }
    }
    for(size_t i = 0; i < this->V; i++){
        _BFS.push(distance[i]);
    }
    delete[] distance;
    delete[] scoped;
    return _BFS;
    /*
    *   BFS tree performed like substitution of vertexes
    *   like: perent -> child
    *   1, 2, 3, 4, 5, 6, 7 ... V
    *   2, 4, 1, 5, V, 7, 3 ... 6    (this sequence will be returned)
    *   WARNING: this function means that static _BFS
    *   will be poped out after return. Every entry time
    *   _BFS should be empty
    */
}

std::stack<uint>& UDirGraph::DFS(uint _root) const {
    static std::stack<uint> _DFS = std::stack<uint>{};
    _DFS = std::stack<uint>{};
    if(_root > this->V || _root == 0){
        return _DFS;    // empty stack is invalid input flag
    }
    bool* scoped = new bool[this->V]{0};     // here contains scoped vertexes
    ushort* distance = new ushort[this->V];
    for(size_t i = 0; i < this->V; i++){
        distance[i] = USHORT_MAX;   // set unscoped flag
    }
    std::stack<uint> S{};
    S.push(_root);
    scoped[_root-1] = true;
    distance[_root-1] = 0;
    while(!S.empty()){
        _root = S.top();
        uint power = this->getDegree(_root);
        uint progress = 0;
        for(size_t i = 1;
             i <= this->V && progress != power;
             i++)
        {
            if(!this->isConnected(i, _root) || scoped[i-1]){
                continue;
            }
            else{
                ++progress;
                distance[i-1] = distance[_root-1] + 1;  // track distance
                scoped[i-1] = true;
                S.push(i);
            }
        }
        if (!S.empty()){
            _root = S.top();  // update queue
        }
    }
    for(size_t i = 0; i < this->V; i++){
        _DFS.push(distance[i]);
    }
    delete[] distance;
    delete[] scoped;
    return _DFS;
    /*
    *   DFS tree performed like substitution of vertexes
    *   like: perent -> child
    *   1, 2, 3, 4, 5, 6, 7 ... V
    *   2, 4, 1, 5, V, 7, 3 ... 6    (this sequence will be returned)
    *   WARNING: this function means that static _DFS
    *   will be poped out after return. Every entry time
    *   _DFS should be empty
    */
}

std::stack<uint>& UDirGraph::EulerCycle(uint _begin) const {
    static std::stack<uint> _EulerCycle = std::stack<uint>{};
    _EulerCycle = std::stack<uint>{};
    if(!this->isPossibleEulerCycle()){
        _EulerCycle.push(UINT_MAX);
        return _EulerCycle; // flag of impossibility to find EulerCycle
    }
    if(_begin > this->V || _begin == 0){
        return _EulerCycle; // empty stack is invalid input flag
    }
    byte* toCheck = new byte[(this->V * (this->V - 1) / 2 + 7) / 8]{0};
    for (size_t i = 0; i < this->V; i++) {
        uint skippedBits = (i + 1) * (i + 2) / 2;
        for (size_t j = i + 1; j < this->V; j++) {	// i + 1 is shift for skipping unnecessary fields
            uint matBits = i * this->V + j;
            uint bit = matBits - skippedBits;
            uint byte = bit / 8;
            if (this->isConnected(i+1, j+1)) {
                toCheck[byte] |= setBit(bit);
            }
        }
    }
    std::stack<uint> S;
    S.push(_begin);
    while (!S.empty()){
        unsigned v = S.top();
        for(size_t i = 1; i <= this->V; i++){
            uint _in = v < i ? v : i;
            uint _out = v > i ? v : i;
            uint complimentIN = this->V - _in;
            uint baseIN = _in * this->V - _in * (_in + 1) / 2 - complimentIN;	// in bits everywhere!
            uint offset = _out - _in - 1;
            uint address = baseIN + offset;
            uint byte = address / 8;
            uint bit = address % 8;
            if(!this->isConnected(i, v) ||
                getBit(bit, toCheck[byte]) ||
                i == v){
                continue;
            }
            else{
                S.push(i);
                setBit(bit, toCheck[byte]);
            }
        }
        S.pop();
        _EulerCycle.push(v);
    }
    delete[] toCheck;
    return _EulerCycle;
}

UDirGraph& UDirGraph::operator+=(const UDirGraph& _Right) {
    /*
    * this operator performs submition of graphs
    * it means that same edges will be deleted at
    *  _Left (this) Graph.
    */
    ushort minV = this->V < _Right.getV() ? this->V : _Right.getV();
    for(size_t i = 1; i <= minV; i++){
        for(size_t j = i + 1; j <= minV; j++){
            if(!this->isConnected(i, j) && _Right.isConnected(i, j)){
                uint offset = this->V - j - 1;                              // how many bits we have to shift to find [v][_Vertex] field
                uint compliment = this->V - j;
                uint base = (i-1) * this->V - ((i-1) * i) / 2 - compliment;	// it's like begin of segment, but for matrix row
                uint address = base + offset;
                uint byte = address / 8;
                setBit(address, this->connectivityVector[byte]);
            }
        }
    }
    return *this;
}

UDirGraph& UDirGraph::operator+(std::stack<uint>& _Right) {
    if(_Right.empty()){ return *this; }
    bool** mat = toMatrix(this->V, this->connectivityVector, this->V + 1);
    /*
    *   here we have mat like:
    *   0 1 1 0
    *   1 0 1 0
    *   1 1 0 0
    *   0 0 0 0     where is one unconnected vertex
    */
    while(!_Right.empty()){
        uint _Vertex = _Right.top();
        if(_Vertex != 0 && _Vertex < this->V + 1){
            mat[_Vertex-1][this->V] = true; // setEdge
            this->E++;
        }
        else if(_Vertex == 0){ break; }
        else {Error(_INVALID_ARGUMENT_);}
        _Right.pop();
    }
    this->V++;
    delete[] this->connectivityVector;
    this->connectivityVector = new byte[(this->V * (this->V - 1) / 2 + 7) / 8]{0};
    for (size_t i = 0; i < this->V; i++) {
        uint skippedBits = (i + 1) * (i + 2) / 2;
        for (size_t j = i + 1; j < this->V; j++) {
            uint matBits = i * this->V + j;
            uint bit = matBits - skippedBits;
            uint byte = bit / 8;
            if (mat[i][j]) {
                this->connectivityVector[byte] |= setBit(bit);
            }
            else{
                this->connectivityVector[byte] &= ~setBit(bit);
            }
        }
    }
    return *this;
}

UDirGraph& UDirGraph::operator-=(const UDirGraph& _Right) {
    /*
    * this operator performs submition of graphs
    * it means that same edges will be deleted at
    *  _Left (this) Graph.
    */
    ushort minV = this->V < _Right.getV() ? this->V : _Right.getV();
    for(size_t i = 1; i <= minV; i++){
        for(size_t j = i + 1; j <= minV; j++){
            if(!this->isConnected(i, j) && _Right.isConnected(i, j)){
                uint offset = this->V - j - 1;
                uint compliment = this->V - j;
                uint base = (i-1) * this->V - ((i-1) * i) / 2 - compliment;
                uint address = base + offset;
                uint byte = address / 8;
                resetBit(address, this->connectivityVector[byte]);
            }
        }
    }
	return *this;
}

UDirGraph& UDirGraph::operator-(uint _Vertex) {
    if (_Vertex > this->V || _Vertex == 0) {
		return *this;
	}
    if(this->V == 1){
        this->V--;
        this->E = 0;
        delete[] this->connectivityVector;
        this->connectivityVector = nullptr;
        return *this;
    }
    this->E = 0;
    bool** mat = toMatrix(this->V, this->connectivityVector);
    delete[] this->connectivityVector;
    this->connectivityVector = new byte[((this->V - 1) * (this->V - 2) / 2 + 7) / 8]{false};
    uint compliment = this->V - _Vertex;
    for(size_t i = 0; i < this->V; i++){
        if(i + 1 == _Vertex){ continue; }
        for(size_t j = i+1; j < this->V; j++){
            if(mat[i][j]){
                if(j + 1 == _Vertex){ continue; }
                uint matBits = i*this->V + j;
                uint skipped = (i+1)*(i+2)/2;
                uint bit = matBits - skipped;
                if(i+1 >= _Vertex && j+1 >= _Vertex){bit-=this->V-1;}
                else if(j+1 >= _Vertex) {bit -= i+1;}
                else if(i+1 >= _Vertex) {bit -= compliment;}
                uint byte = bit / 8;
                this->connectivityVector[byte] |= setBit(bit);
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

int UDirGraph::operator()(uint _Vertex) const {
	return this->getDegree(_Vertex);
}

bool UDirGraph::isPossibleEulerCycle() const{
    for(size_t i = 1; i <= this->V; i++){
        if(this->getDegree(i) % 2){
            return false;
        }
    }
    return true;
}
