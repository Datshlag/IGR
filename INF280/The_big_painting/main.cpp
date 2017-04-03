#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

int hp, wp, hm, wm;
vector<vector<int > > painting;
vector<vector<int > > masterpiece;
vector<vector<int > > startingPositions;
vector<vector<int > > prefixHorizontal; 
vector<vector<int > > prefixVertical;

void buildPrefix() {

	int wp = painting[0].size();
	int hp = painting.size();
	int tmp = 0;

	prefixHorizontal = vector<vector<int > >(hp, vector<int >(wp));
	prefixVertical = vector<vector<int > >(hp, vector<int >(wp));

	for(int i = 0; i < hp; i++) {

		tmp = 0;
		for(int j = 0; j < wp; j++) {

			prefixHorizontal[i][j] = tmp;
			while (tmp >= 0 && painting[i][tmp] != painting[i][j])
				tmp = prefixHorizontal[i][tmp];
			tmp++;
		}
	}

	for(int j = 0; j < wp; j++) {

		tmp = 0;
		for(int i = 0; i < hp; i++) {

			prefixVertical[i][j] = tmp;
			while (tmp >= 0 && painting[tmp][j] != painting[i][j])
				tmp = prefixVertical[tmp][j];
			tmp++;
		}
	}
}

bool checkPattern(int a, int b) {

	int maxH, maxV, tmp;
	bool ans = true;
	maxH = maxV = tmp = 0;

	for(int i = 0; i < hp; i++) {

		tmp = 0;
		for(int j = 0; j < wp; j++) {

			if(painting[i][j] != masterpiece[i + a][j + b]) {

				ans = false;
				maxH = max(maxH, tmp - prefixHorizontal[i][j]);
				break;
			}
			tmp++;
		}
	}

	for(int j = 0; j < wp; j++) {

		tmp = 0;
		for(int i = 0; i < hp; i++) {

			if(painting[i][j] != masterpiece[i + a][j + b]) {

				ans = false;
				maxV = max(maxV, tmp - prefixVertical[i][j]);
				break;
			}
			tmp++;
		}
	}

	for(int i = 0; i < hp; i++) {

		for(int j = 0; j < maxV; j++) {

			startingPositions[a + i][b + j] = 0;
		}
	}

	for(int j = 0; j < wp; j++) {

		for(int i = 0; i < maxH; i++) {

			startingPositions[a + i][b + j] = 0;
		}
	}

	return ans;
}

int main() {

	int cpt;
	scanf("%d%d%d%d", &hp, &wp, &hm, &wm);

	while(hp || wp || hm || wm) {

		cpt = 0;
		painting = vector<vector<int > >(hp, vector<int >(wp, 0));
		masterpiece = vector<vector<int > >(hm, vector<int >(wm, 0));
		startingPositions = vector<vector<int > >(hm, vector<int >(wm, 0));

		for(int i = 0; i < hp; i++) {

			for(int j = 0; j < wp; j++) {

				char c;
				cin >> c;
				if(c == 'o') painting[i][j] = 1;
				else painting[i][j] = 0;

			}
		}

		for(int i = 0; i < hm; i++) {

			for(int j = 0; j < wm; j++) {

				char c;
				cin >> c;
				if(c == 'o') masterpiece[i][j] = 1;
				else masterpiece[i][j] = 0;

				if((i + hp <= hm) && (j + wp <= wm)) startingPositions[i][j] = 1;
			}
		}

		buildPrefix();

		for(int i = 0; i < hm; i++) {

			for(int j = 0; j < wm; j++) {

				if(startingPositions[i][j]) {

					if(checkPattern(i, j)) cpt++;
				}
			}
		}
		hp = wp = hm = wm = 0;
		scanf("%d%d%d%d", &hp, &wp, &hm, &wm);
		cout << cpt << endl;
	}

	return 0;
}