#include <string>
#include<sstream> //文字ストリーム
#include<fstream>
#include<iostream>
#include <windows.h>
#include <stdio.h>
#include <vector>

#include <tuple>
#include <direct.h>//フォルダを作成する

using namespace std;

//メモリ確保を行うためのヘッダ
#define ANSI				
#include "nrutil.h"	

char *EdBM_name1_s = "Angle.csv";
//char *EdBM_name2_s = "threshold2.csv";
char *EdBM_name2_s = "edge_st.csv";

char *EdBM_name1t_s = "Anglet.csv";
//char *EdBM_name2t_s = "threshold2t.csv";
char *EdBM_name2t_s = "edge_st_t.csv";

char inputAngle_directory[255];

char inputAngle_deta[128];
char inputAnglet_deta[128];
char inputthreshold2_deta[128];
char inputthreshold2t_deta[128];

//std::tuple<int, int, std::vector<std::vector<double>>>read_csv(const char *filename);
int write_frame(char date_directory[], char Inputiamge[], int max_x, int max_y, int image_xt, int image_yt);

int Edge_detection_Block_Matching(char date_directory[], int &image_x, int &image_y, int &image_xt, int &image_yt, int paramerter[], int paramerter_count, int sd, char date[],int Bs, double threshold_EdBM, char Inputiamge[], double &threshold_otsu) {
	printf("\n****************************************\n");
	printf("start：Edge-detection_Block_Matching\n");
	printf("****************************************\n");
	
	//Nrutilを用いたメモリの確保
	double **Angle = matrix(0, image_x - 1, 0, image_y - 1);
	double **threshold2 = matrix(0, image_x - 1, 0, image_y - 1);
	double **Anglet = matrix(0, image_xt - 1, 0, image_y - 1);
	double **threshold2t = matrix(0, image_xt - 1, 0, image_yt - 1);

	double **threshold_tMAP = matrix(0, image_xt - 1, 0, image_yt - 1);

	//ブロックの数
	int M, N;
	M = image_xt / Bs;
	N = image_yt / Bs;

	int bm = 0;
	int bn = 0;

	double **CB = matrix(0, image_x - image_xt- 1, 0, image_y - image_yt - 1);
	double **V = matrix(0, image_x - image_xt - 1, 0, image_y - image_yt - 1);

	double min_CB;
	int min_x, min_y;
	

	//確保したメモリを初期化する
	for (int i = 0; i < image_y; i++) {
		for (int j = 0; j < image_x; j++) {
			Angle[j][i] = 0;
			threshold2[j][i] = 0;
		}
	}
	for (int i = 0; i < image_yt; i++) {
		for (int j = 0; j < image_xt; j++) {
			Anglet[j][i] = 0;
			threshold2t[j][i] = 0;
			threshold_tMAP[j][i] = 0;
		}
	}

	for (int i = 0; i < image_y - image_yt; i++) {
		for (int j = 0; j < image_x - image_xt; j++) {
			CB[j][i] = 0;
			V[j][i] = 0;
		}
	}

	
	
	//inputデータの選択(cossimのみ)
	switch (paramerter[0]){
	case 3:
		sprintf(inputAngle_directory, "%s%dk_cossim_sd%d\\", date_directory, paramerter[paramerter_count], sd);
		break;
	case 4:
	case 5:
		sprintf(inputAngle_directory, "%s%d×%dsobel_atan_sd%d\\", date_directory, paramerter[paramerter_count], paramerter[paramerter_count], sd);
		break;
	}

	//Inputファイルのディレクトリ設定
	sprintf(inputAngle_deta, "%s%s", inputAngle_directory, EdBM_name1_s);
	sprintf(inputAnglet_deta, "%s%s", inputAngle_directory, EdBM_name1t_s);
	sprintf(inputthreshold2_deta, "%s%s", inputAngle_directory, EdBM_name2_s);
	sprintf(inputthreshold2t_deta, "%s%s", inputAngle_directory, EdBM_name2t_s);

	//Inputファイルを開く
	ifstream Angle_f(inputAngle_deta);
	ifstream Anglet_f(inputAnglet_deta);
	ifstream threshold2_f(inputthreshold2_deta);
	ifstream threshold2t_f(inputthreshold2t_deta);

	if (!Angle_f) { printf("入力エラー Angle.csvがありません_Edge-detection_Block_Matching_filename=%s", inputAngle_deta); return 1; }
	if (!Anglet_f) { printf("入力エラー Anglet.csvがありません_Edge-detection_Block_Matching_filename=%s", inputAnglet_deta); return 1; }
	if (!threshold2_f) { printf("入力エラー threshold2.csvがありません_Edge-detection_Block_Matching_filename=%s", inputthreshold2_deta); return 1; }
	if (!threshold2t_f) { printf("入力エラー threshold2t.csvがありません_Edge-detection_Block_Matching_filename=%s", inputthreshold2t_deta); return 1; }
	
	int count_large = 0;
	int count_small = 0;
/////////データの読み込み(対象画像)////////////////////////////////////////////////////////////////////////
	int i = 1;
	printf("対象画像の結果を読み取ります\n");
	string str_Angle, str_threshold2;
	count_large = 0;
	while (getline(Angle_f, str_Angle)) {					//このループ内ですべてを行う
		count_small = 0;			//初期化
///////////////いろいろ定義．ここでやらないといけない///////////////////////////////////////////////////////////////////////////
		string token_Angle_f;
		istringstream stream_Angle(str_Angle);

		getline(threshold2_f, str_threshold2);	string token_threshold2_f;	istringstream stream_threshold2(str_threshold2);

//////////////////////////配列に代入//////////////////////////////////////////////////////////////////////////////////////////////////
		while (getline(stream_Angle, token_Angle_f, ',')) {	//一行読み取る．V0のみ，繰り返しの範囲指定に用いる
			double tmp_Angle = stof(token_Angle_f);			//文字を数字に変換
			Angle[count_small][count_large] = tmp_Angle;				//配列に代入

			getline(stream_threshold2, token_threshold2_f, ',');
			double tmp_threshold2 = stof(token_threshold2_f);			//文字を数字に変換
			threshold2[count_small][count_large] = tmp_threshold2;				//配列に代入
			++count_small;
			}
			++count_large;
		}
/////////データの読み込み(テンプレート画像)////////////////////////////////////////////////////////////////////////
	i = 1;
	printf("テンプレート画像の結果を読み取ります\n");
	string str_Anglet, str_threshold2t;
	count_large = 0;
	while (getline(Anglet_f, str_Anglet)) {					//このループ内ですべてを行う
		count_small = 0;			//初期化
									///////////////いろいろ定義．ここでやらないといけない///////////////////////////////////////////////////////////////////////////
		string token_Anglet_f;
		istringstream stream_Anglet(str_Anglet);

		getline(threshold2t_f, str_threshold2t);	string token_threshold2t_f;	istringstream stream_threshold2t(str_threshold2t);

//////////////////////////配列に代入//////////////////////////////////////////////////////////////////////////////////////////////////
		while (getline(stream_Anglet, token_Anglet_f, ',')) {	//一行読み取る．V0のみ，繰り返しの範囲指定に用いる
			double tmp_Anglet = stof(token_Anglet_f);			//文字を数字に変換
			Anglet[count_small][count_large] = tmp_Anglet;				//配列に代入

			getline(stream_threshold2t, token_threshold2t_f, ',');
			double tmp_threshold2t = stof(token_threshold2t_f);			//文字を数字に変換
			threshold2t[count_small][count_large] = tmp_threshold2t;				//配列に代入
			++count_small;
		}
		++count_large;
	}

///////////////Edge-detection_Block_Matching//////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	//arctanの場合は判別分析法で求めた閾値を用いる
	//if(paramerter[0]==4 || paramerter[0] == 5)threshold_EdBM = threshold_otsu;

	threshold_EdBM = threshold_otsu;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//thresholdのflag
	for (int i = 0; i < image_yt; i++) {
		for (int j = 0; j < image_xt; j++) {
			if (threshold2t[j][i] >= threshold_EdBM) {

				threshold_tMAP[j][i] = 1;

				if(j>0 && i>0)threshold_tMAP[j - 1][i - 1] = 1;
				if (i>0)threshold_tMAP[j][i - 1] = 1;
				if (j<image_xt - 1 && i>0)threshold_tMAP[j + 1][i - 1] = 1;

				if(j>0)threshold_tMAP[j - 1][i] = 1;
				if (j<image_xt - 1)threshold_tMAP[j + 1][i] = 1;

				if(j>0 && i<image_yt-1)threshold_tMAP[j - 1][i + 1] = 1;
				if (i < image_yt - 1)threshold_tMAP[j][i + 1] = 1;
				if(j<image_xt - 1 && i<image_yt - 1)threshold_tMAP[j + 1][i + 1] = 1;

			}
		}
	}

//CB
	
	//特定のブロックについて
	for (int n = 0; n < N; n++) {
		for (int m = 0; m < M; m++) {

			bm = Bs*m;
			bn = Bs*n;
			min_CB = 0;
			min_x = 0;
			min_y = 0;
			for (int y = 0; y < image_y - image_yt; y++) {
				for (int x = 0; x < image_x - image_xt; x++) {
					CB[x][y] = 0;
				}
			}


			//座標x,yを比べる
			for (int y = 0; y < image_y - image_yt; y++) {
				for (int x = 0; x < image_x - image_xt; x++) {

					for (int k = 0; k < Bs; k++) {
						for (int l = 0; l < Bs; l++) {
							if (threshold_tMAP[bm + l][bn + k] = 1) {
								CB[x][y] += abs(Angle[x + bm + l][y + bn + k] - Anglet[bm + l][bn + k]);
							}
							else {
								CB[x][y] += 0;
							}

						}
					}

				}
			}
			
			//m,nについてCBが最小となるx,yを求める
			min_CB = CB[0][0];
			for (int y = 0;  y< image_y - image_yt; y++) {
				for (int x = 0; x < image_x - image_xt; x++) {
					if (CB[x][y] < min_CB) {
						min_CB = CB[x][y];
						min_x = x;
						min_y = y;
					}
				}
			}
			//printf("x=%d,y=%d,%f\n", min_x, min_y,min_CB);
			V[min_x][min_y] += 1;
			
		}
	}

	double max_V;
	int max_x=0, max_y=0;
	max_V = V[0][0];
	for (int y = 0; y < image_y - image_yt; y++) {
		for (int x = 0; x < image_x - image_xt; x++) {
			
			if (V[x][y] > max_V) {
				max_V = V[x][y];
				max_x = x;
				max_y = y;
				//printf("V[%d][%d]=%f,", x, y, V[x][y]);
			}

		}
	}

	printf("max_x=%d,max_y=%d\n", max_x, max_y);
	

	write_frame(date_directory, Inputiamge, max_x, max_y, image_xt, image_yt);

//////////////////////////////logの作成///////////////////////////////////////////////////////////////////////////////////
	FILE *fp_date_Matching;
	char filename_log_Matching[128];
	//sprintf(filename_log, "..\\log\\log-%2d-%02d%02d-%02d%02d%02d.txt",pnow->tm_year+1900,pnow->tm_mon + 1,pnow->tm_mday,pnow->tm_hour,pnow->tm_min,pnow->tm_sec);	//logファイル作成のディレクトリ指定
	sprintf(filename_log_Matching, "%s\\log_Matching.txt", date_directory);	//logファイル作成のディレクトリ指定
	if ((fp_date_Matching = fopen(filename_log_Matching, "w")) == NULL) { printf("logファイルが開けません"); exit(1); }
	fprintf(fp_date_Matching, "領域の座標：(x,y)=(%d,%d),(%d,%d)\n", max_x, max_y, max_x+ image_xt, max_y+ image_yt);
	fprintf(fp_date_Matching, "ブロックサイズ：x=%d,y=%d\n", image_xt, image_yt); 
	fprintf(fp_date_Matching, "threshold_otsu=%lf\n", threshold_otsu);
	if (threshold_EdBM == threshold_otsu) {
		fprintf(fp_date_Matching, "判別分析法を用いた：threshold_otsu=%lf\n", threshold_otsu);
	}
	else {
		fprintf(fp_date_Matching, "自分で設定した閾値を用いた：threshold_EdBM=%lf\n", threshold_EdBM);
	}
	
	fclose(fp_date_Matching);
	printf("logファイル %s を保存しました\n", filename_log_Matching);


	//メモリの解放
	free_matrix(Angle, 0, image_x - 1, 0, image_y - 1);
	free_matrix(threshold2, 0, image_x - 1, 0, image_y - 1);
	free_matrix(Anglet, 0, image_xt - 1, 0, image_yt - 1);
	free_matrix(threshold2t, 0, image_xt - 1, 0, image_yt - 1);
	free_matrix(threshold_tMAP, 0, image_xt - 1, 0, image_yt - 1);
	free_matrix(CB, 0, image_x - image_xt - 1, 0, image_y - image_yt - 1);
	free_matrix(V, 0, image_x - image_xt - 1, 0, image_y - image_yt - 1);

	
	return 0;
}