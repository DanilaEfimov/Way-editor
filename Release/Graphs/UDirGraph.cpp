#include "UDirGraph.h"

static bool setBit(uint pos = 0) {
	bool res = 0b00000001;
	pos %= 8;
	res <<= pos;	// res *= 2^pos
	return res;
}

static bool getBit(uint pos = 0, bool value) {
	bool mask = 0b00000001;
	pos %= 8;
	mask <<= pos;	// res *= 2^pos
	bool res = value & mask ? 0b00000001 : 0b00000000;
	return res;
}

UDirGraph::UDirGraph(uint _V, uint _E, bool** mat) : Graph(_V){
	this->E = 0;
	this->connectivityVector = new bool[((_V * _V + 1) / 2 + 7) / 8] {false};
	// (_V * _V + 1) / 2 = B is count of necessary bits of matrix, half without diagonal
	// (B + 7) / 8	is count of new bytes for necessary bits
	for (size_t i = 0; i < _V; i++) {
		for (size_t j = i + 1; j < _V; j++) {	// i + 1 is shift for skipping unnecessary fields
			uint byte = (i * _V + j + 7) / 8;
			uint bit = (i * _V + j) % 8;		// 0b00010000 e.g.
			if (mat[i][j]) {
				this->connectivityVector[byte] |= setBit(bit);
				this->E++;
			}
			// "+ 7" is necessary, because would we have 8n + 1 bits this "+ 7" shift
			// will make 8(n+1) bits i.e n+1 byte
		}
	}
}

UDirGraph::~UDirGraph() {
	delete[] this->connectivityVector;
}

void UDirGraph::print(std::fstream& _to) const {
	if (!_to.is_open()) {
		return;
	}
	_to << "\nbit vector of connectivity mat: ";
	uint bytes = ((this->V * this->V + 1) / 2 + 7) / 8;
	uint bits = (this->V * this->V + 1) / 2;
	for (size_t byte = 0; byte < bytes; byte++) {
		for (size_t bit = 0; 
			bit < 8 && (bytes * 8 + bit) < bits;
			bit++) {
			bool value = this->connectivityVector[byte];
			_to << getBit(bit, value);
		}
	}
	_to << std::endl;
}

int UDirGraph::getDegree(uint _Vertex) const {
	if (_Vertex > this->V || _Vertex == 0) {	// undefined argument
		return -1;
	}
	uint res = 0;
	uint compliment = this->V - _Vertex;
	uint size = compliment;
	uint start = _Vertex * this->V - _Vertex * (_Vertex + 1) / 2;
	return res;
}

Graph& UDirGraph::operator-(uint _Vertex)
{
	
}

int UDirGraph::operator()(uint _Vertex)
{
	return 0;
}
