#include <iostream>
#include <vector>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream> //文字ストリーム
#include<math.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


//メモリ確保を行うためのヘッダ
#define ANSI				
#include "nrutil.h"	


using namespace cv;
using namespace std;
int msectime();

int hist_hozon(char date_directory[], double k,std::vector<int> &hist) {

	FILE *fp_hist;
	char hist_csv_directroy[128];
	sprintf(hist_csv_directroy, "%s_hist.csv", date_directory);
	if ((fp_hist = fopen(hist_csv_directroy, "w")) == NULL) { printf("histファイルが開けません"); exit(1); }

	fprintf(fp_hist, "i,hist,bin\n");
	for (int i = 0; i < 256; ++i) {
		fprintf(fp_hist, "%d,%d,%f\n", i, hist[i],k*i);
	}

	fclose(fp_hist);
}

////////////////////////ヒストグラムの作成及び閾値の設定////////////////////////////////////////////////////////////////////////
double discriminantAnalysis(char date_directory[], int &image_x, int &image_y, std::vector<std::vector<double>> &edge_st){

	printf("****************************************\n");
	printf("start： 判別分析法\n");
	printf("****************************************\n");

	printf("%s\n",date_directory);
	int DATA_NUM = image_x *image_y;
	double tmp_h = 0;
	double tmp_l = 0;
	double k;	//幅

	//Nrutilを用いたメモリの確保
	double **data = matrix(0, image_x *image_y-1, 0, 1);
	double **tmp = matrix(0, image_y - 1, 0, 1);

	//確保したメモリを初期化する
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < image_x *image_y; j++) {
			data[j][i] = 0;
		}
	}
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < image_y - 1; j++) {
			tmp[j][i] = 0;
		}
	}
	//一次元配列に代入
	for (int i = 0; i < image_y; i++) {
		for (int j = 0; j < image_x; j++) {
			data[j*(i+1)][0] = edge_st[j][i];
		}
	}
	//行ごとにデータを大きい順に並び替える
//#pragma omp parallel for
	for (int i = 0; i < image_y; ++i) {
		for (int k = 1; k < image_x; k++)
		{
			for (int j = 0; j < image_x - k; j++)
			{
				if (edge_st[j][i] > edge_st[j + 1][i])
				{
					tmp[i][0] = edge_st[j][i];
					edge_st[j][i] = edge_st[j + 1][i];
					edge_st[j + 1][i] = tmp[i][0];
				}
			}
		}
			//printf("edge_st[image_x-1][%d]=%f\n", i,edge_st[image_x - 1][i]);
	}

//#pragma omp parallel for
	for (int i = 1; i<image_y; i++)
	{
		for (int j = 0; j < image_y - i; j++)
		{
			if (edge_st[image_x - 1][j] > edge_st[image_x - 1][j + 1])
			{
				tmp_h = edge_st[image_x - 1][j];
				edge_st[image_x - 1][j] = edge_st[image_x - 1][j + 1];
				edge_st[image_x - 1][j + 1] = tmp_h;
			}
		}
		
	}

	for (int i = 1; i<image_y; i++)
	{
		for (int j = 0; j < image_y - i; j++)
		{
			if (edge_st[0][j] < edge_st[0][j + 1])
			{
				tmp_l = edge_st[0][j];
				edge_st[0][j] = edge_st[0][j + 1];
				edge_st[0][j + 1] = tmp_l;
			}
		}
	}

	double max_data = edge_st[image_x - 1][image_y-1];
	double min_data = edge_st[0][image_y - 1];
	k = (max_data - min_data) / 256;
	
	printf("max_data=%f,min_data=%fk=%f\n\n", max_data, min_data,k);
	double k2;
	int j_start=0;
//	printf("max_data=%f,min_data=%f,k=%f\n", max_data, min_data,k);

  /* ヒストグラム作成 */
  std::vector<int> hist(256, 0);  // 0-255の256段階のヒストグラム（要素数256、全て0で初期化）
  int kaidan = 0;
  for (int i = 0; i < DATA_NUM; ++i) {
	 // printf("j_start=%d\n", j_start);
	  for (int j = j_start; j < 255; ++j) {
		  
		  if (data[i][0] >= k*j && data[i][0] < k*(j + 1)) {
			  hist[j]++;
			 // printf("j=%d,i=%d\n", j,i);
			//  j_start = j;
			//  break;
		  }
	  }
	 
  }
  /*
  for (int j = 0; j < 255; ++j) {
	  printf("hist[%d]=%d\n", j,hist[j]);
  }
  */
  hist_hozon(date_directory, k,hist);

  //ヒストグラム描画
   int hist_max=0;
   //hist[0]を表示しない
  // for(int i=0;i<256;++i){
   for (int i = 1; i<256; ++i) {
	   if(hist[i]>hist_max)hist_max=hist[i];
   }
  // printf("hist_max=%d\n",hist_max);

   float histf[256];
   //hist[0]を表示しない
   // for(int i=0;i<256;++i){
   for (int i = 1; i<256; ++i) {
	   histf[i]=hist[i]/(float)hist_max;
	   //printf("histf=%f",histf[i]);
   }

   Mat hist_image= Mat(Size(276,140),CV_8UC3,Scalar(255,255,255));
	rectangle(hist_image,Point(10,20),Point(265,100),Scalar(220,220,220),-1);
	//hist[0]を表示しない
	// for(int i=0;i<256;++i){
	for (int i = 1; i<256; ++i) {
	 line(hist_image,Point(10+i,100),Point(10+i,100-(float)(histf[i])*80),Scalar(0,0,255),1,8,0);
	}
//	namedWindow("ヒストグラム");
//	imshow("ヒストグラム",hist_image);

	//ヒストグラム画像の保存
	char hist_directory[128]; 
	sprintf(hist_directory, "%s_hist.bmp", date_directory);
	imwrite(hist_directory, hist_image);

   
  /* 判別分析法 */
  int t = 0;  // 閾値
  double t2 = 0;	//実際の閾値
  double max = 0.0;  // w1 * w2 * (m1 - m2)^2 の最大値
  
  for (int i = 0; i < 256; ++i){
    int w1 = 0;  // クラス１の画素数
    int w2 = 0;  // クラス２の画素数
    long sum1 = 0;  // クラス１の平均を出すための合計値
    long sum2 = 0;  // クラス２の平均を出すための合計値
    double m1 = 0.0;  // クラス１の平均
    double m2 = 0.0;  // クラス２の平均
    
    for (int j = 0; j <= i; ++j){
      w1 += hist[j];
      sum1 += j * hist[j];
    }
    
    for (int j = i+1; j < 256; ++j){
      w2 += hist[j];
      sum2 += j * hist[j];
    }
    
    if (w1)
      m1 = (double)sum1 / w1;
    
    if (w2)
      m2 = (double)sum2 / w2;
    
    double tmp = ((double)w1 * w2 * (m1 - m2) * (m1 - m2));
    
    if (tmp > max){
      max = tmp;
      t = i;
    }
  }
  
  
  t2 = k*(t-1);
  //printf("t=%d\nt2=%f\n", t,t2);

  free_matrix(data, 0, DATA_NUM - 1, 0, 1);
  //  free_matrix(tmp, 0, DATA_NUM - 1, 0, 1);
  free_matrix(tmp, 0, image_y - 1, 0, 1);
  
  return t2;
}

/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
////////////////ファイルの読み込み_2方向のみ//////////////////////////////////////////////////////
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
int readfiles(std::vector<std::vector<double>> &edge_st,char date_directory[], char output_directory[], int &image_x, int &image_y,char &math_name1_s, char &Input_Filename1_s, char &Input_Filename3_s){
	int image_width= image_x;						//入力画像の横幅
	int image_wide=image_width+1;							//入力画像の横幅+1

	int i=1,j=1;
	
	
	double **V0 = matrix(0, image_x - 1, 0, image_y - 1);
	double **V90 = matrix(0, image_x - 1, 0, image_y - 1);

	for (i = 0; i < image_y; i++) {
		for (j = 0; j < image_x; j++) {
			V0[j][i] = 0;
			V90[j][i] = 0;
		}
	}
	
	char Input_Rvectormagni_name[255];
	char Input_Filename1[255];						//入力ファイル名・入力元の設定
	char Input_Filename3[255];
	

	double Rvectormagni[10];						//応答電圧の倍率

	

	//Input
	sprintf(Input_Filename1, "%s%s", date_directory, &Input_Filename1_s);
	sprintf(Input_Filename3, "%s%s", date_directory, &Input_Filename3_s);
	printf("%s", Input_Filename1);
	

////////////////////////////ファイルの読み込み//////////////////////////////////////////////////////////////////////////////
	
	ifstream V_0(Input_Filename1);
	ifstream V_90(Input_Filename3);

	
	if(!V_0){cout<<"入力エラー V(0).csvがありません";exit(1);}
	if(!V_90){cout<<"入力エラー V(90).csvがありません";exit(1);}
	
	
	FILE *fp_edge_st;
	char math_name1[128];

	sprintf(math_name1, "%s%s", output_directory, &math_name1_s);
	
	
		if((fp_edge_st=fopen(math_name1,"w"))==NULL){cout<<"入力エラー edge_st.csvが開けません";exit(1);}

///////////////////////応答電圧のcsvの読み込み//////////////////////////////////////////////////////////////////////////////////////////
		i=1;
		
		string str_0,str_45,str_90,str_135,str_180,str_225,str_270,str_315;
		int count_large = 0;
		while(getline(V_0,str_0)){					//このループ内ですべてを行う
			int count_small=0;			//初期化
			
			


///////////////いろいろ定義．ここでやらないといけない///////////////////////////////////////////////////////////////////////////
        string token_V_0;
        istringstream stream_V_0(str_0);
		getline(V_90, str_90);	string token_V_90;	istringstream stream_V_90(str_90);
		
		

//////////////////////////配列に代入//////////////////////////////////////////////////////////////////////////////////////////////////

        while(getline(stream_V_0,token_V_0,',')){	//一行読み取る．V0のみ，繰り返しの範囲指定に用いる
			double tmp_V_0=stof(token_V_0);			//文字を数字に変換
			V0[count_small][count_large]=tmp_V_0;				//配列に代入
			//V0[count_small]=Rvectormagni[1]*V0[count_small];

			
        
			getline(stream_V_90,token_V_90,',');
			double tmp_V_90=stof(token_V_90);
			V90[count_small][count_large]=tmp_V_90;
			//V90[count_small]=Rvectormagni[3]*V90[count_small];

			edge_st[count_small][count_large]=sqrt(pow(V0[count_small][count_large],2)+pow(V90[count_small][count_large],2));
			++count_small;
		}
	++count_large;
	
		
		}
		for(j=0;j<image_y;++j){
	for(i=0;i<image_x;++i){
		fprintf(fp_edge_st,"%lf,",edge_st[i][j]);
		if(i==image_width-1){fprintf(fp_edge_st,"\n");}
		}
		}
		fclose(fp_edge_st);
	
		
		//return edge_st;
		free_matrix(V0, 0, image_x - 1, 0, image_y - 1);
		free_matrix(V90, 0, image_x - 1, 0, image_y - 1);
		
		
}

/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
////////////////ファイルの読み込み_8方向//////////////////////////////////////////////////////
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
int readfiles_8dire(
	std::vector<std::vector<double>> &edge_st, 
	char date_directory[], 
	char output_directory[], 
	int &image_x, 
	int &image_y, 
	char &math_name1_s, 
	char &Input_Filename1_s, 
	char &Input_Filename2_s,
	char &Input_Filename3_s,
	char &Input_Filename4_s,
	char &Input_Filename5_s,
	char &Input_Filename6_s,
	char &Input_Filename7_s,
	char &Input_Filename8_s
	) {
	int image_width = image_x;						//入力画像の横幅
	int image_wide = image_width + 1;							//入力画像の横幅+1

	int i = 1, j = 1;


	double **V0 = matrix(0, image_x - 1, 0, image_y - 1);
	double **V45 = matrix(0, image_x - 1, 0, image_y - 1);
	double **V90 = matrix(0, image_x - 1, 0, image_y - 1);
	double **V135 = matrix(0, image_x - 1, 0, image_y - 1);
	double **V180 = matrix(0, image_x - 1, 0, image_y - 1);
	double **V225 = matrix(0, image_x - 1, 0, image_y - 1);
	double **V270 = matrix(0, image_x - 1, 0, image_y - 1);
	double **V315 = matrix(0, image_x - 1, 0, image_y - 1);

	for (i = 0; i < image_y; i++) {
		for (j = 0; j < image_x; j++) {
			V0[j][i] = 0;
			V45[j][i] = 0;
			V90[j][i] = 0;
			V135[j][i] = 0;
			V180[j][i] = 0;
			V225[j][i] = 0;
			V270[j][i] = 0;
			V315[j][i] = 0;
		}
	}
	

	//char *Input_Rvectormagniname_s = "\\Rvector_magni.csv";	//応答電圧の倍率
	

	char Input_Rvectormagni_name[255];
	char Input_Filename1[255];						//入力ファイル名・入力元の設定
	char Input_Filename2[255];
	char Input_Filename3[255];
	char Input_Filename4[255];
	char Input_Filename5[255];
	char Input_Filename6[255];
	char Input_Filename7[255];
	char Input_Filename8[255];
	

	double Rvectormagni[10];						//応答電圧の倍率



													//Input
	sprintf(Input_Filename1, "%s%s", date_directory, &Input_Filename1_s);
	sprintf(Input_Filename2, "%s%s", date_directory, &Input_Filename2_s);
	sprintf(Input_Filename3, "%s%s", date_directory, &Input_Filename3_s);
	sprintf(Input_Filename4, "%s%s", date_directory, &Input_Filename4_s);
	sprintf(Input_Filename5, "%s%s", date_directory, &Input_Filename5_s);
	sprintf(Input_Filename6, "%s%s", date_directory, &Input_Filename6_s);
	sprintf(Input_Filename7, "%s%s", date_directory, &Input_Filename7_s);
	sprintf(Input_Filename8, "%s%s", date_directory, &Input_Filename8_s);
	printf("%s", Input_Filename1);


	////////////////////////////ファイルの読み込み//////////////////////////////////////////////////////////////////////////////
	//Input
	//ifstream Rvector_magni(Input_Rvectormagni_name);

	ifstream V_0(Input_Filename1);
	ifstream V_45(Input_Filename2);
	ifstream V_90(Input_Filename3);
	ifstream V_135(Input_Filename4);
	ifstream V_180(Input_Filename5);
	ifstream V_225(Input_Filename6);
	ifstream V_270(Input_Filename7);
	ifstream V_315(Input_Filename8);
	
	//if(!Rvector_magni){cout<<"入力エラー Rvector_magni.csvがありません";return 1;}

	if (!V_0) { cout << "入力エラー V(0).csvがありません"; return 1; }
	if (!V_45) { cout << "入力エラー V(45).csvがありません"; return 1; }
	if (!V_90) { cout << "入力エラー V(90).csvがありません"; return 1; }
	if(!V_135){cout<<"入力エラー V(135).csvがありません";return 1;}
	if(!V_180){cout<<"入力エラー V(180).csvがありません";return 1;}
	if(!V_225){cout<<"入力エラー V(225).csvがありません";return 1;}
	if(!V_270){cout<<"入力エラー V(270).csvがありません";return 1;}
	if(!V_315){cout<<"入力エラー V(315).csvがありません";return 1;}
	
	/*
	string str_Rvector_magni;
	i=1;
	while (getline(Rvector_magni,str_Rvector_magni)){
	string token_Rvector_magni;
	istringstream stream(str_Rvector_magni);
	while(getline(stream,token_Rvector_magni,',')){
	double temp_Rvector_magni=stof(token_Rvector_magni); //stof(string str) : stringをfloatに変換
	Rvectormagni[i]=temp_Rvector_magni;
	i++;
	}
	}
	*/
	FILE *fp_edge_st;
	char math_name1[128];

	sprintf(math_name1, "%s%s", output_directory, &math_name1_s);


	if ((fp_edge_st = fopen(math_name1, "w")) == NULL) { cout << "入力エラー edge_st.csvが開けません"; exit(1); }

	///////////////////////応答電圧のcsvの読み込み//////////////////////////////////////////////////////////////////////////////////////////
	i = 1;

	string str_0, str_45, str_90, str_135, str_180, str_225, str_270, str_315;
	int count_large = 0;
	while (getline(V_0, str_0)) {					//このループ内ですべてを行う
		int count_small = 0;			//初期化




										///////////////いろいろ定義．ここでやらないといけない///////////////////////////////////////////////////////////////////////////
		string token_V_0;
		istringstream stream_V_0(str_0);
		getline(V_90, str_90);	string token_V_90;	istringstream stream_V_90(str_90);
		getline(V_45,str_45);	string token_V_45;	istringstream stream_V_45(str_45);
		getline(V_135,str_135);	string token_V_135;	istringstream stream_V_135(str_135);
		getline(V_180,str_180);	string token_V_180;	istringstream stream_V_180(str_180);
		getline(V_225,str_225);	string token_V_225;	istringstream stream_V_225(str_225);
		getline(V_270,str_270);	string token_V_270;	istringstream stream_V_270(str_270);
		getline(V_315,str_315);	string token_V_315;	istringstream stream_V_315(str_315);
	

		//////////////////////////配列に代入//////////////////////////////////////////////////////////////////////////////////////////////////

		while (getline(stream_V_0, token_V_0, ',')) {	//一行読み取る．V0のみ，繰り返しの範囲指定に用いる
			double tmp_V_0 = stof(token_V_0);			//文字を数字に変換
			V0[count_small][count_large] = tmp_V_0;				//配列に代入
																//V0[count_small]=Rvectormagni[1]*V0[count_small];



			getline(stream_V_90, token_V_90, ',');
			double tmp_V_90 = stof(token_V_90);
			V90[count_small][count_large] = tmp_V_90;
			//V90[count_small]=Rvectormagni[3]*V90[count_small];

			
			getline(stream_V_45, token_V_45, ',');
			double tmp_V_45 = stof(token_V_45);
			V45[count_small][count_large] = tmp_V_45;
			//V45[count_small] = Rvectormagni[2] * V45[count_small];

			getline(stream_V_135,token_V_135,',');
			double tmp_V_135=stof(token_V_135);
			V135[count_small][count_large] =tmp_V_135;
			//V135[count_small]=Rvectormagni[4]*V135[count_small];

			getline(stream_V_180,token_V_180,',');
			double tmp_V_180=stof(token_V_180);
			V180[count_small][count_large] =tmp_V_180;
			//V180[count_small]=Rvectormagni[5]*V180[count_small];

			getline(stream_V_225,token_V_225,',');
			double tmp_V_225=stof(token_V_225);
			V225[count_small][count_large] =tmp_V_225;
			//V225[count_small]=Rvectormagni[6]*V225[count_small];

			getline(stream_V_270,token_V_270,',');
			double tmp_V_270=stof(token_V_270);
			V270[count_small][count_large] =tmp_V_270;
			//V270[count_small]=Rvectormagni[7]*V270[count_small];

			getline(stream_V_315,token_V_315,',');
			double tmp_V_315=stof(token_V_315);
			V315[count_small][count_large] =tmp_V_315;
			//V315[count_small]=Rvectormagni[8]*V315[count_small];
			
			//ここを変える
			edge_st[count_small][count_large] 
				= sqrt(pow(V0[count_small][count_large], 2) 
					+ pow(V45[count_small][count_large], 2) 
					+ pow(V90[count_small][count_large], 2)
					+ pow(V135[count_small][count_large], 2)
					+ pow(V180[count_small][count_large], 2)
					+ pow(V225[count_small][count_large], 2)
					+ pow(V270[count_small][count_large], 2)
					+ pow(V315[count_small][count_large], 2)
				);
			++count_small;
		}
		++count_large;


	}
	for (j = 0; j<image_y; ++j) {
		for (i = 0; i<image_x; ++i) {
			//edge_st[i][j]=pow(V0[j],2)+pow(V90[j],2);

			fprintf(fp_edge_st, "%lf,", edge_st[i][j]);
			if (i == image_width - 1) { fprintf(fp_edge_st, "\n"); }
		}
	}
	fclose(fp_edge_st);
	//	fclose(fp_outputV0);
	//	fclose(fp_outputV90);

	//return edge_st;
	free_matrix(V0, 0, image_x - 1, 0, image_y - 1);
	free_matrix(V45, 0, image_x - 1, 0, image_y - 1);
	free_matrix(V90, 0, image_x - 1, 0, image_y - 1);
	free_matrix(V135, 0, image_x - 1, 0, image_y - 1);
	free_matrix(V180, 0, image_x - 1, 0, image_y - 1);
	free_matrix(V225, 0, image_x - 1, 0, image_y - 1);
	free_matrix(V270, 0, image_x - 1, 0, image_y - 1);
	free_matrix(V315, 0, image_x - 1, 0, image_y - 1);


}


////////////////入出力ディレクトリの設定////////////////////////////////////////////////////
int IO_directory(char *inputdate_directory, char *outputdate_directory, char date_directory[], int paramerter[], int paramerter_count, int sd) {

	switch (paramerter[0]) {
	case 1:
		sprintf(inputdate_directory, "%s%d×%dsobel_conv_sd%d", date_directory, paramerter[paramerter_count], paramerter[paramerter_count], sd);	
		sprintf(outputdate_directory, "%s%d×%dsobel_atan_sd%d", date_directory, paramerter[paramerter_count], paramerter[paramerter_count], sd);
		break;
	case 2:
		sprintf(inputdate_directory, "%s%d×%dsobel_conv_sd%d", date_directory, paramerter[paramerter_count], paramerter[paramerter_count], sd);	
		sprintf(outputdate_directory, "%s%d×%dsobel_atan_sd%d", date_directory, paramerter[paramerter_count], paramerter[paramerter_count], sd);
		break;
	case 3:
		sprintf(inputdate_directory, "%s%dk_conv_sd%d", date_directory, paramerter[paramerter_count], sd);	
		sprintf(outputdate_directory, "%s%dk_cossim_sd%d", date_directory, paramerter[paramerter_count], sd);	
		break;
	case 4:
		sprintf(inputdate_directory, "%s%d×%dsobel_conv_sd%d", date_directory, paramerter[paramerter_count], paramerter[paramerter_count], sd);	
		sprintf(outputdate_directory, "%s%d×%dsobel_atan_sd%d", date_directory, paramerter[paramerter_count], paramerter[paramerter_count], sd);
		break;
	case 5:
		sprintf(inputdate_directory, "%s%d×%dsobel_conv_sd%d", date_directory, paramerter[paramerter_count], paramerter[paramerter_count], sd);	
		sprintf(outputdate_directory, "%s%d×%dsobel_atan_sd%d", date_directory, paramerter[paramerter_count], paramerter[paramerter_count], sd);
		break;
	default:
		sprintf(inputdate_directory, "%s%dk_conv_sd%d", date_directory, paramerter[paramerter_count], sd);	
		sprintf(outputdate_directory, "%s%dk_cossim_sd%d", date_directory, paramerter[paramerter_count], sd);	
		break;
	}

	return 0;
}

///////////////テンプレート画像の際の判別分析法//////////////////////////////////////////////////////////////
double edge_st_temp(char date_directory[], int &image_xt, int &image_yt, int paramerter[], int paramerter_count, int sd,int &Edge_derectory_number, char date[]) {

	char inputdate_directory[128];
	char outputdate_directory[128];

	char *Input_Filename1_s = "\\V(0)t.csv";			//読み込むの指定
	char *Input_Filename2_s = "\\V(45)t.csv";
	char *Input_Filename3_s = "\\V(90)t.csv";
	char *Input_Filename4_s = "\\V(135)t.csv";
	char *Input_Filename5_s = "\\V(180)t.csv";
	char *Input_Filename6_s = "\\V(225)t.csv";
	char *Input_Filename7_s = "\\V(270)t.csv";
	char *Input_Filename8_s = "\\V(315)t.csv";
	char *math_name1_s = "\\edge_st_t.csv";

	//input,outputファイルのディレクトリ設定
	IO_directory(inputdate_directory, outputdate_directory, date_directory, paramerter, paramerter_count, sd);

	std::vector<std::vector<double>>edge_st;
	edge_st.resize(image_xt);
	for (int i = 0; i<image_xt; ++i) {
		edge_st[i].resize(image_yt);
	}
	switch (Edge_derectory_number) {
		//2方向使う場合
		case 2:
			readfiles(edge_st, inputdate_directory, outputdate_directory, image_xt, image_yt, *math_name1_s, *Input_Filename1_s, *Input_Filename3_s);
			printf("\n2方向でエッジ強度を求めます\n");
			break;

		//8方向使う場合
		case 8:
			readfiles_8dire(edge_st, inputdate_directory, outputdate_directory, image_xt, image_yt, *math_name1_s, *Input_Filename1_s, *Input_Filename2_s, *Input_Filename3_s ,*Input_Filename4_s, *Input_Filename5_s, *Input_Filename6_s, *Input_Filename7_s, *Input_Filename8_s );
			printf("\n8方向でエッジ強度を求めます\n");
			break;

		default:
			printf("Edge_derectory_numberがおかしい\nEdge_derectory_number=%d\n", Edge_derectory_number);
			return 0;
		}

	double b = discriminantAnalysis(inputdate_directory, image_xt, image_yt, edge_st);

	//return 0;
	return b;

}

///////////////対象画像での判別分析法//////////////////////////////////////////////////////
int otsu(char date_directory[], int &image_x, int &image_y,int paramerter[], int paramerter_count, int sd, int &Edge_derectory_number, char date[]){

	char inputdate_directory[128];
	char outputdate_directory[128];

	//input,outputファイルのディレクトリ設定
	IO_directory(inputdate_directory, outputdate_directory,date_directory,paramerter,paramerter_count, sd);
	
	//出力する値
	std::vector<std::vector<double>>edge_st;
	edge_st.resize(image_x);
	for (int i = 0; i<image_x; ++i) {
		edge_st[i].resize(image_y);
	}

	char *Input_Filename1_s = "\\V(0)t.csv";			//読み込むの指定
	char *Input_Filename2_s = "\\V(45)t.csv";
	char *Input_Filename3_s = "\\V(90)t.csv";
	char *Input_Filename4_s = "\\V(135)t.csv";
	char *Input_Filename5_s = "\\V(180)t.csv";
	char *Input_Filename6_s = "\\V(225)t.csv";
	char *Input_Filename7_s = "\\V(270)t.csv";
	char *Input_Filename8_s = "\\V(315)t.csv";
	char *math_name1_s = "\\edge_st.csv";

	switch (Edge_derectory_number) {
		//2方向使う場合
		case 2:
			readfiles(edge_st, inputdate_directory, outputdate_directory, image_x, image_y, *math_name1_s, *Input_Filename1_s, *Input_Filename3_s);
			printf("\n2方向でエッジ強度を求めます\n");
			break;

		//8方向使う場合
		case 8:
			readfiles_8dire(edge_st, inputdate_directory, outputdate_directory, image_x, image_y, *math_name1_s, *Input_Filename1_s, *Input_Filename2_s, *Input_Filename3_s, *Input_Filename4_s, *Input_Filename5_s, *Input_Filename6_s, *Input_Filename7_s, *Input_Filename8_s);
			printf("\n8方向でエッジ強度を求めます\n");
			break;

		default:
			printf("Edge_derectory_numberがおかしい\nEdge_derectory_number=%d\n", Edge_derectory_number);
			return 0;
	}

	//対象画像で閾値を求める
	//double b = discriminantAnalysis(inputdate_directory,image_x,image_y, edge_st);
	//printf("b=%e\n", b);

//  waitKey(0);
//  destroyAllWindows();
 
	return 0;
  //return b;
}