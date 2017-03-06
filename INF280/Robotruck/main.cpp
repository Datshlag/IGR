//Validation OK

#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

int computeDistance(const vector<vector<int > >& orders, const int& bot, const int& top);

int main() {

	int nb, C, N, weight, index, minD;
	vector<vector<int > > orders;
	//Answers contient le minimum obtenu pour livrer les colis jusqu'au kémé en answers[k - 1]
	vector<int > answers, load;
	scanf("%d", &nb);

	for(int i = 0; i < nb; i++) {

		scanf("%d", &C);
		scanf("%d", &N);
		orders.clear();
		answers.clear();

		for(int j = 0; j < N; j++) {

			load = vector<int >(3, 0);
			scanf("%d", &load[0]);
			scanf("%d", &load[1]);
			scanf("%d", &load[2]);
			orders.push_back(load);
		}

		for(int l = 0; l < N; l++) {

			weight = orders[l][2];	
			index = l; 
			minD = INT_MAX;
			//On regarde en prenant un, puis deux, ..., jusqu'au maximum de colis possibles et on prends le minimum des 
			//minimums obtenus
			while((weight <= C) && (index >= 0)) {

				if(index == 0) minD = min(minD, 
								computeDistance(orders, index, l));
				else minD = min(minD, answers[index - 1] + 
								computeDistance(orders, index, l));
				index--;
				if(index >= 0) 
					weight += orders[index][2];
			}
			answers.push_back(minD);
		}

		cout << answers[N - 1] << endl;
		if(i != nb - 1) cout << endl;
	}

	return 0;
}

//Calcule la distance parcourue par le robot pour livrer les colis des indices bot à top
int computeDistance(const vector<vector<int > >& orders, const int& bot, const int& top) {

	int d = 0;

	d += orders[bot][0] + orders[bot][1];
	for(int i = bot; i < top; i++) {

		d += abs(orders[i][0] - orders[i + 1][0]);
		d += abs(orders[i][1] - orders[i + 1][1]);
	}
	d += orders[top][0] + orders[top][1];

	return d;
}