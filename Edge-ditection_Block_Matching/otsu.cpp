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

////////////////////////ヒストグラムの作成及び閾値の設定////////////////////////////////////////////////////////////////////////

int discriminantAnalysis(char date_directory[], int &image_x, int &image_y, std::vector<std::vector<double>> &edge_st){

	printf("%s\n",date_directory);
	int DATA_NUM = image_x *image_y;
	double tmp = 0;
	double k;	//幅

	//Nrutilを用いたメモリの確保
	double **data = matrix(0, image_x *image_y-1, 0, 1);

	//確保したメモリを初期化する
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < image_x *image_y; j++) {
			data[j][i] = 0;
		}
	}
	//一次元配列に代入
	for (int i = 0; i < image_y; i++) {
		for (int j = 0; j < image_x; j++) {
			data[j*(i+1)][0] = edge_st[j][i];
		}
	}
	//データを大きい順に並び替える
	for (int i = 1; i<DATA_NUM; i++)
	{
		for (int j = 0; j < DATA_NUM - i; j++)
		{
			if (data[j][0] > data[j + 1][0])
			{
				tmp = data[j][0];
				data[j][0] = data[j + 1][0];
				data[j + 1][0] = tmp;
			}
		}
	}
	double max_data = data[DATA_NUM - 1][0];
	double min_data =  data[0][0];
	k = (max_data - min_data) / 256;
	double k2;
	int j_start=0;
//	printf("max_data=%f,min_data=%f,k=%f\n", max_data, min_data,k);

  /* ヒストグラム作成 */
  std::vector<int> hist(256, 0);  // 0-255の256段階のヒストグラム（要素数256、全て0で初期化）
  int kaidan = 0;
  for (int i = 0; i < DATA_NUM; ++i) {
	 // printf("j_start=%d\n", j_start);
	  for (int j = j_start; j < 255; ++j) {
		  
		  if (data[i][0] > k*j && data[i][0] < k*(j + 1)) {
			  hist[j]++;
			  j_start = j;
			  break;
		  }
	  }

  }
  for (int j = 0; j < 255; ++j) {
	  printf("hist[%d]=%d\n", j,hist[j]);
  }
  
  //ヒストグラム描画
   int hist_max=0;
   for(int i=0;i<256;++i){
	   if(hist[i]>hist_max)hist_max=hist[i];
   }
   printf("hist_max=%d\n",hist_max);

   float histf[256];
   for (int i = 0; i < 256; ++i){
	   histf[i]=hist[i]/(float)hist_max;
	   //printf("histf=%f",histf[i]);
   }

   Mat hist_image= Mat(Size(276,320),CV_8UC3,Scalar(255,255,255));
	rectangle(hist_image,Point(10,20),Point(265,100),Scalar(220,220,220),-1);
	 for(int i=0;i<256;i++){
	 line(hist_image,Point(10+i,100),Point(10+i,100-(float)(histf[i])*80),Scalar(0,0,255),1,8,0);
	}
	namedWindow("ヒストグラム");
	imshow("ヒストグラム",hist_image);

   
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
  
  
  t2 = (k - 1)*t;
//  printf("t=%d\nt2=%f\n", t,t2);
  
  return t2;
}


////////////////ファイルの読み込み/////////////////////////////////////////////////////////////////////////////////////////////////
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
	/*
	double V45[255];
	double V135[255];
	double V180[255];
	double V225[255];
	double V270[255];
	double V315[255];
	*/

	

	//char *Input_Rvectormagniname_s = "\\Rvector_magni.csv";	//応答電圧の倍率
	//char *Input_Filename1_s = "\\V(0).csv";			//読み込むの指定
	//char *Input_Filename3_s = "\\V(90).csv";
	/*
	char *Input_Filename2_s = "\\V(45).csv";
	char *Input_Filename4_s = "\\V(135).csv";
	char *Input_Filename5_s = "\\V(180).csv";
	char *Input_Filename6_s = "\\V(225).csv";
	char *Input_Filename7_s = "\\V(270).csv";
	char *Input_Filename8_s = "\\V(315).csv";
	*/

	char Input_Rvectormagni_name[255];
	char Input_Filename1[255];						//入力ファイル名・入力元の設定
	char Input_Filename3[255];
	/*
	char Input_Filename2[255];
	char Input_Filename4[255];
	char Input_Filename5[255];
	char Input_Filename6[255];
	char Input_Filename7[255];
	char Input_Filename8[255];
	*/

	double Rvectormagni[10];						//応答電圧の倍率

	

	//Input
	sprintf(Input_Filename1, "%s%s", date_directory, &Input_Filename1_s);
	sprintf(Input_Filename3, "%s%s", date_directory, &Input_Filename3_s);
	printf("%s", Input_Filename1);
	

////////////////////////////ファイルの読み込み//////////////////////////////////////////////////////////////////////////////
	//Input
	//ifstream Rvector_magni(Input_Rvectormagni_name);

	ifstream V_0(Input_Filename1);
	ifstream V_90(Input_Filename3);

	/*
	ifstream V_45(Input_Filename2);
	ifstream V_135(Input_Filename4);
	ifstream V_180(Input_Filename5);
	ifstream V_225(Input_Filename6);
	ifstream V_270(Input_Filename7);
	ifstream V_315(Input_Filename8);
	*/
	//if(!Rvector_magni){cout<<"入力エラー Rvector_magni.csvがありません";return 1;}
	
	if(!V_0){cout<<"入力エラー V(0).csvがありません";return 1;}
	if(!V_90){cout<<"入力エラー V(90).csvがありません";return 1;}
	
	/*
	if (!V_45) { cout << "入力エラー V(45).csvがありません"; return 1; }
	if(!V_135){cout<<"入力エラー V(135).csvがありません";return 1;}
	if(!V_180){cout<<"入力エラー V(180).csvがありません";return 1;}
	if(!V_225){cout<<"入力エラー V(225).csvがありません";return 1;}
	if(!V_270){cout<<"入力エラー V(270).csvがありません";return 1;}
	if(!V_315){cout<<"入力エラー V(315).csvがありません";return 1;}
	*/
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
		/*
		getline(V_45,str_45);	string token_V_45;	istringstream stream_V_45(str_45);
		getline(V_135,str_135);	string token_V_135;	istringstream stream_V_135(str_135);
		getline(V_180,str_180);	string token_V_180;	istringstream stream_V_180(str_180);
		getline(V_225,str_225);	string token_V_225;	istringstream stream_V_225(str_225);
		getline(V_270,str_270);	string token_V_270;	istringstream stream_V_270(str_270);
		getline(V_315,str_315);	string token_V_315;	istringstream stream_V_315(str_315);
		*/

		//////////////////////////配列に代入//////////////////////////////////////////////////////////////////////////////////////////////////

        while(getline(stream_V_0,token_V_0,',')){	//一行読み取る．V0のみ，繰り返しの範囲指定に用いる
			double tmp_V_0=stof(token_V_0);			//文字を数字に変換
			V0[count_small][count_large]=tmp_V_0;				//配列に代入
			//V0[count_small]=Rvectormagni[1]*V0[count_small];

			
        
			getline(stream_V_90,token_V_90,',');
			double tmp_V_90=stof(token_V_90);
			V90[count_small][count_large]=tmp_V_90;
			//V90[count_small]=Rvectormagni[3]*V90[count_small];

			/*
			getline(stream_V_45, token_V_45, ',');
			double tmp_V_45 = stof(token_V_45);
			V45[count_small] = tmp_V_45;
			V45[count_small] = Rvectormagni[2] * V45[count_small];

			getline(stream_V_135,token_V_135,',');
			double tmp_V_135=stof(token_V_135);
			V135[count_small]=tmp_V_135;
			V135[count_small]=Rvectormagni[4]*V135[count_small];

			getline(stream_V_180,token_V_180,',');
			double tmp_V_180=stof(token_V_180);
			V180[count_small]=tmp_V_180;
			V180[count_small]=Rvectormagni[5]*V180[count_small];

			getline(stream_V_225,token_V_225,',');
			double tmp_V_225=stof(token_V_225);
			V225[count_small]=tmp_V_225;
			V225[count_small]=Rvectormagni[6]*V225[count_small];

			getline(stream_V_270,token_V_270,',');
			double tmp_V_270=stof(token_V_270);
			V270[count_small]=tmp_V_270;
			V270[count_small]=Rvectormagni[7]*V270[count_small];

			getline(stream_V_315,token_V_315,',');
			double tmp_V_315=stof(token_V_315);
			V315[count_small]=tmp_V_315; 
			V315[count_small]=Rvectormagni[8]*V315[count_small];
			*/

			edge_st[count_small][count_large]=pow(V0[count_small][count_large],2)+pow(V90[count_small][count_large],2);
			++count_small;
		}
	++count_large;
	
		
		}
		for(j=0;j<image_y;++j){
	for(i=0;i<image_x;++i){
			//edge_st[i][j]=pow(V0[j],2)+pow(V90[j],2);

		fprintf(fp_edge_st,"%lf,",edge_st[i][j]);
		if(i==image_width-1){fprintf(fp_edge_st,"\n");}
		}
		}
		fclose(fp_edge_st);
	//	fclose(fp_outputV0);
	//	fclose(fp_outputV90);
		
		//return edge_st;
		free_matrix(V0, 0, image_x - 1, 0, image_y - 1);
		free_matrix(V90, 0, image_x - 1, 0, image_y - 1);
		
		
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
int edge_st_temp(char date_directory[], int &image_xt, int &image_yt, int paramerter[], int paramerter_count, int sd) {

	char inputdate_directory[128];
	char outputdate_directory[128];

	char *Input_Filename1_s = "\\V(0)t.csv";			//読み込むの指定
	char *Input_Filename3_s = "\\V(90)t.csv";
	char *math_name1_s = "\\edge_st_t.csv";

	//input,outputファイルのディレクトリ設定
	IO_directory(inputdate_directory, outputdate_directory, date_directory, paramerter, paramerter_count, sd);

	std::vector<std::vector<double>>edge_st;
	edge_st.resize(image_xt);
	for (int i = 0; i<image_xt; ++i) {
		edge_st[i].resize(image_yt);
	}

	readfiles(edge_st, inputdate_directory, outputdate_directory, image_xt, image_yt, *math_name1_s, *Input_Filename1_s, *Input_Filename3_s);

	return 0;


}

///////////////対象画像での判別分析法//////////////////////////////////////////////////////
int otsu(char date_directory[], int &image_x, int &image_y,int paramerter[], int paramerter_count, int sd){

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

	char *Input_Filename1_s = "\\V(0).csv";			//読み込むの指定
	char *Input_Filename3_s = "\\V(90).csv";
	char *math_name1_s = "\\edge_st.csv";
	readfiles(edge_st, inputdate_directory, outputdate_directory, image_x, image_y,*math_name1_s, *Input_Filename1_s, *Input_Filename3_s);

  double b = discriminantAnalysis(inputdate_directory,image_x,image_y, edge_st);
 // printf("b=%f\n", b);

//  waitKey(0);
//  destroyAllWindows();
 
  return b;
}