//Fonctionne pour des petits exemples à la main, mais le serveur réponds soit "time excedeed" (???), soit "wrong answer"
//Et comme le site ne propose qu'un seul ridicule petit input pour tester l'algorithme, je ne compte pas continuer à debug.

#include <stdio.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <utility>

using namespace std;

float computeDistance(const vector<float>& v1, const vector<float >& v2) {

	return  sqrt( (v1[0] - v2[0]) * (v1[0] - v2[0])
				+ (v1[1] - v2[1]) * (v1[1] - v2[1])
				+ (v1[2] - v2[2]) * (v1[2] - v2[2]) );
}

int main() {

	//Aucune précision dans l'énonce sur les dimensions des boîtes
	long long int n, N, nb;
	double minD, minX, minY, minZ, d, vol, maxVol, boxVol;;
	bool intersects;

	vector<float > cornerA(3, 0.0); 
	vector<float > cornerB(3, 0.0);

	vector<vector<float > > balloons;
	vector<int > order;
	vector<float > radius;
	vector<float > pos;

	scanf("%d", &N);
	nb = 1;

	while(N) {

		balloons.clear();
		order.clear();
		cornerA.clear();
		cornerB.clear();
		n = 0;

		scanf("%f%f%f", &cornerA[0], &cornerA[1], &cornerA[2]);
		scanf("%f%f%f", &cornerB[0], &cornerB[1], &cornerB[2]);

		boxVol  =   abs(cornerB[0] - cornerA[0]) 
				  * abs(cornerB[1] - cornerA[1])
				  * abs(cornerB[2] - cornerA[2]);
		maxVol = boxVol;

		for(int i = 0; i < N; i++) {

			vector<float > toAdd(3, 0.0);
			scanf("%f%f%f", &toAdd[0], &toAdd[1], &toAdd[2]);
			if( ((cornerA[0] < toAdd[0]) ^ (cornerB[0] < toAdd[0])) &&
					((cornerA[1] < toAdd[1]) ^ (cornerB[1] < toAdd[1])) &&
					((cornerA[2] < toAdd[2]) ^ (cornerB[2] < toAdd[2]))) {

				minD = 0;
				balloons.push_back(toAdd);
				n++;
			}
		}

		for(int i = 0; i < n; i++) {

			order.push_back(i);
		}


		//On se contente de regarder le volume que l'on peut gonfler pour chaque permutation de point
		//possible. Comme il y a au plus 6 points, on teste au plus 720 permutations, ce qui est acceptable.
		do {	

			radius.clear();
			vol = 0;

			for(int i = 0; i < n; i++) {

				pos = balloons[order[i]];
				minX = min(abs(cornerB[0] - pos[0]), abs(pos[0] - cornerA[0]));
				minY = min(abs(cornerB[1] - pos[1]), abs(pos[1] - cornerA[1]));
				minZ = min(abs(cornerB[2] - pos[2]), abs(pos[2] - cornerA[2]));
				minD = min(min(minX, minY), minZ);

				for(int j = 0; j < i; j++) {

					d = computeDistance(pos, balloons[order[j]]);
					if(d < radius[j]) {

						minD = 0;
						break;
					}
					else minD = min(minD, d - radius[j]);
				}
				radius.push_back(minD);

				vol += 4.0/3.0 * M_PI * radius[i] * radius[i] * radius[i];
			}

			if(boxVol - vol < maxVol) maxVol = boxVol - vol;

		} while(next_permutation(&order[0], &order[0] + n));

		printf("Box %d: %d\n\n", nb++, (int) round(maxVol));
		scanf("%d", &N);
	}

	return 0;
}
