
//メモリ確保を行うためのヘッダ
#define ANSI				
#include "nrutil.h"	

#include<stdio.h>
#include<math.h>
#include <omp.h>	//openMP

#include<fstream>
#include<iostream>
#include<string>
#include<sstream> //文字ストリーム

#include<time.h>//時間を用いる

#include <direct.h>//フォルダを作成する

#include <opencv2/opencv.hpp>	//画像読み込み

#include <tuple>
std::tuple<int, int, std::vector<std::vector<double>>> read_txt(const char *filename);

using namespace std;

int conv_eco_mode_flag = 1;

int x, y, fx, fy;
int hfs;
FILE *fp;

//property関係_変数
//int &image_x, &image_y;		//画像サイズ
int Rvector_create=0;		//1なら基準ベクトルを作る
int Rvector_pointX;		//基準ベクトルを作成する位置.0をスタートで考える
int Rvector_pointY;		//特殊な指定をする場合は下へ
int fs;					//フィルタの大きさ(奇数)
int All_property[10];	//propertyから上記の情報を読み込む用

//property関係_ディレクトリ
char propety[255];
char inputtemplate_directory[255];
char outputrvector_directory[128];				//Rvectorの場所
char inputimage_directory[128];
char inputfilter_directory[255];
char inputfilter_directory1[255];
char inputfilter_directory2[255];
char inputfilter_directory3[255];
char inputfilter_directory4[255];
char inputfilter_directory5[255];
char inputfilter_directory6[255];
char inputfilter_directory7[255];
char inputfilter_directory8[255];			//繰返し数を増やしたい1/4
char inputfilter_directory11[255];
char inputfilter_directory12[255];
char inputfilter_directory13[255];
char inputfilter_directory14[255];
char inputfilter_directory15[255];
char inputfilter_directory16[255];
char inputfilter_directory17[255];
char inputfilter_directory18[255];
int count_property = 0;
int count_Allproperty = 0;

char InputImage[128];						//入力画像
char Inputtemplate[128];					//入力テンプレート

int filter_reset_flag=0;

//出力ファイル名・出力先の設定
char Filename[64];
char Filenamet[64];
char Filename1[64];
char Filename2[64];
char Filename3[64];
char Filename4[64];
char Filename5[64];
char Filename6[64];
char Filename7[64];
char Filename8[64];
char Filename11[64];
char Filename12[64];
char Filename13[64];
char Filename14[64];
char Filename15[64];
char Filename16[64];
char Filename17[64];
char Filename18[64];

//Rvector関連
char Rvector_directory[128];
double Rvector[9];
char Rvector_Filename[255];
char *Rvector_Filename_s;

//出力ファイルディレクトリ
//char date_directory[128];
char date_directory2[128];
char outputbmp_directory[128];

void read_property(ifstream &propety_dire,int &image_x,int &image_y,int &fs);
void Rvector_createF();
void set_outputfile(char date[],char date_directory[],int paramerter[],int paramerter_count,int sd);
std::tuple<int,std::vector<std::vector<double>>> read_filter(char inputfilter_directory[]);
int convolution_gaus_sobel(int &image_y,int &image_x,int fs, int hfs,double *output1[],double *spfil1[],double *input_bmp[],double magnification);

void read_filter_gaus(int fs,double *spfil1_g[]);

int convolution(int argc, char** argv,char image_nameP2[],int &image_x,int &image_y, int &image_xt, int &image_yt, int paramerter[],int paramerter_count,int sd,char date[],char date_directory[], char InputImage[]) {

	//個別で画像を指定したいとき
	//std::ifstream propety_dire("..\\property_usa\\simulation17-0613\\property_3k_conv_sd0.txt");
	//基準ベクトルを取得するときはこちら
	//int filter_number = 0;
	//sprintf(Rvector_Filename, "%s\\Rvector%d.csv", Rvector_directory, filter_number);
	//フィルタ演算，基準ベクトル作成するときのみ指定．ここを変更する
	/*	char propety_dire_char[128];
	sprintf(propety_dire_char,"..\\property_usa\\simulation17-0203\\Rvector\\8dire_100k\\property_8dire_100k_I~%d.txt",filter_number);
	std::ifstream propety_dire(propety_dire_char);*/
	
	//閾値の設定．今後property.txtに加えるかも
	int Offset = 128;
	double magnification = 1;
	float Upper_threshold = 35;
	float Under_threshold = -35;
	int use_upperthreshold_flag = 1;
	int use_underthreshold_flag = 1;
	int Save_image_flag[9][9];

	//int image_xt, image_yt;
/////////////画像作成の有無////////////////////////////////////////////////////////////////

	//画像作成判断の初期化(デフォルトでは作成しない)
	for (int i = 0; i<9; ++i) {
		Save_image_flag[1][i] = 0;
		Save_image_flag[2][i] = 0;
	}

	//ecoモード
	if (conv_eco_mode_flag == 1) {
		for (int j = 0; j < 2; ++j) {
			for (int i = 0; i < 9; ++i) {
				Save_image_flag[j][i] = 0;
			}
		}
	}

	//多値画像を作成する
	Save_image_flag[2][0] = 1;		//多値画像を作成するとき1


	//8方向に画像を作成する
	for (int i = 1; i <= 8; ++i) {

		Save_image_flag[2][i] = 1;	//作成したい方向の画像の選択．iを指定
	}

	


////////////////////////////////////////////入力画像の読み込み////////////////////////////////////////////////////////////////////////////
	std::ifstream propety_dire;
	propety_dire.open(image_nameP2,ios::in);
	
	read_property(propety_dire,image_x,image_y,fs);	//propertyから読み取り
	
	//入力画像を読み込み
	sprintf(InputImage, inputimage_directory);	//propertyから読み取った入力画像情報を代入
	cv::Mat ImputImageM = cv::imread(InputImage);	//入力画像の読み込み．opencv
	image_x=ImputImageM.cols;
	image_y=ImputImageM.rows;

	//テンプレート画像を読み込み
	sprintf(Inputtemplate, inputtemplate_directory);	//propertyから読み取った入力画像情報を代入
	cv::Mat ImputImageT = cv::imread(Inputtemplate);	//入力画像の読み込み．opencv
	image_xt = ImputImageT.cols;
	image_yt = ImputImageT.rows;
	
//////////////////////////初期設定の確認用//////////////////////////////////////////////////////////////////////////////

	if (Rvector_create == 1)printf("基準ベクトル取得座標：X=%d，Y=%d\n", Rvector_pointX, Rvector_pointY);
	printf("InputImage=%s\n", InputImage);
	printf("画像サイズ：X=%d，Y=%d\n", image_x, image_y);
	printf("Inputtemplate=%s\n", Inputtemplate);
	printf("画像サイズ：X=%d，Y=%d\n", image_xt, image_yt);
	//printf("fs=%d\n", fs);
	printf("Offset=%d\n", Offset);
	printf("Upper_threshold : %f, Under_threshold : %f\n", Upper_threshold, Under_threshold);
	printf("use_upperthreshold_flag : %d, use_underthreshold_flag : %d\n", use_upperthreshold_flag, use_underthreshold_flag);

	//Nrutilを用いたメモリの確保
	double **input_bmp = matrix(0, image_x-1, 0, image_y-1);
	double **output1 = matrix(0, image_x-1, 0, image_y-1);
	double **input_bmp2 = matrix(0, image_x - 1, 0, image_y - 1);
	double **output2 = matrix(0, image_x - 1, 0, image_y - 1);
	double **spfil1 = matrix(1, image_x, 1, image_y);
	double **output_bmp_image = matrix(0, image_x-1, 0, image_y-1);	//追加1
	double **output_bmp_flag = matrix(0, image_x-1, 0, image_y-1);	//追加3
	
	//確保したメモリを初期化する
	for (y = 0; y < image_y; y++) {
		for (x = 0; x < image_x; x++) {
			input_bmp[x][y] = 0;
			output1[x][y] = 0;
			input_bmp2[x][y] = 0;
			output2[x][y] = 0;
			output_bmp_image[x][y] = 0;	//追加2
			output_bmp_flag[x][y] = 0;	//追加3
		}
	}
	
	for (fy = 1; fy <= image_y; fy++) {
		for (fx = 1; fx <= image_x; fx++) {
			spfil1[fx][fy] = 0;
		}
	}
	

	//int hfs = (fs + 1) / 2;			//フィルタの大きさの半分

////////////////////////画像の読み取り//////////////////////////////////////////////////////////////////////////////////////////////
	for (y = 0; y < image_y; ++y) {
		for (x = 0; x < image_x; ++x) {
			//opencvでは画素がR,G,Bの順で並んでいる
			int R = 3 * x;
			int G = 3 * x + 1;
			int B = 3 * x + 2;
			//グレースケールでの読み込み
			input_bmp[x][y] = (((ImputImageM.at<unsigned char>(y, R)+ImputImageM.at<unsigned char>(y, G)+ImputImageM.at<unsigned char>(y, B))/3)-Offset);
			//input_bmp[x][y] = ImputImageM.at<unsigned char>(y, R)-Offset;
		}
	}

	for (y = 0; y < image_yt; ++y) {
		for (x = 0; x < image_xt; ++x) {
			int Rt = 3 * x;
			int Gt = 3 * x + 1;
			int Bt = 3 * x + 2;
			//グレースケールでの読み込み
			input_bmp2[x][y] = (((ImputImageT.at<unsigned char>(y, Rt) + ImputImageT.at<unsigned char>(y, Gt) + ImputImageT.at<unsigned char>(y, Bt)) / 3) - Offset);
		}
	}

	//出力ファイルについての設定１
	set_outputfile(date,date_directory,paramerter,paramerter_count,sd);

	if(Rvector_create==1)Rvector_createF();	//基準ベクトルの作成

	printf("****************************************\n");
	printf("start：convolution\n");
	printf("****************************************\n");

	int Rvector_number = 0;				//Rvectorの番号のカウント

	std::vector<std::vector<double>>spfil1_kari;

	spfil1_kari.resize(image_x);
	for (int i = 0; i<image_x; ++i) {
		spfil1_kari[i].resize(image_y);
	}

///////////////////////フィルタの読み込み・畳み込み/////////////////////////////////////////
	for(int kernel_number=0;kernel_number<8;++kernel_number){

		

		//フィルタの選択．書き込みファイルの選択
		if(kernel_number==0){sprintf(inputfilter_directory,inputfilter_directory1);sprintf(Filename,Filename1);sprintf(Filenamet,Filename11);}
		if(kernel_number==1){sprintf(inputfilter_directory,inputfilter_directory2);sprintf(Filename,Filename2); sprintf(Filenamet, Filename12);}
		if(kernel_number==2){sprintf(inputfilter_directory,inputfilter_directory3);sprintf(Filename,Filename3); sprintf(Filenamet, Filename13);}
		if(kernel_number==3){sprintf(inputfilter_directory,inputfilter_directory4);sprintf(Filename,Filename4); sprintf(Filenamet, Filename14);}
		if(kernel_number==4){sprintf(inputfilter_directory,inputfilter_directory5);sprintf(Filename,Filename5); sprintf(Filenamet, Filename15);}
		if(kernel_number==5){sprintf(inputfilter_directory,inputfilter_directory6);sprintf(Filename,Filename6); sprintf(Filenamet, Filename16);}
		if(kernel_number==6){sprintf(inputfilter_directory,inputfilter_directory7);sprintf(Filename,Filename7); sprintf(Filenamet, Filename17);}
		if(kernel_number==7){sprintf(inputfilter_directory,inputfilter_directory8);sprintf(Filename,Filename8); sprintf(Filenamet, Filename18);}

		//read_filter(inputfilter_directory,fs,spfil1);	//フィルタの読み込み
		std::tie(fs, spfil1_kari) = read_filter(inputfilter_directory);
		

		
		for (fy = 1; fy <= fs; fy++) {
			for (fx = 1; fx <= fs; fx++) {
				//fscanf(fp, "%lf	", &spfil1[fx][fy]);
			//	printf("fs=%d,",fs);
			//	printf("%lf\n", spfil1_kari[fx - 1][fy - 1]);
				spfil1[fx][fy] = spfil1_kari[fx-1][fy-1];
			//	printf("%lf,", spfil1[fx][fy]);
			}
		}
		
		if (kernel_number == 0) {
			printf("fs=%d\n", fs);
			hfs = (fs + 1) / 2;			//フィルタの大きさの半分
		}
		

		//畳み込み
		if(paramerter[0]==2){
		convolution_gaus_sobel(image_y,image_x,fs,hfs,output1,spfil1,input_bmp,magnification);
		}else{
		convolution(image_y,image_x,fs,hfs,output1,spfil1,input_bmp,magnification);
		}
		//ファイルへの書き込み
		write_file(Filename,image_x,image_y,output1,Rvector_create,Rvector_pointX,Rvector_pointY,Rvector,Rvector_number);	

		


		//template画像の畳み込み
		if (paramerter[0] == 2) {
			convolution_gaus_sobel(image_yt, image_xt, fs, hfs, output2, spfil1, input_bmp2, magnification);
		}else{
			convolution(image_yt, image_xt, fs, hfs, output2, spfil1, input_bmp2, magnification);
		}
		//ファイルへの書き込み
		write_file(Filenamet, image_xt, image_yt, output2, Rvector_create, Rvector_pointX, Rvector_pointY, Rvector, Rvector_number);


		printf("convolution：filter %s\n", inputfilter_directory);	//用いたフィルタの表示
		Rvector_number++;					//Rvectorの番号のカウント

	}

	Rvector_number=0;					//Rvectorの番号のカウント

	

///////////////////////////////////基準ベクトルの作成結果ファイルの作成/////////////////////////////////////////////////////

	if (Rvector_create == 1) {
		if ((fp = fopen(Rvector_Filename, "w")) == NULL) { exit(1); }
		Rvector_number = 0;
		for (Rvector_number = 0 ; Rvector_number < 8 ; ++Rvector_number) {
			fprintf(fp, "%lf,", Rvector[Rvector_number]);
		}
		fclose(fp);
	}

//////////////////////////////opencvを用いた畳み込み画像の作成///////////////////////////////////////////////////////////////////////////////////

	if (Save_image_flag[2][0] == 1) {
		for (int make_image_repeat = 1; make_image_repeat <= 8; ++make_image_repeat) {
			//画像の作成
			make_bmp(date_directory2, Filename1, image_x, image_y, make_image_repeat, Save_image_flag);
			make_bmp(date_directory2, Filename1, image_xt, image_yt, make_image_repeat, Save_image_flag);
		}
	}

//////////////////////////////logの作成///////////////////////////////////////////////////////////////////////////////////
	FILE *fp_date;
	char filename_log[128];
	//sprintf(filename_log, "..\\log\\log-%2d-%02d%02d-%02d%02d%02d.txt",pnow->tm_year+1900,pnow->tm_mon + 1,pnow->tm_mday,pnow->tm_hour,pnow->tm_min,pnow->tm_sec);	//logファイル作成のディレクトリ指定
	sprintf(filename_log, "%s\\log.txt",date_directory2);	//logファイル作成のディレクトリ指定
	if ((fp_date = fopen(filename_log, "w")) == NULL) { printf("logファイルが開けません"); exit(1); }
	fprintf(fp_date, "Time : %s\n", date);								//時間
	fprintf(fp_date, "Rvector_create=%d\n", Rvector_create);			//Rvectorの作成したかどうか
	if (Rvector_create == 1)fprintf(fp_date, "Rvector_Coordinate：X=%d，Y=%d\n", Rvector_pointX, Rvector_pointY);
	fprintf(fp_date, "ImputImage=%s\n", InputImage);					//入力画像
	fprintf(fp_date, "ImputImage_size：X=%d，Y=%d\n", image_x, image_y);
	fprintf(fp_date, "Upper_threshold : %f, Under_threshold : %f\n", Upper_threshold, Under_threshold);
	fprintf(fp_date, "use_upperthreshold_flag : %d, use_underthreshold_flag : %d\n", use_upperthreshold_flag, use_underthreshold_flag);
	fprintf(fp_date, "Offset : %d\n", Offset);						//畳み込みの際のoffset
	fprintf(fp_date, "Kernel\n");									//使用したカーネルの記録
	fprintf(fp_date, "Kernel_size=%d\n", fs);
	fprintf(fp_date, "V0   = %s\n", inputfilter_directory1);
	fprintf(fp_date, "V45  = %s\n", inputfilter_directory2);
	fprintf(fp_date, "V90  = %s\n", inputfilter_directory3);
	fprintf(fp_date, "V135 = %s\n", inputfilter_directory4);
	fprintf(fp_date, "V180 = %s\n", inputfilter_directory5);
	fprintf(fp_date, "V225 = %s\n", inputfilter_directory6);
	fprintf(fp_date, "V270 = %s\n", inputfilter_directory7);
	fprintf(fp_date, "V315 = %s\n", inputfilter_directory8);
	fclose(fp_date);
	printf("logファイル %s を保存しました\n", filename_log);
	

	//メモリの開放
	free_matrix(input_bmp, 0, image_x-1, 0, image_y-1);
	free_matrix(output1,0, image_x-1, 0, image_y-1);
	free_matrix(spfil1,  1, image_x,  1, image_y);
	free_matrix(output_bmp_image, 0, image_x-1, 0, image_y-1);
	free_matrix(output_bmp_flag, 0, image_x-1, 0, image_y-1);

	printf("finish：convolution\n");

	
	
	return *InputImage;
	
}

void set_outputfile(char date[],char date_directory[],int paramerter[],int paramerter_count,int sd){
	
//////////////////////////////Outputディレクトリの設定//////////////////////////////////////////////////////////////////////////////////
	//出力結果のファイル名の指定
	char *Filename1_s = "V(0).csv";
	char *Filename2_s = "V(45).csv";
	char *Filename3_s = "V(90).csv";
	char *Filename4_s = "V(135).csv";
	char *Filename5_s = "V(180).csv";
	char *Filename6_s = "V(225).csv";
	char *Filename7_s = "V(270).csv";
	char *Filename8_s = "V(315).csv";
	char *Filename11_s = "V(0)t.csv";
	char *Filename12_s = "V(45)t.csv";
	char *Filename13_s = "V(90)t.csv";
	char *Filename14_s = "V(135)t.csv";
	char *Filename15_s = "V(180)t.csv";
	char *Filename16_s = "V(225)t.csv";
	char *Filename17_s = "V(270)t.csv";
	char *Filename18_s = "V(315)t.csv";


	//結果を保存するフォルダの作成
	//フォルダ名は実行日時になる
	sprintf(date_directory, "..\\result_usa\\%s\\", date);
	if (_mkdir(date_directory) == 0) {
		printf("フォルダ %s を作成しました\n", date_directory);
	}
	else {
		printf("フォルダ作成に失敗しました。もしくは作成済みです\n");
	}

	if(paramerter[0]==1 || paramerter[0]==2){
		sprintf(date_directory2, "%s%d×%dsobel_conv_sd%d\\", date_directory,paramerter[paramerter_count],paramerter[paramerter_count],sd);		
	}else{
		sprintf(date_directory2, "%s%dk_conv_sd%d\\", date_directory,paramerter[paramerter_count],sd);
	}
	//Outputディレクトリの作成
	if (_mkdir(date_directory2) == 0) {
		printf("フォルダ %s を作成しました\n", date_directory2);
	}
	else {
		printf("フォルダ作成に失敗しました。\n");
	}
	

	//Outputファイルのディレクトリ設定
	sprintf(Filename1,"%s%s", date_directory2, Filename1_s);
	sprintf(Filename2, "%s%s", date_directory2, Filename2_s);
	sprintf(Filename3, "%s%s", date_directory2, Filename3_s);
	sprintf(Filename4, "%s%s", date_directory2, Filename4_s);
	sprintf(Filename5, "%s%s", date_directory2, Filename5_s);
	sprintf(Filename6, "%s%s", date_directory2, Filename6_s);
	sprintf(Filename7, "%s%s", date_directory2, Filename7_s);
	sprintf(Filename8, "%s%s", date_directory2, Filename8_s);
	sprintf(Filename11, "%s%s", date_directory2, Filename11_s);
	sprintf(Filename12, "%s%s", date_directory2, Filename12_s);
	sprintf(Filename13, "%s%s", date_directory2, Filename13_s);
	sprintf(Filename14, "%s%s", date_directory2, Filename14_s);
	sprintf(Filename15, "%s%s", date_directory2, Filename15_s);
	sprintf(Filename16, "%s%s", date_directory2, Filename16_s);
	sprintf(Filename17, "%s%s", date_directory2, Filename17_s);
	sprintf(Filename18, "%s%s", date_directory2, Filename18_s);

	printf("使用したカーネルは\n");
	printf("V0   = %s\n", inputfilter_directory1);
	printf("V45  = %s\n", inputfilter_directory2);
	printf("V90  = %s\n", inputfilter_directory3);
	printf("V135 = %s\n", inputfilter_directory4);
	printf("V180 = %s\n", inputfilter_directory5);
	printf("V225 = %s\n", inputfilter_directory6);
	printf("V270 = %s\n", inputfilter_directory7);
	printf("V315 = %s\n", inputfilter_directory8);
}

	///////////////////////////////初期設定2 : 基準ベクトル作成か否か(修正をしなければならない）/////////////////////////////////////////////////////////////
void Rvector_createF(){

//if(Rvector_create==1)sprintf(InputImage, "..\\bmp\\255-0\\%d.bmp", filter_number);	//Rvectorを作成する際の入力画像指定.
											
	sprintf(Rvector_directory, outputrvector_directory, propety);		//Rvectorを保存するフォルダ名を指定.property.txtの2行目																//Rvectorを保存するフォルダの作成
	printf("Rvector_create=%d\n", Rvector_create);
	if (Rvector_create == 1) {

		if (_mkdir(Rvector_directory) == 0) {
			printf("Rvecor保存フォルダ %s を作成しました\n", Rvector_directory);
		}
		else {
			printf("Rvecor保存フォルダ %s は作成済みです\n", Rvector_directory);
		}
	}
}


void read_property(ifstream &propety_dire,int &image_x,int &image_y,int &fs){

	count_property=0;

	//プロパティtxtファイルの読み込み
	if (propety_dire.fail())
	{
		printf("propertyテキストを読み取ることができません\n");
		printf("propertyテキスト : %s\n",propety_dire);
	}
	while (propety_dire.getline(propety, 256 - 1)){

		//if (count_property == 0)++count_property;

		if (count_property == 0)sprintf(inputtemplate_directory, propety);				//一行目の画像サイズなど
		if (count_property == 1)sprintf(outputrvector_directory, propety);	//使用する基準ベクトル
		if (count_property == 2)sprintf(inputimage_directory, propety);		//入力画像
		if (count_property == 3)sprintf(inputfilter_directory1, propety);	//使用するカーネル1
		if (count_property == 4)sprintf(inputfilter_directory2, propety);	//使用するカーネル2
		if (count_property == 5)sprintf(inputfilter_directory3, propety);	//使用するカーネル3
		if (count_property == 6)sprintf(inputfilter_directory4, propety);	//使用するカーネル4
		if (count_property == 7)sprintf(inputfilter_directory5, propety);	//使用するカーネル5
		if (count_property == 8)sprintf(inputfilter_directory6, propety);	//使用するカーネル6
		if (count_property == 9)sprintf(inputfilter_directory7, propety);	//使用するカーネル7
		if (count_property == 10)sprintf(inputfilter_directory8, propety);	//使用するカーネル8

		++count_property;
	}

	//プロパティtxtファイルの一行目を文字列から数値に変換して関数に代入
/*	std::string tmp_Allpropety;
	std::istringstream stream_Allpropety(Allpropety);
	count_Allproperty=0;
	while (getline(stream_Allpropety, tmp_Allpropety, ',')) {

		int All_tmp_property = stof(tmp_Allpropety); //stof(string str) : stringをintに変換
		All_property[count_Allproperty] = All_tmp_property;

////////////////property.txtの一行目///////////////////////////////////////////////////////////
		if (count_Allproperty == 0)image_x = All_property[count_Allproperty];				//&image_x:画像の横サイズ(256pixelの画像の場合は256と指定)
		if (count_Allproperty == 1)image_y = All_property[count_Allproperty];				//&image_y:画像の縦サイズ(256pixelの画像の場合は256と指定)
		if (count_Allproperty == 2)Rvector_create = All_property[count_Allproperty];		//Rvector_create:1なら基準ベクトルを作成．
		if (count_Allproperty == 3)Rvector_pointX = All_property[count_Allproperty];		//Rvector_pointX:基準ベクトル取得X座標．0からスタート
		if (count_Allproperty == 4)Rvector_pointY = All_property[count_Allproperty];		//Rvector_pointY:基準ベクトル取得Y座標．0からスタート
		if (count_Allproperty == 5)fs = All_property[count_Allproperty];					//fs:フィルタサイズ（奇数）

		++count_Allproperty;
	}
	*/
	propety_dire.close();
}


//////////////////////////フィルタ読み込み////////////////////////////
std::tuple<int,std::vector<std::vector<double>>> read_filter(char inputfilter_directory[]){

	std::vector<std::vector<double>>filter_txt;
	filter_txt.resize(100);
	for (int i = 0; i<100; ++i) {
		filter_txt[i].resize(100);
	}

	int filter_x, filter_y;

	//サイズ不定のtxtファイルの読み取り
	std::tie(filter_x, filter_y, filter_txt) = read_txt(inputfilter_directory);
	
	if (filter_x != filter_y) {
		printf("フィルターサイズがおかしい");
		exit(1);
	}
	fs = filter_x;

	std::vector<std::vector<double>>spfil1_kari;
	spfil1_kari.resize(fs);
	for (int i = 0; i<fs; ++i) {
		spfil1_kari[i].resize(fs);
	}
	
	for (fy = 1; fy <= fs; fy++) {
		for (fx = 1; fx <= fs; fx++) {
			spfil1_kari[fx-1][fy-1] = filter_txt[fx-1][fy-1];
		}
	}
	return std::forward_as_tuple(fs, spfil1_kari);

}

///////////////////////畳み込み結果の書き込み////////////////////////////////////////
void write_file(char *Filename,int &image_x,int &image_y,double *output1[],int Rvector_create,int Rvector_pointX,int Rvector_pointY,double Rvector[],int Rvector_number){

	//------------------------ファイルへの書き込み--------------------------//
	if ((fp = fopen(Filename, "w")) == NULL){
		printf("ファイル：%sが開けません\n",Filename);
		exit(1);
	}
	for (y = 0; y < image_y; y = y + 1) {
		for (x = 0; x < image_x; x = x + 1) {
			fprintf(fp, "%lf,", output1[x][y]);


			if (Rvector_create == 1) {
				if (y == Rvector_pointY) {
					if (x == Rvector_pointX) {
						Rvector[Rvector_number] = output1[x][y];
					}
				}
			}

		}
		fprintf(fp, "\n");
	}
	fclose(fp);

	//確保したメモリを初期化する
	for (y = 0; y < image_y; y++) {
		for (x = 0; x < image_x; x++) {
			output1[x][y] = 0;
		}
	}

}

///////////////////////畳み込み演算//////////////////////////////////////////////////
void convolution(int &image_y, int &image_x, int fs, int hfs, double *output1[], double *spfil1[], double *input_bmp[], double magnification) {
	

	//#pragma omp parallel num_threads(4)
	for (y = 0; y < image_y; y++) {

		//#pragma omp parallel num_threads(2)
#pragma omp parallel for
		for (x = 0; x < image_x; x++) {
			//#pragma omp parallel for
			for (fy = 1; fy <= fs; fy++) {
				//#pragma omp parallel for
				//#pragma omp parallel num_threads(2)
				for (fx = 1; fx <= fs; fx++) {

					//フィルタの真ん中が描画点になるようにhsfを用いて調整する
					if (0 <= x + (fx - hfs) && x + (fx - hfs) < image_x
						&& 0 <= y + (fy - hfs) && y + (fy - hfs) < image_y)
					{
						output1[x][y] += spfil1[fx][fy] * input_bmp[x + (fx - hfs)][y + (fy - hfs)];
					}
				}
			}
			output1[x][y] = output1[x][y] * magnification;		//倍率に応じてゲインを変化
		}

	}
	++filter_reset_flag;

	//フィルタの初期化
	if (filter_reset_flag == 2) {
		//#pragma omp parallel for
		for (fy = 1; fy <= fs; fy++) {
			for (fx = 1; fx <= fs; fx++) {
				spfil1[fx][fy] = 0;
			}
		}
		filter_reset_flag = 0;
	}

}

///////////////////sobelフィルタに対応したガウスフィルタを用いた畳み込み演算//////////////////////////////////////////////////////////
int convolution_gaus_sobel(int &image_y,int &image_x,int fs, int hfs,double *output1[],double *spfil1[],double *input_bmp[],double magnification){

	//Nrutilを用いたメモリの確保
	double **spfil1_g = matrix(1, fs, 1, fs);
	double **output1_g = matrix(0, image_x-1, 0, image_y-1);

	read_filter_gaus(fs,spfil1_g);

	//初期化
	for (y = 0; y < image_y; y++) {
		for (x = 0; x < image_x; x++) {
			output1_g[x][y] = 0;
		}
	}

	//gausフィルタによるなめし
	for (y = 0; y < image_y; y++) {
#pragma omp parallel for
		for (x = 0; x < image_x; x++) {
			for (fy = 1; fy <= fs; fy++) {
				for (fx = 1; fx <= fs; fx++) {
					
					//フィルタの真ん中が描画点になるようにhsfを用いて調整する
					if(		0<=x+(fx-hfs) && x+(fx-hfs)<image_x
						&&	0<=y+(fy-hfs) && y+(fy-hfs)<image_y )
					{
						output1_g[x][y] += spfil1_g[fx][fy] * input_bmp[x + (fx - hfs)][y + (fy - hfs)];
					}
				}
			}
			//output1_g[x][y] = output1_g[x][y] * magnification;		//倍率に応じてゲインを変化
		}
	
	}
	
	
	//なめした画像に対してsobelフィルタを掛ける
	for (y = 0; y < image_y; y++) {
#pragma omp parallel for
		for (x = 0; x < image_x; x++) {
			
			for (fy = 1; fy <= fs; fy++) {
				for (fx = 1; fx <= fs; fx++) {
					
					//フィルタの真ん中が描画点になるようにhsfを用いて調整する
					if(		0<=x+(fx-hfs) && x+(fx-hfs)<image_x
						&&	0<=y+(fy-hfs) && y+(fy-hfs)<image_y )
					{
						output1[x][y] += spfil1[fx][fy] * output1_g[x + (fx - hfs)][y + (fy - hfs)];
					}
				}
			}
			output1[x][y] = output1[x][y] * magnification;		//倍率に応じてゲインを変化
		}
	
	}
	
	//フィルタの初期化
	for (fy = 1; fy <= fs; fy++) {
		for (fx = 1; fx <= fs; fx++) {
			spfil1[fx][fy] = 0;
		}
	}

	free_matrix(output1_g,0, image_x-1, 0, image_y-1);
	free_matrix(spfil1_g,  1,  fs,  1,  fs);

	//return 0;
	return **input_bmp;

}

void read_filter_gaus(int fs,double *spfil1_g[]){

	//初期化
	for (y = 1; y <= fs; y++) {
		for (x = 1; x <= fs; x++) {
			spfil1_g[x][y] = 0;
		}
	}

	char inputfilter_gaus[128];
	sprintf(inputfilter_gaus,"..\\filter\\gaus\\%d×%dgaus.txt",fs,fs);

	if ((fp = fopen(inputfilter_gaus, "r")) == NULL){
		printf("ガウスフィルタ：%sが読み込めません\n",inputfilter_gaus);
		exit(1);
	}
	
	for (fy = 1; fy <= fs; fy++) {
		for (fx = 1; fx <= fs; fx++) {
			fscanf(fp, "%lf	", &spfil1_g[fx][fy]);
		}
	}
	fclose(fp); 
}