#include <stdio.h>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <climits>
#include <queue>

using namespace std;

typedef pair<int, int > node;

vector<list<int > > dijkstra(const vector<list<int > >& next, map<int, map<int, int> >& weights, vector<int >& distances, int root);
void clearPath(int D, const vector<list<int > >& pred, vector<list<int > >& next);

int main() {

	vector<int > distances;
	vector<list<int > > next;
	map<int, map<int, int> > weights;

	int N, M, S, D;
	scanf("%d%d", &N, &M);

	while(N && M) {

		scanf("%d%d", &S, &D);

		next = vector<list<int > > (N, list<int >(0));
		distances = vector<int >(N, INT_MAX);

		for(int i = 0; i < M; i++) {

			int a, b, c;
			scanf("%d%d%d", &a, &b, &c);

			next[a].push_back(b);
			weights[a][b] = c;
		}
			
		auto pred = dijkstra(next, weights, distances, S);
		/*for(int i = 0; i < distances.size(); i++) {

			cout << i << " " << distances[i] << endl;
		}*/
		clearPath(D, pred, next);
		dijkstra(next, weights, distances, S);

		if(distances[D] == INT_MAX) cout << "-1" << endl;
		else cout << distances[D] << endl;

		scanf("%d%d", &N, &M);
	}

	return 0;
}

//https://a3nm.net/work/teaching/2016-2017/inf280/3/graph.pdf
vector<list<int > > dijkstra(const vector<list<int > >& next, map<int, map<int, int> >& weights, vector<int >& distances, int root) {

	priority_queue<node, vector<node>, greater<node> > Q;
	Q.push(make_pair(0, root));
	vector<list<int > > shortestPathPredecessor(next.size(), list<int >(0));

	//Setting distances to int_max
	fill(distances.begin(), distances.end(), INT_MAX);
	distances[root] = 0;

	while (!Q.empty()) {

		int u = Q.top().second; // get node with least priority
		Q.pop();
		for (auto tmp : next[u]) {

			int v = tmp;
			unsigned int weight = weights[u][v];
			if (distances[v] >= distances[u] + weight) { // shorter path found
			                                           
               	//updating best paths
				if(distances[v] > distances[u] + weight) shortestPathPredecessor[v].clear();
				shortestPathPredecessor[v].push_back(u);

				distances[v] = distances[u] + weight;
				Q.push(make_pair(distances[v], v)); // simply push, no update here
			}
		}
	}

	return shortestPathPredecessor;
}

void clearPath(int D, const vector<list<int > >& pred, vector<list<int > >& next) {

	for(auto tmp : pred[D]) {

		next[tmp].remove(D);
		clearPath(tmp, pred, next);
	}
}