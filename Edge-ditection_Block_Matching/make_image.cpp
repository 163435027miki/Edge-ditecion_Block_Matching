#include <string>
#include<sstream> //文字ストリーム
#include<fstream>
#include<iostream>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <stdio.h>




using namespace std;

//opencvのため必要
int argc;
char** argv;

	

int cols;
int rows;
int make_image_offset;
int image_number;
//int make_image_repeat=3;


char Output_Image_name[128];
char Input_Image_name[128];						//入力ファイル名・入力元の設定
char input_csv_name[128];
char iutput_csv_name[128];
static int makeimage[1024];
char use_name[128];

int ikkaioki = 0;



void make_bmp(char date_directory2[],char Filename1[],int &image_x,int &image_y,int make_image_repeat,int Save_image_flag[][9]) {
	++ikkaioki;

	if(Save_image_flag[2][make_image_repeat]==0)return;		//特定の箇所の画像を作成しない時に帰る
	
////////////////////////////////初期設定//////////////////////////////////////////////////////////////////
	cols=image_x;			//横幅
	rows=image_y;			//縦幅
	make_image_offset=128;	//反応しなかった場所(0)の画素値

	//cols=320;				//横幅
	//rows=128;				//縦幅

	//読み取るcsvファイルの名前を指定
	image_number=45*(make_image_repeat-1);
	sprintf(use_name,"V(%d)",image_number);
	if (ikkaioki == 2) {
		sprintf(use_name, "V(%d)t", image_number);
		ikkaioki = 0;
	}


	
//////////////////入出力ファイルのディレクトリ生成//////////////////////////////////////////////////////////

	sprintf(Output_Image_name,"%s%s.bmp",date_directory2,use_name);	//出力画像.bmp
	
	sprintf(iutput_csv_name,"%s%s.csv",date_directory2,use_name);		//入力画像の画素値.csv

	

////////////////////////InputImageの画素値の取得////////////////////////////////////////////////////////////////////////////////////
	
	static int OutputRGB[1024][1024];




/////////////////////////////////////////////////////////////////////////////////////////////////////////////


	cv::Mat make_image = cv::Mat::zeros(rows, cols, CV_8UC1);


	int i=0,j=0;

	int count_small=0;
	ifstream make(iutput_csv_name);
	if(!make){printf("%sがありません\n",iutput_csv_name);exit(1);}

	string str_make;
		while(getline(make,str_make)){
		string token_make;
        istringstream stream_make(str_make);
		
        while(getline(stream_make,token_make,',')){	//一行読み取る．makeimageのみ，繰り返しの範囲指定に用いる
			int tmp_make=stof(token_make);			//文字を数字に変換
			makeimage[count_small]=tmp_make;				//配列に代入
			
			OutputRGB[count_small][j]=makeimage[count_small]+make_image_offset;
			
			if(OutputRGB[count_small][j]>255)OutputRGB[count_small][j]=255;
			if(OutputRGB[count_small][j]<0)OutputRGB[count_small][j]=0;

			

			make_image.at<uchar>(j, i) = OutputRGB[count_small][j];

			++count_small;
			++i;
		}++j;count_small=0;i=0;
		}				
   


		FILE *fp_Output_image;
		fp_Output_image = fopen( Output_Image_name, "w" );
	if( fp_Output_image == NULL ){
		printf( "%sファイルが開けません\n", Output_Image_name );
		//exit(1);
	}
	for(int y=0;y<rows;y++){
		for(int x=0;x<cols;x++){

			fprintf( fp_Output_image, "%d,",OutputRGB[x][y]);
		}
			fprintf( fp_Output_image, "\n");
	}

	fclose( fp_Output_image );

	printf( "%sファイル書き込みが終わりました\n", Output_Image_name );

	imwrite(Output_Image_name,make_image); 

	 //名前をつける
	//std::string windowName = "make_image";
	//std::string windowName = use_name;
	
	//cv::namedWindow(windowName);

	//画面表示
	//cv::imshow(windowName,make_image);
	 
 
	
		//cv::waitKey(0);

	
	
	
}