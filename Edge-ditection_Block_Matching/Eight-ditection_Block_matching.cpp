#include <string>
#include<sstream> //文字ストリーム
#include<fstream>
#include<iostream>
#include <windows.h>
#include <stdio.h>
#include <vector>

#include <tuple>
#include <direct.h>//フォルダを作成する

#include <opencv2/opencv.hpp>	//画像読み込み
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

#define PI 3.141592

//メモリ確保を行うためのヘッダ
#define ANSI				
#include "nrutil.h"	

char inputtemplate_directory_E[255];
char inputimage_directory_E[128];
char Inputtemplate_E[128];					//入力テンプレート
void Spline(double C_spline_buf[], double C_spline[]);

void read_property_Eight(ifstream &propety_dire, int &image_x, int &image_y, int &fs) {
	char propety[255];
	int count_property = 0;

	//プロパティtxtファイルの読み込み
	if (propety_dire.fail())
	{
		printf("propertyテキストを読み取ることができません\n");
		printf("propertyテキスト : %s\n", propety_dire);
	}
	while (propety_dire.getline(propety, 256 - 1)) {

		//if (count_property == 0)++count_property;

		if (count_property == 0)sprintf(inputtemplate_directory_E, propety);				//一行目の画像サイズなど
		//if (count_property == 1)sprintf(outputrvector_directory, propety);	//使用する基準ベクトル
		if (count_property == 2)sprintf(inputimage_directory_E, propety);		//入力画像
/*		if (count_property == 3)sprintf(inputfilter_directory1, propety);	//使用するカーネル1
		if (count_property == 4)sprintf(inputfilter_directory2, propety);	//使用するカーネル2
		if (count_property == 5)sprintf(inputfilter_directory3, propety);	//使用するカーネル3
		if (count_property == 6)sprintf(inputfilter_directory4, propety);	//使用するカーネル4
		if (count_property == 7)sprintf(inputfilter_directory5, propety);	//使用するカーネル5
		if (count_property == 8)sprintf(inputfilter_directory6, propety);	//使用するカーネル6
		if (count_property == 9)sprintf(inputfilter_directory7, propety);	//使用するカーネル7
		if (count_property == 10)sprintf(inputfilter_directory8, propety);	//使用するカーネル8
*/
		++count_property;
	}

	propety_dire.close();
}

int Eight_ditection_Block_Matching(char image_nameP2[], int &image_x, int &image_y, int &image_xt, int &image_yt, char date[], char date_directory[], char InputImage[]){
	
	int Bs = 3;
	int Bs_w = 5;
	int Block_number_x=0;
	int Block_number_y=0;
	int Block_number=0;

	
	////////////////////////////////////////////入力画像の読み込み////////////////////////////////////////////////////////////////////////////
	std::ifstream propety_dire;
	propety_dire.open(image_nameP2, ios::in);
	int fs;

	read_property_Eight(propety_dire, image_x, image_y, fs);	//propertyから読み取り
	//printf("%s", inputimage_directory_E);
														//入力画像を読み込み
	sprintf(InputImage, inputimage_directory_E);	//propertyから読み取った入力画像情報を代入
	cv::Mat ImputImageM_buf = cv::imread(InputImage);	//入力画像の読み込み．opencv
	cv::Mat ImputImageM;
	cvtColor(ImputImageM_buf, ImputImageM, CV_RGB2GRAY);
	image_x = ImputImageM.cols;
	image_y = ImputImageM.rows;

	//テンプレート画像を読み込み
	sprintf(Inputtemplate_E, inputtemplate_directory_E);	//propertyから読み取った入力画像情報を代入
	cv::Mat ImputImageT_buf = cv::imread(Inputtemplate_E);	//入力画像の読み込み．opencv
	cv::Mat ImputImageT;
	cvtColor(ImputImageT_buf, ImputImageT, CV_RGB2GRAY);
	image_xt = ImputImageT.cols;
	image_yt = ImputImageT.rows;
	int Xc = image_xt / 2;
	int Yc = image_yt / 2;

	Block_number_x = (image_xt - 2) / Bs;
	Block_number_y = (image_yt - 2) / Bs;
	Block_number = Block_number_x*Block_number_y;
	printf("Block_number=%d,x=%d,y=%d\n", Block_number, Block_number_x, Block_number_y);
	
	//テンプレート画像を回転させる
	cv::Mat Rotation_InputImageT = cv::Mat::zeros(image_xt-2, image_yt-2, CV_8UC1);
	//cv::Mat InputImageT_buf = cv::Mat::zeros(Bs_w, Bs_w, CV_8UC1);;
	//cv::Mat Rotation_InputImageT_buf = cv::Mat::zeros(Bs_w, Bs_w, CV_8UC1);
	cv::Mat InputImageT_buf = cv::Mat::zeros(image_xt, image_yt, CV_8UC1);;
	cv::Mat Rotation_InputImageT_buf1 = cv::Mat::zeros(image_xt, image_yt, CV_8UC1);
	cv::Mat Rotation_InputImageT_buf2 = cv::Mat::zeros(image_xt, image_yt, CV_8UC1);
	cv::Mat Rotation_InputImageT_buf3 = cv::Mat::zeros(image_xt, image_yt, CV_8UC1);
	cv::Mat Rotation_InputImageT_buf4 = cv::Mat::zeros(image_xt, image_yt, CV_8UC1);
	cv::Mat Rotation_InputImageT_buf5 = cv::Mat::zeros(image_xt, image_yt, CV_8UC1);
	cv::Mat Rotation_InputImageT_buf6 = cv::Mat::zeros(image_xt, image_yt, CV_8UC1);
	cv::Mat Rotation_InputImageT_buf7 = cv::Mat::zeros(image_xt, image_yt, CV_8UC1);


	std::vector<std::vector<std::vector<double>>>C_memory;
	std::vector<std::vector<std::vector<double>>>D_memory;
	C_memory.resize(image_x);
	D_memory.resize(image_x);
	for (int i = 0; i < image_x; ++i) {
		C_memory[i].resize(image_y);
		D_memory[i].resize(image_y);
	}
	for (int j = 0; j < image_y; ++j) {
		for (int i = 0; i < image_x; ++i) {
			C_memory[i][j].resize(Block_number);
			D_memory[i][j].resize(Block_number);
		}
	}
	//初期化
	for (int k = 0; k < Block_number; ++k) {
		for (int j = 0; j < image_y; ++j) {
			for (int i = 0; i < image_x; ++i) {
				C_memory[i][j][k] = 0;
				D_memory[i][j][k] = 0;
			}
		}
	}
	int memory_count = 0;
/*
	cv::namedWindow("a", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::namedWindow("b", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow("a", ImputImageM);
	cv::imshow("b", ImputImageT);
	*/
	double **T = matrix(0, image_x - 3, 0, image_y - 3);
	double **T0 = matrix(0, image_x - 3, 0, image_y - 3);
	double **T45 = matrix(0, image_x - 3, 0, image_y - 3);
	double **T90 = matrix(0, image_x - 3, 0, image_y - 3);
	double **T135 = matrix(0, image_x - 3, 0, image_y - 3);
	double **T180 = matrix(0, image_x - 3, 0, image_y - 3);
	double **T225 = matrix(0, image_x - 3, 0, image_y - 3);
	double **T270 = matrix(0, image_x - 3, 0, image_y - 3);
	double **T315 = matrix(0, image_x - 3, 0, image_y - 3);
	for (int j = 0; j < image_y-2; ++j) {
		for (int i = 0; i < image_x-2; ++i) {
			T[i][j] = 0;
			T0[i][j] = 0;
			T45[i][j] = 0;
			T90[i][j] = 0;
			T135[i][j] = 0;
			T180[i][j] = 0;
			T225[i][j] = 0;
			T270[i][j] = 0;
			T315[i][j] = 0;
		}
	}

	//回転のパラメータの設定
	cv::Point2f Rotation_center(Bs_w / 2, Bs_w / 2);
	double angle_deg1 = 45;
	double angle_deg2 = 90;
	double angle_deg3 = 135;
	double angle_deg4 = 180;
	double angle_deg5 = 225;
	double angle_deg6 = 270;
	double angle_deg7 = 315;
	const cv::Mat affine_matrix1 = cv::getRotationMatrix2D(Rotation_center, angle_deg1, 1);
	const cv::Mat affine_matrix2 = cv::getRotationMatrix2D(Rotation_center, angle_deg2, 1);
	const cv::Mat affine_matrix3 = cv::getRotationMatrix2D(Rotation_center, angle_deg3, 1);
	const cv::Mat affine_matrix4 = cv::getRotationMatrix2D(Rotation_center, angle_deg4, 1);
	const cv::Mat affine_matrix5 = cv::getRotationMatrix2D(Rotation_center, angle_deg5, 1);
	const cv::Mat affine_matrix6 = cv::getRotationMatrix2D(Rotation_center, angle_deg6, 1);
	const cv::Mat affine_matrix7 = cv::getRotationMatrix2D(Rotation_center, angle_deg7, 1);
	
	//テンプレート画像の作成
	for (int Block_number_count_y = 0; Block_number_count_y < Block_number_y; ++Block_number_count_y) {
		for (int Block_number_count_x = 0; Block_number_count_x < Block_number_x; ++Block_number_count_x) {
			int p = Bs * Block_number_count_x;
			int q = Bs* Block_number_count_y;
			//回転させる5×5の画像を作成する
			for (int j = 0; j < Bs_w; ++j) {
				for (int i = 0; i < Bs_w; ++i) {
					InputImageT_buf.at<uchar>(j, i) = ImputImageT.at<uchar>(q + j, p + i);
				}
			}

			//回転
			cv::warpAffine(InputImageT_buf, Rotation_InputImageT_buf1, affine_matrix1, InputImageT_buf.size(), INTER_LINEAR);
			cv::warpAffine(InputImageT_buf, Rotation_InputImageT_buf2, affine_matrix1, InputImageT_buf.size(), INTER_LINEAR);
			cv::warpAffine(InputImageT_buf, Rotation_InputImageT_buf3, affine_matrix1, InputImageT_buf.size(), INTER_LINEAR);
			cv::warpAffine(InputImageT_buf, Rotation_InputImageT_buf4, affine_matrix1, InputImageT_buf.size(), INTER_LINEAR);
			cv::warpAffine(InputImageT_buf, Rotation_InputImageT_buf5, affine_matrix1, InputImageT_buf.size(), INTER_LINEAR);
			cv::warpAffine(InputImageT_buf, Rotation_InputImageT_buf6, affine_matrix1, InputImageT_buf.size(), INTER_LINEAR);
			cv::warpAffine(InputImageT_buf, Rotation_InputImageT_buf7, affine_matrix1, InputImageT_buf.size(), INTER_LINEAR);


			for (int j = 1; j < Bs_w - 1; ++j) {
				for (int i = 1; i < Bs_w - 1; ++i) {
					T0[p + i - 1][q + j - 1] = InputImageT_buf.at<uchar>(j, i);
					T45[p + i - 1][q + j - 1] = Rotation_InputImageT_buf1.at<uchar>(j, i);
					T90[p + i - 1][q + j - 1] = Rotation_InputImageT_buf2.at<uchar>(j, i);
					T135[p + i - 1][q + j - 1] = Rotation_InputImageT_buf3.at<uchar>(j, i);
					T180[p + i - 1][q + j - 1] = Rotation_InputImageT_buf4.at<uchar>(j, i);
					T225[p + i - 1][q + j - 1] = Rotation_InputImageT_buf5.at<uchar>(j, i);
					T270[p + i - 1][q + j - 1] = Rotation_InputImageT_buf6.at<uchar>(j, i);
					T315[p + i - 1][q + j - 1] = Rotation_InputImageT_buf7.at<uchar>(j, i);
				}
			}
		}
	}
	//cv::namedWindow("src", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	//cv::imshow("src", Rotation_InputImageT);

	
	
	double uI;
	double uT;
	int N = Bs;
	int M = Bs;
	//ブロックごとのループ
	for (int Block_number_count_y = 0; Block_number_count_y < Block_number_y; ++Block_number_count_y) {
		for (int Block_number_count_x = 0; Block_number_count_x < Block_number_x; ++Block_number_count_x) {
			printf("ブロック=%d,%d\n", Block_number_count_x, Block_number_count_y);
			int p = Bs * Block_number_count_x;
			int q = Bs* Block_number_count_y;
			int X = p + 2;
			int Y = q + 2;
	
			
			double **Ci_peak = matrix(0, image_x - 1, 0, image_y - 1);
			
			for (int j = 0; j < image_y; ++j) {
				for (int i = 0; i < image_x; ++i) {
					Ci_peak[i][j] = 0;
				}
			}
			//その座標に対してのCを求める
			for (int y = 0; y < image_y-M; ++y) {
				for (int x = 0; x < image_x-N; ++x) {

					double C_spline_buf[10];
					double C=0;
					double C_buf[9];
					double C_child=0;
					double C_mother=0;
					double C_mother1=0;
					double C_mother2=0;
					double C_spline[360];

					

					for (int degree = 0; degree < 8; ++degree) {

						switch (degree) {
						case 0:
							for (int j = 0; j < image_y - 3; ++j) {	for (int i = 0; i < image_x - 3; ++i) { T[i][j] = T0[i][j]; }}
							break;
						case 1:
							for (int j = 0; j < image_y - 3; ++j) { for (int i = 0; i < image_x - 3; ++i) { T[i][j] = T45[i][j]; } }
							break;
						case 2:
							for (int j = 0; j < image_y - 3; ++j) { for (int i = 0; i < image_x - 3; ++i) { T[i][j] = T90[i][j]; } }
							break;
						case 3:
							for (int j = 0; j < image_y - 3; ++j) { for (int i = 0; i < image_x - 3; ++i) { T[i][j] = T135[i][j]; } }
							break;
						case 4:
							for (int j = 0; j < image_y - 3; ++j) { for (int i = 0; i < image_x - 3; ++i) { T[i][j] = T180[i][j]; } }
							break;
						case 5:
							for (int j = 0; j < image_y - 3; ++j) { for (int i = 0; i < image_x - 3; ++i) { T[i][j] = T225[i][j]; } }
							break;
						case 6:
							for (int j = 0; j < image_y - 3; ++j) { for (int i = 0; i < image_x - 3; ++i) { T[i][j] = T270[i][j]; } }
							break;
						case 7:
							for (int j = 0; j < image_y - 3; ++j) { for (int i = 0; i < image_x - 3; ++i) { T[i][j] = T315[i][j]; } }
							break;
						default:
							break;

						}

						for (int j = 0; j < M; ++j) {
							for (int i = 0; i < N; ++i) {
								uI += ImputImageM.at<uchar>(q + j + y, p + i + x);
								uT +=T[p+i][q+j];
							}
						}
						uI = uI / (M*N);
						uT = uT / (M*N);

						for (int j = 0; j < M; ++j) {
							for (int i = 0; i < N; ++i) {

								C_child += (ImputImageM.at<uchar>(q + j + y, p + i + x) - uI)*(T[p + i][q + j] - uT);
								C_mother1 += pow(ImputImageM.at<uchar>(q + j + y, p + i + x) - uI, 2);
								C_mother2 += pow(T[p + i][q + j] - uT, 2);
							}
						}
						C_mother = sqrt(C_mother1 * C_mother2);
						C = C_child / C_mother;
						C_buf[degree] = C;
					}
					//C_bufの最大値を求める
					double C_buf_MAX= C_buf[0];
					int C_buf_MAX_degree=0;
					for (int degree = 0; degree < 8; ++degree) {
						if (C_buf_MAX < C_buf[degree]) {
							C_buf_MAX = C_buf[degree];
							C_buf_MAX_degree = degree;
						}
					}
					//配列に代入しなおす
					switch(C_buf_MAX_degree) {
					case 0:
						for (int i = 0; i<3; ++i)C_spline_buf[i] = C_buf[i + 5];
						for (int i = 3; i<9; ++i)C_spline_buf[i] = C_buf[i - 3];
						break;
					case 1:
						for (int i = 0; i<2; ++i)C_spline_buf[i] = C_buf[i + 6];
						for (int i = 3; i<9; ++i)C_spline_buf[i] = C_buf[i - 2];
						break;
					case 2:
						for (int i = 0; i<1; ++i)C_spline_buf[i] = C_buf[i + 7];
						for (int i = 3; i<9; ++i)C_spline_buf[i] = C_buf[i - 1];
						break;
					case 3:
						for(int i=0;i<8;++i)C_spline_buf[i] = C_buf[i];
						C_spline_buf[9] = C_buf[0];
						break;
					case 4:
						for (int i = 0; i<7; ++i)C_spline_buf[i] = C_buf[i+1];
						for (int i = 7; i<9; ++i)C_spline_buf[i] = C_buf[i-7];
						break;
					case 5:
						for (int i = 0; i<6; ++i)C_spline_buf[i] = C_buf[i + 2];
						for (int i = 6; i<9; ++i)C_spline_buf[i] = C_buf[i-6];
						break;
					case 6:
						for (int i = 0; i<5; ++i)C_spline_buf[i] = C_buf[i + 3];
						for (int i = 5; i<9; ++i)C_spline_buf[i] = C_buf[i - 5];
						break;
					case 7:
						for (int i = 0; i<4; ++i)C_spline_buf[i] = C_buf[i + 4];
						for (int i = 4; i<9; ++i)C_spline_buf[i] = C_buf[i - 4];
						break;
					default:
						break;
					}
					Spline(C_spline_buf, C_spline);
				//	for (int i = 0; i <9; ++i)printf("C_spline_buf[%d]=%lf,", i, C_spline_buf[i]);
				//	printf("\n");
				//	for (int i = 0; i < 360; ++i)printf("C_spline[%d]=%lf,",i, C_spline[i]);

					//C_splineの最大値を求める
					double C_spline_MAX = C_spline[0];
					int C_spline_MAX_degree = 0;
					for (int degree = 0; degree < 360; ++degree) {
						if (C_spline_MAX < C_spline[degree]) {
							C_spline_MAX = C_spline[degree];
							C_spline_MAX_degree = degree;
						}
					}
					//printf("max=%lf\n", C_spline_MAX);
					if (C_buf_MAX_degree < 5) {
						C_spline_MAX_degree = C_spline_MAX_degree - 135;
					}
					else {
						C_spline_MAX_degree = C_spline_MAX_degree + 225;
					}
					if (C_spline_MAX_degree > 360)C_spline_MAX_degree = C_spline_MAX_degree - 360;
					if (C_spline_MAX_degree <0)C_spline_MAX_degree = C_spline_MAX_degree + 360;


					Ci_peak[x][y] = C_spline_MAX_degree;

					int lx = Xc - X;
					int ly = Yc - Y;
					int Xv = (lx*cos(Ci_peak[x][y] * PI / 180)) + (ly*sin(Ci_peak[x][y] * PI / 180)) + X;
					int Yv = (-lx*sin(Ci_peak[x][y] * PI / 180)) + (ly*cos(Ci_peak[x][y] * PI / 180)) + Y;
					
					printf("x,y=%d,%d\n", x, y);
					for (memory_count = 0; memory_count < Block_number; ++memory_count) {
						if (C_memory[Xv][Yv][memory_count] == 0) {
							C_memory[Xv][Yv][memory_count] = C_spline_MAX;
							D_memory[Xv][Yv][memory_count] = C_spline_MAX_degree;
							break;
						}

						//データが満杯になったとき
						if (memory_count == Block_number) {
							double min_C = C_memory[Xv][Yv][0];
							int min_memory_count = 0;
							//メモリ内の最小値より，大きかった場合入れ替える
							for (memory_count = 0; memory_count < Block_number; ++memory_count) {
								if (min_C > C_memory[Xv][Yv][memory_count]) {
									min_C = C_memory[Xv][Yv][memory_count];
									min_memory_count = memory_count;
								}
								if (min_C < C_spline_MAX) {
									C_memory[Xv][Yv][min_memory_count] = C_spline_MAX;
									D_memory[Xv][Yv][min_memory_count] = C_spline_MAX_degree;
								}
							}
						}

						
					}
				}
			}
			

			printf("ここまできたよ\n");
			cv::waitKey(0);


		
			free_matrix(Ci_peak, 0, image_x - 1, 0, image_y - 1);
		}
	}

	std::vector<std::vector<double>>C_vote;
	std::vector<std::vector<double>>D_vote;
	C_vote.resize(image_x);
	D_vote.resize(image_x);
	for (int i = 0; i < image_x; ++i) {
		C_vote[i].resize(image_y);
		D_vote[i].resize(image_y);
	}
	
	for (int k = 0; k < Block_number; ++k) {
		for (int j = 0; j < image_y; ++j) {
			for (int i = 0; i < image_x; ++i) {
				C_vote[i][j] += C_memory[i][j][k];
			}
		}
	}
	double max_C_vote= C_vote[0][0];
	int max_x = 0;
	int max_y = 0;
	for (int j = 0; j < image_y; ++j) {
		for (int i = 0; i < image_x; ++i) {
			if (max_C_vote < C_vote[i][j]) {
				max_C_vote = C_vote[i][j];
				max_x = i;
				max_y = j;
			}
		}
	}
	int max_degree_vote_buf[360];
	for (memory_count = 0; memory_count < Block_number; ++memory_count) {
		for (int k = 0; k < 360; ++k) {
			if (D_memory[max_x][max_y][memory_count] == k)++max_degree_vote_buf[k];
		}
	}
	int max_degee_vote = max_degree_vote_buf[0];
	int max_degee = 0;
	for (int i = 0; i < 360; ++i) {
		if (max_degee_vote < max_degree_vote_buf[i]) {
			max_degee_vote = max_degree_vote_buf[i];
			max_degee = i;
		}
	}

	printf("max_x=%d,max_y=%d,max_degree=%d\n", max_x, max_y, max_degee);
	
	free_matrix(T0, 0, image_x - 3, 0, image_y - 3);
	free_matrix(T45, 0, image_x - 3, 0, image_y - 3);
	free_matrix(T90, 0, image_x - 3, 0, image_y - 3);
	free_matrix(T135, 0, image_x - 3, 0, image_y - 3);
	free_matrix(T180, 0, image_x - 3, 0, image_y - 3);
	free_matrix(T225, 0, image_x - 3, 0, image_y - 3);
	free_matrix(T270, 0, image_x - 3, 0, image_y - 3);
	free_matrix(T315, 0, image_x - 3, 0, image_y - 3);

	cv::waitKey(0);

	return 0;
}

