#include <stdio.h>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>	//文字ストリーム
#include <direct.h>	//フォルダを作成する
#include<thread>	//複数スレッド

using namespace std;

int image_x, image_y;		//画像サイズ
int image_xt, image_yt;		//画像サイズ

char Inputimage[128];
char date[128] = "";
//出力ファイルディレクトリ
char date_directory[128];
char image_nameP[128];
char image_nameP2[256];
int sd;



//標準偏差の調整箇所
int sd_max = 0;
int paramerter_count_max = 3;


int timeset(char date[]);
int notimeset(char date[], int pixel[], int Togire[], int z2, int z);
int convolution(int argc, char** argv,char image_nameP2[],int &image_x,int &image_y, int &image_xt, int &image_yt, int paramerter[],int paramerter_count,int sd,char date[],char date_directory[], char Inputimage[]);
int cossim(char date_directory[], int &image_x, int &image_y,int paramerter[],int paramerter_count,int sd,char date[]);
int arctan(char date_directory[], int &image_x, int &image_y,int paramerter[],int paramerter_count,int sd,char date[]);
int Edge_detection_Block_Matching(char date_directory[], int &image_x, int &image_y, int &image_xt, int &image_yt, int paramerter[], int paramerter_count, int sd, char date[],int Bs,double threshold_EdBM, char Inputimage[],double &threshold_ostu);
int otsu(char date_directory[], int &image_x, int &image_y, int paramerter[], int paramerter_count, int sd );
int edge_st_temp(char date_directory[], int &image_xt, int &image_yt, int paramerter[], int paramerter_count, int sd);


int main(int argc, char** argv){

	int pixel[10]={0,1,3,5,7,9,13,17};
	int Togire[10] = { 0,1,3,5,7,9,13,17 };
	int Bs=5;
	double threshold_EdBM=3;
	double threshold_otsu = 0;
	

	int paramerter[4]={1,3,10,100};		//paramerter[0]=1でsobelフィルタ,paramerter[0]=2でgaus×sobelフィルタ
	int paramerter_count=0;

	//for (int z2 = 1; z2 <= 7; ++z2) {		//pixel
	//	for (int z = 1; z <= 7; ++z) {		//Togire
		

			timeset(date);
		//	notimeset(date, pixel, Togire,z2,z);

			for (int paramerter_count = 1; paramerter_count <= 1; ++paramerter_count) {

				for (sd = 0; sd <= sd_max; sd = sd + 10) {
				//for (sd = 0; sd <= 0; sd = sd + 10) {

					if (paramerter[0] == 1 || paramerter[0] == 2) {
						sprintf(image_nameP, "..\\property_usa\\property_3k_conv_",paramerter[paramerter_count],paramerter[paramerter_count]);
					//	sprintf(image_nameP, "..\\property_usa\\simulation17-0725\\sobel\\15-%dp-%dT_sobel", pixel[z2], Togire[z], paramerter[paramerter_count]);
						sprintf(image_nameP2, "%ssd%d.txt", image_nameP, sd);
						//sprintf(image_nameP2, "%s\\property_%d×%dsobel_conv_sd%d.txt", image_nameP, paramerter[paramerter_count], paramerter[paramerter_count], sd);
					}
					else {
						//sprintf(image_nameP, "..\\property_usa\\simulation17-0821\\kernel\\15-%dp-%dT\\property_%dk_conv_", pixel[z2], Togire[z], paramerter[paramerter_count]);
						sprintf(image_nameP,"..\\property_usa\\property_3k_conv_", paramerter[paramerter_count]);
						sprintf(image_nameP2, "%ssd%d.txt", image_nameP, sd);
						//sprintf(image_nameP, "..\\property_usa\\simulation17-0824-2\\property_B135");
						//sprintf(image_nameP2, "%s.txt", image_nameP);
					}

					convolution(argc, argv, image_nameP2, image_x, image_y, image_xt, image_yt, paramerter, paramerter_count, sd, date, date_directory, Inputimage);
					
					printf("x=%d,y=%d\nxt=%d,yt=%d\n", image_x, image_y, image_xt, image_yt);
				
					//対象画像の角度推定
					//cossim(date_directory,image_x,image_y,paramerter,paramerter_count,sd,date);
					arctan(date_directory,image_x,image_y,paramerter,paramerter_count,sd,date);
				
					switch (paramerter[0]) {
					case 1: paramerter[0] = 4; break;
					case 2: paramerter[0] = 5; break;
					default: paramerter[0] = 3; break;
					}
					
					//テンプレート画像の角度推定
					arctan(date_directory, image_xt, image_yt, paramerter, paramerter_count, sd, date);
				//	cossim(date_directory, image_xt, image_yt, paramerter, paramerter_count, sd, date);

					otsu(date_directory, image_x, image_y, paramerter, paramerter_count, sd);

					

					threshold_otsu = edge_st_temp(date_directory, image_xt, image_yt, paramerter, paramerter_count, sd);

					printf("threshold_otsu=%f\n", threshold_otsu);
					
					Edge_detection_Block_Matching(date_directory, image_x, image_y, image_xt, image_yt ,paramerter, paramerter_count, sd, date,Bs, threshold_EdBM, Inputimage, threshold_otsu);

				
				}
			}
		//}
	//}
		
		printf("全ての処理が終了しました\n");

		return 0;
}




