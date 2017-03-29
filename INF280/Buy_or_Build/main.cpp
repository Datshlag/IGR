#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;	

struct Point {

	int x, y;
};

struct Edge {

	int u, v, d;
	bool operator<(const Edge &e) const {

		return d < e.d;
	}
};

map<int, pair<int, unsigned int> > set; 

int getWeight(Point& p1, Point& p2) {

	return ((p1.x - p2.x) * (p1.x - p2.x) + (p1.y -p2.y) * (p1.y - p2.y));
}

void makeSet(const int& x) {
	
	set.insert(make_pair(x, make_pair(x, 0)));
}

int find(const int& x) {

	if (set[x].first == x) return x;
	else return set[x].first = find(set[x].first); 
}

void unite(const int& x, const int& y){

	int parentX = find(x), parentY = find(y);
	int rankX = set[parentX].second, rankY = set[parentY].second;

	if (parentX == parentY) return;
	else if (rankX < rankY) set[parentX].first = parentY;
	else set[parentY].first = parentX;
	if (rankX == rankY) set[parentX].second++;
}

int kruskal(const vector<Edge>& edges, int nbVertex, int n) {

	int cost = 0;
	int count = n;
	for(auto &tmp : edges) {

		if(count == nbVertex - 1) break;
		if (find(tmp.u) != find(tmp.v)) {

			unite(tmp.u, tmp.v);
			cost += tmp.d;
			count++;
		}
	}
	return cost;
}

int main() {

	int T, n, q, ans;
	vector<int> costs;
	vector<vector<int > > networks;
	vector<Point> points;
	vector<Edge> edges;

	scanf("%d", &T);
	for(int i = 0; i < T; i++) {

		scanf("%d%d", &n, &q);
		costs = vector<int >(q);
		networks = vector<vector<int > >(q);
		points = vector<Point>(n);
		edges.clear();

		for(int j = 0; j < q; j++) {

			int nb;
			scanf("%d%d", &nb, &costs[j]);
			networks[j] = vector<int >(nb);

			for(int k = 0; k < nb; k++) {

				scanf("%d", &networks[j][k]);
				networks[j][k]--;
			}
		}

		for(int j = 0; j < n; j++) {

			scanf("%d%d", &points[j].x, &points[j].y);
		}

		for(int j = 0; j < n; j++) {

			for(int k = j + 1; k < n; k++) {

				Edge e;
				e.u = j;
				e.v = k;
				e.d = getWeight(points[j], points[k]);
				edges.push_back(e);
			}
		}

		set.clear();
		for(int j = 0; j < n; j++) {

			makeSet(j);
		}
		sort(edges.begin(), edges.end());

		ans = kruskal(edges, n, 0);

		for (int j = 1; j < (1 << q); j++)
		{	

			set.clear();
			for(int k = 0; k < n; k++) {

				makeSet(k);
			}

			int cost = 0;
			int nb = 0;
			for (int k = 0; k < q; k++)
			{	
				if (j & (1 << k))
				{
					cost += costs[k];
					for (int l = 1; l < networks[k].size(); l++)
					{
						if(find(networks[k][l - 1]) != find(networks[k][l])) {

							unite(networks[k][l - 1], networks[k][l]);
							nb++;
						}
					}
				}
			}
			int tmp = kruskal(edges, n, nb);
			ans = min(ans, cost + tmp);
		}
		cout << ans << endl;
		if(i < T - 1) cout << endl;
	}

	return 0;
}