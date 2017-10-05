
#include<fstream>
#include<iostream>
#include<string>
#include<sstream> //文字ストリーム
#include<math.h>
#include<time.h>//時間を用いる
#include <direct.h>//フォルダを作成す
#include<stdio.h>
#include <windows.h>

int cos_eco_mode_flag = 0;

//メモリ確保を行うためのヘッダ
#define ANSI				
#include "nrutil.h"	

#define PI 3.14159265

using namespace std;

char date_directory3[128];

int i,j;

//////////////////定義///////////////////////////////////////////////////////////////////////////////////////////
	double Rvector[10][10];
	double V0[255];
	double V45[255];
	double V90[255];
	double V135[255];
	double V180[255];
	double V225[255];
	double V270[255];
	double V315[255];

	char inputdate_directory[255];					//inputdateの場所
	char inputrvector_directory[128];				//Rvectorの場所

	char *Input_Rvectormagniname_s = "\\Rvector_magni.csv";	//基準ベクトルの倍率
	char *Input_Rvectorname1_s = "\\Rvector0.csv";			//基準ベクトル名の指定
	char *Input_Rvectorname2_s = "\\Rvector45.csv";
	char *Input_Rvectorname3_s = "\\Rvector90.csv";
	char *Input_Rvectorname4_s = "\\Rvector135.csv";
	char *Input_Rvectorname5_s = "\\Rvector180.csv";
	char *Input_Rvectorname6_s = "\\Rvector225.csv";
	char *Input_Rvectorname7_s = "\\Rvector270.csv";
	char *Input_Rvectorname8_s = "\\Rvector315.csv";
	

	char *Input_Filename1_s = "\\V(0).csv";			//入力ファイル名の指定
	char *Input_Filename2_s = "\\V(45).csv";
	char *Input_Filename3_s = "\\V(90).csv";
	char *Input_Filename4_s = "\\V(135).csv";
	char *Input_Filename5_s = "\\V(180).csv";
	char *Input_Filename6_s = "\\V(225).csv";
	char *Input_Filename7_s = "\\V(270).csv";
	char *Input_Filename8_s = "\\V(315).csv";
	
	char *math_name1_s = "innerp.csv";				//出力結果のファイル名の指定
	char *math_name2_s = "V_sqrt.csv";
	char *math_name3_s = "Cos_similarity.csv";
	char *math_name4_s = "Angle.csv";
	char *math_name5_s = "threshold_high.csv";
	char *math_name6_s = "use_Rvector_flag.csv";
	char *math_name7_s = "use_Rvector_number.csv";
	char *math_name8_s = "threshold2.csv";
	char *math_name9_s = "threshold_low.csv";
	char *math_name10_s = "threshold3.csv";
	char *math_name11_s = "use_Rvector_number_LC.csv";

	char Input_Rvectormagni_name[255];
	char Input_Rvectorname1[250];					//基準ベクトル名・基準ベクトルの入力先の設定
	char Input_Rvectorname2[250];
	char Input_Rvectorname3[250];
	char Input_Rvectorname4[250];
	char Input_Rvectorname5[250];
	char Input_Rvectorname6[250];
	char Input_Rvectorname7[250];
	char Input_Rvectorname8[250];

	char Input_Filename1[255];						//入力ファイル名・入力元の設定
	char Input_Filename2[255];
	char Input_Filename3[255];
	char Input_Filename4[255];
	char Input_Filename5[255];
	char Input_Filename6[255];
	char Input_Filename7[255];
	char Input_Filename8[255];
													//出力ファイル名・出力先の設定
	char math_name1[128];							//内積						
	char math_name2[128];							//応答電圧Vの大きさ
	char math_name3[128];							//cos類似度
	char math_name4[128];							//角度．最終結果
	char math_name5[128];							//threshold_high
	char math_name6[128];							//use_Rvector_flag(応答電圧の最小の位置)
	char math_name7[128];							//use_Rvector_number(内積でしようする基準ベクトルの番号）
	char math_name8[128];							//2つの閾値を一つに
	char math_name9[128];							//threshold(use_Rvector_flagの応答電圧Vの大きさ）
	char math_name10[128];							//局所連結性を考慮したthreshold(use_Rvector_flagの応答電圧Vの大きさ）
	char math_name11[128];							//局所連結性を確認したときのuse_Rvector_number

	double Rvectormagni[10];						//基準ベクトルの倍率		
	double Rvector_sqrt[9];							//Rvectorの大きさ
	double Rvector_square[9][9];
	double Rvector_square_sum[9];
	int use_Rvector_flag;
	int use_Rvector_number;
	//double threshold_high;
	//double threshold_low;
	double threshold_low_abs;
	double threshold_high_abs;

	int direction_number=8;					//8方向，4方向，2方向の指定
	int curcuit_number=8;						//使用する回路
	int Rvector_number=8;						//使用する基準ベクトルの数
	int minor_flag=0;							//十字以外の変な基準ベクトルを取るときに1にする

	ifstream Rvector_0;
	ifstream Rvector_45;
	ifstream Rvector_90;
	ifstream Rvector_135;
	ifstream Rvector_180;
	ifstream Rvector_225;
	ifstream Rvector_270;
	ifstream Rvector_315;

	FILE *fp_innerp,*fp_V_sqrt,*fp_Cos_similarity,*fp_Angle,*fp_use_Rvector_flag,*fp_use_Rvector_number,*fp_threshold,*fp_threshold_high,*fp_threshold2,*fp_threshold3,*fp_threshold_number_LC;

	void Rvector_read();
	void Read_output();
	int local_connectivity(int image_x,int image_y,double *local_flag[],double *threshold_LC_number[]);

int cossim(char date_directory[],int &image_x,int &image_y,int paramerter[],int paramerter_count,int sd,char date[]){
	printf("****************************************\n");
	printf("start： cos-sim\n");
	printf("****************************************\n");
	//FILE *fp_newfunction,*fp_Angle2,,*fp_Correction_flag;									//出力増やす用3

	int i=1,j=1;
	int count_small=0,count_large=0;
	int count_property=0;
	int count_Allproperty=0;

/////////////////////////////////初期設定 : input設定//////////////////////////////////////////////////////////////////

	//Nrutilを用いたメモリの確保
	double **threshold2 = matrix(0, image_x - 1, 0, image_y - 1);
	double **threshold3 = matrix(0, image_x - 1, 0, image_y - 1);
	double **threshold_high = matrix(0, image_x - 1, 0, image_y - 1);
	double **threshold_low = matrix(0, image_x - 1, 0, image_y - 1);
	double **V0 = matrix(0, image_x-1, 0, image_y-1);
	double **V45 = matrix(0, image_x-1, 0, image_y-1);
	double **V90 = matrix(0, image_x-1, 0, image_y-1);
	double **V135 = matrix(0, image_x-1, 0, image_y-1);
	double **V180 = matrix(0, image_x-1, 0, image_y-1);
	double **V225 = matrix(0, image_x-1, 0, image_y-1);
	double **V270 = matrix(0, image_x-1, 0, image_y-1);
	double **V315 = matrix(0, image_x-1, 0, image_y-1);
	//double **use_Rvector_flag = matrix(0, image_x-1, 0, image_y-1);
	double **threshold_low_flag = matrix(0, image_x-1, 0, image_y-1);
	double **threshold_high_flag = matrix(0, image_x-1, 0, image_y-1);
	double **V_sqrt = matrix(0, image_x-1, 0, image_y-1);
	double **Cos_similarity = matrix(0, image_x-1, 0, image_y-1);
	double **Angle = matrix(0, image_x-1, 0, image_y-1);
	double **innerp = matrix(0, image_x-1, 0, image_y-1);
	double **threshold_local_flag = matrix(0, image_x - 1, 0, image_y - 1);

	
	
	//確保したメモリを初期化する
			for (i = 0; i < image_y; i++) {
				for (j = 0; j < image_x; j++) {
					threshold_high[j][i] = 0;
					threshold_low[j][i] = 0;
					threshold2[j][i] = 0;
					threshold3[j][i] = 0;
					V0[j][i]=0;
					V45[j][i]=0;
					V90[j][i]=0;
					V135[j][i]=0;
					V180[j][i]=0;
					V225[j][i]=0;
					V270[j][i]=0;
					V315[j][i]=0;
					threshold_low_flag[j][i]=0;
					threshold_high_flag[j][i]=0;
					V_sqrt[j][i]=0;
					Cos_similarity[j][i]=0;
					Angle[j][i]=0;
					innerp[j][i]=0;
					threshold_local_flag[j][i] = 0;
				}
			}
	
			if(paramerter[0]==1 || paramerter[0]==2){
				sprintf(inputdate_directory,"%s%d×%dsobel_conv_sd%d",date_directory,paramerter[paramerter_count],paramerter[paramerter_count],sd);	//入力する畳み込み結果の名前
			}else{
				sprintf(inputdate_directory,"%s%dk_conv_sd%d",date_directory,paramerter[paramerter_count],sd);	//入力する畳み込み結果の名前
			}
	
	
	printf("inputdata：%s\n",inputdate_directory);

////////////////////////////入出力ディレクトリの作成//////////////////////////////////////////////////////////////////////////////
	
	//Inputファイルのディレクトリ設定
	sprintf(Input_Filename1,"%s%s",inputdate_directory,Input_Filename1_s);
	sprintf(Input_Filename2,"%s%s",inputdate_directory,Input_Filename2_s);
	sprintf(Input_Filename3,"%s%s",inputdate_directory,Input_Filename3_s);
	sprintf(Input_Filename4,"%s%s",inputdate_directory,Input_Filename4_s);
	sprintf(Input_Filename5,"%s%s",inputdate_directory,Input_Filename5_s);
	sprintf(Input_Filename6,"%s%s",inputdate_directory,Input_Filename6_s);
	sprintf(Input_Filename7,"%s%s",inputdate_directory,Input_Filename7_s);
	sprintf(Input_Filename8,"%s%s",inputdate_directory,Input_Filename8_s);

	//Outputディレクトリの設定
	if(paramerter[0]==1 || paramerter[0]==2){
		sprintf(date_directory3, "%s%d×%dsobel_cossim_sd%d",date_directory, paramerter[paramerter_count],paramerter[paramerter_count],sd);	
			}else{
		sprintf(date_directory3, "%s%dk_cossim_sd%d",date_directory, paramerter[paramerter_count],sd);
	}
	//Outputディレクトリの作成
	if (_mkdir(date_directory3) == 0) {
		printf("フォルダ %s を作成しました\n", date_directory3);
	}
	else {
		printf("フォルダ作成に失敗しました。\n");
	}
	
////////////////////////////ファイルの読み込み//////////////////////////////////////////////////////////////////////////////
	ifstream Rvector_magni();
	//Inputファイルを開く
	ifstream V_0(Input_Filename1);
	ifstream V_45(Input_Filename2);
	ifstream V_90(Input_Filename3);
	ifstream V_135(Input_Filename4);
	ifstream V_180(Input_Filename5);
	ifstream V_225(Input_Filename6);
	ifstream V_270(Input_Filename7);
	ifstream V_315(Input_Filename8);
	

////////////////////////エラー出力/////////////////////////////////////////////////////////////////////////////////////////////
	
	if(!V_0){printf("入力エラー V(0).csvがありません_cos-sim\nInput_Filename=%s",Input_Filename1);return 1;}
	if(!V_45){printf("入力エラー V(45).csvがありません_cos-sim\nInput_Filename=%s",Input_Filename2);return 1;}
	if(!V_90){printf("入力エラー V(90).csvがありません_cos-sim\nInput_Filename=%s",Input_Filename3);return 1;}
	if(!V_135){printf("入力エラー V(135).csvがありません_cos-sim\nInput_Filename=%s",Input_Filename4);return 1;}
	if(!V_180){printf("入力エラー V(180).csvがありません_cos-sim\nInput_Filename=%s",Input_Filename5);return 1;}
	if(!V_225){printf("入力エラー V(225).csvがありません_cos-sim\nInput_Filename=%s",Input_Filename6);return 1;}
	if(!V_270){printf("入力エラー V(270).csvがありません_cos-sim\nInput_Filename=%s",Input_Filename7);return 1;}
	if(!V_315){printf("入力エラー V(315).csvがありません_cos-sim\nInput_Filename=%s",Input_Filename8);return 1;}
	
/////////////////////////出力ファイルを開く///////////////////////////////////////////////////////////////////////////////////

	if(sd==0){
		//Outputディレクトリの設定
		if(paramerter[0]==1 || paramerter[0]==2){
			sprintf(inputrvector_directory,"..\\Rvector\\%d×%dsobel_under",paramerter[paramerter_count],paramerter[paramerter_count]);
		}else{
			sprintf(inputrvector_directory,"..\\Rvector\\8dire_%dk_0203",paramerter[paramerter_count]);
		}

		//基準ベクトルの読み込み
		Rvector_read();
		printf("基準ベクトルを読み込みました");
		
		for(i=1;i<=8;i++){
		for(j=1;j<=8;j++){
			printf("%lf, ",Rvector[i][j]);
		}
		printf("\n");
		}
	}

	//出力するファイルを開く
	Read_output();

  
///////////////////////応答電圧のcsvの読み込み//////////////////////////////////////////////////////////////////////////////////////////	
		i=1;
		printf("応答電圧を読み取ります\n");
		string str_0,str_45,str_90,str_135,str_180,str_225,str_270,str_315;
		count_large=0;
		while(getline(V_0,str_0)){					//このループ内ですべてを行う
			count_small=0;			//初期化

///////////////いろいろ定義．ここでやらないといけない///////////////////////////////////////////////////////////////////////////
        string token_V_0;
        istringstream stream_V_0(str_0);
		
		getline(V_45,str_45);	string token_V_45;	istringstream stream_V_45(str_45);
		getline(V_90,str_90);	string token_V_90;	istringstream stream_V_90(str_90);
		getline(V_135,str_135);	string token_V_135;	istringstream stream_V_135(str_135);
		getline(V_180,str_180);	string token_V_180;	istringstream stream_V_180(str_180);
		getline(V_225,str_225);	string token_V_225;	istringstream stream_V_225(str_225);
		getline(V_270,str_270);	string token_V_270;	istringstream stream_V_270(str_270);
		getline(V_315,str_315);	string token_V_315;	istringstream stream_V_315(str_315);
	
//////////////////////////配列に代入//////////////////////////////////////////////////////////////////////////////////////////////////

        while(getline(stream_V_0,token_V_0,',')){	//一行読み取る．V0のみ，繰り返しの範囲指定に用いる
			double tmp_V_0=stof(token_V_0);			//文字を数字に変換
			V0[count_small][count_large]=tmp_V_0;				//配列に代入
			V0[count_small][count_large]=Rvectormagni[1]*V0[count_small][count_large];

			getline(stream_V_45,token_V_45,',');
			double tmp_V_45=stof(token_V_45);
			V45[count_small][count_large]=tmp_V_45;
			V45[count_small][count_large]=Rvectormagni[2]*V45[count_small][count_large];
        
			getline(stream_V_90,token_V_90,',');
			double tmp_V_90=stof(token_V_90);
			V90[count_small][count_large]=tmp_V_90;
			V90[count_small][count_large]=Rvectormagni[3]*V90[count_small][count_large];

			getline(stream_V_135,token_V_135,',');
			double tmp_V_135=stof(token_V_135);
			V135[count_small][count_large]=tmp_V_135;
			V135[count_small][count_large]=Rvectormagni[4]*V135[count_small][count_large];

			getline(stream_V_180,token_V_180,',');
			double tmp_V_180=stof(token_V_180);
			V180[count_small][count_large]=tmp_V_180;
			V180[count_small][count_large]=Rvectormagni[5]*V180[count_small][count_large];

			getline(stream_V_225,token_V_225,',');
			double tmp_V_225=stof(token_V_225);
			V225[count_small][count_large]=tmp_V_225;
			V225[count_small][count_large]=Rvectormagni[6]*V225[count_small][count_large];

			getline(stream_V_270,token_V_270,',');
			double tmp_V_270=stof(token_V_270);
			V270[count_small][count_large]=tmp_V_270;
			V270[count_small][count_large]=Rvectormagni[7]*V270[count_small][count_large];

			getline(stream_V_315,token_V_315,',');
			double tmp_V_315=stof(token_V_315);
			V315[count_small][count_large]=tmp_V_315; 
			V315[count_small][count_large]=Rvectormagni[8]*V315[count_small][count_large];

			++count_small;
			}
		++count_large;
		}
///////////////////ここからしばらく大小判定////////////////////////////////////////////////////////////////////////////////////////////


///////////////////負の閾値判定//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		printf("用いる基準ベクトルの判定を行います\n");

///////////////////////8方向取得のflag(負の閾値)/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		for(count_small=0;count_small<image_x;++count_small){
			for(count_large=0;count_large<image_y;++count_large){
//	if(direction_number==8){		//取得方向を変化させてたい場合アンコメント
			if(V0[count_small][count_large]<=V45[count_small][count_large] && V0[count_small][count_large]<=V90[count_small][count_large] && V0[count_small][count_large]<=V135[count_small][count_large] &&
				V0[count_small][count_large]<=V180[count_small][count_large] && V0[count_small][count_large]<=V225[count_small][count_large] && V0[count_small][count_large]<=V270[count_small][count_large] && V0[count_small][count_large]<=V315[count_small][count_large])
			{threshold_low_flag[count_small][count_large]=1;}			//Vの大小比較

			if(V45[count_small][count_large]<=V0[count_small][count_large] && V45[count_small][count_large]<=V90[count_small][count_large] && V45[count_small][count_large]<=V135[count_small][count_large] &&
				V45[count_small][count_large]<=V180[count_small][count_large] && V45[count_small][count_large]<=V225[count_small][count_large] && V45[count_small][count_large]<=V270[count_small][count_large] && V45[count_small][count_large]<=V315[count_small][count_large])
			{threshold_low_flag[count_small][count_large]=2;}

			if(V90[count_small][count_large]<=V0[count_small][count_large] && V90[count_small][count_large]<=V45[count_small][count_large] && V90[count_small][count_large]<=V135[count_small][count_large] &&
				V90[count_small][count_large]<=V180[count_small][count_large] && V90[count_small][count_large]<=V225[count_small][count_large] && V90[count_small][count_large]<=V270[count_small][count_large] && V90[count_small][count_large]<=V315[count_small][count_large])
			{threshold_low_flag[count_small][count_large]=3;}

			if(V135[count_small][count_large]<=V0[count_small][count_large] && V135[count_small][count_large]<=V45[count_small][count_large] && V135[count_small][count_large]<=V90[count_small][count_large] &&
				V135[count_small][count_large]<=V180[count_small][count_large] && V135[count_small][count_large]<=V225[count_small][count_large] && V135[count_small][count_large]<=V270[count_small][count_large] && V135[count_small][count_large]<=V315[count_small][count_large])
			{threshold_low_flag[count_small][count_large]=4;}

			if(V180[count_small][count_large]<=V0[count_small][count_large] && V180[count_small][count_large]<=V45[count_small][count_large] && V180[count_small][count_large]<=V90[count_small][count_large] &&
				V180[count_small][count_large]<=V135[count_small][count_large] && V180[count_small][count_large]<=V225[count_small][count_large] && V180[count_small][count_large]<=V270[count_small][count_large] && V180[count_small][count_large]<=V315[count_small][count_large])
			{threshold_low_flag[count_small][count_large]=5;}

			if(V225[count_small][count_large]<=V0[count_small][count_large] && V225[count_small][count_large]<=V45[count_small][count_large] && V225[count_small][count_large]<=V90[count_small][count_large] &&
				V225[count_small][count_large]<=V135[count_small][count_large] && V225[count_small][count_large]<=V180[count_small][count_large] && V225[count_small][count_large]<=V270[count_small][count_large] && V225[count_small][count_large]<=V315[count_small][count_large])
			{threshold_low_flag[count_small][count_large]=6;}

			if(V270[count_small][count_large]<=V0[count_small][count_large] && V270[count_small][count_large]<=V45[count_small][count_large] && V270[count_small][count_large]<=V90[count_small][count_large] &&
				V270[count_small][count_large]<=V135[count_small][count_large] && V270[count_small][count_large]<=V180[count_small][count_large] && V270[count_small][count_large]<=V225[count_small][count_large] && V270[count_small][count_large]<=V315[count_small][count_large])
			{threshold_low_flag[count_small][count_large]=7;}

			if(V315[count_small][count_large]<=V0[count_small][count_large] && V315[count_small][count_large]<=V45[count_small][count_large] && V315[count_small][count_large]<=V90[count_small][count_large] &&
				V315[count_small][count_large]<=V135[count_small][count_large] && V315[count_small][count_large]<=V180[count_small][count_large] && V315[count_small][count_large]<=V225[count_small][count_large] && V315[count_small][count_large]<=V270[count_small][count_large])
			{threshold_low_flag[count_small][count_large]=8;}
//	}		//取得方向を変化させてたい場合アンコメント
			}
		}
			
///////////////////////4方向取得のflag(負の閾値)/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//if(direction_number==4){		//取得方向を変化させてたい場合アンコメント
/*				if(minor_flag==0){
					if(V0[count_small][count_large]<=V90[count_small][count_large] &&	V0[count_small][count_large]<=V180[count_small][count_large] && V0[count_small][count_large]<=V270[count_small][count_large])
						{threshold_low_flag[count_small][count_large]=1;}			//Vの大小比較


					if(V90[count_small][count_large]<=V0[count_small][count_large] &&	V90[count_small][count_large]<=V180[count_small][count_large] && V90[count_small][count_large]<=V270[count_small][count_large])
						{threshold_low_flag[count_small][count_large]=3;}


					if(V180[count_small][count_large]<=V0[count_small][count_large] && V180[count_small][count_large]<=V90[count_small][count_large] &&V180[count_small][count_large]<=V270[count_small][count_large])
						{threshold_low_flag[count_small][count_large]=5;}


					if(V270[count_small][count_large]<=V0[count_small][count_large] && V270[count_small][count_large]<=V90[count_small][count_large] && V270[count_small][count_large]<=V180[count_small][count_large])
						{threshold_low_flag[count_small][count_large]=7;}

				}
				if(minor_flag==1){

							if(V0[count_small][count_large]<=V45[count_small][count_large] && V0[count_small][count_large]<=V90[count_small][count_large] && V0[count_small][count_large]<=V135[count_small][count_large])
								{threshold_low_flag[count_small][count_large]=1;}			//Vの大小比較

							if(V45[count_small][count_large]<=V0[count_small][count_large] && V45[count_small][count_large]<=V90[count_small][count_large] && V45[count_small][count_large]<=V135[count_small][count_large])
								{threshold_low_flag[count_small][count_large]=2;}

							if(V90[count_small][count_large]<=V0[count_small][count_large] && V90[count_small][count_large]<=V45[count_small][count_large] && V90[count_small][count_large]<=V135[count_small][count_large])
								{threshold_low_flag[count_small][count_large]=3;}

							if(V135[count_small][count_large]<=V0[count_small][count_large] && V135[count_small][count_large]<=V45[count_small][count_large] && V135[count_small][count_large]<=V90[count_small][count_large])
								{threshold_low_flag[count_small][count_large]=4;}

				}
			}

*/
///////////////////////2方向取得のflag(負の閾値)/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//if(direction_number==2){		//取得方向を変化させてたい場合アンコメント


/*					if(minor_flag==0){
						
						if(V0[count_small][count_large]<=V90[count_small][count_large]){
							threshold_low_flag[count_small][count_large]=1;			//Vの大小比較
						}else{
							threshold_low_flag[count_small][count_large]=3;
						}
					}
					if(minor_flag==1){

						if(V0[count_small][count_large]<=V45[count_small][count_large]){
							threshold_low_flag[count_small][count_large]=1;			//Vの大小比較
						}else{
							threshold_low_flag[count_small][count_large]=2;
						}	
					}
				//else{
					//printf("curcuit_number=%d のエラー",curcuit_number);
				//}
			}
*/
//////////////正の閾値の判定////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////8方向取得のflag(正の閾値)/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			for(count_small=0;count_small<image_x;++count_small){
			for(count_large=0;count_large<image_y;++count_large){
			
//if(direction_number==8){		//取得方向を変化させてたい場合アンコメント
			if(V0[count_small][count_large]>=V45[count_small][count_large] && V0[count_small][count_large]>=V90[count_small][count_large] && V0[count_small][count_large]>=V135[count_small][count_large] &&
				V0[count_small][count_large]>=V180[count_small][count_large] && V0[count_small][count_large]>=V225[count_small][count_large] && V0[count_small][count_large]>=V270[count_small][count_large] && V0[count_small][count_large]>=V315[count_small][count_large])
			{threshold_high_flag[count_small][count_large]=1;}			//Vの大小比較

			if(V45[count_small][count_large]>=V0[count_small][count_large] && V45[count_small][count_large]>=V90[count_small][count_large] && V45[count_small][count_large]>=V135[count_small][count_large] &&
				V45[count_small][count_large]>=V180[count_small][count_large] && V45[count_small][count_large]>=V225[count_small][count_large] && V45[count_small][count_large]>=V270[count_small][count_large] && V45[count_small][count_large]>=V315[count_small][count_large])
			{threshold_high_flag[count_small][count_large]=2;}

			if(V90[count_small][count_large]>=V0[count_small][count_large] && V90[count_small][count_large]>=V45[count_small][count_large] && V90[count_small][count_large]>=V135[count_small][count_large] &&
				V90[count_small][count_large]>=V180[count_small][count_large] && V90[count_small][count_large]>=V225[count_small][count_large] && V90[count_small][count_large]>=V270[count_small][count_large] && V90[count_small][count_large]>=V315[count_small][count_large])
			{threshold_high_flag[count_small][count_large]=3;}

			if(V135[count_small][count_large]>=V0[count_small][count_large] && V135[count_small][count_large]>=V45[count_small][count_large] && V135[count_small][count_large]>=V90[count_small][count_large] &&
				V135[count_small][count_large]>=V180[count_small][count_large] && V135[count_small][count_large]>=V225[count_small][count_large] && V135[count_small][count_large]>=V270[count_small][count_large] && V135[count_small][count_large]>=V315[count_small][count_large])
			{threshold_high_flag[count_small][count_large]=4;}

			if(V180[count_small][count_large]>=V0[count_small][count_large] && V180[count_small][count_large]>=V45[count_small][count_large] && V180[count_small][count_large]>=V90[count_small][count_large] &&
				V180[count_small][count_large]>=V135[count_small][count_large] && V180[count_small][count_large]>=V225[count_small][count_large] && V180[count_small][count_large]>=V270[count_small][count_large] && V180[count_small][count_large]>=V315[count_small][count_large])
			{threshold_high_flag[count_small][count_large]=5;}

			if(V225[count_small][count_large]>=V0[count_small][count_large] && V225[count_small][count_large]>=V45[count_small][count_large] && V225[count_small][count_large]>=V90[count_small][count_large] &&
				V225[count_small][count_large]>=V135[count_small][count_large] && V225[count_small][count_large]>=V180[count_small][count_large] && V225[count_small][count_large]>=V270[count_small][count_large] && V225[count_small][count_large]>=V315[count_small][count_large])
			{threshold_high_flag[count_small][count_large]=6;}

			if(V270[count_small][count_large]>=V0[count_small][count_large] && V270[count_small][count_large]>=V45[count_small][count_large] && V270[count_small][count_large]>=V90[count_small][count_large] &&
				V270[count_small][count_large]>=V135[count_small][count_large] && V270[count_small][count_large]>=V180[count_small][count_large] && V270[count_small][count_large]>=V225[count_small][count_large] && V270[count_small][count_large]>=V315[count_small][count_large])
			{threshold_high_flag[count_small][count_large]=7;}

			if(V315[count_small][count_large]>=V0[count_small][count_large] && V315[count_small][count_large]>=V45[count_small][count_large] && V315[count_small][count_large]>=V90[count_small][count_large] &&
				V315[count_small][count_large]>=V135[count_small][count_large] && V315[count_small][count_large]>=V180[count_small][count_large] && V315[count_small][count_large]>=V225[count_small][count_large] && V315[count_small][count_large]>=V270[count_small][count_large])
			{threshold_high_flag[count_small][count_large]=8;}
//}		//取得方向を変化させてたい場合アンコメント

///////////////////////4方向取得のflag(正の閾値)/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//if(direction_number==4){		//取得方向を変化させてたい場合アンコメント
/*				if(minor_flag==0){
					if(V0[count_small][count_large]>=V90[count_small][count_large] &&	V0[count_small][count_large]>=V180[count_small][count_large] && V0[count_small][count_large]>=V270[count_small][count_large])
						{threshold_high_flag[count_small][count_large]=1;}			//Vの大小比較


					if(V90[count_small][count_large]>=V0[count_small][count_large] &&	V90[count_small][count_large]>=V180[count_small][count_large] && V90[count_small][count_large]>=V270[count_small][count_large])
						{threshold_high_flag[count_small][count_large]=3;}


					if(V180[count_small][count_large]>=V0[count_small][count_large] && V180[count_small][count_large]>=V90[count_small][count_large] &&V180[count_small][count_large]>=V270[count_small][count_large])
						{threshold_high_flag[count_small][count_large]=5;}


					if(V270[count_small][count_large]>=V0[count_small][count_large] && V270[count_small][count_large]>=V90[count_small][count_large] && V270[count_small][count_large]>=V180[count_small][count_large])
						{threshold_high_flag[count_small][count_large]=7;}

				}
				if(minor_flag==1){

							if(V0[count_small][count_large]>=V45[count_small][count_large] && V0[count_small][count_large]>=V90[count_small][count_large] && V0[count_small][count_large]>=V135[count_small][count_large])
								{threshold_high_flag[count_small][count_large]=1;}			//Vの大小比較

							if(V45[count_small][count_large]>=V0[count_small][count_large] && V45[count_small][count_large]>=V90[count_small][count_large] && V45[count_small][count_large]>=V135[count_small][count_large])
								{threshold_high_flag[count_small][count_large]=2;}

							if(V90[count_small][count_large]>=V0[count_small][count_large] && V90[count_small][count_large]>=V45[count_small][count_large] && V90[count_small][count_large]>=V135[count_small][count_large])
								{threshold_high_flag[count_small][count_large]=3;}

							if(V135[count_small][count_large]>=V0[count_small][count_large] && V135[count_small][count_large]>=V45[count_small][count_large] && V135[count_small][count_large]>=V90[count_small][count_large])
								{threshold_high_flag[count_small][count_large]=4;}

				}
			}

*/
///////////////////////2方向取得のflag(正の閾値)/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//if(direction_number==2){		//取得方向を変化させてたい場合アンコメント

/*					if(minor_flag==0){
						
						if(V0[count_small][count_large]>=V90[count_small][count_large]){
							threshold_high_flag[count_small][count_large]=1;			//Vの大小比較
						}else{
							threshold_high_flag[count_small][count_large]=3;
						}
					}
					if(minor_flag==1){

						if(V0[count_small][count_large]>=V45[count_small][count_large]){
							threshold_high_flag[count_small][count_large]=1;			//Vの大小比較
						}else{
							threshold_high_flag[count_small][count_large]=2;
						}	
					}
				//else{
					//printf("curcuit_number=%d のエラー",curcuit_number);
				//}
				}
*/
	}
}
			//count_small++;							//一行読みとったとき用のカウント

//////////////閾値判定ここまで/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////break条件を記す//////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*	if(count_small==&image_x){
				count_large++;
				break;
				}
			}
			*/
////////////////////////////計算条件//////////////////////////////////////////////////////////////////////////////////////////////
			printf("cos類似度の計算を始めます\n");
			for(i=0;i<image_y;++i){	
				for (j = 0; j < image_x; ++j) {

					//cout<<"処理中"<<endl;
					//if(minor_flag==0){
					if (cos_eco_mode_flag != 1) {

					//ここで正負の2つの閾値を取れるように改修する
					if (threshold_low_flag[j][i] == 1) { threshold_low[j][i] = V0[j][i]; fprintf(fp_threshold, "%lf,", threshold_low[j][i]); }
					if (threshold_low_flag[j][i] == 2) { threshold_low[j][i] = V45[j][i]; fprintf(fp_threshold, "%lf,", threshold_low[j][i]); }
					if (threshold_low_flag[j][i] == 3) { threshold_low[j][i] = V90[j][i]; fprintf(fp_threshold, "%lf,", threshold_low[j][i]); }
					if (threshold_low_flag[j][i] == 4) { threshold_low[j][i] = V135[j][i]; fprintf(fp_threshold, "%lf,", threshold_low[j][i]); }
					if (threshold_low_flag[j][i] == 5) { threshold_low[j][i] = V180[j][i]; fprintf(fp_threshold, "%lf,", threshold_low[j][i]); }
					if (threshold_low_flag[j][i] == 6) { threshold_low[j][i] = V225[j][i]; fprintf(fp_threshold, "%lf,", threshold_low[j][i]); }
					if (threshold_low_flag[j][i] == 7) { threshold_low[j][i] = V270[j][i]; fprintf(fp_threshold, "%lf,", threshold_low[j][i]); }
					if (threshold_low_flag[j][i] == 8) { threshold_low[j][i] = V315[j][i]; fprintf(fp_threshold, "%lf,", threshold_low[j][i]); }

					if (j == image_x - 1) { fprintf(fp_threshold, "\n"); }

					if (threshold_high_flag[j][i] == 1) { threshold_high[j][i] = V0[j][i]; fprintf(fp_threshold_high, "%lf,", threshold_high[j][i]); }
					if (threshold_high_flag[j][i] == 2) { threshold_high[j][i] = V45[j][i]; fprintf(fp_threshold_high, "%lf,", threshold_high[j][i]); }
					if (threshold_high_flag[j][i] == 3) { threshold_high[j][i] = V90[j][i]; fprintf(fp_threshold_high, "%lf,", threshold_high[j][i]); }
					if (threshold_high_flag[j][i] == 4) { threshold_high[j][i] = V135[j][i]; fprintf(fp_threshold_high, "%lf,", threshold_high[j][i]); }
					if (threshold_high_flag[j][i] == 5) { threshold_high[j][i] = V180[j][i]; fprintf(fp_threshold_high, "%lf,", threshold_high[j][i]); }
					if (threshold_high_flag[j][i] == 6) { threshold_high[j][i] = V225[j][i]; fprintf(fp_threshold_high, "%lf,", threshold_high[j][i]); }
					if (threshold_high_flag[j][i] == 7) { threshold_high[j][i] = V270[j][i]; fprintf(fp_threshold_high, "%lf,", threshold_high[j][i]); }
					if (threshold_high_flag[j][i] == 8) { threshold_high[j][i] = V315[j][i]; fprintf(fp_threshold_high, "%lf,", threshold_high[j][i]); }
					if (j == image_x - 1) { fprintf(fp_threshold_high, "\n"); }
					}
					else {
						//ここで正負の2つの閾値を取れるように改修する
						if (threshold_low_flag[j][i] == 1) { threshold_low[j][i] = V0[j][i];}
						if (threshold_low_flag[j][i] == 2) { threshold_low[j][i] = V45[j][i];}
						if (threshold_low_flag[j][i] == 3) { threshold_low[j][i] = V90[j][i];}
						if (threshold_low_flag[j][i] == 4) { threshold_low[j][i] = V135[j][i];}
						if (threshold_low_flag[j][i] == 5) { threshold_low[j][i] = V180[j][i];}
						if (threshold_low_flag[j][i] == 6) { threshold_low[j][i] = V225[j][i];}
						if (threshold_low_flag[j][i] == 7) { threshold_low[j][i] = V270[j][i];}
						if (threshold_low_flag[j][i] == 8) { threshold_low[j][i] = V315[j][i];}

						

						if (threshold_high_flag[j][i] == 1) { threshold_high[j][i] = V0[j][i];}
						if (threshold_high_flag[j][i] == 2) { threshold_high[j][i] = V45[j][i];}
						if (threshold_high_flag[j][i] == 3) { threshold_high[j][i] = V90[j][i];}
						if (threshold_high_flag[j][i] == 4) { threshold_high[j][i] = V135[j][i];}
						if (threshold_high_flag[j][i] == 5) { threshold_high[j][i] = V180[j][i];}
						if (threshold_high_flag[j][i] == 6) { threshold_high[j][i] = V225[j][i];}
						if (threshold_high_flag[j][i] == 7) { threshold_high[j][i] = V270[j][i];}
						if (threshold_high_flag[j][i] == 8) { threshold_high[j][i] = V315[j][i];}
						

					}
					

			use_Rvector_flag = threshold_high_flag[j][i];	//基本は正の閾値を取る

			if(threshold_low<0){
				threshold_low_abs= threshold_low[j][i] *-1;
			}else{
				threshold_low_abs= threshold_low[j][i];
			}
			if(threshold_high<0){
				threshold_high_abs= threshold_high[j][i] *-1;
			}else{
				threshold_high_abs= threshold_high[j][i];
			}

			if(threshold_low_abs>threshold_high_abs){
				use_Rvector_flag = threshold_low_flag[j][i];
				threshold2[j][i]= threshold_low[j][i];
				//fprintf(fp_threshold2,"%lf,",threshold_low);

				//if(minor_flag==1)use_Rvector_flag=threshold_low_flag[j][i]+4;
				//if(direction_number==2)use_Rvector_flag=threshold_low_flag[j][i]+4;
			}
			else{
				threshold2[j][i] = threshold_high[j][i];
				//fprintf(fp_threshold2,"%lf,", threshold_high[j][i]);
			}

			fprintf(fp_threshold2,"%lf,", threshold2[j][i]);
			if(j==image_x-1){fprintf(fp_threshold2,"\n");}

//基準を90度ずらす
				if(use_Rvector_flag>=3){							//-90°を用いる
					use_Rvector_number=use_Rvector_flag-2;		//90,135,180,225,270,315のとき
				}else{
					use_Rvector_number=use_Rvector_flag+6;		//0,45のとき
				}

				threshold_local_flag[j][i] = use_Rvector_flag;

//基準を90°ずらさない
			/*
			use_Rvector_number=use_Rvector_flag[j];
			printf("%d",use_Rvector_number);
			printf("j=%d:%d,",j,use_Rvector_flag[j]);
			*/
//////////////ファイルへの書き込み(fp_use_Rvector_flag,fp_use_Rvector_number)/////////////////////////////////////////////
				if (cos_eco_mode_flag != 1) {
					fprintf(fp_use_Rvector_flag, "%d,", use_Rvector_flag);
					if (j == image_x - 1) { fprintf(fp_use_Rvector_flag, "\n"); }

					fprintf(fp_use_Rvector_number, "%d,", use_Rvector_number);
					if (j == image_x - 1) { fprintf(fp_use_Rvector_number, "\n"); }
				}
/////////////////////////計算//////////////////////////////////////////////////////////////////////////////////////////////
			if(direction_number==8){
				innerp[j][i] = (Rvector[use_Rvector_number][1]*V0[j][i]) + (Rvector[use_Rvector_number][2]*V45[j][i]) + (Rvector[use_Rvector_number][3]*V90[j][i]) + (Rvector[use_Rvector_number][4]*V135[j][i]) 
					+ (Rvector[use_Rvector_number][5]*V180[j][i]) + (Rvector[use_Rvector_number][6]*V225[j][i]) + (Rvector[use_Rvector_number][7]*V270[j][i]) + (Rvector[use_Rvector_number][8]*V315[j][i]);
				V_sqrt[j][i]=sqrt( pow(V0[j][i],2) + pow(V45[j][i],2)  + pow(V90[j][i],2)  + pow(V135[j][i],2)  + pow(V180[j][i],2)  + pow(V225[j][i],2)  + pow(V270[j][i],2)  + pow(V315[j][i],2) );
				}else{
					if(direction_number==4){
						if(minor_flag==0){
						innerp[j][i] = (Rvector[use_Rvector_number][1]*V0[j][i]) + (Rvector[use_Rvector_number][3]*V90[j][i]) + (Rvector[use_Rvector_number][5]*V180[j][i]) + (Rvector[use_Rvector_number][7]*V270[j][i]);
						V_sqrt[j][i]=sqrt( pow(V0[j][i],2) + pow(V90[j][i],2)  + pow(V180[j][i],2)  + pow(V270[j][i],2));
						}
						if(minor_flag==1){
						innerp[j][i] = (Rvector[use_Rvector_number][1]*V0[j][i]) + (Rvector[use_Rvector_number][2]*V45[j][i]) + (Rvector[use_Rvector_number][3]*V90[j][i]) + (Rvector[use_Rvector_number][4]*V135[j][i]);
						V_sqrt[j][i]=sqrt( pow(V0[j][i],2) + pow(V45[j][i],2)  + pow(V90[j][i],2)  + pow(V135[j][i],2));
						}
						}else{
							if(direction_number==2){
								if(minor_flag==0){
								innerp[j][i] = (Rvector[use_Rvector_number][1]*V0[j][i]) + (Rvector[use_Rvector_number][3]*V90[j][i]);
								V_sqrt[j][i]=sqrt( pow(V0[j][i],2) + pow(V90[j][i],2));
								}
								if(minor_flag==1){
								innerp[j][i] = (Rvector[use_Rvector_number][1]*V0[j][i]) + (Rvector[use_Rvector_number][2]*V45[j][i]);
								V_sqrt[j][i]=sqrt( pow(V0[j][i],2) + pow(V45[j][i],2));
								}
								}else{
									cout<<"direction_numberの設定がおかしいエラー"<<endl;
									return 1;
								}
						}
				}

/////////////////////cos類似度の計算///////////////////////////////////////////////////////////////////////////////////////////////

			Cos_similarity[j][i] = innerp[j][i] / (Rvector_sqrt[use_Rvector_number] * V_sqrt[j][i]);

			//基準をずらす
			if(use_Rvector_number<7){Angle[j][i] = acos(Cos_similarity[j][i])*180/PI+((use_Rvector_number-1)*45);}
			if(use_Rvector_number>6){Angle[j][i] = acos(Cos_similarity[j][i])*180/PI+((use_Rvector_number-9)*45);}

			//基準をずらさない
			/*
			Angle[j][i] = acos(Cos_similarity[j][i])*180/PI+((use_Rvector_number-1)*45);
			if(Angle[j]-45*(use_Rvector_number-1)>20)Angle[j]=Angle[j]-45*(use_Rvector_number-1);
			Angle[j] = acos(Cos_similarity[j])*180/PI;
			*/

			if(Angle[j][i]<0){
				Angle[j][i] = Angle[j][i] +360;
			}

			if (Rvector_sqrt[use_Rvector_number] == 0 || V_sqrt[j][i] == 0)Angle[j][i] = 334000;

/////////////////////////計算終わり/////////////////////////////////////////////////////////////////////////////////////////////

///////////////////ファイルへの書き込み(一行ずつ書き込み）////////////////////////////////////////////////////////////////////////
			if (cos_eco_mode_flag != 1) {
				fprintf(fp_innerp, "%lf,", innerp[j][i]);
				if (j == image_x - 1) { fprintf(fp_innerp, "\n"); }

				fprintf(fp_V_sqrt, "%lf,", V_sqrt[j][i]);
				if (j == image_x - 1) { fprintf(fp_V_sqrt, "\n"); }

				fprintf(fp_Cos_similarity, "%lf,", Cos_similarity[j][i]);
				if (j == image_x - 1) { fprintf(fp_Cos_similarity, "\n"); }

				
			}

			fprintf(fp_Angle,"%lf,",Angle[j][i]);
			if(j==image_x-1){fprintf(fp_Angle,"\n");}
		
			//fprintf(fp_Correction_flag,"%d,",Correction_flag[j][i]);
			//if(j==image_width){fprintf(fp_Correction_flag,"\n");}

			}
		}

/////////////////////////////局所連結性のチェック///////////////////////////////////////////////////////
		double **threshold_LC_number = matrix(0, image_x - 1, 0, image_y - 1);
		for (i = 0; i < image_y; i++) {
			for (j = 0; j < image_x; j++) {
				threshold_LC_number[j][i] = 0;
			}
		}
		
		local_connectivity(image_x, image_y, threshold_local_flag, threshold_LC_number);
		
		for (i = 0; i < image_y; ++i) {
			for (j = 0; j < image_x; ++j) {
				fprintf(fp_threshold_number_LC, "%lf,", threshold_LC_number[j][i]);
				if (j == image_x - 1) { fprintf(fp_threshold_number_LC, "\n"); }
			}
		}

		for (i = 0; i < image_y; ++i) {
			for (j = 0; j < image_x; ++j) {
			//	printf("%lf,", threshold_LC_number[j][i]);
				if (threshold_LC_number[j][i] == 0) {
					threshold3[j][i] = 0;
				}
				else {
					threshold3[j][i] = threshold2[j][i];
				}
				fprintf(fp_threshold3, "%lf,", threshold3[j][i]);
				if (j == image_x - 1) { fprintf(fp_threshold3, "\n"); }
			}
		}
		
		
///////////////////////////書き込み終わり/////////////////////////////////////////////////////////			

	//ファイルを閉じる
			if (cos_eco_mode_flag != 1) {
				fclose(fp_innerp);
				fclose(fp_V_sqrt);
				fclose(fp_Cos_similarity);
				fclose(fp_threshold_high);
				fclose(fp_threshold);
				fclose(fp_threshold_number_LC);
			}

	fclose(fp_Angle);
	fclose(fp_threshold2);
	fclose(fp_threshold3);

////////////////////////logファイルの作成//////////////////////////////////////////////////////////////////////////
	FILE *fp_date;
	char filename_log[128];

	sprintf(filename_log, "%s\\log.txt",date_directory3);	//logファイル作成のディレクトリ指定
	printf("%s\n",filename_log);	//logファイル作成のディレクトリ指定
	if((fp_date=fopen(filename_log,"w"))==NULL){printf("logファイルが開けません");exit(1);}
	fprintf(fp_date,"Time       : %s\n",date);						//時間
	fprintf(fp_date,"direction  : %d\n",direction_number);			//方向
	fprintf(fp_date,"curcuit    : %d\n",curcuit_number);			//使用回路
	//fprintf(fp_date,"property   : %s\n",inputdate_directory1);	//propretyファイル
	fprintf(fp_date,"Rvector    : %d\n",Rvector_number);			//基準ベクトルの数
	fprintf(fp_date,"minor_flag : %d\n",minor_flag);				//マイナーナンバー
	fprintf(fp_date,"Rvector    : %s\n",inputrvector_directory);	//使用したRvectorデータ
	for(i=1;i<=8;i++){for(j=1;j<=8;j++){fprintf(fp_date,"%lf, ",Rvector[i][j]);}fprintf(fp_date,"\n");}	//使用したRvector
	fprintf(fp_date,"Rvector *	: %f,%f,%f,%f,%f,%f,%f,%f\n",Rvectormagni[1],Rvectormagni[2],Rvectormagni[3],Rvectormagni[4],Rvectormagni[5],Rvectormagni[6],Rvectormagni[7],Rvectormagni[8]);	//使用したRvectorの倍率
	fprintf(fp_date,"使用データ : %s\n",inputdate_directory);		//使用した畳み込み済みデータ
	fprintf(fp_date,"保存先     : %s\n",date_directory3);			//保存先
	fclose(fp_date);

	printf("direction  : %d\n",direction_number);					//方向
	printf("curcuit	   : %d\n",curcuit_number);						//使用回路
	printf("Rvector	   : %d\n",Rvector_number);						//基準ベクトルの数
	printf("minor_flag : %d\n",minor_flag);							//マイナーナンバー
	printf("Rvector    : %s\n",inputrvector_directory);				//使用したRvectorデータ
	printf("使用データ : %sのcos類似度計算が終了しました\n",inputdate_directory);				//使用した畳み込み済みデータ


	//メモリの開放
	free_matrix(threshold_LC_number, 0, image_x - 1, 0, image_y - 1);
	free_matrix(threshold2, 0, image_x - 1, 0, image_y - 1);
	free_matrix(threshold3, 0, image_x - 1, 0, image_y - 1);
	free_matrix(threshold_high, 0, image_x - 1, 0, image_y - 1);
	free_matrix(threshold_low, 0, image_x - 1, 0, image_y - 1);
	free_matrix(V0, 0, image_x-1, 0, image_y-1);
	free_matrix(V45,0, image_x-1, 0, image_y-1);
	free_matrix(V90, 0, image_x-1, 0, image_y-1);
	free_matrix(V135, 0, image_x-1, 0, image_y-1);
	free_matrix(V180, 0, image_x-1, 0, image_y-1);
	free_matrix(V225,0, image_x-1, 0, image_y-1);
	free_matrix(V270, 0, image_x-1, 0, image_y-1);
	free_matrix(V315, 0, image_x-1, 0, image_y-1);
	free_matrix(threshold_low_flag,0, image_x-1, 0, image_y-1);
	free_matrix(threshold_high_flag, 0, image_x-1, 0, image_y-1);
	free_matrix(V_sqrt, 0, image_x-1, 0, image_y-1);
	free_matrix(Cos_similarity,0, image_x-1, 0, image_y-1);
	free_matrix(Angle, 0, image_x-1, 0, image_y-1);
	free_matrix(innerp, 0, image_x-1, 0, image_y-1);

	return 0;
}


void Read_output(){
///////////////////Outputファイルのディレクトリ設定////////////////////////////////////////////////
	sprintf(math_name1,"%s\\%s",date_directory3, math_name1_s);
	sprintf(math_name2,"%s\\%s",date_directory3, math_name2_s);
	sprintf(math_name3,"%s\\%s",date_directory3, math_name3_s);
	sprintf(math_name4,"%s\\%s",date_directory3, math_name4_s);
	sprintf(math_name5,"%s\\%s",date_directory3, math_name5_s);
	sprintf(math_name6,"%s\\%s",date_directory3, math_name6_s);
	sprintf(math_name7,"%s\\%s",date_directory3, math_name7_s);
	sprintf(math_name8,"%s\\%s",date_directory3, math_name8_s);
	sprintf(math_name9,"%s\\%s",date_directory3, math_name9_s);
	sprintf(math_name10, "%s\\%s", date_directory3, math_name10_s);
	sprintf(math_name11, "%s\\%s", date_directory3, math_name11_s);

	//ファイルオープン及び確認
	if (cos_eco_mode_flag != 1) {
		if ((fp_innerp = fopen(math_name1, "w")) == NULL) { printf("入力エラー innerp.csvが開けません\nFile_name : %s", math_name1); exit(1); }
		if ((fp_V_sqrt = fopen(math_name2, "w")) == NULL) { printf("入力エラー V_sqrt.csvが開けません\nFile_name : %s", math_name2); exit(1); }
		if ((fp_Cos_similarity = fopen(math_name3, "w")) == NULL) { printf("入力エラー Cos_similarity.csvが開けません\nFile_name : %s", math_name3); exit(1); }
		
		if ((fp_threshold_high = fopen(math_name5, "w")) == NULL) { printf("入力エラー threshold_high.csvが開けません\nFile_name : %s", math_name5); exit(1); }
		if ((fp_use_Rvector_flag = fopen(math_name6, "w")) == NULL) { printf("入力エラー use_Rvector_flag.csvが開けません\nFile_name : %s", math_name6); exit(1); }
		if ((fp_use_Rvector_number = fopen(math_name7, "w")) == NULL) { printf("入力エラー use_Rvector_number.csvが開けません\nFile_name : %s", math_name7); exit(1); }
		
		if ((fp_threshold = fopen(math_name9, "w")) == NULL) { printf("入力エラー threshold.csvが開けません\nFile_name : %s", math_name9); exit(1); }
		if ((fp_threshold_number_LC = fopen(math_name11, "w")) == NULL) { printf("入力エラー threshold_number_LC.csvが開けません\nFile_name : %s", math_name11); exit(1); }
		
	}
	if ((fp_Angle = fopen(math_name4, "w")) == NULL) { printf("入力エラー Angle.csvが開けません\nFile_name : %s", math_name4); exit(1); }
	if ((fp_threshold2 = fopen(math_name8, "w")) == NULL) { printf("入力エラー threshold2.csvが開けません\nFile_name : %s", math_name8); exit(1); }
	if ((fp_threshold3 = fopen(math_name10, "w")) == NULL) { printf("入力エラー threshold3.csvが開けません\nFile_name : %s", math_name10); exit(1); }

}

void Rvector_size(){
//////////////////////Rvectorの大きさを求めている(cosに用いる)//////////////////////////////////////////////////////////////////////////////////////////////////////////////
   if(direction_number==8){
	   for(i=1;i<=8;i++){
		Rvector_square_sum[i]=Rvector_square[i][1]+Rvector_square[i][2]+Rvector_square[i][3]+Rvector_square[i][4]+Rvector_square[i][5]+Rvector_square[i][6]+Rvector_square[i][7]+Rvector_square[i][8];
	   }
   }else{
	   if(direction_number==4){
		   for(i=1;i<=8;i++){
		   if(minor_flag==0)Rvector_square_sum[i]=Rvector_square[i][1]+Rvector_square[i][3]+Rvector_square[i][5]+Rvector_square[i][7];
		   if(minor_flag==1)Rvector_square_sum[i]=Rvector_square[i][1]+Rvector_square[i][2]+Rvector_square[i][3]+Rvector_square[i][4];//未定
		   }
	   }else{
		   if(direction_number==2){
			   for(i=1;i<=8;i++){
			    if(minor_flag==0)Rvector_square_sum[i]=Rvector_square[i][1]+Rvector_square[i][3];
				if(minor_flag==1)Rvector_square_sum[i]=Rvector_square[i][1]+Rvector_square[i][2];
			   }
		   }else{
			   cout<<"direction_numberの設定がおかしいエラー"<<endl;
			   exit(1);
		   }
	   }
   }
   for(i=1;i<=8;i++){
		Rvector_sqrt[i]=sqrt(Rvector_square_sum[i]);
   }
}


void Rvector_read(){
	/////////////////基準ベクトルの読み込み/////////////////////////////////////////////
	
	sprintf(Input_Rvectormagni_name,"%s%s",inputrvector_directory,Input_Rvectormagniname_s);
	printf("%s\n",Input_Rvectormagni_name);
	ifstream Rvector_magni;
	Rvector_magni.open(Input_Rvectormagni_name,ios::in);

	if(!Rvector_magni){cout<<"入力エラー Rvector_magni.csvがありません";exit(1);}
	i=1,j=1;

	string str_Rvector_magni;
	while (getline(Rvector_magni,str_Rvector_magni)){
		string token_Rvector_magni;
		istringstream stream(str_Rvector_magni);
		while(getline(stream,token_Rvector_magni,',')){
			double temp_Rvector_magni=stof(token_Rvector_magni); //stof(string str) : stringをfloatに変換
			Rvectormagni[i]=temp_Rvector_magni;
			i++;
		}
	}

	//Rvector
	sprintf(Input_Rvectorname1,"%s%s",inputrvector_directory,Input_Rvectorname1_s);
	sprintf(Input_Rvectorname2,"%s%s",inputrvector_directory,Input_Rvectorname2_s);
	sprintf(Input_Rvectorname3,"%s%s",inputrvector_directory,Input_Rvectorname3_s);
	sprintf(Input_Rvectorname4,"%s%s",inputrvector_directory,Input_Rvectorname4_s);
	sprintf(Input_Rvectorname5,"%s%s",inputrvector_directory,Input_Rvectorname5_s);
	sprintf(Input_Rvectorname6,"%s%s",inputrvector_directory,Input_Rvectorname6_s);
	sprintf(Input_Rvectorname7,"%s%s",inputrvector_directory,Input_Rvectorname7_s);
	sprintf(Input_Rvectorname8,"%s%s",inputrvector_directory,Input_Rvectorname8_s);

	//printf("%s\n",Input_Rvectorname1);
	//Rvector

	printf("inputrvector_directory=%s,\nInput_Rvectorname4_s=%s\n",inputrvector_directory,Input_Rvectorname4_s);
	printf("Input_Rvectorname4=%s\n",Input_Rvectorname4);
	
	ifstream Rvector_0(Input_Rvectorname1);
	ifstream Rvector_45(Input_Rvectorname2);
	ifstream Rvector_90(Input_Rvectorname3);
	ifstream Rvector_135(Input_Rvectorname4);
	ifstream Rvector_180(Input_Rvectorname5);
	ifstream Rvector_225(Input_Rvectorname6);
	ifstream Rvector_270(Input_Rvectorname7);
	ifstream Rvector_315(Input_Rvectorname8);

	printf("R0:%s\nR45:%s\nR90:%s\nR135:%s\nR180:%s\nR225:%s\nR270:%s\nR315:%s\n",Input_Rvectorname1,Input_Rvectorname2,Input_Rvectorname3,Input_Rvectorname4,Input_Rvectorname5,Input_Rvectorname6,Input_Rvectorname7,Input_Rvectorname8);

	//if(!Rvector_magni){cout<<"入力エラー Rvector_magni.csvがありません";exit(1);}
    if(!Rvector_0){cout<<"入力エラー Rvector0.csvがありません";exit(1);}
	if(!Rvector_45){cout<<"入力エラー Rvector45.csvがありません";exit(1);}
	if(!Rvector_90){cout<<"入力エラー Rvector90.csvがありません";exit(1);}
	if(!Rvector_135){cout<<"入力エラー Rvector135.csvがありません";exit(1);}
	if(!Rvector_180){cout<<"入力エラー Rvector180.csvがありません";exit(1);}
	if(!Rvector_225){cout<<"入力エラー Rvector225.csvがありません";exit(1);}
	if(!Rvector_270){cout<<"入力エラー Rvector270.csvがありません";exit(1);}
	if(!Rvector_315){cout<<"入力エラー Rvector315.csvがありません";exit(1);}

	i=1,j=1;
	string str_Rvector0,str_Rvector45,str_Rvector90,str_Rvector135,str_Rvector180,str_Rvector225,str_Rvector270,str_Rvector315;
	while(getline(Rvector_0,str_Rvector0)){
        string token_Rvector0;
        istringstream stream(str_Rvector0);

		getline(Rvector_45,str_Rvector45);	string token_Rvector45;	istringstream stream_Rvector45(str_Rvector45);
		getline(Rvector_90,str_Rvector90);	string token_Rvector90;	istringstream stream_Rvector90(str_Rvector90);
		getline(Rvector_135,str_Rvector135);	string token_Rvector135;	istringstream stream_Rvector135(str_Rvector135);
		getline(Rvector_180,str_Rvector180);	string token_Rvector180;	istringstream stream_Rvector180(str_Rvector180);
		getline(Rvector_225,str_Rvector225);	string token_Rvector225;	istringstream stream_Rvector225(str_Rvector225);
		getline(Rvector_270,str_Rvector270);	string token_Rvector270;	istringstream stream_Rvector270(str_Rvector270);
		getline(Rvector_315,str_Rvector315);	string token_Rvector315;	istringstream stream_Rvector315(str_Rvector315);

        //1行のうち、文字列とコンマを分割する
        while(getline(stream,token_Rvector0,',')){
            //すべて文字列として読み込まれるため
            //数値は変換が必要
            double temp_Rvecrot0=stof(token_Rvector0);	//stof(string str) : stringをfloatに変換
			Rvector[i][j]=temp_Rvecrot0;				//0度入力画像の時の値を読む
			Rvector[i][j]=Rvectormagni[j]*Rvector[i][j];//倍率をかける
			Rvector_square[i][j]=pow(Rvector[i][j],2);	//基準ベクトルの2倍を求める
			i++;										//iは入力した画像の角度を番号で，jは取得位置を示す

			getline(stream_Rvector45,token_Rvector45,',');
			double tmp_Rvector45=stof(token_Rvector45);
			Rvector[i][j]=tmp_Rvector45;
			Rvector[i][j]=Rvectormagni[j]*Rvector[i][j];
			Rvector_square[i][j]=pow(Rvector[i][j],2);
			i++;
        
			getline(stream_Rvector90,token_Rvector90,',');
			double tmp_Rvector90=stof(token_Rvector90);
			Rvector[i][j]=tmp_Rvector90;
			Rvector[i][j]=Rvectormagni[j]*Rvector[i][j];
			Rvector_square[i][j]=pow(Rvector[i][j],2);
			i++;

			getline(stream_Rvector135,token_Rvector135,',');
			double tmp_Rvector135=stof(token_Rvector135);
			Rvector[i][j]=tmp_Rvector135;
			Rvector[i][j]=Rvectormagni[j]*Rvector[i][j];
			Rvector_square[i][j]=pow(Rvector[i][j],2);
			i++;

			getline(stream_Rvector180,token_Rvector180,',');
			double tmp_Rvector180=stof(token_Rvector180);
			Rvector[i][j]=tmp_Rvector180;
			Rvector[i][j]=Rvectormagni[j]*Rvector[i][j];
			Rvector_square[i][j]=pow(Rvector[i][j],2);
			i++;

			getline(stream_Rvector225,token_Rvector225,',');
			double tmp_Rvector225=stof(token_Rvector225);
			Rvector[i][j]=tmp_Rvector225;
			Rvector[i][j]=Rvectormagni[j]*Rvector[i][j];
			Rvector_square[i][j]=pow(Rvector[i][j],2);
			i++;

			getline(stream_Rvector270,token_Rvector270,',');
			double tmp_Rvector270=stof(token_Rvector270);
			Rvector[i][j]=tmp_Rvector270;
			Rvector[i][j]=Rvectormagni[j]*Rvector[i][j];
			Rvector_square[i][j]=pow(Rvector[i][j],2);
			i++;

			getline(stream_Rvector315,token_Rvector315,',');
			double tmp_Rvector315=stof(token_Rvector315);
			Rvector[i][j]=tmp_Rvector315;
			Rvector[i][j]=Rvectormagni[j]*Rvector[i][j];
			Rvector_square[i][j]=pow(Rvector[i][j],2);
			i=1;
			j++;
           
        }
	}

   Rvector_0.close();
   Rvector_45.close();
   Rvector_90.close();
   Rvector_135.close();
   Rvector_180.close();
   Rvector_225.close();
   Rvector_270.close();
   Rvector_315.close();
   Rvector_magni.close();
 
	Rvector_size();
}

int local_connectivity(int image_x, int image_y, double *local_flag[],double *threshold_LC_number[]) {

	//Nrutilを用いたメモリの確保
	double **threshold_LC_flag = matrix(0, image_x - 1, 0, image_y - 1);
	for (i = 0; i < image_y; i++) {
		for (j = 0; j < image_x; j++) {
			threshold_LC_flag[j][i] = 0;
		}
	}

	//4辺は0とする
	for (i = 1; i < image_y - 1; i++) {
		for (j = 1; j < image_x - 1; j++) {

			//直線の太さ方法に繋がることを考える
			//エッジ方向とに隣接する方向がθ±45°以内に存在しない場合，flagを0→threshold3=0とする
			//0°の時
			if (local_flag[j][i] == 1) {
				if (local_flag[j][i - 1]>local_flag[j][i] + 1 && local_flag[j][i - 1] < local_flag[j][i] + 7 && local_flag[j][i + 1]>local_flag[j][i] + 1 &&  local_flag[j][i + 1]< local_flag[j][i] + 7) {
					threshold_LC_flag[j][i] = 0;
				}else {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
			}

			//45°,225°の時
			if (local_flag[j][i] == 2 || local_flag[j][i] == 6) {
				if (local_flag[j - 1][i - 1] >= local_flag[j][i] - 1 && local_flag[j - 1][i - 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else if (local_flag[j + 1][i + 1] >= local_flag[j][i] - 1 && local_flag[j + 1][i + 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else {
					threshold_LC_flag[j][i] = 0;
				}
			}

			//90°,270°の時
			if (local_flag[j][i] == 3 || local_flag[j][i] == 7) {
				if (local_flag[j - 1][i] >= local_flag[j][i] - 1 && local_flag[j - 1][i] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else if (local_flag[j + 1][i] >= local_flag[j][i] - 1 && local_flag[j + 1][i] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else {
					threshold_LC_flag[j][i] = 0;
				}
			}

			//135°の時
			if (local_flag[j][i] == 4) {
				if (local_flag[j + 1][i - 1] >= local_flag[j][i] - 1 && local_flag[j + 1][i - 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else if (local_flag[j - 1][i + 1] >= local_flag[j][i] - 1 && local_flag[j - 1][i + 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else {
					threshold_LC_flag[j][i] = 0;
				}
			}
			
			//180°の時
			if (local_flag[j][i] == 5) {
				if (local_flag[j][i - 1] >= local_flag[j][i] - 1 && local_flag[j][i - 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else if (local_flag[j][i + 1] >= local_flag[j][i] - 1 && local_flag[j][i + 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else {
					threshold_LC_flag[j][i] = 0;
				}
			}
			
			//315°の時
			if (local_flag[j][i] == 8) {
				if (local_flag[j + 1][i - 1]>local_flag[j][i] - 7 && local_flag[j + 1][i - 1]  < local_flag[j][i] - 1 && local_flag[j - 1][i + 1]>local_flag[j][i] - 7 && local_flag[j - 1][i + 1]< local_flag[j][i] - 1) {
					threshold_LC_flag[j][i] = 0;
				}else {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
			}

/*
			//直線の長さ方向に繋がることを考える
			//エッジ方向とに隣接する方向がθ±45°以内に存在しない場合，flagを0→threshold3=0とする
			//0°の時
			if (local_flag[j][i] == 1) {
				if (local_flag[j - 1][i]>local_flag[j][i] + 1 && local_flag[j - 1][i] < local_flag[j][i] + 7 && local_flag[j + 1][i]>local_flag[j][i] + 1 && local_flag[j + 1][i]< local_flag[j][i] + 7) {
					threshold_LC_flag[j][i] = 0;
				}
				else {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
			}
			//45°,225°の時
			if (local_flag[j][i] == 2 || local_flag[j][i] == 6) {
				if (local_flag[j + 1][i - 1] >= local_flag[j][i] - 1 && local_flag[j + 1][i - 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else if (local_flag[j - 1][i + 1] >= local_flag[j][i] - 1 && local_flag[j - 1][i + 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else {
					threshold_LC_flag[j][i] = 0;
				}
			}
			//90°,270°の時
			if (local_flag[j][i] == 3 || local_flag[j][i] == 7) {
				if (local_flag[j][i - 1] >= local_flag[j][i] - 1 && local_flag[j][i - 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else if (local_flag[j][i + 1] >= local_flag[j][i] - 1 && local_flag[j][i + 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else {
					threshold_LC_flag[j][i] = 0;
				}
			}
			//135°の時
			if (local_flag[j][i] == 4) {
				if (local_flag[j - 1][i - 1] >= local_flag[j][i] - 1 && local_flag[j - 1][i - 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else if (local_flag[j + 1][i + 1] >= local_flag[j][i] - 1 && local_flag[j + 1][i + 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else {
					threshold_LC_flag[j][i] = 0;
				}
			}
			//180°の時
			if (local_flag[j][i] == 5) {
				if (local_flag[j - 1][i] >= local_flag[j][i] - 1 && local_flag[j - 1][i] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else if (local_flag[j + 1][i] >= local_flag[j][i] - 1 && local_flag[j + 1][i] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else {
					threshold_LC_flag[j][i] = 0;
				}
			}
			//315°の時
			if (local_flag[j][i] == 8) {
				if (local_flag[j - 1][i - 1]>local_flag[j][i] - 7 && local_flag[j - 1][i - 1]  < local_flag[j][i] - 1 && local_flag[j + 1][i + 1]>local_flag[j][i] - 7 && local_flag[j + 1][i + 1]< local_flag[j][i] - 1) {
					threshold_LC_flag[j][i] = 0;
				}
				else {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
			}

			//45°ずらす
			if (threshold_LC_flag[j][i] != 0) {
				if (threshold_LC_flag[j][i] >= 3) {							//-90°を用いる
					threshold_LC_number[j][i] = threshold_LC_flag[j][i] - 2;		//90,135,180,225,270,315のとき
				}else {
					threshold_LC_number[j][i] = threshold_LC_flag[j][i] + 6;		//0,45のとき
				}
			}
*/
		}
	}

	free_matrix(threshold_LC_flag, 0, image_x - 1, 0, image_y - 1);
	return **threshold_LC_number;
}
 