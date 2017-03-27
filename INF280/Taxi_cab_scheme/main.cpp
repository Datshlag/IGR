#include <stdio.h>
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <cmath>

using namespace std;

#define NIL 0
#define INF numeric_limits<unsigned int>::max()

vector<int> X, Y;
vector<vector<int > > adj;

vector<int > pairX;
vector<int > pairY;

vector<unsigned int> dist;


bool BFS() {
	queue<int> Q;
	dist[NIL] = INF;

	for(auto x : X) {

		dist[x] = (pairX[x] == NIL) ? 0 : INF;
		if (pairX[x] == NIL) Q.push(x);
	}

	while (!Q.empty()) {

		int x = Q.front(); 
		Q.pop();
		if (dist[x] < dist[NIL]) {

			for (auto y : adj[x]) {

				if (dist[pairY[y]] == INF) {

					dist[pairY[y]] = dist[x] + 1;
					Q.push(pairY[y]);
				}
			}
		}
	}

	return dist[NIL] != INF;
}

bool DFS(int x) {

	if (x != NIL) {

		for (auto y : adj[x]) {

			if (dist[pairY[y]] == dist[x] + 1 && DFS(pairY[y])) {

				pairX[x] = y;
				pairY[y] = x;
				return true;
			}
		}

		dist[x] = INF;
		return false;
	}

	return true;
}

int HopcroftKarp() {

	pairX = vector<int >(X.size() + 1, NIL);
	pairY = vector<int >(Y.size() + 1, NIL);

	int matching = 0;

	while(BFS()) { 

		for(auto x : X) {

			if (pairX[x] == NIL && DFS(x)) matching++;
		}
	}

	return matching;
}

int main() {

	int N, M;
	int h, m;
	vector<int > xCoord, yCoord, time, xDist, yDist, travelTime;
	scanf("%d", &N);

	while(N--) {

		scanf("%d", &M);
		X = vector<int >(M);
		Y = vector<int >(M);
		xCoord = vector<int >(M);
		yCoord = vector<int >(M);
		xDist = vector<int >(M); 
		yDist = vector<int >(M);
		time = vector<int >(M);
		travelTime = vector<int >(M);
		dist = vector<unsigned int >(M + 1);
		adj = vector<vector<int > >(M + 1);

		for(int i = 0; i < M; i++) {

			scanf("%d:%d%d%d%d%d", &h, &m, &xCoord[i], &yCoord[i], &xDist[i], &yDist[i]);
			travelTime[i] = abs(xCoord[i] - xDist[i]) + abs(yDist[i] - yCoord[i]);
			time[i] = h * 60 + m;
			X[i] = Y[i] = i + 1;
		}

		for (int i = 0; i < M; i++) {

			for (int j = 0; j < M; j++) {

				int transTime = abs(xDist[i] - xCoord[j]) + abs(yDist[i] - yCoord[j]);
				if (time[j] > time[i] + travelTime[i] + transTime) {

					adj[i + 1].push_back(j + 1);
				}
			}
		}

		int ans = HopcroftKarp();
		cout << M - ans << endl;	
	}

	return 0;
}