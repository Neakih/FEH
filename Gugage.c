#define A 1
#define eta 0.1
#define sikou 1000
#define kosuu 1000
#define testko 1000
#define L1 16
#define L2 16

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int x[6];
double w1[L1][60];
double b1[L1];
double z1[L1];
double w2[L2][L1];
double b2[L2];
double z2[L2];
double w3[L2];
double b3;
double y;
double ys;
int t;
double dEdy;
double dEdz2[L2];
double dEdz1[L1];
int number;
int sosuu[kosuu];
int gouseisuu[kosuu];
int testsosuu[testko];
int testgouseisuu[testko];

int sosuuhantei(int num)
{
	if(num == 3) return 1;
	if(num == 5) return 1;
	if(num == 7) return 1;
	if(num % 2 == 0){
		return 0;
	}else{
		for(int i = 3; i <= sqrt(num); i += 2){
			if(num % i == 0){
				return 0;
			}
		}
		return 1;
	}
}

void makekazu()
{
	int x;
	int sosuucount = 0;
	int gouseicount = 0;
	while(gouseicount <= kosuu)
	{
		x = rand() % 999998 + 2;
		if(sosuuhantei(x) == 1){
			sosuu[sosuucount] = x;
			sosuucount += 1;
		}else{
			gouseisuu[gouseicount] = x;
			gouseicount += 1;
		}
	}
	while(sosuucount <= kosuu)
	{
		x = rand() % 999998 + 2;
		if(sosuuhantei(x)){
			sosuu[sosuucount] = x;
			sosuucount += 1;
		}
	}
}

void maketestkazu()
{
	int x;
	int sosuucount = 0;
	int gouseicount = 0;
	while(gouseicount <= testko)
	{
		x = rand() % 999998 + 2;
		if(sosuuhantei(x) == 1){
			testsosuu[sosuucount] = x;
			sosuucount += 1;
		}else{
			testgouseisuu[gouseicount] = x;
			gouseicount += 1;
		}
	}
	while(sosuucount <= testko)
	{
		x = rand() % 999998 + 2;
		if(sosuuhantei(x)){
			testsosuu[sosuucount] = x;
			sosuucount += 1;
		}
	}
}
// w[行][列] すなわちw[出力ニューロン][入力ニューロン]

void forward_w1()
{
	for(int i = 0;i < L1; i ++)
	{
		z1[i] = b1[i];
		for(int j = 0;j < 6; j ++)
		{
			z1[i] += w1[i][x[j]];
		}
		if(z1[i] < 0)
			z1[i] = 0;
	}
}

void forward_w2()
{
	for(int i = 0;i < L2; i ++)
	{
		z2[i] = b2[i];
		for(int j = 0;j < L1; j ++)
		{
			z2[i] += z1[j] * w2[i][j];
		}
		if(z2[i] < 0)
			z2[i] = 0;
	}
}

void forward_w3()
{
	y = b3;
	for(int i = 0;i < L2; i ++)
	{
		y += z2[i] * w3[i];
	}
	ys = (1/(1 + exp(A * (-y))));
}

void backward_z2()
{
	for(int i = 0;i < L2; i ++)
	{
		dEdz2[i] = dEdy * w3[i];
	}
}

void backward_z1()
{
	for(int i = 0;i < L1; i ++)
	{
		dEdz1[i] = 0;
		for(int j = 0; j < L2; j ++)
		{
			if(z2[j] != 0)
				dEdz1[i] += dEdz2[j] * w2[j][i];
		}
	}
}

void backward_w3()
{
	for(int i = 0;i < L2; i ++)
	{
		if(z2[i] != 0)
			w3[i] -= eta * dEdy * z2[i];
	}
	b3 -= eta * dEdy;
}

void backward_w2()
{
	for(int i = 0;i < L2; i ++)
	{
		if(z2[i] != 0)
		{
			for(int j = 0;j < L1; j ++)
			{
				w2[i][j] -= eta * dEdz2[i] * z1[j];
			}
			b2[i] -= eta * dEdz2[i];
		}
	}
}
	
void backward_w1()
{
	for(int i = 0;i < L1; i ++)
	{
		if(z1[i] != 0)
		{
			for(int j = 0;j < 6; j ++)
			{
				w1[i][x[j]] -= eta * dEdz1[i];
			}
			b1[i] -= eta * dEdz1[i];
		}
	}
}

/*
void makenumber()
{
	number = rand() % 999998 + 2;
	t = sosuuhantei(number);
	x[0] = number % 10;
	x[1] = (number/10)%10 + 10;
	x[2] = (number/100)%10 + 20;
	x[3] = (number/1000)%10 + 30;
	x[4] = (number/10000)%10 + 40;
	x[5] = (number/100000)%10 + 50;
}
*/

void dainyu_x()
{
	t = sosuuhantei(number);
	x[0] = number % 10;
	x[1] = (number/10)%10 + 10;
	x[2] = (number/100)%10 + 20;
	x[3] = (number/1000)%10 + 30;
	x[4] = (number/10000)%10 + 40;
	x[5] = (number/100000)%10 + 50;
}

void randamsyokika()
{
	for(int i = 0; i < 16; i ++){
		for(int j = 0; j < 60; j ++){
			w1[i][j] = (double)rand()/RAND_MAX - 0.5;
		}
		b1[i] = (double)rand()/RAND_MAX - 0.5;
	}
	for(int i = 0; i < 8; i ++){
		for(int j = 0; j < 16; j ++){
			w2[i][j] = (double)rand()/RAND_MAX - 0.5;
		}
		b2[i] = (double)rand()/RAND_MAX - 0.5;
	}
	for(int i = 0; i < 8; i ++){
		w3[i] = (double)rand()/RAND_MAX - 0.5;
		b3 = (double)rand()/RAND_MAX - 0.5;
	}
}


void test(int *Tcount, int *Fcount)
{
	
	for(int i = 0; i < testko; i ++)
	{
	number = testsosuu[i];
	dainyu_x();
	t = 1;
	forward_w1();
	forward_w2();
	forward_w3();
	if((t == 1) && (y >= 0))
		*Tcount += 1;
	if((t == 1) && (y < 0))
		*Fcount += 1;
	if((t == 0) && (y >= 0))
		*Fcount += 1;
	if((t == 0) && (y < 0))
		*Tcount += 1;
	}
	
	
	for(int i = 0; i < testko; i ++)
	{
	number = testgouseisuu[i];
	dainyu_x();
	t = 0;
	forward_w1();
	forward_w2();
	forward_w3();
	if((t == 1) && (y >= 0))
		*Tcount += 1;
	if((t == 1) && (y < 0))
		*Fcount += 1;
	if((t == 0) && (y >= 0))
		*Fcount += 1;
	if((t == 0) && (y < 0))
		*Tcount += 1;
	}
	
}

void test100()
{
	for(int i = 1; i <= 100; i ++)
	{
		number = i;
		dainyu_x();
		t = sosuuhantei(number);
		forward_w1();
		forward_w2();
		forward_w3();
		
		if((ys-t)*(ys-t) < 0.25)
			printf("%d 当たり  %lf\n",i,ys);
		else
			printf("%d ハズレ  %lf\n",i,ys);
		
	}
}
	
int main()
{
	srand((unsigned int)time(NULL));
	randamsyokika();
	
	makekazu();
	/*
	number = sosuu[0];
	dainyu_x();
	forward_w1();
	forward_w2();
	forward_w3();
	dEdy = 2 * A * ys * (ys - t) * (1 - ys);
	printf("y = %lf\n",y);
	printf("ys = %lf\n",ys);
	printf("%lf\n",dEdy);
	*/
	
	for(int i = 0; i < kosuu; i ++)
	{
	number = sosuu[i];
	dainyu_x();
	forward_w1();
	forward_w2();
	forward_w3();
	dEdy = 2 * A * ys * (ys - t) * (1 - ys);
	//printf("y = %lf\n",y);
	//printf("ys = %lf\n",ys);
	backward_z2();
	backward_z1();
	backward_w3();
	backward_w2();
	backward_w1();
	//printf("%d回目更新完了\n", i+1);
	number = gouseisuu[i];
		dainyu_x();
	forward_w1();
	forward_w2();
	forward_w3();
	dEdy = 2 * A * ys * (ys - t) * (1 - ys);
		backward_z2();
	backward_z1();
	backward_w3();
	backward_w2();
	backward_w1();
		
	}
	printf("更新完了\n");
	

	
	maketestkazu();
	int Tcount = 0;
	int Fcount = 0;
	test(&Tcount, &Fcount);
	printf("当たり %d\n",Tcount);
	printf("ハズレ %d\n",Fcount);
	
	for(int i = 0; i < 5;i ++)
	{
		printf("w1 = %lf\n",w1[i][i]);
	}
	
	for(int i = 0; i < 5;i ++)
	{
		printf("w2 = %lf\n",w2[i][i]);
	}
	
	for(int i = 0; i < 8;i ++)
	{
		printf("w3 = %lf\n",w3[i]);
	}
	printf("b3 = %lf\n", b3);
	
	test100();
	
	return 0;
}
		
		
