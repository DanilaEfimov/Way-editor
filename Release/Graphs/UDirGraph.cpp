#include "UDirGraph.h"
#include<bitset>

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

UDirGraph::UDirGraph(uint _V, byte** mat) : Graph(_V){
	this->E = 0;
	if (mat == nullptr) {
		this->V = 0;
        this->connectivityVector = nullptr;     // empty graph branch
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
	this->Graph::~Graph();
	delete this->connectivityVector;
}

void UDirGraph::print(std::fstream& _to) const {
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
    uint size = STATIC_MEMORY + ((this->V*(this->V-1)/2+7)/8)*8;    // in bits everywhere!
    _to << "\n" << size << std::endl;
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

bool UDirGraph::isConnected(uint _in, uint _out) const {                // can't be equals arguments
	bool res = false;
    if (_in > this->V || _out > this->V || _in == _out) {
		return res;
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
	res = getBit(bit, this->connectivityVector[byte]);
	return res;
}

uint UDirGraph::getEdges() const {
	return this->E;
}

void UDirGraph::setEdge(uint _in, uint _out) {                          // can't be equals arguments
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

Graph& UDirGraph::operator+(const Graph& _Right) {
	return *this;
}

Graph& UDirGraph::operator+(std::stack<uint>& _Right) {
	return *this;
}

Graph& UDirGraph::operator-(const Graph& _Right) {
	return *this;
}

Graph& UDirGraph::operator-(uint _Vertex) {
	if (_Vertex > this->V) {
		return *this;
	}
	return *this;
}

int UDirGraph::operator()(uint _Vertex) const {
	return this->getDegree(_Vertex);
}
