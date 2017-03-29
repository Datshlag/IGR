#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

string str;

//comptage des sous-chaînes multiple de 3 sur la sous-chaîne de l à r inclus
unsigned long long int request(int l, int r) {

	unsigned long long int cpt, temp, c;
	cpt = temp = c = 0;
    unsigned long long int reminders[3];

    reminders[0] = reminders[1] = reminders[2] = 0;

    for(int i = l; i <= r; i++) {

        c = (str[i] - 48)%3;
        if(c == 0) {

        	reminders[0] += 1;
        }
        else if(c == 1) {

        	temp = reminders[2];
        	reminders[2] = reminders[1];
        	reminders[1] = reminders[0];
        	reminders[0] = temp;
            reminders[1]++;
        }
        else if(c == 2) {

            temp = reminders[0];
            reminders[0] = reminders[1];
            reminders[1] = reminders[2];
            reminders[2] = temp;
            reminders[2]++;
        }
        cpt += reminders[0];
	}

    return(cpt);
}

int main() {

	getline(cin, str);
	bool stop = false;
	while(!str.empty()) {

		unsigned long long int l, r, cpt;
		l = 0;
		r = 0;
		cpt = 0;

		//Ici on coupe la string en entrée en morceaux composés uniquement
		//composés d'entiers
		for(int i = 0; i < str.size(); i++) {

			int c = str[i] - 48;
			r = i;
			//si c ne représente pas un entier
			if(!((c >= 0) && (c <= 9))) {

				if(r > l) cpt += request(l, r - 1);
				l = i + 1;
			}
		}
		if(r >= l) cpt += request(l, r);
		cout << cpt;

		str = "";
		getline(cin, str);
		cout << endl;	
	}

	return 0;
}