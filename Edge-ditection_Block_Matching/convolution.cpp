
//�������m�ۂ��s�����߂̃w�b�_
#define ANSI				
#include "nrutil.h"	

#include<stdio.h>
#include<math.h>
#include <omp.h>	//openMP

#include<fstream>
#include<iostream>
#include<string>
#include<sstream> //�����X�g���[��

#include<time.h>//���Ԃ�p����

#include <direct.h>//�t�H���_���쐬����

#include <opencv2/opencv.hpp>	//�摜�ǂݍ���

#include <tuple>
std::tuple<int, int, std::vector<std::vector<double>>> read_txt(const char *filename);

using namespace std;

int conv_eco_mode_flag = 1;

int x, y, fx, fy;
int hfs;
FILE *fp;

//property�֌W_�ϐ�
//int &image_x, &image_y;		//�摜�T�C�Y
int Rvector_create=0;		//1�Ȃ��x�N�g�������
int Rvector_pointX;		//��x�N�g�����쐬����ʒu.0���X�^�[�g�ōl����
int Rvector_pointY;		//����Ȏw�������ꍇ�͉���
int fs;					//�t�B���^�̑傫��(�)
int All_property[10];	//property�����L�̏���ǂݍ��ޗp

//property�֌W_�f�B���N�g��
char propety[255];
char inputtemplate_directory[255];
char outputrvector_directory[128];				//Rvector�̏ꏊ
char inputimage_directory[128];
char inputfilter_directory[255];
char inputfilter_directory1[255];
char inputfilter_directory2[255];
char inputfilter_directory3[255];
char inputfilter_directory4[255];
char inputfilter_directory5[255];
char inputfilter_directory6[255];
char inputfilter_directory7[255];
char inputfilter_directory8[255];			//�J�Ԃ����𑝂₵����1/4
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

char InputImage[128];						//���͉摜
char Inputtemplate[128];					//���̓e���v���[�g

int filter_reset_flag=0;

//�o�̓t�@�C�����E�o�͐�̐ݒ�
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

//Rvector�֘A
char Rvector_directory[128];
double Rvector[9];
char Rvector_Filename[255];
char *Rvector_Filename_s;

//�o�̓t�@�C���f�B���N�g��
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

	//�ʂŉ摜���w�肵�����Ƃ�
	//std::ifstream propety_dire("..\\property_usa\\simulation17-0613\\property_3k_conv_sd0.txt");
	//��x�N�g�����擾����Ƃ��͂�����
	//int filter_number = 0;
	//sprintf(Rvector_Filename, "%s\\Rvector%d.csv", Rvector_directory, filter_number);
	//�t�B���^���Z�C��x�N�g���쐬����Ƃ��̂ݎw��D������ύX����
	/*	char propety_dire_char[128];
	sprintf(propety_dire_char,"..\\property_usa\\simulation17-0203\\Rvector\\8dire_100k\\property_8dire_100k_I~%d.txt",filter_number);
	std::ifstream propety_dire(propety_dire_char);*/
	
	//臒l�̐ݒ�D����property.txt�ɉ����邩��
	int Offset = 128;
	double magnification = 1;
	float Upper_threshold = 35;
	float Under_threshold = -35;
	int use_upperthreshold_flag = 1;
	int use_underthreshold_flag = 1;
	int Save_image_flag[9][9];

	//int image_xt, image_yt;
/////////////�摜�쐬�̗L��////////////////////////////////////////////////////////////////

	//�摜�쐬���f�̏�����(�f�t�H���g�ł͍쐬���Ȃ�)
	for (int i = 0; i<9; ++i) {
		Save_image_flag[1][i] = 0;
		Save_image_flag[2][i] = 0;
	}

	//eco���[�h
	if (conv_eco_mode_flag == 1) {
		for (int j = 0; j < 2; ++j) {
			for (int i = 0; i < 9; ++i) {
				Save_image_flag[j][i] = 0;
			}
		}
	}

	//���l�摜���쐬����
	Save_image_flag[2][0] = 1;		//���l�摜���쐬����Ƃ�1


	//8�����ɉ摜���쐬����
	for (int i = 1; i <= 8; ++i) {

		Save_image_flag[2][i] = 1;	//�쐬�����������̉摜�̑I���Di���w��
	}

	


////////////////////////////////////////////���͉摜�̓ǂݍ���////////////////////////////////////////////////////////////////////////////
	std::ifstream propety_dire;
	propety_dire.open(image_nameP2,ios::in);
	
	read_property(propety_dire,image_x,image_y,fs);	//property����ǂݎ��
	
	//���͉摜��ǂݍ���
	sprintf(InputImage, inputimage_directory);	//property����ǂݎ�������͉摜������
	cv::Mat ImputImageM = cv::imread(InputImage);	//���͉摜�̓ǂݍ��݁Dopencv
	image_x=ImputImageM.cols;
	image_y=ImputImageM.rows;

	//�e���v���[�g�摜��ǂݍ���
	sprintf(Inputtemplate, inputtemplate_directory);	//property����ǂݎ�������͉摜������
	cv::Mat ImputImageT = cv::imread(Inputtemplate);	//���͉摜�̓ǂݍ��݁Dopencv
	image_xt = ImputImageT.cols;
	image_yt = ImputImageT.rows;
	
//////////////////////////�����ݒ�̊m�F�p//////////////////////////////////////////////////////////////////////////////

	if (Rvector_create == 1)printf("��x�N�g���擾���W�FX=%d�CY=%d\n", Rvector_pointX, Rvector_pointY);
	printf("InputImage=%s\n", InputImage);
	printf("�摜�T�C�Y�FX=%d�CY=%d\n", image_x, image_y);
	printf("Inputtemplate=%s\n", Inputtemplate);
	printf("�摜�T�C�Y�FX=%d�CY=%d\n", image_xt, image_yt);
	//printf("fs=%d\n", fs);
	printf("Offset=%d\n", Offset);
	printf("Upper_threshold : %f, Under_threshold : %f\n", Upper_threshold, Under_threshold);
	printf("use_upperthreshold_flag : %d, use_underthreshold_flag : %d\n", use_upperthreshold_flag, use_underthreshold_flag);

	//Nrutil��p�����������̊m��
	double **input_bmp = matrix(0, image_x-1, 0, image_y-1);
	double **output1 = matrix(0, image_x-1, 0, image_y-1);
	double **input_bmp2 = matrix(0, image_x - 1, 0, image_y - 1);
	double **output2 = matrix(0, image_x - 1, 0, image_y - 1);
	double **spfil1 = matrix(1, image_x, 1, image_y);
	double **output_bmp_image = matrix(0, image_x-1, 0, image_y-1);	//�ǉ�1
	double **output_bmp_flag = matrix(0, image_x-1, 0, image_y-1);	//�ǉ�3
	
	//�m�ۂ���������������������
	for (y = 0; y < image_y; y++) {
		for (x = 0; x < image_x; x++) {
			input_bmp[x][y] = 0;
			output1[x][y] = 0;
			input_bmp2[x][y] = 0;
			output2[x][y] = 0;
			output_bmp_image[x][y] = 0;	//�ǉ�2
			output_bmp_flag[x][y] = 0;	//�ǉ�3
		}
	}
	
	for (fy = 1; fy <= image_y; fy++) {
		for (fx = 1; fx <= image_x; fx++) {
			spfil1[fx][fy] = 0;
		}
	}
	

	//int hfs = (fs + 1) / 2;			//�t�B���^�̑傫���̔���

////////////////////////�摜�̓ǂݎ��//////////////////////////////////////////////////////////////////////////////////////////////
	for (y = 0; y < image_y; ++y) {
		for (x = 0; x < image_x; ++x) {
			//opencv�ł͉�f��R,G,B�̏��ŕ���ł���
			int R = 3 * x;
			int G = 3 * x + 1;
			int B = 3 * x + 2;
			//�O���[�X�P�[���ł̓ǂݍ���
			input_bmp[x][y] = (((ImputImageM.at<unsigned char>(y, R)+ImputImageM.at<unsigned char>(y, G)+ImputImageM.at<unsigned char>(y, B))/3)-Offset);
			//input_bmp[x][y] = ImputImageM.at<unsigned char>(y, R)-Offset;
		}
	}

	for (y = 0; y < image_yt; ++y) {
		for (x = 0; x < image_xt; ++x) {
			int Rt = 3 * x;
			int Gt = 3 * x + 1;
			int Bt = 3 * x + 2;
			//�O���[�X�P�[���ł̓ǂݍ���
			input_bmp2[x][y] = (((ImputImageT.at<unsigned char>(y, Rt) + ImputImageT.at<unsigned char>(y, Gt) + ImputImageT.at<unsigned char>(y, Bt)) / 3) - Offset);
		}
	}

	//�o�̓t�@�C���ɂ��Ă̐ݒ�P
	set_outputfile(date,date_directory,paramerter,paramerter_count,sd);

	if(Rvector_create==1)Rvector_createF();	//��x�N�g���̍쐬

	printf("****************************************\n");
	printf("start�Fconvolution\n");
	printf("****************************************\n");

	int Rvector_number = 0;				//Rvector�̔ԍ��̃J�E���g

	std::vector<std::vector<double>>spfil1_kari;

	spfil1_kari.resize(image_x);
	for (int i = 0; i<image_x; ++i) {
		spfil1_kari[i].resize(image_y);
	}

///////////////////////�t�B���^�̓ǂݍ��݁E��ݍ���/////////////////////////////////////////
	for(int kernel_number=0;kernel_number<8;++kernel_number){

		

		//�t�B���^�̑I���D�������݃t�@�C���̑I��
		if(kernel_number==0){sprintf(inputfilter_directory,inputfilter_directory1);sprintf(Filename,Filename1);sprintf(Filenamet,Filename11);}
		if(kernel_number==1){sprintf(inputfilter_directory,inputfilter_directory2);sprintf(Filename,Filename2); sprintf(Filenamet, Filename12);}
		if(kernel_number==2){sprintf(inputfilter_directory,inputfilter_directory3);sprintf(Filename,Filename3); sprintf(Filenamet, Filename13);}
		if(kernel_number==3){sprintf(inputfilter_directory,inputfilter_directory4);sprintf(Filename,Filename4); sprintf(Filenamet, Filename14);}
		if(kernel_number==4){sprintf(inputfilter_directory,inputfilter_directory5);sprintf(Filename,Filename5); sprintf(Filenamet, Filename15);}
		if(kernel_number==5){sprintf(inputfilter_directory,inputfilter_directory6);sprintf(Filename,Filename6); sprintf(Filenamet, Filename16);}
		if(kernel_number==6){sprintf(inputfilter_directory,inputfilter_directory7);sprintf(Filename,Filename7); sprintf(Filenamet, Filename17);}
		if(kernel_number==7){sprintf(inputfilter_directory,inputfilter_directory8);sprintf(Filename,Filename8); sprintf(Filenamet, Filename18);}

		//read_filter(inputfilter_directory,fs,spfil1);	//�t�B���^�̓ǂݍ���
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
			hfs = (fs + 1) / 2;			//�t�B���^�̑傫���̔���
		}
		

		//��ݍ���
		if(paramerter[0]==2){
		convolution_gaus_sobel(image_y,image_x,fs,hfs,output1,spfil1,input_bmp,magnification);
		}else{
		convolution(image_y,image_x,fs,hfs,output1,spfil1,input_bmp,magnification);
		}
		//�t�@�C���ւ̏�������
		write_file(Filename,image_x,image_y,output1,Rvector_create,Rvector_pointX,Rvector_pointY,Rvector,Rvector_number);	

		


		//template�摜�̏�ݍ���
		if (paramerter[0] == 2) {
			convolution_gaus_sobel(image_yt, image_xt, fs, hfs, output2, spfil1, input_bmp2, magnification);
		}else{
			convolution(image_yt, image_xt, fs, hfs, output2, spfil1, input_bmp2, magnification);
		}
		//�t�@�C���ւ̏�������
		write_file(Filenamet, image_xt, image_yt, output2, Rvector_create, Rvector_pointX, Rvector_pointY, Rvector, Rvector_number);


		printf("convolution�Ffilter %s\n", inputfilter_directory);	//�p�����t�B���^�̕\��
		Rvector_number++;					//Rvector�̔ԍ��̃J�E���g

	}

	Rvector_number=0;					//Rvector�̔ԍ��̃J�E���g

	

///////////////////////////////////��x�N�g���̍쐬���ʃt�@�C���̍쐬/////////////////////////////////////////////////////

	if (Rvector_create == 1) {
		if ((fp = fopen(Rvector_Filename, "w")) == NULL) { exit(1); }
		Rvector_number = 0;
		for (Rvector_number = 0 ; Rvector_number < 8 ; ++Rvector_number) {
			fprintf(fp, "%lf,", Rvector[Rvector_number]);
		}
		fclose(fp);
	}

//////////////////////////////opencv��p������ݍ��݉摜�̍쐬///////////////////////////////////////////////////////////////////////////////////

	if (Save_image_flag[2][0] == 1) {
		for (int make_image_repeat = 1; make_image_repeat <= 8; ++make_image_repeat) {
			//�摜�̍쐬
			make_bmp(date_directory2, Filename1, image_x, image_y, make_image_repeat, Save_image_flag);
			make_bmp(date_directory2, Filename1, image_xt, image_yt, make_image_repeat, Save_image_flag);
		}
	}

//////////////////////////////log�̍쐬///////////////////////////////////////////////////////////////////////////////////
	FILE *fp_date;
	char filename_log[128];
	//sprintf(filename_log, "..\\log\\log-%2d-%02d%02d-%02d%02d%02d.txt",pnow->tm_year+1900,pnow->tm_mon + 1,pnow->tm_mday,pnow->tm_hour,pnow->tm_min,pnow->tm_sec);	//log�t�@�C���쐬�̃f�B���N�g���w��
	sprintf(filename_log, "%s\\log.txt",date_directory2);	//log�t�@�C���쐬�̃f�B���N�g���w��
	if ((fp_date = fopen(filename_log, "w")) == NULL) { printf("log�t�@�C�����J���܂���"); exit(1); }
	fprintf(fp_date, "Time : %s\n", date);								//����
	fprintf(fp_date, "Rvector_create=%d\n", Rvector_create);			//Rvector�̍쐬�������ǂ���
	if (Rvector_create == 1)fprintf(fp_date, "Rvector_Coordinate�FX=%d�CY=%d\n", Rvector_pointX, Rvector_pointY);
	fprintf(fp_date, "ImputImage=%s\n", InputImage);					//���͉摜
	fprintf(fp_date, "ImputImage_size�FX=%d�CY=%d\n", image_x, image_y);
	fprintf(fp_date, "Upper_threshold : %f, Under_threshold : %f\n", Upper_threshold, Under_threshold);
	fprintf(fp_date, "use_upperthreshold_flag : %d, use_underthreshold_flag : %d\n", use_upperthreshold_flag, use_underthreshold_flag);
	fprintf(fp_date, "Offset : %d\n", Offset);						//��ݍ��݂̍ۂ�offset
	fprintf(fp_date, "Kernel\n");									//�g�p�����J�[�l���̋L�^
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
	printf("log�t�@�C�� %s ��ۑ����܂���\n", filename_log);
	

	//�������̊J��
	free_matrix(input_bmp, 0, image_x-1, 0, image_y-1);
	free_matrix(output1,0, image_x-1, 0, image_y-1);
	free_matrix(spfil1,  1, image_x,  1, image_y);
	free_matrix(output_bmp_image, 0, image_x-1, 0, image_y-1);
	free_matrix(output_bmp_flag, 0, image_x-1, 0, image_y-1);

	printf("finish�Fconvolution\n");

	
	
	return *InputImage;
	
}

void set_outputfile(char date[],char date_directory[],int paramerter[],int paramerter_count,int sd){
	
//////////////////////////////Output�f�B���N�g���̐ݒ�//////////////////////////////////////////////////////////////////////////////////
	//�o�͌��ʂ̃t�@�C�����̎w��
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


	//���ʂ�ۑ�����t�H���_�̍쐬
	//�t�H���_���͎��s�����ɂȂ�
	sprintf(date_directory, "..\\result_usa\\%s\\", date);
	if (_mkdir(date_directory) == 0) {
		printf("�t�H���_ %s ���쐬���܂���\n", date_directory);
	}
	else {
		printf("�t�H���_�쐬�Ɏ��s���܂����B�������͍쐬�ς݂ł�\n");
	}

	if(paramerter[0]==1 || paramerter[0]==2){
		sprintf(date_directory2, "%s%d�~%dsobel_conv_sd%d\\", date_directory,paramerter[paramerter_count],paramerter[paramerter_count],sd);		
	}else{
		sprintf(date_directory2, "%s%dk_conv_sd%d\\", date_directory,paramerter[paramerter_count],sd);
	}
	//Output�f�B���N�g���̍쐬
	if (_mkdir(date_directory2) == 0) {
		printf("�t�H���_ %s ���쐬���܂���\n", date_directory2);
	}
	else {
		printf("�t�H���_�쐬�Ɏ��s���܂����B\n");
	}
	

	//Output�t�@�C���̃f�B���N�g���ݒ�
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

	printf("�g�p�����J�[�l����\n");
	printf("V0   = %s\n", inputfilter_directory1);
	printf("V45  = %s\n", inputfilter_directory2);
	printf("V90  = %s\n", inputfilter_directory3);
	printf("V135 = %s\n", inputfilter_directory4);
	printf("V180 = %s\n", inputfilter_directory5);
	printf("V225 = %s\n", inputfilter_directory6);
	printf("V270 = %s\n", inputfilter_directory7);
	printf("V315 = %s\n", inputfilter_directory8);
}

	///////////////////////////////�����ݒ�2 : ��x�N�g���쐬���ۂ�(�C�������Ȃ���΂Ȃ�Ȃ��j/////////////////////////////////////////////////////////////
void Rvector_createF(){

//if(Rvector_create==1)sprintf(InputImage, "..\\bmp\\255-0\\%d.bmp", filter_number);	//Rvector���쐬����ۂ̓��͉摜�w��.
											
	sprintf(Rvector_directory, outputrvector_directory, propety);		//Rvector��ۑ�����t�H���_�����w��.property.txt��2�s��																//Rvector��ۑ�����t�H���_�̍쐬
	printf("Rvector_create=%d\n", Rvector_create);
	if (Rvector_create == 1) {

		if (_mkdir(Rvector_directory) == 0) {
			printf("Rvecor�ۑ��t�H���_ %s ���쐬���܂���\n", Rvector_directory);
		}
		else {
			printf("Rvecor�ۑ��t�H���_ %s �͍쐬�ς݂ł�\n", Rvector_directory);
		}
	}
}


void read_property(ifstream &propety_dire,int &image_x,int &image_y,int &fs){

	count_property=0;

	//�v���p�e�Btxt�t�@�C���̓ǂݍ���
	if (propety_dire.fail())
	{
		printf("property�e�L�X�g��ǂݎ�邱�Ƃ��ł��܂���\n");
		printf("property�e�L�X�g : %s\n",propety_dire);
	}
	while (propety_dire.getline(propety, 256 - 1)){

		//if (count_property == 0)++count_property;

		if (count_property == 0)sprintf(inputtemplate_directory, propety);				//��s�ڂ̉摜�T�C�Y�Ȃ�
		if (count_property == 1)sprintf(outputrvector_directory, propety);	//�g�p�����x�N�g��
		if (count_property == 2)sprintf(inputimage_directory, propety);		//���͉摜
		if (count_property == 3)sprintf(inputfilter_directory1, propety);	//�g�p����J�[�l��1
		if (count_property == 4)sprintf(inputfilter_directory2, propety);	//�g�p����J�[�l��2
		if (count_property == 5)sprintf(inputfilter_directory3, propety);	//�g�p����J�[�l��3
		if (count_property == 6)sprintf(inputfilter_directory4, propety);	//�g�p����J�[�l��4
		if (count_property == 7)sprintf(inputfilter_directory5, propety);	//�g�p����J�[�l��5
		if (count_property == 8)sprintf(inputfilter_directory6, propety);	//�g�p����J�[�l��6
		if (count_property == 9)sprintf(inputfilter_directory7, propety);	//�g�p����J�[�l��7
		if (count_property == 10)sprintf(inputfilter_directory8, propety);	//�g�p����J�[�l��8

		++count_property;
	}

	//�v���p�e�Btxt�t�@�C���̈�s�ڂ𕶎��񂩂琔�l�ɕϊ����Ċ֐��ɑ��
/*	std::string tmp_Allpropety;
	std::istringstream stream_Allpropety(Allpropety);
	count_Allproperty=0;
	while (getline(stream_Allpropety, tmp_Allpropety, ',')) {

		int All_tmp_property = stof(tmp_Allpropety); //stof(string str) : string��int�ɕϊ�
		All_property[count_Allproperty] = All_tmp_property;

////////////////property.txt�̈�s��///////////////////////////////////////////////////////////
		if (count_Allproperty == 0)image_x = All_property[count_Allproperty];				//&image_x:�摜�̉��T�C�Y(256pixel�̉摜�̏ꍇ��256�Ǝw��)
		if (count_Allproperty == 1)image_y = All_property[count_Allproperty];				//&image_y:�摜�̏c�T�C�Y(256pixel�̉摜�̏ꍇ��256�Ǝw��)
		if (count_Allproperty == 2)Rvector_create = All_property[count_Allproperty];		//Rvector_create:1�Ȃ��x�N�g�����쐬�D
		if (count_Allproperty == 3)Rvector_pointX = All_property[count_Allproperty];		//Rvector_pointX:��x�N�g���擾X���W�D0����X�^�[�g
		if (count_Allproperty == 4)Rvector_pointY = All_property[count_Allproperty];		//Rvector_pointY:��x�N�g���擾Y���W�D0����X�^�[�g
		if (count_Allproperty == 5)fs = All_property[count_Allproperty];					//fs:�t�B���^�T�C�Y�i��j

		++count_Allproperty;
	}
	*/
	propety_dire.close();
}


//////////////////////////�t�B���^�ǂݍ���////////////////////////////
std::tuple<int,std::vector<std::vector<double>>> read_filter(char inputfilter_directory[]){

	std::vector<std::vector<double>>filter_txt;
	filter_txt.resize(100);
	for (int i = 0; i<100; ++i) {
		filter_txt[i].resize(100);
	}

	int filter_x, filter_y;

	//�T�C�Y�s���txt�t�@�C���̓ǂݎ��
	std::tie(filter_x, filter_y, filter_txt) = read_txt(inputfilter_directory);
	
	if (filter_x != filter_y) {
		printf("�t�B���^�[�T�C�Y����������");
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

///////////////////////��ݍ��݌��ʂ̏�������////////////////////////////////////////
void write_file(char *Filename,int &image_x,int &image_y,double *output1[],int Rvector_create,int Rvector_pointX,int Rvector_pointY,double Rvector[],int Rvector_number){

	//------------------------�t�@�C���ւ̏�������--------------------------//
	if ((fp = fopen(Filename, "w")) == NULL){
		printf("�t�@�C���F%s���J���܂���\n",Filename);
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

	//�m�ۂ���������������������
	for (y = 0; y < image_y; y++) {
		for (x = 0; x < image_x; x++) {
			output1[x][y] = 0;
		}
	}

}

///////////////////////��ݍ��݉��Z//////////////////////////////////////////////////
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

					//�t�B���^�̐^�񒆂��`��_�ɂȂ�悤��hsf��p���Ē�������
					if (0 <= x + (fx - hfs) && x + (fx - hfs) < image_x
						&& 0 <= y + (fy - hfs) && y + (fy - hfs) < image_y)
					{
						output1[x][y] += spfil1[fx][fy] * input_bmp[x + (fx - hfs)][y + (fy - hfs)];
					}
				}
			}
			output1[x][y] = output1[x][y] * magnification;		//�{���ɉ����ăQ�C����ω�
		}

	}
	++filter_reset_flag;

	//�t�B���^�̏�����
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

///////////////////sobel�t�B���^�ɑΉ������K�E�X�t�B���^��p������ݍ��݉��Z//////////////////////////////////////////////////////////
int convolution_gaus_sobel(int &image_y,int &image_x,int fs, int hfs,double *output1[],double *spfil1[],double *input_bmp[],double magnification){

	//Nrutil��p�����������̊m��
	double **spfil1_g = matrix(1, fs, 1, fs);
	double **output1_g = matrix(0, image_x-1, 0, image_y-1);

	read_filter_gaus(fs,spfil1_g);

	//������
	for (y = 0; y < image_y; y++) {
		for (x = 0; x < image_x; x++) {
			output1_g[x][y] = 0;
		}
	}

	//gaus�t�B���^�ɂ��Ȃ߂�
	for (y = 0; y < image_y; y++) {
#pragma omp parallel for
		for (x = 0; x < image_x; x++) {
			for (fy = 1; fy <= fs; fy++) {
				for (fx = 1; fx <= fs; fx++) {
					
					//�t�B���^�̐^�񒆂��`��_�ɂȂ�悤��hsf��p���Ē�������
					if(		0<=x+(fx-hfs) && x+(fx-hfs)<image_x
						&&	0<=y+(fy-hfs) && y+(fy-hfs)<image_y )
					{
						output1_g[x][y] += spfil1_g[fx][fy] * input_bmp[x + (fx - hfs)][y + (fy - hfs)];
					}
				}
			}
			//output1_g[x][y] = output1_g[x][y] * magnification;		//�{���ɉ����ăQ�C����ω�
		}
	
	}
	
	
	//�Ȃ߂����摜�ɑ΂���sobel�t�B���^���|����
	for (y = 0; y < image_y; y++) {
#pragma omp parallel for
		for (x = 0; x < image_x; x++) {
			
			for (fy = 1; fy <= fs; fy++) {
				for (fx = 1; fx <= fs; fx++) {
					
					//�t�B���^�̐^�񒆂��`��_�ɂȂ�悤��hsf��p���Ē�������
					if(		0<=x+(fx-hfs) && x+(fx-hfs)<image_x
						&&	0<=y+(fy-hfs) && y+(fy-hfs)<image_y )
					{
						output1[x][y] += spfil1[fx][fy] * output1_g[x + (fx - hfs)][y + (fy - hfs)];
					}
				}
			}
			output1[x][y] = output1[x][y] * magnification;		//�{���ɉ����ăQ�C����ω�
		}
	
	}
	
	//�t�B���^�̏�����
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

	//������
	for (y = 1; y <= fs; y++) {
		for (x = 1; x <= fs; x++) {
			spfil1_g[x][y] = 0;
		}
	}

	char inputfilter_gaus[128];
	sprintf(inputfilter_gaus,"..\\filter\\gaus\\%d�~%dgaus.txt",fs,fs);

	if ((fp = fopen(inputfilter_gaus, "r")) == NULL){
		printf("�K�E�X�t�B���^�F%s���ǂݍ��߂܂���\n",inputfilter_gaus);
		exit(1);
	}
	
	for (fy = 1; fy <= fs; fy++) {
		for (fx = 1; fx <= fs; fx++) {
			fscanf(fp, "%lf	", &spfil1_g[fx][fy]);
		}
	}
	fclose(fp); 
}