#include <stdio.h>
#include <iostream>
#include <vector>
#include <stack>
#include <numeric>

using namespace std;

int findEulerianCircuit(vector<vector<int > > &adj, vector<int > &path, int start);

int main() {

	int T, N;
	int first;
	vector<vector<int > > adj;
	vector<int > circuit;

	scanf("%d", &T);

	for(int i = 0; i < T; i++) {

		scanf("%d", &N);
		adj = vector<vector<int > >(50, vector<int >(50, 0));
		circuit.clear();

		for(int j = 0; j < N; j++) {

			int a, b;
			scanf("%d%d", &a, &b);
			first = a - 1;
			adj[a-1][b-1]++;
			if(a != b) adj[b-1][a-1]++;
		}
		
		int b = findEulerianCircuit(adj, circuit, first);

		cout << "Case #" << i + 1 << endl;
		if(b && circuit.size() == N + 1) {

			for(int k = 0; k < N; k++) {

				cout << circuit[k] + 1<< " " << circuit[(k+1+N)%N] + 1 << endl;
			}
			cout << endl;
		}
		else cout << "some beads may be lost" << endl << endl;
	}

	return 0;
}

int findEulerianCircuit(vector<vector<int > > &adj, vector<int > &circuit, int start) {

	int N = adj.size();
	for(int i = 0; i < N; i++) {

		if((accumulate(adj[i].begin(), adj[i].end(), 0) - adj[i][i])%2) return 0;
	}

	int v, index;
	v = start;
	stack<int> stack;
	stack.push(v);
	while (!stack.empty()) {

		index = -1;
		for(int i = 0; i < 50; i++) {

			if(adj[v][i]) {

				index = i;
				break;
			}
		}

		if (index >= 0) {
		 // follow edges until stuck
			stack.push(v);
			adj[v][index]--; // remove edge, modifying graph
			if(v != index) adj[index][v]--;
			v = index;
		} 
		else {
		 // got stuck: stack contains a circuit
			circuit.push_back(v); // append node at the end of circuit
			v = stack.top(); // backtrack using stack, find larger circuit
			stack.pop();
		}
	}

	return 1;
}