#include <stdio.h>
#include <iostream>
#include <vector>
#include <climits>
#include <cmath>

#define EPS 0.000001

using namespace std;

struct point {
	double x, y;

	point() {

		x=0.0; y=0.0;
	}
	point(double in_x, double in_y): x(in_x), y(in_y) { }

	bool operator == (point pt2) const {

		return (fabs(x - pt2.x) < EPS
		&& (fabs(y - pt2.y) < EPS));
	}
};

double squareDistance(const point& a, const point& b) {

	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

double biggestDistance(const vector<point >& polygone) {

	double dist, min = INT_MAX, max = 0;

	for(int i = 0; i < polygone.size(); i++) {

		for(int j = 0; j < polygone.size(); j++) {

			if(i == j) continue;
			dist = squareDistance(polygone[i], polygone[j]);
			cout << dist << endl;
			if(dist > max) max = dist;
		}
		if(max < min) min = max;
	}
	return max;
}

int main() {

	int n, i = 0;
	vector<point > polygone;
	vector<point > enveloppe;

	scanf("%d", &n);
	while(n) {
		i++;
		polygone.clear();
		for(int i = 0; i < n; i++) {

			point tmp;
			scanf("%lf%lf", &tmp.x, &tmp.y);
			cout << tmp.x << " " << tmp.y << endl;
			polygone.push_back(tmp);
		}

		cout << "Case " << i << ": " << sqrt(biggestDistance(polygone)) << endl;
		scanf("%d", &n);
	}

	return 0;
}