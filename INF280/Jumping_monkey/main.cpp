#include <stdio.h>
#include <vector>
#include <queue>
#include <iostream>
#include <list>

using namespace std;

vector<list<int > > adj;

void SEARCH(int root, vector<int >& path, int n) {

	queue<int> Q;
	bool visited[n] = {};

	Q.push(root);
	while (!Q.empty()) {

		int u = Q.front();
		Q.pop();
		if(!adj[u].empty()) path.push_back(u);
		if(!visited[u]) {

			for (auto v : adj[u]) {

				Q.push(u);
				Q.push(v);
				adj[u].remove(v);
				adj[v].remove(u);
			}
		}

		visited[u] = true;
	}
}

int main() {
	
	int n, m;
	int i = 1;
	vector<int > path;
	scanf("%d%d", &n, &m);

	while(n && m) {

		path.clear();
		adj = vector<list<int > >(n);

		for(int j = 0; j < m; j++) {

			int a, b;
			scanf("%d%d", &a, &b);
			adj[a].push_back(b);
			adj[b].push_back(a);
		}

		if(m != n - 1) cout << "Impossible";
		else {

			SEARCH(0, path, n);
			cout << path.size() << ":";
			for(auto tmp : path) cout << " " << tmp;
		}
		scanf("%d%d", &n, &m);
		if(n && m) cout << endl;
	}

	return 0;
}