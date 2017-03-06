#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

int main() {

	int nx, ny;
	bool bad;
	float w, f;

	scanf("%d", &nx);
	scanf("%d", &ny);
	scanf("%f", &w);

	while(nx || ny || w) {

		vector<float> x(nx + 2);
		vector<float> y(ny + 2);

		bad = false;

		for(unsigned int i = 0; i < nx; i++) {

			scanf("%f", &f);
			x[i + 1] = f;
		}

		for(unsigned int j = 0; j < ny; j++) {

			scanf("%f", &f);
			y[j + 1] = f;
		}

		x[0] = 0;
		y[0] = 0;
		x[nx + 1] = 75.0;
		y[ny + 1] = 100.0;

		sort(x.begin(), x.end());
		sort(y.begin(), y.end());

		x[0] = -x[1];
		x[nx + 1] = 150.0 - x[nx];
		y[0] = -y[1];
		y[ny + 1] = 200.0 - y[ny];

		for(unsigned int i = 0; i < nx + 1; i++) {

			if( (x[i+1] - x[i]) > w) {

				cout << "NO" << endl;
				bad = true;
				break;
			}
		}

		for(unsigned int j = 0; j < ny + 1; j++) {

			if(bad) break;
			if((y[j+1] - y[j]) > w) {

				cout << "NO" <<  endl;
				bad = true;
				break;
			}
		}

		if(!bad) cout << "YES" << endl;

		scanf("%d", &nx);
		scanf("%d", &ny);
		scanf("%f", &w);
	}

	return 0;
}