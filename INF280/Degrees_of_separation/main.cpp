#include <stdio.h>
#include <iostream>
#include <map>
#include <climits>
#include <algorithm>
#define MAXP 50

using namespace std;

int main() {

	int P, R, index, nb, maxD;

	string name1, name2;
	map<string, int> names; 

	int dist[MAXP][MAXP];
	fill_n((int*)dist, MAXP*MAXP, 100);

	scanf("%d%d", &P, &R);
	nb = 1;

	while(P && R) {

		names.clear();
		fill_n((int*)dist, MAXP*MAXP, 100);
		index = 0;


		//input
		for(int i = 0; i < R; i++) {

			cin >> name1;
			cin >> name2;

			if(names.find(name1) == names.end()) {

				names[name1] = index;
				dist[index][index] = 0;
				index++;
			}

			if(names.find(name2) == names.end()) {

				names[name2] = index;
				dist[index][index] = 0;
				index++;
			}

			dist[names[name1]][names[name2]] = 1;
			dist[names[name2]][names[name1]] = 1;
		}

		//floydwarshall
		for(int k = 0; k < P; k++) {
			
			for(int i = 0; i < P; i++) {
			
				for(int j = 0; j < P; j++) {

					dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
				}
			}
		}

		//output
		cout << "Network " << nb << ": ";
		maxD = 0;
		for(int i = 0; i < P; i++) {

			for(int j = 0; j < P; j++) {

				if(dist[i][j] > maxD) maxD = dist[i][j];
				//cout << "i: " << i << " j: " << j  << " " << dist[i][j] << endl;
			}
		}

		if(maxD == 100) cout << "DISCONNECTED" <<endl;
		else cout << maxD <<endl;

		scanf("%d%d", &P, &R);
		cout << endl;
		nb++;
	}

	return 0;
}