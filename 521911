#include <stdio.h>
#include <math.h>

int main(void)
{
	int number;
	int wari = 2;
	int jou = 0;
	scanf ("%d", &number);
	while (wari <= floor(sqrt(number))) {
		while (number % wari == 0) {
			jou += 1;
			number /= wari;
		}
		if (jou > 1){
			printf ("%d^%d", wari, jou);
		}else if (jou == 1){
			printf ("%d", wari);
		}
		if ( (number > 1) && (jou > 0) ) {
			printf ("×");
		}
		if (wari == 2){
			wari ++;
		} else {
			wari += 2;
		}
		jou = 0;
	}
	if (number > 1) {
		printf ("%d", number);
	}
	return 0;
}
		
