/////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////関数の階層/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	--main
		|--timeset																	//時間の取得	
		|--convolution																//畳み込みを行う
		|		|--read_property													//畳み込みのpropertyを読み取る
		|		|--set_outputfile													//畳み込み結果を保存するファイル・フォルダの設定
		|		|--read_filter														//畳み込みに用いるフィルタを読み込む
		|		|--or--convolution_calculation										//探索対象画像でカーネルを用いた畳み込みを行う場合
		|		|  or--convolution_gaus_sobel										//探索対象画像でsobelフィルタかつgausフィルタを用いる場合
		|		|		|--read_filter_gaus											//探索対象画像で畳み込みに用いるフィルタと同等のサイズのgausフィルタを読み込む
		|		|--write_file														//探索対象画像で畳み込み結果のcsvファイル出力
		|		|
		|		|--or--convolution_calculation										//template画像でカーネルを用いた畳み込みを行う場合
		|		|  or--convolution_gaus_sobel										//template画像でsobelフィルタかつgausフィルタを用いる場合
		|		|		|--read_filter_gaus											//template画像で畳み込みに用いるフィルタと同等のサイズのgausフィルタを読み込む
		|		|--write_file														//template画像で畳み込み結果のcsvファイル出食
		|		|
		|		|--make_bmp				×2×8										//畳み込み画像の作成
		|
		|--or--cossim																//cos類似度の計算
		|		|--Rvector_read														//基準ベクトルの読み込み
		|		|--Read_output														//出力ファイルの設定
		|		|--local_connectivity												//局所連結性のチェック
		|
		|--or--atan																	//arctanによる角度の計算
		|
		|--otsu																		//探索対象画像に対して判別分析法を行う（普段は行わない）
		|	|--IO_directory															//入出力ディレクトリの設定
		|	|--or--readfiles														//ファイルの読み込み
		|	|--or--readfiles_8dire													//ファイルの読み込み
		|	|--discriminantAnalysis													//判別分析法(大津の2値化)本体
		|		|--hist_hozon														//判別分析法で用いたヒストグラムの作成
		|
		|--edge_st_temp																//テンプレート画像に対して判別分析法を用いる
		|	|--IO_directory															//入出力ディレクトリの設定
		|	|--or--readfiles														//ファイルの読み込み
		|	|--or--readfiles_8dire													//ファイルの読み込み
		|	|--discriminantAnalysis													//判別分析法(大津の2値化)本体
		|		|--hist_hozon														//判別分析法で用いたヒストグラムの作成
		|
		|--Edge_detection_Block_Matching											//EBMの本体
		|	|--write_frame															//画像に枠を描画
		|
		|--score																	//統計処理を行うため，得点を計算する
		|	|--read_correct_xy														//統計処理のため，正しい位置のデータセットを読み込む
		|
		|--score_record																//得点を記録する
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////




#include <stdio.h>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>	//文字ストリーム
#include <direct.h>	//フォルダを作成する
#include<thread>	//複数スレッド
#include<vector>

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

//kernelのパラメータとsobelのサイズを記す
int paramerter_kernel[4] = { 1,3,10,100 };
int paramerter_sobel[4] = { 0,3,5,7 };




int timeset(char date[]);
int notimeset(char date[], int pixel[], int Togire[], int z2, int z);
int convolution(int argc, char** argv,char image_nameP2[],int &image_x,int &image_y, int &image_xt, int &image_yt, int paramerter[],int paramerter_count,int sd,char date[],char date_directory[], char Inputimage[]);
int cossim(char date_directory[], int &image_x, int &image_y,int paramerter[],int paramerter_count,int sd,char date[]);
int arctan(char date_directory[], int &image_x, int &image_y,int paramerter[],int paramerter_count,int sd,char date[]);
std::tuple<std::vector<int>, std::vector<int>, int, int >Edge_detection_Block_Matching(char date_directory[], int &image_x, int &image_y, int &image_xt, int &image_yt, int paramerter[], int paramerter_count, int sd, char date[],int Bs,double threshold_EdBM, char Inputimage[],double &threshold_ostu, int &frame_allowable_error,double &use_threshold_flag);
int otsu(char date_directory[], int &image_x, int &image_y, int paramerter[], int paramerter_count, int sd ,int &Edge_derectory_number, char date[]);
double edge_st_temp(char date_directory[], int &image_xt, int &image_yt, int paramerter[], int paramerter_count, int sd,int &Edge_derectory_number, char date[]);
int score(int correct_x, int correct_y, int image_xt, int image_yt, std::vector<int> max_x, std::vector<int> max_y, int count_tied_V_vote, int V_vote_max, int data_num, char correct_scv[], int high_score_range_x, int high_score_range_y);
int score_record(char date_directory[], int data_num, char Inputimage[], int correct_score, int count_tied_V_vote, int V_vote_max, std::vector<int> max_x, std::vector<int> max_y);
int Eight_ditection_Block_Matching(char image_nameP2[], int &image_x, int &image_y, int &image_xt, int &image_yt, char date[], char date_directory[], char InputImage[]);



int main(int argc, char** argv) {



	std::vector<int>max_x;
	std::vector<int>max_y;
	int count_tied_V_vote;
	int V_vote_max;

	int simulation_pattern[8] = { 8, 0,1,1,0,2,180,5 };
	//手法を区別するためにsimulation_pattern[0]を変更する
	/*
							|		0			|		1			|		2		|		3		|		4				|		5			|
	ブロックサイズBs[0]		|		8																|
	畳み込むかどうか：[1]	|	畳み込みあり	|	畳み込みなし	|
	用いるフィルタ：[2]		|					|		sobel		|	sobel×gaus	|
	用いるthreshold	:[3]	|		なし		|	大津の2値化		|	任意の値	|
	[4]						|	---																	|8:ブロック数×8		|	9:ブロック数	|10:応答電圧によって用いる部分を変更する|
	エッジ強度用いる方向:[5]|2または8
	統計処理を行う	:[6]	|	行わない		|行う，かつデータの数|
	枠をまとめる	:[7]	|まとめない			|	許容するズレを入れる
	*/

	int pixel[10] = { 0,1,3,5,7,9,13,17 };
	int Togire[10] = { 0,1,3,5,7,9,13,17 };
	int Bs = simulation_pattern[0];
	double threshold_EdBM = 3;
	double threshold_otsu = 0;
	char *correct_scv = "..\\bmp\\simulation18-0115\\correct.csv";		//用いる探索正解座標
	//sprintf(correct_scv,"..\\bmp\\simulation18-0115\\correct.csv");

	int paramerter[4];					//paramerter[0]=1でsobelフィルタ,paramerter[0]=2でgaus×sobelフィルタ

	//標準偏差の調整箇所
	int sd_max = 50;
	int paramerter_count_max = 1;
	int cossim_atan_switch = 1;			//cossim_atan_switch=0でcossim,cossim_atan_switch=1でarctan
	paramerter[0] = simulation_pattern[2];					//paramerter[0]=1でsobelフィルタ,paramerter[0]=2でgaus×sobelフィルタ
	double use_threshold_flag = simulation_pattern[3];
	int Edge_derectory_number = simulation_pattern[5];		//エッジ強度の計算に用いる成分の数．2（0,90)または8(0~315)
	int &frame_allowable_error = simulation_pattern[7];

	int data_num = simulation_pattern[6];
	int no_convolution_frag = simulation_pattern[1];		//1だと畳み込まない
	//この時の画像サイズを設定する
	int image_x_buf = 320;
	int image_y_buf = 320;
	int image_xt_buf = 80;
	int image_yt_buf = 80;
	char no_date[128];


	//用いるパラメータを代入
	switch (paramerter[0]) {
	case 0:
		for (int i = 1; i < 4; ++i) {
			paramerter[i] = paramerter_kernel[i];
		}
		break;
	case 1:
	case 2:
		for (int i = 1; i < 4; ++i) {
			paramerter[i] = paramerter_sobel[i];
		}
		break;
	default:
		printf("paramerter[0]の値がおかしい\nparamerter[0]=%d\n", paramerter[0]);
		return 0;
	}

	int paramerter_count = 0;



	//for (int z2 = 1; z2 <= 7; ++z2) {		//pixel
	//	for (int z = 1; z <= 7; ++z) {		//Togire
	//正解座標の誤差
	int high_score_range_x = 0;
	int high_score_range_y = 0;
	//int kurikaeshi_number = 0;
	for (int high_score_range = 0; high_score_range<=4; ++high_score_range) {
		simulation_pattern[7] = high_score_range;
		high_score_range_x = simulation_pattern[7];
		high_score_range_y = simulation_pattern[7];
		
		for (sd = 0; sd <= sd_max; sd = sd + 10) {

	int image_kurikaeshi = 2;
	//kurikaeshi_number = 0;

	//	notimeset(date, pixel, Togire,z2,z);
	for (int image_kurikaeshi = 1; image_kurikaeshi < data_num + 1; ++image_kurikaeshi) {
		timeset(date);
		//sprintf(date, "%d", image_kurikaeshi);

		
		//if(kurikaeshi_number== simulation_pattern[6])kurikaeshi_number = 0;

		
		for (int paramerter_count = 1; paramerter_count <= 1; ++paramerter_count) {
			int sd_buf = 0;
			
		//	for (sd = sd_buf; sd <= sd_buf; sd = sd + 10) {

				sprintf(date, "%d\\sd%d\\%d", high_score_range, sd, image_kurikaeshi);

				//対象画像とテンプレート画像を区別するためにparamerter[0]を変更する
				/*		|kernel	|	sobel	|	sobel×gaus	|
				探索対象|	0	|	1		|		2		|
				template|	3	|	4		|		5		|*/
			//	if (kurikaeshi_number != 0) {
					//	if (sd != 0) {
					switch (paramerter[0]) {
					case 4: paramerter[0] = 1; break;
					case 5: paramerter[0] = 2; break;
					case 6:paramerter[0] = 3;
					default:  break;
					}
			//	}
		//		printf("\n\n%d", kurikaeshi_number);
		//		printf("\n\n%d\n\n", paramerter[0]);
				if (paramerter[0] == 1 || paramerter[0] == 2) {
					//	sprintf(image_nameP, "..\\property_usa\\property_3k_conv_", paramerter[paramerter_count]);
					//	sprintf(image_nameP, "..\\property_usa\\property_%d×%dsobel_conv_", paramerter[paramerter_count], paramerter[paramerter_count]);
				//	sprintf(image_nameP, "..\\property_usa\\simulation18-0111_hide_sobel\\property_%d×%dsobel_conv", paramerter[paramerter_count], paramerter[paramerter_count]);
					sprintf(image_nameP, "..\\property_usa\\simulation18-0115\\property_%d×%dsobel_conv", paramerter[paramerter_count], paramerter[paramerter_count]);
					//	sprintf(image_nameP, "..\\property_usa\\simulation17-0725\\sobel\\15-%dp-%dT_sobel", pixel[z2], Togire[z], paramerter[paramerter_count]);
					sprintf(image_nameP2, "%s_sd%d_%d.txt", image_nameP, sd, image_kurikaeshi);
					//	sprintf(image_nameP2, "%s_sd%d_3.txt", image_nameP, sd);
						//sprintf(image_nameP2, "%s_sd0_sd%d.txt", image_nameP, sd);
						//sprintf(image_nameP2, "%s_%d.txt", image_nameP, image_kurikaeshi);

						//sprintf(image_nameP2, "%s\\property_%d×%dsobel_conv_sd%d.txt", image_nameP, paramerter[paramerter_count], paramerter[paramerter_count], sd);
				}
				else {
					//sprintf(image_nameP, "..\\property_usa\\simulation17-0821\\kernel\\15-%dp-%dT\\property_%dk_conv_", pixel[z2], Togire[z], paramerter[paramerter_count]);
					sprintf(image_nameP, "..\\property_usa\\simulation18-0108\\property_kernel_image4\\property_%dk_conv_", paramerter[paramerter_count]);
					//	sprintf(image_nameP, "..\\property_usa\\property_3k_conv_", paramerter[paramerter_count]);
					sprintf(image_nameP2, "%ssd%d.txt", image_nameP, sd);
					//sprintf(image_nameP, "..\\property_usa\\simulation17-0824-2\\property_B135");
					//sprintf(image_nameP2, "%s.txt", image_nameP);
				}
				//	Eight_ditection_Block_Matching(image_nameP2, image_x, image_y, image_xt, image_yt, date, date_directory, Inputimage);
				if (no_convolution_frag == 1) {
					image_x = image_x_buf;
					image_y = image_y_buf;
					image_xt = image_xt_buf;
					image_yt = image_yt_buf;

				}
				else {
					convolution(argc, argv, image_nameP2, image_x, image_y, image_xt, image_yt, paramerter, paramerter_count, sd, date, date_directory, Inputimage);

					printf("x=%d,y=%d\nxt=%d,yt=%d\n", image_x, image_y, image_xt, image_yt);

					

				}

				//対象画像の角度推定
				switch (cossim_atan_switch) {
				case 0:
					cossim(date_directory, image_x, image_y, paramerter, paramerter_count, sd, date);
					break;
				case 1:
					arctan(date_directory, image_x, image_y, paramerter, paramerter_count, sd, date);
					break;
				default:
					printf("cossim_atan_switchの値がおかしい\ncossim_atan_switch=%d\n", cossim_atan_switch);
					return 0;
				}
				//対象画像とテンプレート画像を区別するためにparamerter[0]を変更する
				/*		|kernel	|	sobel	|	sobel×gaus	|
				探索対象|	0	|	1		|		2		|
				template|	3	|	4		|		5		|*/
				switch (paramerter[0]) {
				case 1: paramerter[0] = 4; break;
				case 2: paramerter[0] = 5; break;
				case 0: paramerter[0] = 3;
				default:  break;
				}

				//テンプレート画像の角度推定
				switch (cossim_atan_switch) {
				case 0:
					cossim(date_directory, image_xt, image_yt, paramerter, paramerter_count, sd, date);
					break;
				case 1:
					arctan(date_directory, image_xt, image_yt, paramerter, paramerter_count, sd, date);
					break;
				default:
					printf("cossim_atan_switchの値がおかしい\ncossim_atan_switch=%d\n", cossim_atan_switch);
					return 0;
				}


				otsu(date_directory, image_x, image_y, paramerter, paramerter_count, sd, Edge_derectory_number, date);



				threshold_otsu = edge_st_temp(date_directory, image_xt, image_yt, paramerter, paramerter_count, sd, Edge_derectory_number, date);


				printf("threshold_otsu=%f\n", threshold_otsu);
				/*
				int max_x = 0;
				int max_y = 0;
				int count_tied_V_vote = 1;
				int V_vote_max = 1;
*/
//arctanを用いる場合は，otsuで求めたthreshold_otsuをthreshold_EdBMに代入して閾値として用いるようにしている．
				std::tie(max_x, max_y, count_tied_V_vote, V_vote_max) = Edge_detection_Block_Matching(date_directory, image_x, image_y, image_xt, image_yt, paramerter, paramerter_count, sd, date, Bs, threshold_EdBM, Inputimage, threshold_otsu, frame_allowable_error, use_threshold_flag);


				if (data_num != 0) {
					int correct_score = 0;
					int correct_x = 0;
					int correct_y = 0;
					correct_score = score(correct_x, correct_y, image_xt, image_yt, max_x, max_y, count_tied_V_vote, V_vote_max, data_num, correct_scv, high_score_range_x, high_score_range_y);
					score_record(date_directory, data_num, Inputimage, correct_score, count_tied_V_vote, V_vote_max, max_x, max_y);
				}



			//	++kurikaeshi_number;
				//if (kurikaeshi_number == simulation_pattern[6])kurikaeshi_number = 0;
			}
		}
		//}
	//}
//detanam
		
	}
	
}
		printf("全ての処理が終了しました\n");

		return 0;
}




