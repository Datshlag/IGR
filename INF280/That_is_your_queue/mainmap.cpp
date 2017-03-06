#include <iostream>
#include <stdio.h>
#include <map>

using namespace std;

int main() {	

	int p, c, pos, m, temp, mov;
	int k = 1;
	char s;

	map<int, int > next;
	map<int, int > prev;

	pos = 0;
	scanf("%d", &p);
	scanf("%d", &c);
	m = min(c, p);

	while(p || c) {


		next.clear();
		prev.clear();

		for(int i = 0; i < m; i++) {

			next[i] = (i + 1)%m;
			prev[i] = (i + p - 1)%m;
		}

		pos = 0;

		cout << "Case " << k << ":" << endl;
		for(int j = 0; j < c; j++) {

			scanf(" %c", &s);
			if(s == 'N') {

				cout << pos + 1 << endl;
				pos = next[pos];
			}

			if(s == 'E') {

				scanf("%d", &mov);
				mov--;

				if(next[pos] = mov) pos = next[pos];
				else if(pos != mov) {

					auto it = next.find(mov);
					if(it != next.end()) {

						prev[next[mov]] = prev[mov];
						next[prev[mov]] = next[mov];
					}

					next[mov] = next[pos];
					next[pos] = mov;
					prev[mov] = pos;
					prev[next[mov]] = mov;
					pos = next[pos];
				}

			}
		}

		k++;
		pos = 0;
		scanf("%d", &p);
		scanf("%d", &c);
		m = min(c, p);
	}

	return 0;
}