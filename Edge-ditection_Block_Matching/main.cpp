/////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////�֐��̊K�w/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	--main
		|--timeset																	//���Ԃ̎擾	
		|--convolution																//��ݍ��݂��s��
		|		|--read_property													//��ݍ��݂�property��ǂݎ��
		|		|--set_outputfile													//��ݍ��݌��ʂ�ۑ�����t�@�C���E�t�H���_�̐ݒ�
		|		|--read_filter														//��ݍ��݂ɗp����t�B���^��ǂݍ���
		|		|--or--convolution_calculation										//�T���Ώۉ摜�ŃJ�[�l����p������ݍ��݂��s���ꍇ
		|		|  or--convolution_gaus_sobel										//�T���Ώۉ摜��sobel�t�B���^����gaus�t�B���^��p����ꍇ
		|		|		|--read_filter_gaus											//�T���Ώۉ摜�ŏ�ݍ��݂ɗp����t�B���^�Ɠ����̃T�C�Y��gaus�t�B���^��ǂݍ���
		|		|--write_file														//�T���Ώۉ摜�ŏ�ݍ��݌��ʂ�csv�t�@�C���o��
		|		|
		|		|--or--convolution_calculation										//template�摜�ŃJ�[�l����p������ݍ��݂��s���ꍇ
		|		|  or--convolution_gaus_sobel										//template�摜��sobel�t�B���^����gaus�t�B���^��p����ꍇ
		|		|		|--read_filter_gaus											//template�摜�ŏ�ݍ��݂ɗp����t�B���^�Ɠ����̃T�C�Y��gaus�t�B���^��ǂݍ���
		|		|--write_file														//template�摜�ŏ�ݍ��݌��ʂ�csv�t�@�C���o�H
		|		|
		|		|--make_bmp				�~2�~8										//��ݍ��݉摜�̍쐬
		|
		|--or--cossim																//cos�ގ��x�̌v�Z
		|		|--Rvector_read														//��x�N�g���̓ǂݍ���
		|		|--Read_output														//�o�̓t�@�C���̐ݒ�
		|		|--local_connectivity												//�Ǐ��A�����̃`�F�b�N
		|
		|--or--atan																	//arctan�ɂ��p�x�̌v�Z
		|
		|--otsu																		//�T���Ώۉ摜�ɑ΂��Ĕ��ʕ��͖@���s���i���i�͍s��Ȃ��j
		|	|--IO_directory															//���o�̓f�B���N�g���̐ݒ�
		|	|--or--readfiles														//�t�@�C���̓ǂݍ���
		|	|--or--readfiles_8dire													//�t�@�C���̓ǂݍ���
		|	|--discriminantAnalysis													//���ʕ��͖@(��Â�2�l��)�{��
		|		|--hist_hozon														//���ʕ��͖@�ŗp�����q�X�g�O�����̍쐬
		|
		|--edge_st_temp																//�e���v���[�g�摜�ɑ΂��Ĕ��ʕ��͖@��p����
		|	|--IO_directory															//���o�̓f�B���N�g���̐ݒ�
		|	|--or--readfiles														//�t�@�C���̓ǂݍ���
		|	|--or--readfiles_8dire													//�t�@�C���̓ǂݍ���
		|	|--discriminantAnalysis													//���ʕ��͖@(��Â�2�l��)�{��
		|		|--hist_hozon														//���ʕ��͖@�ŗp�����q�X�g�O�����̍쐬
		|
		|--Edge_detection_Block_Matching											//EBM�̖{��
		|	|--write_frame															//�摜�ɘg��`��
		|
		|--score																	//���v�������s�����߁C���_���v�Z����
		|	|--read_correct_xy														//���v�����̂��߁C�������ʒu�̃f�[�^�Z�b�g��ǂݍ���
		|
		|--score_record																//���_���L�^����
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////




#include <stdio.h>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>	//�����X�g���[��
#include <direct.h>	//�t�H���_���쐬����
#include<thread>	//�����X���b�h
#include<vector>

using namespace std;

int image_x, image_y;		//�摜�T�C�Y
int image_xt, image_yt;		//�摜�T�C�Y

char Inputimage[128];
char date[128] = "";
//�o�̓t�@�C���f�B���N�g��
char date_directory[128];
char image_nameP[128];
char image_nameP2[256];
int sd;

//kernel�̃p�����[�^��sobel�̃T�C�Y���L��
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
	//��@����ʂ��邽�߂�simulation_pattern[0]��ύX����
	/*
							|		0			|		1			|		2		|		3		|		4				|		5			|
	�u���b�N�T�C�YBs[0]		|		8																|
	��ݍ��ނ��ǂ����F[1]	|	��ݍ��݂���	|	��ݍ��݂Ȃ�	|
	�p����t�B���^�F[2]		|					|		sobel		|	sobel�~gaus	|
	�p����threshold	:[3]	|		�Ȃ�		|	��Â�2�l��		|	�C�ӂ̒l	|
	[4]						|	---																	|8:�u���b�N���~8		|	9:�u���b�N��	|10:�����d���ɂ���ėp���镔����ύX����|
	�G�b�W���x�p�������:[5]|2�܂���8
	���v�������s��	:[6]	|	�s��Ȃ�		|�s���C���f�[�^�̐�|
	�g���܂Ƃ߂�	:[7]	|�܂Ƃ߂Ȃ�			|	���e����Y��������
	*/

	int pixel[10] = { 0,1,3,5,7,9,13,17 };
	int Togire[10] = { 0,1,3,5,7,9,13,17 };
	int Bs = simulation_pattern[0];
	double threshold_EdBM = 3;
	double threshold_otsu = 0;
	char *correct_scv = "..\\bmp\\simulation18-0115\\correct.csv";		//�p����T���������W
	//sprintf(correct_scv,"..\\bmp\\simulation18-0115\\correct.csv");

	int paramerter[4];					//paramerter[0]=1��sobel�t�B���^,paramerter[0]=2��gaus�~sobel�t�B���^

	//�W���΍��̒����ӏ�
	int sd_max = 50;
	int paramerter_count_max = 1;
	int cossim_atan_switch = 1;			//cossim_atan_switch=0��cossim,cossim_atan_switch=1��arctan
	paramerter[0] = simulation_pattern[2];					//paramerter[0]=1��sobel�t�B���^,paramerter[0]=2��gaus�~sobel�t�B���^
	double use_threshold_flag = simulation_pattern[3];
	int Edge_derectory_number = simulation_pattern[5];		//�G�b�W���x�̌v�Z�ɗp���鐬���̐��D2�i0,90)�܂���8(0~315)
	int &frame_allowable_error = simulation_pattern[7];

	int data_num = simulation_pattern[6];
	int no_convolution_frag = simulation_pattern[1];		//1���Ə�ݍ��܂Ȃ�
	//���̎��̉摜�T�C�Y��ݒ肷��
	int image_x_buf = 320;
	int image_y_buf = 320;
	int image_xt_buf = 80;
	int image_yt_buf = 80;
	char no_date[128];


	//�p����p�����[�^����
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
		printf("paramerter[0]�̒l����������\nparamerter[0]=%d\n", paramerter[0]);
		return 0;
	}

	int paramerter_count = 0;



	//for (int z2 = 1; z2 <= 7; ++z2) {		//pixel
	//	for (int z = 1; z <= 7; ++z) {		//Togire
	//�������W�̌덷
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

				//�Ώۉ摜�ƃe���v���[�g�摜����ʂ��邽�߂�paramerter[0]��ύX����
				/*		|kernel	|	sobel	|	sobel�~gaus	|
				�T���Ώ�|	0	|	1		|		2		|
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
					//	sprintf(image_nameP, "..\\property_usa\\property_%d�~%dsobel_conv_", paramerter[paramerter_count], paramerter[paramerter_count]);
				//	sprintf(image_nameP, "..\\property_usa\\simulation18-0111_hide_sobel\\property_%d�~%dsobel_conv", paramerter[paramerter_count], paramerter[paramerter_count]);
					sprintf(image_nameP, "..\\property_usa\\simulation18-0115\\property_%d�~%dsobel_conv", paramerter[paramerter_count], paramerter[paramerter_count]);
					//	sprintf(image_nameP, "..\\property_usa\\simulation17-0725\\sobel\\15-%dp-%dT_sobel", pixel[z2], Togire[z], paramerter[paramerter_count]);
					sprintf(image_nameP2, "%s_sd%d_%d.txt", image_nameP, sd, image_kurikaeshi);
					//	sprintf(image_nameP2, "%s_sd%d_3.txt", image_nameP, sd);
						//sprintf(image_nameP2, "%s_sd0_sd%d.txt", image_nameP, sd);
						//sprintf(image_nameP2, "%s_%d.txt", image_nameP, image_kurikaeshi);

						//sprintf(image_nameP2, "%s\\property_%d�~%dsobel_conv_sd%d.txt", image_nameP, paramerter[paramerter_count], paramerter[paramerter_count], sd);
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

				//�Ώۉ摜�̊p�x����
				switch (cossim_atan_switch) {
				case 0:
					cossim(date_directory, image_x, image_y, paramerter, paramerter_count, sd, date);
					break;
				case 1:
					arctan(date_directory, image_x, image_y, paramerter, paramerter_count, sd, date);
					break;
				default:
					printf("cossim_atan_switch�̒l����������\ncossim_atan_switch=%d\n", cossim_atan_switch);
					return 0;
				}
				//�Ώۉ摜�ƃe���v���[�g�摜����ʂ��邽�߂�paramerter[0]��ύX����
				/*		|kernel	|	sobel	|	sobel�~gaus	|
				�T���Ώ�|	0	|	1		|		2		|
				template|	3	|	4		|		5		|*/
				switch (paramerter[0]) {
				case 1: paramerter[0] = 4; break;
				case 2: paramerter[0] = 5; break;
				case 0: paramerter[0] = 3;
				default:  break;
				}

				//�e���v���[�g�摜�̊p�x����
				switch (cossim_atan_switch) {
				case 0:
					cossim(date_directory, image_xt, image_yt, paramerter, paramerter_count, sd, date);
					break;
				case 1:
					arctan(date_directory, image_xt, image_yt, paramerter, paramerter_count, sd, date);
					break;
				default:
					printf("cossim_atan_switch�̒l����������\ncossim_atan_switch=%d\n", cossim_atan_switch);
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
//arctan��p����ꍇ�́Cotsu�ŋ��߂�threshold_otsu��threshold_EdBM�ɑ������臒l�Ƃ��ėp����悤�ɂ��Ă���D
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
		printf("�S�Ă̏������I�����܂���\n");

		return 0;
}




