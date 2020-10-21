//各レアリティの初期確率
#define PU5RATES 0.03
#define NORMAL5RATES 0.03
#define PU4RATES 0.03
#define NORMAL4RATES 0.55
#define NORMAL3RATES 0.36

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

double FloatRand(); //0~1の小数を発生させる乱数
int chusen(double ratio[]); //1回のガチャ抽選の結果を返す関数
void dainyu(void); //キャラの数から確率に変換する
void Turn(void); //召喚1ターン(最大抽選5回)
void Shokika(void); // 初期化

	/* 
	引く色の指定
	例えば{1,3,4,1}なら、赤と白を開け、どちらもなければ青、それもなければ緑
	*/
	int color[4] = {3,1,1,4};
	
	// 各レアリティ各色のキャラ個数を書いた配列
	int PU5[4] = {1,1,1,1};
	int hosi5[4] = {31,25,22,18};
	int PU4[4] = {0,0,0,1};
	int hosi4[4] = {33,35,25,37};
	int hosi3[4] = {33,34,25,37};
	
	/*
	確率配列
	00 PU赤,　 01 PU青,  02 PU緑,   03 PU白,
	04 PU赤,　 05 PU青,  06 PU緑,   07 PU白,
	08 PU赤,　 09 PU青,  10 PU緑,   11 PU白,
	12 星5赤,  13 星5青,  14 星5緑,  15 星5白,
	16 4PU赤,  17 4PU青,  18 4PU緑,  19 4PU白,
	20 星4赤,  21 星4青,  22 星4緑,  23 星4白,
	24 星3赤,  25 星3青,  26 星3緑,  27 星3白,
	*/
	double BasicRatio[28];
	double NowRatio[28];
	
	int kekka[28]; //各レアリティの出た体数を格納
	int orb; //消費オーブ数
	int hosi5denai;

int main(void){
	dainyu();
	Shokika();
	
	//期待値求め
	while((kekka[1] < 1) || (kekka[2] < 1))
	{
		Turn();
	}
	printf("消費オーブ : %d\n",orb);
	printf("赤ピックアップ : %d\n",kekka[0]);
	printf("ニニチキ : %d\n",kekka[1]);
	printf("ギム子 : %d\n",kekka[2]);
	printf("白ピックアップ : %d\n",kekka[3]);
	printf("赤すり抜け : %d\n",kekka[12]);
	printf("青すり抜け : %d\n",kekka[13]);
	printf("緑すり抜け : %d\n",kekka[14]);
	printf("白すり抜け : %d\n",kekka[15]);
	printf("☆4ピックアップ : %d\n",kekka[19]);
	printf("赤☆4☆3 : %d\n",kekka[20] + kekka[24]);
	printf("青☆4☆3 : %d\n",kekka[21] + kekka[24]);
	printf("緑☆4☆3 : %d\n",kekka[22] + kekka[26]);
	printf("白☆4☆3 : %d\n",kekka[23] + kekka[27]);
	
	//オーブの消費固定で出る確率を求める時
	/*
	int deta = 0;
	int denai = 0;
	for(int i = 0; i < 10000; i ++)
	{
		while(orb < 100){
			Turn();
		}
		if(kekka[1] + kekka[2] < 1){
			denai ++;
		}else {
			deta ++;
		}
		Shokika();
	}
	printf("出た : %d\n",deta);
	printf("出ない : %d\n",denai);
	*/
	
	return 0;
}
	
double FloatRand(){
	double num = (double)rand() / RAND_MAX;
	return num;
}

void dainyu(void)
{
	// 個数から確率に変換して代入
	for(int i = 0; i < 4; i ++){
		//星5PUの代入
		if(PU5[i] >= 1){
			BasicRatio[i] = PU5RATES / (PU5[0]+PU5[1]+PU5[2]+PU5[3]);
		}
		if (PU5[i] >= 2){
			BasicRatio[i + 4] = PU5RATES / (PU5[0]+PU5[1]+PU5[2]+PU5[3]);
		}
		if (PU5[i] == 3){
			BasicRatio[i + 8] = PU5RATES / (PU5[0]+PU5[1]+PU5[2]+PU5[3]);
		}
		//星4PUの代入
		for(int i = 0; i < 4; i ++){
			BasicRatio[i + 16] = PU4RATES * PU4[i];
		}
		//恒常の星5,4,3の代入
		BasicRatio[i + 12] = NORMAL5RATES * (double)hosi5[i] / (hosi5[0] + hosi5[1] + hosi5[2] + hosi5[3]);
		BasicRatio[i + 20] = NORMAL4RATES * (double)hosi4[i] / (hosi4[0] + hosi4[1] + hosi4[2] + hosi4[3]);
		BasicRatio[i + 24] = NORMAL3RATES * (double)hosi3[i] / (hosi3[0] + hosi3[1] + hosi3[2] + hosi3[3]);
	}
}

void Shokika(void)
{
	memcpy(NowRatio,BasicRatio,sizeof(BasicRatio));
	hosi5denai = 0;
	orb = 0;
	memset(kekka, 0, sizeof(kekka));
}

int chusen(double ratio[]) //引数は確率テーブル
{
	int result;
	double rnd = FloatRand();
	int i;
	for(i = 0 ; i < 28 ; i ++)
	{
		if(rnd < ratio[i]){
			result = i;
			break;
		}
		else
			rnd -= ratio[i];
	}
	return result; // 戻り値は、出たレアリティの数字(0~27)
}

void Turn(void)
{
	int or = 0;  //このターンで引いたキャラ数と星5が出たかを判定するための変数。10の位が星5の数,1の位が引いたキャラ数
	int FalRslt[5]; //目当色で無かった場合にその結果を格納する(False Result の略)
	for(int i = 0; i < 5; i ++)
	{
		int result = chusen(NowRatio); 
		// 目当ての色かどうかの判定
		if(color[result % 4] == 1){
			kekka[result] += 1;
			// 星5かどうかの判定
			if(result / 4 < 4 ){
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
	else if(or == 0){
		// kは色を、jは召喚する物体を
		for(int k = 2; k <= 4; k ++){
			for(int j = 0; j <= 4; j ++){
				if(color[FalRslt[j] % 4] == k){
					kekka[FalRslt[j]] += 1;
					orb += 5;
					if(FalRslt[j] / 4 < 4 ){
						hosi5denai = 0;
					} else{
						hosi5denai += 1;
					}
					goto OUT;
				}
			}
		}
	}
	OUT:;
	// ここに確率上昇を書く
	if ((4 < hosi5denai) && (hosi5denai < 120)){
		for(int i = 0; i < 12; i ++){
			NowRatio[i] = (floor(hosi5denai / 5) * 0.0025 + PU5RATES) / PU5RATES * BasicRatio[i];
			NowRatio[i + 16] = ((PU4RATES + NORMAL4RATES + NORMAL3RATES) - floor(hosi5denai / 5) * 0.005) / (PU4RATES + NORMAL4RATES + NORMAL3RATES) * BasicRatio[i + 16];
		}
		for(int i = 12; i < 16; i ++){
			NowRatio[i] = (NORMAL5RATES + floor(hosi5denai / 5) * 0.0025) / NORMAL5RATES * BasicRatio[i];
		}
	}
	if (hosi5denai == 0){
		memcpy(NowRatio,BasicRatio,sizeof(BasicRatio));
	}
	if (hosi5denai >= 120)
	{
		for(int i = 0; i < 16; i ++){
			NowRatio[i] = BasicRatio[i] / (PU5RATES + NORMAL5RATES);
		}
		for(int i = 16; i < 28; i ++){
			NowRatio[i] = 0;
		}
	}
}

