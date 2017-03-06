//Timeout sur le site de validation, mais donne une solution au problème. Petit problème : pas la solution
//recherchée dans l'énoncé, à savoir avec le poids le plus petit possible pour le premier, puis le deuxième, etc.
//Finalement on retiendra de cette série de problèmes la difficulté à debugger tous ces algorithmes : tout fonctionne sur le papier
//mais l'implémentation est une autre affaire, et il faut dire que la plate-forme n'aide pas du tout : je pense qu'il serait beaucoup plus intéressant
//pédagogiquement d'opter pour un système différent, permettant par exemple aux élèves de voir leurs erreurs, ou au moins de fournir
//une base d'exemples dont le nombre dépasserait un.

#include <stdio.h>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>
#include <climits>

using namespace std;


int main() {
	
	int N, k, m, maxD, sum, tmp;
	vector<int > pages;
	list<int > temp;
	//Answers contient la meilleure répartion pour k' écrivains et les m' premiers livres dans answers[k' - 1][m' - 1]
	//Les tâches sont séparées par des 0 et la liste commence par la valeur du max, à minimiser
	vector<vector<list<int > > > answers;

	scanf("%d", &N);
	for(int i = 0; i < N; i++ ) {

		pages.clear();
		answers.clear();
		scanf("%d%d", &m, &k);

		answers = vector<vector<list<int > > >(k, vector<list<int > >(m, list<int>(0)));
		for(int j = 0; j < m; j++) {

			pages.push_back(0);
			scanf("%d", &pages[j]);
		}

		//On remplit le tableau answers
		for(int kp = 0; kp < k; kp++) {
			
			for(int mp = 0; mp < m; mp++) {

				//un seul écrivain
				if(kp == 0) {

					sum = 0;
					//l'écrivain copie tous les livres
					for(int l = 0; l <= mp; l++) {

						answers[kp][mp].push_back(pages[l]);
						sum += pages[l];
					}
					answers[kp][mp].push_back(0);
					answers[kp][mp].push_front(sum);

				}

				//autant d'écrivains que de livres
				else if(mp == kp) {

					maxD = 0;
					//chaque écrivain copie un livre
					for(int l = 0; l <= mp; l++) {

						answers[kp][mp].push_back(pages[l]);
						answers[kp][mp].push_back(0);
						if (pages[l] > maxD) maxD = pages[l];
					}
					answers[kp][mp].push_front(maxD);
				}

				//On enlève 1, 2, ..., mp - kp livres et 1 écrivain et on cherche le min
				else if(mp > kp) {

					temp.clear();
					//On veut minimiser maxD
					maxD = INT_MAX;

					for(int l = 0; l <= mp - kp; l++) {

						sum = 0;
						for(int u = mp - l; u <= mp; u++) {

							sum += pages[u];
						}
						tmp = max(sum, answers[kp - 1][mp - 1 - l].front());

						if( tmp <= maxD ) {

							maxD = tmp;
							temp = list<int >(pages.begin() + mp - l, pages.begin() + mp + 1);
							temp.push_back(0);
							temp.insert(temp.begin(), answers[kp - 1][mp - 1 - l].begin(), answers[kp - 1][mp - 1 - l].end());
							temp.pop_front();
							temp.push_front(maxD);
						}
						answers[kp][mp] = temp;
					}
				}
			}
		}


		temp = answers[k - 1][m - 1];
		temp.pop_front();
		temp.pop_back();
		for(auto it = temp.begin(); it != temp.end(); ++it) {

			if(it != temp.begin()) cout << " ";
			if(!(*it)) cout << "/";
			else cout << *it;
			if(it != temp.end()--) cout << " ";
		}
		cout << endl;
	}

	return 0;
}