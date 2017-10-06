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
char *EdBM_name2_s = "threshold2.csv";

char *EdBM_name1t_s = "Anglet.csv";
char *EdBM_name2t_s = "threshold2t.csv";

char inputAngle_directory[255];

char inputAngle_deta[128];
char inputAnglet_deta[128];
char inputthreshold2_deta[128];
char inputthreshold2t_deta[128];

//std::tuple<int, int, std::vector<std::vector<double>>>read_csv(const char *filename);

int Edge_detection_Block_Matching(char date_directory[], int &image_x, int &image_y, int &image_xt, int &image_yt, int paramerter[], int paramerter_count, int sd, char date[]) {
	printf("****************************************\n");
	printf("start：Edge-detection_Block_Matching\n");
	printf("****************************************\n");

	//Nrutilを用いたメモリの確保
	double **Angle = matrix(0, image_x - 1, 0, image_y - 1);
	double **threshold2 = matrix(0, image_x - 1, 0, image_y - 1);
	double **Anglet = matrix(0, image_xt - 1, 0, image_y - 1);
	double **threshold2t = matrix(0, image_xt - 1, 0, image_yt - 1);

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
		}
	}
	
	//inputデータの選択(cossimのみ)
	switch (paramerter[0]){
	case 3:
		sprintf(inputAngle_directory, "%s%dk_cossim_sd%d\\", date_directory, paramerter[paramerter_count], sd);
		break;
	case 4:
	case 5:
		sprintf(inputAngle_directory, "%s%d×%dsobel_cossim_sd%d\\", date_directory, paramerter[paramerter_count], paramerter[paramerter_count], sd);
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



	//メモリの解放
	free_matrix(Angle, 0, image_x - 1, 0, image_y - 1);
	free_matrix(threshold2, 0, image_x - 1, 0, image_y - 1);
	free_matrix(Anglet, 0, image_xt - 1, 0, image_yt - 1);
	free_matrix(threshold2t, 0, image_xt - 1, 0, image_yt - 1);

	return 0;
}