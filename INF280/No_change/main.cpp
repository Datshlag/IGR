#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

int main() {

	int n, p, sum, total, q, remain;
	int ival, jval, kval, lval, mval;
	bool ok;
	vector<int> val;

	scanf("%d", &n);

	for(int u = 0; u < n; u++) {

		sum = 0;
		ok = false;
		val.clear();

		scanf("%d", &total);
		scanf("%d", &p);

		for(int j = 0; j < p; j++) {

			scanf("%d", &q);
			sum += q;
			val.push_back(sum);
		}

		sort(val.begin(), val.end());

		if(p == 1) {

			for(int m = 0; m <= total/val[0]; m++) {
				mval = m * val[0];

				if(mval == total) {

					ok = true;
					goto end;
				}

			}

		}

		if(p == 2) {

			for(int l = 0; l <= total/val[1]; l++) {
				lval = l * val[1];

				for(int m = 0; m <= (total - lval)/val[0]; m++) {
					mval = m * val[0];

					if(lval + mval == total) {

						ok = true;
						goto end;
					}

				}

			}

		}

		if(p == 3) {

			for(int k = 0; k <= total/val[2]; k++) {
				kval = k * val[2];

				for(int l = 0; l <= (total - kval)/val[1]; l++) {
					lval = l * val[1];

					for(int m = 0; m <= (total - kval - lval)/val[0]; m++) {
						mval = m * val[0];

						if(kval + lval + mval == total) {

							ok = true;
							goto end;
						}

					}

				}

			}

		}

		if(p == 4) {

			for(int j = 0; j <= total/val[3]; j++) {

				jval = j * val[3];

				for(int k = 0; k <= (total - jval)/val[2]; k++) {
					kval = k * val[2];

					for(int l = 0; l <= (total - jval - kval)/val[1]; l++) {
						lval = l * val[1];

						for(int m = 0; m <= (total - jval - kval - lval)/val[0]; m++) {
							mval = m * val[0];

							if(jval + kval + lval + mval == total) {

								ok = true;
								goto end;
							}

						}

					}

				}

			}
		}

		if(p == 5) {

			for(int i = 0; i <= total/val[4]; i++) {
			ival = i * val[4];

				for(int j = 0; j <= (total - ival)/val[3]; j++) {
					jval = j * val[3];

					for(int k = 0; k <= (total - ival - jval)/val[2]; k++) {
						kval = k * val[2];

						for(int l = 0; l <= (total - ival - jval - kval)/val[1]; l++) {
							lval = l * val[1];

							for(int m = 0; m <= (total - ival - jval - kval - lval)/val[0]; m++) {
								mval = m * val[0];

								if(ival + jval + kval + lval + mval == total) {

									ok = true;
									goto end;
								}

							}

						}

					}

				}

			}

		}
		
		end :
		if(ok) cout << "YES" << endl;
		else cout << "NO" << endl;

		if(u != n - 1) cout << endl;

	}

	return 0;
}