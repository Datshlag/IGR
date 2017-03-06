#include <iostream>
#include <stdio.h>
#include <list>

using namespace std;

int main() {	

	int p, c, m, val, size;
	int k = 1;
	char s;

	list<int > queue;

	scanf("%d", &p);
	scanf("%d", &c);
	m = min(c, p);

	while(p || c) {


		queue.clear();

		for(int i = 0; i < m; i++) {

			queue.push_back(i);
		}

		cout << "Case " << k << ":" << endl;
		for(int j = 0; j < c; j++) {

			scanf(" %c", &s);
			if(s == 'N') {

				val = queue.front();
				queue.pop_front();
				cout << val + 1 << endl;
				queue.push_back(val);
			}

			if(s == 'E') {

				scanf("%d", &val);
				val--;

				size = queue.size();
				queue.remove(val);
				queue.push_front(val);
				if (size != queue.size()) queue.pop_back();
			}
		}

		k++;
		scanf("%d", &p);
		scanf("%d", &c);
		m = min(c, p);
	}

	return 0;
}