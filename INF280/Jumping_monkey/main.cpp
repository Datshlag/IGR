#include <stdio.h>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <iostream>
#include <list>

using namespace std;

vector<vector<int > > adj;
vector<vector<int > > adjcp;

void SEARCH(int root, vector<int >& path, int n) {

	stack<int> S;
	bool visited[n] = {};
	path.clear();
	adjcp = adj;

	S.push(root);
	while (!S.empty()) {

		int u = S.top();
		S.pop();
		if(!adj[u].empty()) path.push_back(u);
		if(!visited[u]) {

			for (auto v : adj[u]) {

				S.push(u);
				S.push(v);				
				auto it = std::find(adj[v].begin(), adj[v].end(), u);
				if(it != adj[v].end()) adj[v].erase(it);
			}
		}

		visited[u] = true;
	}

	adj = adjcp;
}

int main() {
	
	int n, m, ans;
	int i = 1;
	vector<int > path, pathcp;
	scanf("%d%d", &n, &m);

	while(n && m) {

		path.clear();
		adj = vector<vector<int > >(n);

		for(int j = 0; j < m; j++) {

			int a, b;
			scanf("%d%d", &a, &b);
			adj[a].push_back(b);
			adj[b].push_back(a);
		}

		if(m != n - 1) cout << "Impossible";
		else {

			for(int j = 0; j < n; j++) {

				SEARCH(j, path, n);
				cout << path.size() << ":";
				for(auto tmp : path) cout << " " << tmp;
					cout << endl;
			}
		}
		scanf("%d%d", &n, &m);
		if(n && m) cout << endl;
	}

	return 0;
}