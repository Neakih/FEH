#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

double FloatRand(); //0~1の小数を発生させる乱数
int chusen(double ratio[]); //1回のガチャ抽選の結果を返す関数
void dainyu(void); //キャラの数から確率に変換する
void Turn(void); //召喚1ターン(最大抽選5回)
	
	// 引く色の指定(第何候補か)
	int color[4] = {1,4,2,3};
	// 各レアリティ各色のキャラ個数を書いた配列
	int hosi4[4] = {33,34,25,37};
	int hosi3[4] = {33,33,25,37};
	/*
	確率配列
	00 PU赤,　 01 PU青,  02 PU緑,   03 PU白,
	04 PU赤,　 05 PU青,  06 PU緑,   07 PU白,
	08 PU赤,　 09 PU青,  10 PU緑,   11 PU白,
	12 星4赤,  13 星4青,  14 星4緑,  15 星4白,
	16 星3赤,  17 星3青,  18 星3緑,  19 星3白,
	*/
	double BasicRatio[20];
	double NowRatio[20];
	int kekka[20]; //各レアリティの出た体数を格納
	int orb; //消費オーブ数
	int hosi5denai;

int main(void){
	
	int output;
	for(int a = 0; a < 4; a ++){
		if(color[a] == 1){
			output = a;
		}
	}
	
	dainyu();
	/*
	for(int i = 0;i < 20; i++)
	{
		printf("%lf,",BasicRatio[i]);
	}
	*/
	
	/*確率検算
	double ratiosum = 0;
	for(int i = 0;i < 20; i++)
	{ratiosum += BasicRatio[i];
	}
	printf("%lf,",ratiosum);
	*/
	
	int deta = 0;
	
	// 結果配列の初期化
	for(int i = 0; i < 20; i ++){
		kekka[i] = 0;
	}
	memcpy(NowRatio,BasicRatio,sizeof(BasicRatio));
	orb = 0;
	hosi5denai = 0;
	/*
	srand((unsigned int)time(NULL));
	*/
	
		while(kekka[0] < 5){
			Turn();
		}
	
		
		
		/*オーブが決まってる場合
		while(orb < 100){
			Turn();
		}
		if(kekka[0] > 0){
				deta += 1;
		}
		*/
	
	
	printf("消費オーブ:%d\n",orb);
	printf("淫夢 : %d\n",kekka[0]);
	printf("セリス : %d\n",kekka[4]);
	printf("リーヴ : %d\n",kekka[8]);
	printf("青 : %d\n",kekka[1] + kekka[5] + kekka[9]);
	printf("緑 : %d\n",kekka[2] + kekka[6] + kekka[10]);
	printf("白 : %d\n",kekka[3] + kekka[7] + kekka[11]);
	printf("赤☆4☆3 : %d\n",kekka[12] + kekka[16]);
	printf("青☆4☆3 : %d\n",kekka[13] + kekka[17]);
	printf("緑☆4☆3 : %d\n",kekka[14] + kekka[18]);
	printf("白☆4☆3 : %d\n",kekka[15] + kekka[19]);
	

	
	/*
	printf("星3,4:%d\n",kekka[output + 12] + kekka[output + 16]);
*/
	return 0;
}
	
double FloatRand(){
	double num = (double)rand() / RAND_MAX;
	return num;
}

int chusen(double ratio[]){ //引数は確率テーブル
	int result;
	double rnd = FloatRand();
	int i;
	for(i=0 ; i<20 ; i++)
	{
		if(rnd < ratio[i]){
			result = i;
			break;
		}
		else
			rnd -= ratio[i];
	}
	return result; // 戻り値は、出たレアリティの数字(0~19)
}
			
void dainyu(void){
	// 個数から確率に変換して代入
	for(int i = 0; i < 4; i ++){
		BasicRatio[i + 12] = 0.58 * (double)hosi4[i] / (hosi4[0] + hosi4[1] + hosi4[2] + hosi4[3]);
		BasicRatio[i + 16] = 0.34 * (double)hosi3[i] / (hosi3[0] + hosi3[1] + hosi3[2] + hosi3[3]);
	}
	for(int i = 0; i < 12; i ++){
		BasicRatio[i] = (double)2 / 300;
	}
}

void Turn(void){
	int or = 0;  //このターンで引いたキャラ数と星5が出たかを判定するための変数。10の位が星5の数,1の位が引いたキャラ数
	int FalRslt[5]; //目当色が無かった場合にその結果を格納する(False Result の略)
	for(int i = 0; i < 5; i ++)
	{
		int result = chusen(NowRatio); 
		// 目当ての色かどうかの判定
		if(color[result % 4] == 1){
			kekka[result] += 1;
			// 星5かどうかの判定
			if(result / 4 < 3 ){
				or += 11;
			} else{
				or += 1;
				hosi5denai += 1;
			}
		} else{
			FalRslt[i] = result;
		}
	}
	// 星5が出たなら確率リセット
	if(or > 10){
		hosi5denai = 0;
	}
	// 引いた数に応じて消費オーブを加算
	if(or % 10 == 1){
		orb += 5;
	} else if(or % 10 == 2){
		orb += 9;
	} else if(or % 10 == 3){
		orb += 13;
	} else if(or % 10 == 4){
		orb += 17;
	} else if(or % 10 == 5){
		orb += 20;
	}
	// 目当ての色が出なかった場合の処理
	if(or == 0){
		// kは色を、jは召喚する物体を
		for(int k = 2; k <= 4; k ++){
			for(int j = 0; j <= 4; j ++){
				if(color[FalRslt[j] % 4] == k){
					kekka[FalRslt[j]] += 1;
					orb += 5;
					if(FalRslt[j] / 4 < 3 ){
						hosi5denai = 0;
					} else{
						hosi5denai += 1;
					}
					goto OUT;
				}
			}
		}
	}
	OUT:
	// ここに確率上昇を書く
	if (hosi5denai != 0){
		for(int i = 0; i < 12; i ++){
			NowRatio[i] = BasicRatio[i] + floor(hosi5denai / 5) * 0.005 / 8;
		}
		for(int i = 12; i < 20; i ++)
			NowRatio[i] = ( 92 - floor(hosi5denai / 5) * 0.5) / 92 * BasicRatio[i];
		}
	if(hosi5denai == 0){
		memcpy(NowRatio,BasicRatio,sizeof(BasicRatio));
	}
	
}

