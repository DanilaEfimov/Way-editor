#include<iostream>
#include"UDirGraph.h"

int main(int argc, char* argv[]) {

	byte** graph = new byte* [4];
	for (int i = 0; i < 4; i++) {
		graph[i] = new byte[4] {1,1,1,1};
		graph[i][i] = 0;
	}
	uint V = 4;
	UDirGraph G(V, graph);
	std::cout << G(3);

	return 0;
}