#include <iostream>
#include <stdio.h>
#include <set>

using namespace std;

int main() {

	int nx, ny;
	bool bad;
	float w, f;

	set<float> x;
	set<float> y;

	scanf("%d", &nx);
	scanf("%d", &ny);
	scanf("%f", &w);

	while(nx || ny || w) {


		bad = false;

		for(unsigned int i = 0; i < nx; i++) {

			scanf("%f", &f);
			x.insert(f);
		}
		x.insert(-*(x.begin()));
		x.insert(150.0 - *(prev(x.end())));

		for(auto it = x.begin(); it != prev(x.end()); ++it) {

			if( (*(next(it)) - *it) > w) {

				bad = true;
				break;
			}
		}

		for(unsigned int j = 0; j < ny; j++) {

			scanf("%f", &f);
			y.insert(f);
		}

		if(!bad) {
			
			y.insert(-*(y.begin()));
			y.insert(200.0 - *(prev(y.end())));

			for(auto it = y.begin(); it != prev(y.end()); ++it) {

				if( (*(next(it)) - *(it) > w)) {

					bad = true;
					break;
				}
			}
		}

		if(!bad) cout << "YES" << endl;
		else cout << "NO" << endl;

		scanf("%d", &nx);
		scanf("%d", &ny);
		scanf("%f", &w);

		x.clear();
		y.clear();
	}

	return 0;
}