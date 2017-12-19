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
std::tuple<std::vector<int>, std::vector<int>, int, int >Edge_detection_Block_Matching(char date_directory[], int &image_x, int &image_y, int &image_xt, int &image_yt, int paramerter[], int paramerter_count, int sd, char date[],int Bs,double threshold_EdBM, char Inputimage[],double &threshold_ostu);
int otsu(char date_directory[], int &image_x, int &image_y, int paramerter[], int paramerter_count, int sd ,int &Edge_derectory_number);
double edge_st_temp(char date_directory[], int &image_xt, int &image_yt, int paramerter[], int paramerter_count, int sd,int &Edge_derectory_number);
int score(int correct_x, int correct_y, int image_xt, int image_yt, std::vector<int> max_x, std::vector<int> max_y, int count_tied_V_vote, int V_vote_max, int data_num);
int score_record(char date_directory[], char Inputimage[], int correct_score, int count_tied_V_vote, int V_vote_max);




int main(int argc, char** argv){

	std::vector<int>max_x;
	std::vector<int>max_y;
	int count_tied_V_vote;
	int V_vote_max;

	int pixel[10]={0,1,3,5,7,9,13,17};
	int Togire[10] = { 0,1,3,5,7,9,13,17 };
	int Bs=5;
	double threshold_EdBM=3;
	double threshold_otsu = 0;
	
	int paramerter[4];					//paramerter[0]=1��sobel�t�B���^,paramerter[0]=2��gaus�~sobel�t�B���^

	//�W���΍��̒����ӏ�
	int sd_max = 50;
	int paramerter_count_max = 1;
	int cossim_atan_switch = 1;			//cossim_atan_switch=0��cossim,cossim_atan_switch=1��arctan
	paramerter[0] = 1;					//paramerter[0]=1��sobel�t�B���^,paramerter[0]=2��gaus�~sobel�t�B���^
	int Edge_derectory_number = 2;		//�G�b�W���x�̌v�Z�ɗp���鐬���̐��D2�i0,90)�܂���8(0~315)

	int data_num = 33;


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

	int paramerter_count=0;



	//for (int z2 = 1; z2 <= 7; ++z2) {		//pixel
	//	for (int z = 1; z <= 7; ++z) {		//Togire
		

			
		//	notimeset(date, pixel, Togire,z2,z);
			for (int image_kurikaeshi = 1; image_kurikaeshi < data_num + 1; ++image_kurikaeshi) {
				timeset(date);
				for (int paramerter_count = 1; paramerter_count <= 1; ++paramerter_count) {

					//for (sd = 0; sd <= sd_max; sd = sd + 10) {
					for (sd = 40; sd <=40; sd = sd + 10) {


						//�Ώۉ摜�ƃe���v���[�g�摜����ʂ��邽�߂�paramerter[0]��ύX����
						/*		|kernel	|	sobel	|	sobel�~gaus	|
						�T���Ώ�|	0	|	1		|		2		|
						template|	3	|	4		|		5		|*/
						if (image_kurikaeshi!=1) {
							switch (paramerter[0]) {
							case 4: paramerter[0] = 1; break;
							case 5: paramerter[0] = 2; break;
							default: paramerter[0] = 3; break;
							}
						}
						if (paramerter[0] == 1 || paramerter[0] == 2) {
							//	sprintf(image_nameP, "..\\property_usa\\property_3k_conv_", paramerter[paramerter_count]);
							//	sprintf(image_nameP, "..\\property_usa\\property_%d�~%dsobel_conv_", paramerter[paramerter_count], paramerter[paramerter_count]);
							sprintf(image_nameP, "..\\property_usa\\simulation17-1218\\property_%d�~%dsobel_conv", paramerter[paramerter_count], paramerter[paramerter_count]);
							//	sprintf(image_nameP, "..\\property_usa\\simulation17-0725\\sobel\\15-%dp-%dT_sobel", pixel[z2], Togire[z], paramerter[paramerter_count]);
							sprintf(image_nameP2, "%s_sd%d_%d.txt", image_nameP, sd,image_kurikaeshi);
							//sprintf(image_nameP2, "%s\\property_%d�~%dsobel_conv_sd%d.txt", image_nameP, paramerter[paramerter_count], paramerter[paramerter_count], sd);
						}
						else {
							//sprintf(image_nameP, "..\\property_usa\\simulation17-0821\\kernel\\15-%dp-%dT\\property_%dk_conv_", pixel[z2], Togire[z], paramerter[paramerter_count]);
							sprintf(image_nameP, "..\\property_usa\\simulation17-1024\\property_kernel_image4\\property_%dk_conv_", paramerter[paramerter_count]);
							//	sprintf(image_nameP, "..\\property_usa\\property_3k_conv_", paramerter[paramerter_count]);
							sprintf(image_nameP2, "%ssd%d.txt", image_nameP, sd);
							//sprintf(image_nameP, "..\\property_usa\\simulation17-0824-2\\property_B135");
							//sprintf(image_nameP2, "%s.txt", image_nameP);
						}

						convolution(argc, argv, image_nameP2, image_x, image_y, image_xt, image_yt, paramerter, paramerter_count, sd, date, date_directory, Inputimage);

						printf("x=%d,y=%d\nxt=%d,yt=%d\n", image_x, image_y, image_xt, image_yt);

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
						default: paramerter[0] = 3; break;
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


						otsu(date_directory, image_x, image_y, paramerter, paramerter_count, sd, Edge_derectory_number);



						threshold_otsu = edge_st_temp(date_directory, image_xt, image_yt, paramerter, paramerter_count, sd, Edge_derectory_number);

						printf("threshold_otsu=%f\n", threshold_otsu);
						/*
						int max_x = 0;
						int max_y = 0;
						int count_tied_V_vote = 1;
						int V_vote_max = 1;
*/
						//arctan��p����ꍇ�́Cotsu�ŋ��߂�threshold_otsu��threshold_EdBM�ɑ������臒l�Ƃ��ėp����悤�ɂ��Ă���D
						std::tie(max_x, max_y, count_tied_V_vote, V_vote_max) = Edge_detection_Block_Matching(date_directory, image_x, image_y, image_xt, image_yt, paramerter, paramerter_count, sd, date, Bs, threshold_EdBM, Inputimage, threshold_otsu);


						if (data_num != 0) {

							int correct_score = 0;
							int correct_x=0;
							int correct_y=0;
							/*
							std::vector<int>max_xt;
							std::vector<int>max_yt;
							max_xt.resize(count_tied_V_vote);
							max_yt.resize(count_tied_V_vote);
							max_xt[0] = max_x;
							max_yt[0] = max_y;*/
							correct_score = score(correct_x, correct_y, image_xt, image_yt, max_x, max_y, count_tied_V_vote, V_vote_max, data_num);

							score_record(date_directory, Inputimage, correct_score, count_tied_V_vote, V_vote_max);
						}




					}
				}
				//}
			//}
			}
		printf("�S�Ă̏������I�����܂���\n");

		return 0;
}




