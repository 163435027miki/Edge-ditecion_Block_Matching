
#include<fstream>
#include<iostream>
#include<string>
#include<sstream> //�����X�g���[��
#include<math.h>
#include<time.h>//���Ԃ�p����
#include <direct.h>//�t�H���_���쐬��
#include<stdio.h>
#include <windows.h>

int cos_eco_mode_flag = 0;

//�������m�ۂ��s�����߂̃w�b�_
#define ANSI				
#include "nrutil.h"	

#define PI 3.14159265

using namespace std;

char date_directory3[128];

int i,j;

//////////////////��`///////////////////////////////////////////////////////////////////////////////////////////
	double Rvector[10][10];
	double V0[255];
	double V45[255];
	double V90[255];
	double V135[255];
	double V180[255];
	double V225[255];
	double V270[255];
	double V315[255];

	char inputdate_directory[255];					//inputdate�̏ꏊ
	char inputrvector_directory[128];				//Rvector�̏ꏊ

	char *Input_Rvectormagniname_s = "\\Rvector_magni.csv";	//��x�N�g���̔{��
	char *Input_Rvectorname1_s = "\\Rvector0.csv";			//��x�N�g�����̎w��
	char *Input_Rvectorname2_s = "\\Rvector45.csv";
	char *Input_Rvectorname3_s = "\\Rvector90.csv";
	char *Input_Rvectorname4_s = "\\Rvector135.csv";
	char *Input_Rvectorname5_s = "\\Rvector180.csv";
	char *Input_Rvectorname6_s = "\\Rvector225.csv";
	char *Input_Rvectorname7_s = "\\Rvector270.csv";
	char *Input_Rvectorname8_s = "\\Rvector315.csv";
	

	char *Input_Filename1_s = "\\V(0).csv";			//���̓t�@�C�����̎w��
	char *Input_Filename2_s = "\\V(45).csv";
	char *Input_Filename3_s = "\\V(90).csv";
	char *Input_Filename4_s = "\\V(135).csv";
	char *Input_Filename5_s = "\\V(180).csv";
	char *Input_Filename6_s = "\\V(225).csv";
	char *Input_Filename7_s = "\\V(270).csv";
	char *Input_Filename8_s = "\\V(315).csv";
	
	char *math_name1_s = "innerp.csv";				//�o�͌��ʂ̃t�@�C�����̎w��
	char *math_name2_s = "V_sqrt.csv";
	char *math_name3_s = "Cos_similarity.csv";
	char *math_name4_s = "Angle.csv";
	char *math_name5_s = "threshold_high.csv";
	char *math_name6_s = "use_Rvector_flag.csv";
	char *math_name7_s = "use_Rvector_number.csv";
	char *math_name8_s = "threshold2.csv";
	char *math_name9_s = "threshold_low.csv";
	char *math_name10_s = "threshold3.csv";
	char *math_name11_s = "use_Rvector_number_LC.csv";

	char Input_Rvectormagni_name[255];
	char Input_Rvectorname1[250];					//��x�N�g�����E��x�N�g���̓��͐�̐ݒ�
	char Input_Rvectorname2[250];
	char Input_Rvectorname3[250];
	char Input_Rvectorname4[250];
	char Input_Rvectorname5[250];
	char Input_Rvectorname6[250];
	char Input_Rvectorname7[250];
	char Input_Rvectorname8[250];

	char Input_Filename1[255];						//���̓t�@�C�����E���͌��̐ݒ�
	char Input_Filename2[255];
	char Input_Filename3[255];
	char Input_Filename4[255];
	char Input_Filename5[255];
	char Input_Filename6[255];
	char Input_Filename7[255];
	char Input_Filename8[255];
													//�o�̓t�@�C�����E�o�͐�̐ݒ�
	char math_name1[128];							//����						
	char math_name2[128];							//�����d��V�̑傫��
	char math_name3[128];							//cos�ގ��x
	char math_name4[128];							//�p�x�D�ŏI����
	char math_name5[128];							//threshold_high
	char math_name6[128];							//use_Rvector_flag(�����d���̍ŏ��̈ʒu)
	char math_name7[128];							//use_Rvector_number(���ςł��悤�����x�N�g���̔ԍ��j
	char math_name8[128];							//2��臒l�����
	char math_name9[128];							//threshold(use_Rvector_flag�̉����d��V�̑傫���j
	char math_name10[128];							//�Ǐ��A�������l������threshold(use_Rvector_flag�̉����d��V�̑傫���j
	char math_name11[128];							//�Ǐ��A�������m�F�����Ƃ���use_Rvector_number

	double Rvectormagni[10];						//��x�N�g���̔{��		
	double Rvector_sqrt[9];							//Rvector�̑傫��
	double Rvector_square[9][9];
	double Rvector_square_sum[9];
	int use_Rvector_flag;
	int use_Rvector_number;
	//double threshold_high;
	//double threshold_low;
	double threshold_low_abs;
	double threshold_high_abs;

	int direction_number=8;					//8�����C4�����C2�����̎w��
	int curcuit_number=8;						//�g�p�����H
	int Rvector_number=8;						//�g�p�����x�N�g���̐�
	int minor_flag=0;							//�\���ȊO�̕ςȊ�x�N�g�������Ƃ���1�ɂ���

	ifstream Rvector_0;
	ifstream Rvector_45;
	ifstream Rvector_90;
	ifstream Rvector_135;
	ifstream Rvector_180;
	ifstream Rvector_225;
	ifstream Rvector_270;
	ifstream Rvector_315;

	FILE *fp_innerp,*fp_V_sqrt,*fp_Cos_similarity,*fp_Angle,*fp_use_Rvector_flag,*fp_use_Rvector_number,*fp_threshold,*fp_threshold_high,*fp_threshold2,*fp_threshold3,*fp_threshold_number_LC;

	void Rvector_read();
	void Read_output();
	int local_connectivity(int image_x,int image_y,double *local_flag[],double *threshold_LC_number[]);

int cossim(char date_directory[],int &image_x,int &image_y,int paramerter[],int paramerter_count,int sd,char date[]){
	printf("****************************************\n");
	printf("start�F cos-sim\n");
	printf("****************************************\n");
	//FILE *fp_newfunction,*fp_Angle2,,*fp_Correction_flag;									//�o�͑��₷�p3

	int i=1,j=1;
	int count_small=0,count_large=0;
	int count_property=0;
	int count_Allproperty=0;

/////////////////////////////////�����ݒ� : input�ݒ�//////////////////////////////////////////////////////////////////

	//Nrutil��p�����������̊m��
	double **threshold2 = matrix(0, image_x - 1, 0, image_y - 1);
	double **threshold3 = matrix(0, image_x - 1, 0, image_y - 1);
	double **threshold_high = matrix(0, image_x - 1, 0, image_y - 1);
	double **threshold_low = matrix(0, image_x - 1, 0, image_y - 1);
	double **V0 = matrix(0, image_x-1, 0, image_y-1);
	double **V45 = matrix(0, image_x-1, 0, image_y-1);
	double **V90 = matrix(0, image_x-1, 0, image_y-1);
	double **V135 = matrix(0, image_x-1, 0, image_y-1);
	double **V180 = matrix(0, image_x-1, 0, image_y-1);
	double **V225 = matrix(0, image_x-1, 0, image_y-1);
	double **V270 = matrix(0, image_x-1, 0, image_y-1);
	double **V315 = matrix(0, image_x-1, 0, image_y-1);
	//double **use_Rvector_flag = matrix(0, image_x-1, 0, image_y-1);
	double **threshold_low_flag = matrix(0, image_x-1, 0, image_y-1);
	double **threshold_high_flag = matrix(0, image_x-1, 0, image_y-1);
	double **V_sqrt = matrix(0, image_x-1, 0, image_y-1);
	double **Cos_similarity = matrix(0, image_x-1, 0, image_y-1);
	double **Angle = matrix(0, image_x-1, 0, image_y-1);
	double **innerp = matrix(0, image_x-1, 0, image_y-1);
	double **threshold_local_flag = matrix(0, image_x - 1, 0, image_y - 1);

	
	
	//�m�ۂ���������������������
			for (i = 0; i < image_y; i++) {
				for (j = 0; j < image_x; j++) {
					threshold_high[j][i] = 0;
					threshold_low[j][i] = 0;
					threshold2[j][i] = 0;
					threshold3[j][i] = 0;
					V0[j][i]=0;
					V45[j][i]=0;
					V90[j][i]=0;
					V135[j][i]=0;
					V180[j][i]=0;
					V225[j][i]=0;
					V270[j][i]=0;
					V315[j][i]=0;
					threshold_low_flag[j][i]=0;
					threshold_high_flag[j][i]=0;
					V_sqrt[j][i]=0;
					Cos_similarity[j][i]=0;
					Angle[j][i]=0;
					innerp[j][i]=0;
					threshold_local_flag[j][i] = 0;
				}
			}
	
			if(paramerter[0]==1 || paramerter[0]==2){
				sprintf(inputdate_directory,"%s%d�~%dsobel_conv_sd%d",date_directory,paramerter[paramerter_count],paramerter[paramerter_count],sd);	//���͂����ݍ��݌��ʂ̖��O
			}else{
				sprintf(inputdate_directory,"%s%dk_conv_sd%d",date_directory,paramerter[paramerter_count],sd);	//���͂����ݍ��݌��ʂ̖��O
			}
	
	
	printf("inputdata�F%s\n",inputdate_directory);

////////////////////////////���o�̓f�B���N�g���̍쐬//////////////////////////////////////////////////////////////////////////////
	
	//Input�t�@�C���̃f�B���N�g���ݒ�
	sprintf(Input_Filename1,"%s%s",inputdate_directory,Input_Filename1_s);
	sprintf(Input_Filename2,"%s%s",inputdate_directory,Input_Filename2_s);
	sprintf(Input_Filename3,"%s%s",inputdate_directory,Input_Filename3_s);
	sprintf(Input_Filename4,"%s%s",inputdate_directory,Input_Filename4_s);
	sprintf(Input_Filename5,"%s%s",inputdate_directory,Input_Filename5_s);
	sprintf(Input_Filename6,"%s%s",inputdate_directory,Input_Filename6_s);
	sprintf(Input_Filename7,"%s%s",inputdate_directory,Input_Filename7_s);
	sprintf(Input_Filename8,"%s%s",inputdate_directory,Input_Filename8_s);

	//Output�f�B���N�g���̐ݒ�
	if(paramerter[0]==1 || paramerter[0]==2){
		sprintf(date_directory3, "%s%d�~%dsobel_cossim_sd%d",date_directory, paramerter[paramerter_count],paramerter[paramerter_count],sd);	
			}else{
		sprintf(date_directory3, "%s%dk_cossim_sd%d",date_directory, paramerter[paramerter_count],sd);
	}
	//Output�f�B���N�g���̍쐬
	if (_mkdir(date_directory3) == 0) {
		printf("�t�H���_ %s ���쐬���܂���\n", date_directory3);
	}
	else {
		printf("�t�H���_�쐬�Ɏ��s���܂����B\n");
	}
	
////////////////////////////�t�@�C���̓ǂݍ���//////////////////////////////////////////////////////////////////////////////
	ifstream Rvector_magni();
	//Input�t�@�C�����J��
	ifstream V_0(Input_Filename1);
	ifstream V_45(Input_Filename2);
	ifstream V_90(Input_Filename3);
	ifstream V_135(Input_Filename4);
	ifstream V_180(Input_Filename5);
	ifstream V_225(Input_Filename6);
	ifstream V_270(Input_Filename7);
	ifstream V_315(Input_Filename8);
	

////////////////////////�G���[�o��/////////////////////////////////////////////////////////////////////////////////////////////
	
	if(!V_0){printf("���̓G���[ V(0).csv������܂���_cos-sim\nInput_Filename=%s",Input_Filename1);return 1;}
	if(!V_45){printf("���̓G���[ V(45).csv������܂���_cos-sim\nInput_Filename=%s",Input_Filename2);return 1;}
	if(!V_90){printf("���̓G���[ V(90).csv������܂���_cos-sim\nInput_Filename=%s",Input_Filename3);return 1;}
	if(!V_135){printf("���̓G���[ V(135).csv������܂���_cos-sim\nInput_Filename=%s",Input_Filename4);return 1;}
	if(!V_180){printf("���̓G���[ V(180).csv������܂���_cos-sim\nInput_Filename=%s",Input_Filename5);return 1;}
	if(!V_225){printf("���̓G���[ V(225).csv������܂���_cos-sim\nInput_Filename=%s",Input_Filename6);return 1;}
	if(!V_270){printf("���̓G���[ V(270).csv������܂���_cos-sim\nInput_Filename=%s",Input_Filename7);return 1;}
	if(!V_315){printf("���̓G���[ V(315).csv������܂���_cos-sim\nInput_Filename=%s",Input_Filename8);return 1;}
	
/////////////////////////�o�̓t�@�C�����J��///////////////////////////////////////////////////////////////////////////////////

	if(sd==0){
		//Output�f�B���N�g���̐ݒ�
		if(paramerter[0]==1 || paramerter[0]==2){
			sprintf(inputrvector_directory,"..\\Rvector\\%d�~%dsobel_under",paramerter[paramerter_count],paramerter[paramerter_count]);
		}else{
			sprintf(inputrvector_directory,"..\\Rvector\\8dire_%dk_0203",paramerter[paramerter_count]);
		}

		//��x�N�g���̓ǂݍ���
		Rvector_read();
		printf("��x�N�g����ǂݍ��݂܂���");
		
		for(i=1;i<=8;i++){
		for(j=1;j<=8;j++){
			printf("%lf, ",Rvector[i][j]);
		}
		printf("\n");
		}
	}

	//�o�͂���t�@�C�����J��
	Read_output();

  
///////////////////////�����d����csv�̓ǂݍ���//////////////////////////////////////////////////////////////////////////////////////////	
		i=1;
		printf("�����d����ǂݎ��܂�\n");
		string str_0,str_45,str_90,str_135,str_180,str_225,str_270,str_315;
		count_large=0;
		while(getline(V_0,str_0)){					//���̃��[�v���ł��ׂĂ��s��
			count_small=0;			//������

///////////////���낢���`�D�����ł��Ȃ��Ƃ����Ȃ�///////////////////////////////////////////////////////////////////////////
        string token_V_0;
        istringstream stream_V_0(str_0);
		
		getline(V_45,str_45);	string token_V_45;	istringstream stream_V_45(str_45);
		getline(V_90,str_90);	string token_V_90;	istringstream stream_V_90(str_90);
		getline(V_135,str_135);	string token_V_135;	istringstream stream_V_135(str_135);
		getline(V_180,str_180);	string token_V_180;	istringstream stream_V_180(str_180);
		getline(V_225,str_225);	string token_V_225;	istringstream stream_V_225(str_225);
		getline(V_270,str_270);	string token_V_270;	istringstream stream_V_270(str_270);
		getline(V_315,str_315);	string token_V_315;	istringstream stream_V_315(str_315);
	
//////////////////////////�z��ɑ��//////////////////////////////////////////////////////////////////////////////////////////////////

        while(getline(stream_V_0,token_V_0,',')){	//��s�ǂݎ��DV0�̂݁C�J��Ԃ��͈͎̔w��ɗp����
			double tmp_V_0=stof(token_V_0);			//�����𐔎��ɕϊ�
			V0[count_small][count_large]=tmp_V_0;				//�z��ɑ��
			V0[count_small][count_large]=Rvectormagni[1]*V0[count_small][count_large];

			getline(stream_V_45,token_V_45,',');
			double tmp_V_45=stof(token_V_45);
			V45[count_small][count_large]=tmp_V_45;
			V45[count_small][count_large]=Rvectormagni[2]*V45[count_small][count_large];
        
			getline(stream_V_90,token_V_90,',');
			double tmp_V_90=stof(token_V_90);
			V90[count_small][count_large]=tmp_V_90;
			V90[count_small][count_large]=Rvectormagni[3]*V90[count_small][count_large];

			getline(stream_V_135,token_V_135,',');
			double tmp_V_135=stof(token_V_135);
			V135[count_small][count_large]=tmp_V_135;
			V135[count_small][count_large]=Rvectormagni[4]*V135[count_small][count_large];

			getline(stream_V_180,token_V_180,',');
			double tmp_V_180=stof(token_V_180);
			V180[count_small][count_large]=tmp_V_180;
			V180[count_small][count_large]=Rvectormagni[5]*V180[count_small][count_large];

			getline(stream_V_225,token_V_225,',');
			double tmp_V_225=stof(token_V_225);
			V225[count_small][count_large]=tmp_V_225;
			V225[count_small][count_large]=Rvectormagni[6]*V225[count_small][count_large];

			getline(stream_V_270,token_V_270,',');
			double tmp_V_270=stof(token_V_270);
			V270[count_small][count_large]=tmp_V_270;
			V270[count_small][count_large]=Rvectormagni[7]*V270[count_small][count_large];

			getline(stream_V_315,token_V_315,',');
			double tmp_V_315=stof(token_V_315);
			V315[count_small][count_large]=tmp_V_315; 
			V315[count_small][count_large]=Rvectormagni[8]*V315[count_small][count_large];

			++count_small;
			}
		++count_large;
		}
///////////////////�������炵�΂炭�召����////////////////////////////////////////////////////////////////////////////////////////////


///////////////////����臒l����//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		printf("�p�����x�N�g���̔�����s���܂�\n");

///////////////////////8�����擾��flag(����臒l)/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		for(count_small=0;count_small<image_x;++count_small){
			for(count_large=0;count_large<image_y;++count_large){
//	if(direction_number==8){		//�擾������ω������Ă����ꍇ�A���R�����g
			if(V0[count_small][count_large]<=V45[count_small][count_large] && V0[count_small][count_large]<=V90[count_small][count_large] && V0[count_small][count_large]<=V135[count_small][count_large] &&
				V0[count_small][count_large]<=V180[count_small][count_large] && V0[count_small][count_large]<=V225[count_small][count_large] && V0[count_small][count_large]<=V270[count_small][count_large] && V0[count_small][count_large]<=V315[count_small][count_large])
			{threshold_low_flag[count_small][count_large]=1;}			//V�̑召��r

			if(V45[count_small][count_large]<=V0[count_small][count_large] && V45[count_small][count_large]<=V90[count_small][count_large] && V45[count_small][count_large]<=V135[count_small][count_large] &&
				V45[count_small][count_large]<=V180[count_small][count_large] && V45[count_small][count_large]<=V225[count_small][count_large] && V45[count_small][count_large]<=V270[count_small][count_large] && V45[count_small][count_large]<=V315[count_small][count_large])
			{threshold_low_flag[count_small][count_large]=2;}

			if(V90[count_small][count_large]<=V0[count_small][count_large] && V90[count_small][count_large]<=V45[count_small][count_large] && V90[count_small][count_large]<=V135[count_small][count_large] &&
				V90[count_small][count_large]<=V180[count_small][count_large] && V90[count_small][count_large]<=V225[count_small][count_large] && V90[count_small][count_large]<=V270[count_small][count_large] && V90[count_small][count_large]<=V315[count_small][count_large])
			{threshold_low_flag[count_small][count_large]=3;}

			if(V135[count_small][count_large]<=V0[count_small][count_large] && V135[count_small][count_large]<=V45[count_small][count_large] && V135[count_small][count_large]<=V90[count_small][count_large] &&
				V135[count_small][count_large]<=V180[count_small][count_large] && V135[count_small][count_large]<=V225[count_small][count_large] && V135[count_small][count_large]<=V270[count_small][count_large] && V135[count_small][count_large]<=V315[count_small][count_large])
			{threshold_low_flag[count_small][count_large]=4;}

			if(V180[count_small][count_large]<=V0[count_small][count_large] && V180[count_small][count_large]<=V45[count_small][count_large] && V180[count_small][count_large]<=V90[count_small][count_large] &&
				V180[count_small][count_large]<=V135[count_small][count_large] && V180[count_small][count_large]<=V225[count_small][count_large] && V180[count_small][count_large]<=V270[count_small][count_large] && V180[count_small][count_large]<=V315[count_small][count_large])
			{threshold_low_flag[count_small][count_large]=5;}

			if(V225[count_small][count_large]<=V0[count_small][count_large] && V225[count_small][count_large]<=V45[count_small][count_large] && V225[count_small][count_large]<=V90[count_small][count_large] &&
				V225[count_small][count_large]<=V135[count_small][count_large] && V225[count_small][count_large]<=V180[count_small][count_large] && V225[count_small][count_large]<=V270[count_small][count_large] && V225[count_small][count_large]<=V315[count_small][count_large])
			{threshold_low_flag[count_small][count_large]=6;}

			if(V270[count_small][count_large]<=V0[count_small][count_large] && V270[count_small][count_large]<=V45[count_small][count_large] && V270[count_small][count_large]<=V90[count_small][count_large] &&
				V270[count_small][count_large]<=V135[count_small][count_large] && V270[count_small][count_large]<=V180[count_small][count_large] && V270[count_small][count_large]<=V225[count_small][count_large] && V270[count_small][count_large]<=V315[count_small][count_large])
			{threshold_low_flag[count_small][count_large]=7;}

			if(V315[count_small][count_large]<=V0[count_small][count_large] && V315[count_small][count_large]<=V45[count_small][count_large] && V315[count_small][count_large]<=V90[count_small][count_large] &&
				V315[count_small][count_large]<=V135[count_small][count_large] && V315[count_small][count_large]<=V180[count_small][count_large] && V315[count_small][count_large]<=V225[count_small][count_large] && V315[count_small][count_large]<=V270[count_small][count_large])
			{threshold_low_flag[count_small][count_large]=8;}
//	}		//�擾������ω������Ă����ꍇ�A���R�����g
			}
		}
			
///////////////////////4�����擾��flag(����臒l)/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//if(direction_number==4){		//�擾������ω������Ă����ꍇ�A���R�����g
/*				if(minor_flag==0){
					if(V0[count_small][count_large]<=V90[count_small][count_large] &&	V0[count_small][count_large]<=V180[count_small][count_large] && V0[count_small][count_large]<=V270[count_small][count_large])
						{threshold_low_flag[count_small][count_large]=1;}			//V�̑召��r


					if(V90[count_small][count_large]<=V0[count_small][count_large] &&	V90[count_small][count_large]<=V180[count_small][count_large] && V90[count_small][count_large]<=V270[count_small][count_large])
						{threshold_low_flag[count_small][count_large]=3;}


					if(V180[count_small][count_large]<=V0[count_small][count_large] && V180[count_small][count_large]<=V90[count_small][count_large] &&V180[count_small][count_large]<=V270[count_small][count_large])
						{threshold_low_flag[count_small][count_large]=5;}


					if(V270[count_small][count_large]<=V0[count_small][count_large] && V270[count_small][count_large]<=V90[count_small][count_large] && V270[count_small][count_large]<=V180[count_small][count_large])
						{threshold_low_flag[count_small][count_large]=7;}

				}
				if(minor_flag==1){

							if(V0[count_small][count_large]<=V45[count_small][count_large] && V0[count_small][count_large]<=V90[count_small][count_large] && V0[count_small][count_large]<=V135[count_small][count_large])
								{threshold_low_flag[count_small][count_large]=1;}			//V�̑召��r

							if(V45[count_small][count_large]<=V0[count_small][count_large] && V45[count_small][count_large]<=V90[count_small][count_large] && V45[count_small][count_large]<=V135[count_small][count_large])
								{threshold_low_flag[count_small][count_large]=2;}

							if(V90[count_small][count_large]<=V0[count_small][count_large] && V90[count_small][count_large]<=V45[count_small][count_large] && V90[count_small][count_large]<=V135[count_small][count_large])
								{threshold_low_flag[count_small][count_large]=3;}

							if(V135[count_small][count_large]<=V0[count_small][count_large] && V135[count_small][count_large]<=V45[count_small][count_large] && V135[count_small][count_large]<=V90[count_small][count_large])
								{threshold_low_flag[count_small][count_large]=4;}

				}
			}

*/
///////////////////////2�����擾��flag(����臒l)/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//if(direction_number==2){		//�擾������ω������Ă����ꍇ�A���R�����g


/*					if(minor_flag==0){
						
						if(V0[count_small][count_large]<=V90[count_small][count_large]){
							threshold_low_flag[count_small][count_large]=1;			//V�̑召��r
						}else{
							threshold_low_flag[count_small][count_large]=3;
						}
					}
					if(minor_flag==1){

						if(V0[count_small][count_large]<=V45[count_small][count_large]){
							threshold_low_flag[count_small][count_large]=1;			//V�̑召��r
						}else{
							threshold_low_flag[count_small][count_large]=2;
						}	
					}
				//else{
					//printf("curcuit_number=%d �̃G���[",curcuit_number);
				//}
			}
*/
//////////////����臒l�̔���////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////8�����擾��flag(����臒l)/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			for(count_small=0;count_small<image_x;++count_small){
			for(count_large=0;count_large<image_y;++count_large){
			
//if(direction_number==8){		//�擾������ω������Ă����ꍇ�A���R�����g
			if(V0[count_small][count_large]>=V45[count_small][count_large] && V0[count_small][count_large]>=V90[count_small][count_large] && V0[count_small][count_large]>=V135[count_small][count_large] &&
				V0[count_small][count_large]>=V180[count_small][count_large] && V0[count_small][count_large]>=V225[count_small][count_large] && V0[count_small][count_large]>=V270[count_small][count_large] && V0[count_small][count_large]>=V315[count_small][count_large])
			{threshold_high_flag[count_small][count_large]=1;}			//V�̑召��r

			if(V45[count_small][count_large]>=V0[count_small][count_large] && V45[count_small][count_large]>=V90[count_small][count_large] && V45[count_small][count_large]>=V135[count_small][count_large] &&
				V45[count_small][count_large]>=V180[count_small][count_large] && V45[count_small][count_large]>=V225[count_small][count_large] && V45[count_small][count_large]>=V270[count_small][count_large] && V45[count_small][count_large]>=V315[count_small][count_large])
			{threshold_high_flag[count_small][count_large]=2;}

			if(V90[count_small][count_large]>=V0[count_small][count_large] && V90[count_small][count_large]>=V45[count_small][count_large] && V90[count_small][count_large]>=V135[count_small][count_large] &&
				V90[count_small][count_large]>=V180[count_small][count_large] && V90[count_small][count_large]>=V225[count_small][count_large] && V90[count_small][count_large]>=V270[count_small][count_large] && V90[count_small][count_large]>=V315[count_small][count_large])
			{threshold_high_flag[count_small][count_large]=3;}

			if(V135[count_small][count_large]>=V0[count_small][count_large] && V135[count_small][count_large]>=V45[count_small][count_large] && V135[count_small][count_large]>=V90[count_small][count_large] &&
				V135[count_small][count_large]>=V180[count_small][count_large] && V135[count_small][count_large]>=V225[count_small][count_large] && V135[count_small][count_large]>=V270[count_small][count_large] && V135[count_small][count_large]>=V315[count_small][count_large])
			{threshold_high_flag[count_small][count_large]=4;}

			if(V180[count_small][count_large]>=V0[count_small][count_large] && V180[count_small][count_large]>=V45[count_small][count_large] && V180[count_small][count_large]>=V90[count_small][count_large] &&
				V180[count_small][count_large]>=V135[count_small][count_large] && V180[count_small][count_large]>=V225[count_small][count_large] && V180[count_small][count_large]>=V270[count_small][count_large] && V180[count_small][count_large]>=V315[count_small][count_large])
			{threshold_high_flag[count_small][count_large]=5;}

			if(V225[count_small][count_large]>=V0[count_small][count_large] && V225[count_small][count_large]>=V45[count_small][count_large] && V225[count_small][count_large]>=V90[count_small][count_large] &&
				V225[count_small][count_large]>=V135[count_small][count_large] && V225[count_small][count_large]>=V180[count_small][count_large] && V225[count_small][count_large]>=V270[count_small][count_large] && V225[count_small][count_large]>=V315[count_small][count_large])
			{threshold_high_flag[count_small][count_large]=6;}

			if(V270[count_small][count_large]>=V0[count_small][count_large] && V270[count_small][count_large]>=V45[count_small][count_large] && V270[count_small][count_large]>=V90[count_small][count_large] &&
				V270[count_small][count_large]>=V135[count_small][count_large] && V270[count_small][count_large]>=V180[count_small][count_large] && V270[count_small][count_large]>=V225[count_small][count_large] && V270[count_small][count_large]>=V315[count_small][count_large])
			{threshold_high_flag[count_small][count_large]=7;}

			if(V315[count_small][count_large]>=V0[count_small][count_large] && V315[count_small][count_large]>=V45[count_small][count_large] && V315[count_small][count_large]>=V90[count_small][count_large] &&
				V315[count_small][count_large]>=V135[count_small][count_large] && V315[count_small][count_large]>=V180[count_small][count_large] && V315[count_small][count_large]>=V225[count_small][count_large] && V315[count_small][count_large]>=V270[count_small][count_large])
			{threshold_high_flag[count_small][count_large]=8;}
//}		//�擾������ω������Ă����ꍇ�A���R�����g

///////////////////////4�����擾��flag(����臒l)/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//if(direction_number==4){		//�擾������ω������Ă����ꍇ�A���R�����g
/*				if(minor_flag==0){
					if(V0[count_small][count_large]>=V90[count_small][count_large] &&	V0[count_small][count_large]>=V180[count_small][count_large] && V0[count_small][count_large]>=V270[count_small][count_large])
						{threshold_high_flag[count_small][count_large]=1;}			//V�̑召��r


					if(V90[count_small][count_large]>=V0[count_small][count_large] &&	V90[count_small][count_large]>=V180[count_small][count_large] && V90[count_small][count_large]>=V270[count_small][count_large])
						{threshold_high_flag[count_small][count_large]=3;}


					if(V180[count_small][count_large]>=V0[count_small][count_large] && V180[count_small][count_large]>=V90[count_small][count_large] &&V180[count_small][count_large]>=V270[count_small][count_large])
						{threshold_high_flag[count_small][count_large]=5;}


					if(V270[count_small][count_large]>=V0[count_small][count_large] && V270[count_small][count_large]>=V90[count_small][count_large] && V270[count_small][count_large]>=V180[count_small][count_large])
						{threshold_high_flag[count_small][count_large]=7;}

				}
				if(minor_flag==1){

							if(V0[count_small][count_large]>=V45[count_small][count_large] && V0[count_small][count_large]>=V90[count_small][count_large] && V0[count_small][count_large]>=V135[count_small][count_large])
								{threshold_high_flag[count_small][count_large]=1;}			//V�̑召��r

							if(V45[count_small][count_large]>=V0[count_small][count_large] && V45[count_small][count_large]>=V90[count_small][count_large] && V45[count_small][count_large]>=V135[count_small][count_large])
								{threshold_high_flag[count_small][count_large]=2;}

							if(V90[count_small][count_large]>=V0[count_small][count_large] && V90[count_small][count_large]>=V45[count_small][count_large] && V90[count_small][count_large]>=V135[count_small][count_large])
								{threshold_high_flag[count_small][count_large]=3;}

							if(V135[count_small][count_large]>=V0[count_small][count_large] && V135[count_small][count_large]>=V45[count_small][count_large] && V135[count_small][count_large]>=V90[count_small][count_large])
								{threshold_high_flag[count_small][count_large]=4;}

				}
			}

*/
///////////////////////2�����擾��flag(����臒l)/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//if(direction_number==2){		//�擾������ω������Ă����ꍇ�A���R�����g

/*					if(minor_flag==0){
						
						if(V0[count_small][count_large]>=V90[count_small][count_large]){
							threshold_high_flag[count_small][count_large]=1;			//V�̑召��r
						}else{
							threshold_high_flag[count_small][count_large]=3;
						}
					}
					if(minor_flag==1){

						if(V0[count_small][count_large]>=V45[count_small][count_large]){
							threshold_high_flag[count_small][count_large]=1;			//V�̑召��r
						}else{
							threshold_high_flag[count_small][count_large]=2;
						}	
					}
				//else{
					//printf("curcuit_number=%d �̃G���[",curcuit_number);
				//}
				}
*/
	}
}
			//count_small++;							//��s�ǂ݂Ƃ����Ƃ��p�̃J�E���g

//////////////臒l���肱���܂�/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////break�������L��//////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*	if(count_small==&image_x){
				count_large++;
				break;
				}
			}
			*/
////////////////////////////�v�Z����//////////////////////////////////////////////////////////////////////////////////////////////
			printf("cos�ގ��x�̌v�Z���n�߂܂�\n");
			for(i=0;i<image_y;++i){	
				for (j = 0; j < image_x; ++j) {

					//cout<<"������"<<endl;
					//if(minor_flag==0){
					if (cos_eco_mode_flag != 1) {

					//�����Ő�����2��臒l������悤�ɉ��C����
					if (threshold_low_flag[j][i] == 1) { threshold_low[j][i] = V0[j][i]; fprintf(fp_threshold, "%lf,", threshold_low[j][i]); }
					if (threshold_low_flag[j][i] == 2) { threshold_low[j][i] = V45[j][i]; fprintf(fp_threshold, "%lf,", threshold_low[j][i]); }
					if (threshold_low_flag[j][i] == 3) { threshold_low[j][i] = V90[j][i]; fprintf(fp_threshold, "%lf,", threshold_low[j][i]); }
					if (threshold_low_flag[j][i] == 4) { threshold_low[j][i] = V135[j][i]; fprintf(fp_threshold, "%lf,", threshold_low[j][i]); }
					if (threshold_low_flag[j][i] == 5) { threshold_low[j][i] = V180[j][i]; fprintf(fp_threshold, "%lf,", threshold_low[j][i]); }
					if (threshold_low_flag[j][i] == 6) { threshold_low[j][i] = V225[j][i]; fprintf(fp_threshold, "%lf,", threshold_low[j][i]); }
					if (threshold_low_flag[j][i] == 7) { threshold_low[j][i] = V270[j][i]; fprintf(fp_threshold, "%lf,", threshold_low[j][i]); }
					if (threshold_low_flag[j][i] == 8) { threshold_low[j][i] = V315[j][i]; fprintf(fp_threshold, "%lf,", threshold_low[j][i]); }

					if (j == image_x - 1) { fprintf(fp_threshold, "\n"); }

					if (threshold_high_flag[j][i] == 1) { threshold_high[j][i] = V0[j][i]; fprintf(fp_threshold_high, "%lf,", threshold_high[j][i]); }
					if (threshold_high_flag[j][i] == 2) { threshold_high[j][i] = V45[j][i]; fprintf(fp_threshold_high, "%lf,", threshold_high[j][i]); }
					if (threshold_high_flag[j][i] == 3) { threshold_high[j][i] = V90[j][i]; fprintf(fp_threshold_high, "%lf,", threshold_high[j][i]); }
					if (threshold_high_flag[j][i] == 4) { threshold_high[j][i] = V135[j][i]; fprintf(fp_threshold_high, "%lf,", threshold_high[j][i]); }
					if (threshold_high_flag[j][i] == 5) { threshold_high[j][i] = V180[j][i]; fprintf(fp_threshold_high, "%lf,", threshold_high[j][i]); }
					if (threshold_high_flag[j][i] == 6) { threshold_high[j][i] = V225[j][i]; fprintf(fp_threshold_high, "%lf,", threshold_high[j][i]); }
					if (threshold_high_flag[j][i] == 7) { threshold_high[j][i] = V270[j][i]; fprintf(fp_threshold_high, "%lf,", threshold_high[j][i]); }
					if (threshold_high_flag[j][i] == 8) { threshold_high[j][i] = V315[j][i]; fprintf(fp_threshold_high, "%lf,", threshold_high[j][i]); }
					if (j == image_x - 1) { fprintf(fp_threshold_high, "\n"); }
					}
					else {
						//�����Ő�����2��臒l������悤�ɉ��C����
						if (threshold_low_flag[j][i] == 1) { threshold_low[j][i] = V0[j][i];}
						if (threshold_low_flag[j][i] == 2) { threshold_low[j][i] = V45[j][i];}
						if (threshold_low_flag[j][i] == 3) { threshold_low[j][i] = V90[j][i];}
						if (threshold_low_flag[j][i] == 4) { threshold_low[j][i] = V135[j][i];}
						if (threshold_low_flag[j][i] == 5) { threshold_low[j][i] = V180[j][i];}
						if (threshold_low_flag[j][i] == 6) { threshold_low[j][i] = V225[j][i];}
						if (threshold_low_flag[j][i] == 7) { threshold_low[j][i] = V270[j][i];}
						if (threshold_low_flag[j][i] == 8) { threshold_low[j][i] = V315[j][i];}

						

						if (threshold_high_flag[j][i] == 1) { threshold_high[j][i] = V0[j][i];}
						if (threshold_high_flag[j][i] == 2) { threshold_high[j][i] = V45[j][i];}
						if (threshold_high_flag[j][i] == 3) { threshold_high[j][i] = V90[j][i];}
						if (threshold_high_flag[j][i] == 4) { threshold_high[j][i] = V135[j][i];}
						if (threshold_high_flag[j][i] == 5) { threshold_high[j][i] = V180[j][i];}
						if (threshold_high_flag[j][i] == 6) { threshold_high[j][i] = V225[j][i];}
						if (threshold_high_flag[j][i] == 7) { threshold_high[j][i] = V270[j][i];}
						if (threshold_high_flag[j][i] == 8) { threshold_high[j][i] = V315[j][i];}
						

					}
					

			use_Rvector_flag = threshold_high_flag[j][i];	//��{�͐���臒l�����

			if(threshold_low<0){
				threshold_low_abs= threshold_low[j][i] *-1;
			}else{
				threshold_low_abs= threshold_low[j][i];
			}
			if(threshold_high<0){
				threshold_high_abs= threshold_high[j][i] *-1;
			}else{
				threshold_high_abs= threshold_high[j][i];
			}

			if(threshold_low_abs>threshold_high_abs){
				use_Rvector_flag = threshold_low_flag[j][i];
				threshold2[j][i]= threshold_low[j][i];
				//fprintf(fp_threshold2,"%lf,",threshold_low);

				//if(minor_flag==1)use_Rvector_flag=threshold_low_flag[j][i]+4;
				//if(direction_number==2)use_Rvector_flag=threshold_low_flag[j][i]+4;
			}
			else{
				threshold2[j][i] = threshold_high[j][i];
				//fprintf(fp_threshold2,"%lf,", threshold_high[j][i]);
			}

			fprintf(fp_threshold2,"%lf,", threshold2[j][i]);
			if(j==image_x-1){fprintf(fp_threshold2,"\n");}

//���90�x���炷
				if(use_Rvector_flag>=3){							//-90����p����
					use_Rvector_number=use_Rvector_flag-2;		//90,135,180,225,270,315�̂Ƃ�
				}else{
					use_Rvector_number=use_Rvector_flag+6;		//0,45�̂Ƃ�
				}

				threshold_local_flag[j][i] = use_Rvector_flag;

//���90�����炳�Ȃ�
			/*
			use_Rvector_number=use_Rvector_flag[j];
			printf("%d",use_Rvector_number);
			printf("j=%d:%d,",j,use_Rvector_flag[j]);
			*/
//////////////�t�@�C���ւ̏�������(fp_use_Rvector_flag,fp_use_Rvector_number)/////////////////////////////////////////////
				if (cos_eco_mode_flag != 1) {
					fprintf(fp_use_Rvector_flag, "%d,", use_Rvector_flag);
					if (j == image_x - 1) { fprintf(fp_use_Rvector_flag, "\n"); }

					fprintf(fp_use_Rvector_number, "%d,", use_Rvector_number);
					if (j == image_x - 1) { fprintf(fp_use_Rvector_number, "\n"); }
				}
/////////////////////////�v�Z//////////////////////////////////////////////////////////////////////////////////////////////
			if(direction_number==8){
				innerp[j][i] = (Rvector[use_Rvector_number][1]*V0[j][i]) + (Rvector[use_Rvector_number][2]*V45[j][i]) + (Rvector[use_Rvector_number][3]*V90[j][i]) + (Rvector[use_Rvector_number][4]*V135[j][i]) 
					+ (Rvector[use_Rvector_number][5]*V180[j][i]) + (Rvector[use_Rvector_number][6]*V225[j][i]) + (Rvector[use_Rvector_number][7]*V270[j][i]) + (Rvector[use_Rvector_number][8]*V315[j][i]);
				V_sqrt[j][i]=sqrt( pow(V0[j][i],2) + pow(V45[j][i],2)  + pow(V90[j][i],2)  + pow(V135[j][i],2)  + pow(V180[j][i],2)  + pow(V225[j][i],2)  + pow(V270[j][i],2)  + pow(V315[j][i],2) );
				}else{
					if(direction_number==4){
						if(minor_flag==0){
						innerp[j][i] = (Rvector[use_Rvector_number][1]*V0[j][i]) + (Rvector[use_Rvector_number][3]*V90[j][i]) + (Rvector[use_Rvector_number][5]*V180[j][i]) + (Rvector[use_Rvector_number][7]*V270[j][i]);
						V_sqrt[j][i]=sqrt( pow(V0[j][i],2) + pow(V90[j][i],2)  + pow(V180[j][i],2)  + pow(V270[j][i],2));
						}
						if(minor_flag==1){
						innerp[j][i] = (Rvector[use_Rvector_number][1]*V0[j][i]) + (Rvector[use_Rvector_number][2]*V45[j][i]) + (Rvector[use_Rvector_number][3]*V90[j][i]) + (Rvector[use_Rvector_number][4]*V135[j][i]);
						V_sqrt[j][i]=sqrt( pow(V0[j][i],2) + pow(V45[j][i],2)  + pow(V90[j][i],2)  + pow(V135[j][i],2));
						}
						}else{
							if(direction_number==2){
								if(minor_flag==0){
								innerp[j][i] = (Rvector[use_Rvector_number][1]*V0[j][i]) + (Rvector[use_Rvector_number][3]*V90[j][i]);
								V_sqrt[j][i]=sqrt( pow(V0[j][i],2) + pow(V90[j][i],2));
								}
								if(minor_flag==1){
								innerp[j][i] = (Rvector[use_Rvector_number][1]*V0[j][i]) + (Rvector[use_Rvector_number][2]*V45[j][i]);
								V_sqrt[j][i]=sqrt( pow(V0[j][i],2) + pow(V45[j][i],2));
								}
								}else{
									cout<<"direction_number�̐ݒ肪���������G���["<<endl;
									return 1;
								}
						}
				}

/////////////////////cos�ގ��x�̌v�Z///////////////////////////////////////////////////////////////////////////////////////////////

			Cos_similarity[j][i] = innerp[j][i] / (Rvector_sqrt[use_Rvector_number] * V_sqrt[j][i]);

			//������炷
			if(use_Rvector_number<7){Angle[j][i] = acos(Cos_similarity[j][i])*180/PI+((use_Rvector_number-1)*45);}
			if(use_Rvector_number>6){Angle[j][i] = acos(Cos_similarity[j][i])*180/PI+((use_Rvector_number-9)*45);}

			//������炳�Ȃ�
			/*
			Angle[j][i] = acos(Cos_similarity[j][i])*180/PI+((use_Rvector_number-1)*45);
			if(Angle[j]-45*(use_Rvector_number-1)>20)Angle[j]=Angle[j]-45*(use_Rvector_number-1);
			Angle[j] = acos(Cos_similarity[j])*180/PI;
			*/

			if(Angle[j][i]<0){
				Angle[j][i] = Angle[j][i] +360;
			}

			if (Rvector_sqrt[use_Rvector_number] == 0 || V_sqrt[j][i] == 0)Angle[j][i] = 334000;

/////////////////////////�v�Z�I���/////////////////////////////////////////////////////////////////////////////////////////////

///////////////////�t�@�C���ւ̏�������(��s���������݁j////////////////////////////////////////////////////////////////////////
			if (cos_eco_mode_flag != 1) {
				fprintf(fp_innerp, "%lf,", innerp[j][i]);
				if (j == image_x - 1) { fprintf(fp_innerp, "\n"); }

				fprintf(fp_V_sqrt, "%lf,", V_sqrt[j][i]);
				if (j == image_x - 1) { fprintf(fp_V_sqrt, "\n"); }

				fprintf(fp_Cos_similarity, "%lf,", Cos_similarity[j][i]);
				if (j == image_x - 1) { fprintf(fp_Cos_similarity, "\n"); }

				
			}

			fprintf(fp_Angle,"%lf,",Angle[j][i]);
			if(j==image_x-1){fprintf(fp_Angle,"\n");}
		
			//fprintf(fp_Correction_flag,"%d,",Correction_flag[j][i]);
			//if(j==image_width){fprintf(fp_Correction_flag,"\n");}

			}
		}

/////////////////////////////�Ǐ��A�����̃`�F�b�N///////////////////////////////////////////////////////
		double **threshold_LC_number = matrix(0, image_x - 1, 0, image_y - 1);
		for (i = 0; i < image_y; i++) {
			for (j = 0; j < image_x; j++) {
				threshold_LC_number[j][i] = 0;
			}
		}
		
		local_connectivity(image_x, image_y, threshold_local_flag, threshold_LC_number);
		
		for (i = 0; i < image_y; ++i) {
			for (j = 0; j < image_x; ++j) {
				fprintf(fp_threshold_number_LC, "%lf,", threshold_LC_number[j][i]);
				if (j == image_x - 1) { fprintf(fp_threshold_number_LC, "\n"); }
			}
		}

		for (i = 0; i < image_y; ++i) {
			for (j = 0; j < image_x; ++j) {
			//	printf("%lf,", threshold_LC_number[j][i]);
				if (threshold_LC_number[j][i] == 0) {
					threshold3[j][i] = 0;
				}
				else {
					threshold3[j][i] = threshold2[j][i];
				}
				fprintf(fp_threshold3, "%lf,", threshold3[j][i]);
				if (j == image_x - 1) { fprintf(fp_threshold3, "\n"); }
			}
		}
		
		
///////////////////////////�������ݏI���/////////////////////////////////////////////////////////			

	//�t�@�C�������
			if (cos_eco_mode_flag != 1) {
				fclose(fp_innerp);
				fclose(fp_V_sqrt);
				fclose(fp_Cos_similarity);
				fclose(fp_threshold_high);
				fclose(fp_threshold);
				fclose(fp_threshold_number_LC);
			}

	fclose(fp_Angle);
	fclose(fp_threshold2);
	fclose(fp_threshold3);

////////////////////////log�t�@�C���̍쐬//////////////////////////////////////////////////////////////////////////
	FILE *fp_date;
	char filename_log[128];

	sprintf(filename_log, "%s\\log.txt",date_directory3);	//log�t�@�C���쐬�̃f�B���N�g���w��
	printf("%s\n",filename_log);	//log�t�@�C���쐬�̃f�B���N�g���w��
	if((fp_date=fopen(filename_log,"w"))==NULL){printf("log�t�@�C�����J���܂���");exit(1);}
	fprintf(fp_date,"Time       : %s\n",date);						//����
	fprintf(fp_date,"direction  : %d\n",direction_number);			//����
	fprintf(fp_date,"curcuit    : %d\n",curcuit_number);			//�g�p��H
	//fprintf(fp_date,"property   : %s\n",inputdate_directory1);	//proprety�t�@�C��
	fprintf(fp_date,"Rvector    : %d\n",Rvector_number);			//��x�N�g���̐�
	fprintf(fp_date,"minor_flag : %d\n",minor_flag);				//�}�C�i�[�i���o�[
	fprintf(fp_date,"Rvector    : %s\n",inputrvector_directory);	//�g�p����Rvector�f�[�^
	for(i=1;i<=8;i++){for(j=1;j<=8;j++){fprintf(fp_date,"%lf, ",Rvector[i][j]);}fprintf(fp_date,"\n");}	//�g�p����Rvector
	fprintf(fp_date,"Rvector *	: %f,%f,%f,%f,%f,%f,%f,%f\n",Rvectormagni[1],Rvectormagni[2],Rvectormagni[3],Rvectormagni[4],Rvectormagni[5],Rvectormagni[6],Rvectormagni[7],Rvectormagni[8]);	//�g�p����Rvector�̔{��
	fprintf(fp_date,"�g�p�f�[�^ : %s\n",inputdate_directory);		//�g�p������ݍ��ݍς݃f�[�^
	fprintf(fp_date,"�ۑ���     : %s\n",date_directory3);			//�ۑ���
	fclose(fp_date);

	printf("direction  : %d\n",direction_number);					//����
	printf("curcuit	   : %d\n",curcuit_number);						//�g�p��H
	printf("Rvector	   : %d\n",Rvector_number);						//��x�N�g���̐�
	printf("minor_flag : %d\n",minor_flag);							//�}�C�i�[�i���o�[
	printf("Rvector    : %s\n",inputrvector_directory);				//�g�p����Rvector�f�[�^
	printf("�g�p�f�[�^ : %s��cos�ގ��x�v�Z���I�����܂���\n",inputdate_directory);				//�g�p������ݍ��ݍς݃f�[�^


	//�������̊J��
	free_matrix(threshold_LC_number, 0, image_x - 1, 0, image_y - 1);
	free_matrix(threshold2, 0, image_x - 1, 0, image_y - 1);
	free_matrix(threshold3, 0, image_x - 1, 0, image_y - 1);
	free_matrix(threshold_high, 0, image_x - 1, 0, image_y - 1);
	free_matrix(threshold_low, 0, image_x - 1, 0, image_y - 1);
	free_matrix(V0, 0, image_x-1, 0, image_y-1);
	free_matrix(V45,0, image_x-1, 0, image_y-1);
	free_matrix(V90, 0, image_x-1, 0, image_y-1);
	free_matrix(V135, 0, image_x-1, 0, image_y-1);
	free_matrix(V180, 0, image_x-1, 0, image_y-1);
	free_matrix(V225,0, image_x-1, 0, image_y-1);
	free_matrix(V270, 0, image_x-1, 0, image_y-1);
	free_matrix(V315, 0, image_x-1, 0, image_y-1);
	free_matrix(threshold_low_flag,0, image_x-1, 0, image_y-1);
	free_matrix(threshold_high_flag, 0, image_x-1, 0, image_y-1);
	free_matrix(V_sqrt, 0, image_x-1, 0, image_y-1);
	free_matrix(Cos_similarity,0, image_x-1, 0, image_y-1);
	free_matrix(Angle, 0, image_x-1, 0, image_y-1);
	free_matrix(innerp, 0, image_x-1, 0, image_y-1);

	return 0;
}


void Read_output(){
///////////////////Output�t�@�C���̃f�B���N�g���ݒ�////////////////////////////////////////////////
	sprintf(math_name1,"%s\\%s",date_directory3, math_name1_s);
	sprintf(math_name2,"%s\\%s",date_directory3, math_name2_s);
	sprintf(math_name3,"%s\\%s",date_directory3, math_name3_s);
	sprintf(math_name4,"%s\\%s",date_directory3, math_name4_s);
	sprintf(math_name5,"%s\\%s",date_directory3, math_name5_s);
	sprintf(math_name6,"%s\\%s",date_directory3, math_name6_s);
	sprintf(math_name7,"%s\\%s",date_directory3, math_name7_s);
	sprintf(math_name8,"%s\\%s",date_directory3, math_name8_s);
	sprintf(math_name9,"%s\\%s",date_directory3, math_name9_s);
	sprintf(math_name10, "%s\\%s", date_directory3, math_name10_s);
	sprintf(math_name11, "%s\\%s", date_directory3, math_name11_s);

	//�t�@�C���I�[�v���y�ъm�F
	if (cos_eco_mode_flag != 1) {
		if ((fp_innerp = fopen(math_name1, "w")) == NULL) { printf("���̓G���[ innerp.csv���J���܂���\nFile_name : %s", math_name1); exit(1); }
		if ((fp_V_sqrt = fopen(math_name2, "w")) == NULL) { printf("���̓G���[ V_sqrt.csv���J���܂���\nFile_name : %s", math_name2); exit(1); }
		if ((fp_Cos_similarity = fopen(math_name3, "w")) == NULL) { printf("���̓G���[ Cos_similarity.csv���J���܂���\nFile_name : %s", math_name3); exit(1); }
		
		if ((fp_threshold_high = fopen(math_name5, "w")) == NULL) { printf("���̓G���[ threshold_high.csv���J���܂���\nFile_name : %s", math_name5); exit(1); }
		if ((fp_use_Rvector_flag = fopen(math_name6, "w")) == NULL) { printf("���̓G���[ use_Rvector_flag.csv���J���܂���\nFile_name : %s", math_name6); exit(1); }
		if ((fp_use_Rvector_number = fopen(math_name7, "w")) == NULL) { printf("���̓G���[ use_Rvector_number.csv���J���܂���\nFile_name : %s", math_name7); exit(1); }
		
		if ((fp_threshold = fopen(math_name9, "w")) == NULL) { printf("���̓G���[ threshold.csv���J���܂���\nFile_name : %s", math_name9); exit(1); }
		if ((fp_threshold_number_LC = fopen(math_name11, "w")) == NULL) { printf("���̓G���[ threshold_number_LC.csv���J���܂���\nFile_name : %s", math_name11); exit(1); }
		
	}
	if ((fp_Angle = fopen(math_name4, "w")) == NULL) { printf("���̓G���[ Angle.csv���J���܂���\nFile_name : %s", math_name4); exit(1); }
	if ((fp_threshold2 = fopen(math_name8, "w")) == NULL) { printf("���̓G���[ threshold2.csv���J���܂���\nFile_name : %s", math_name8); exit(1); }
	if ((fp_threshold3 = fopen(math_name10, "w")) == NULL) { printf("���̓G���[ threshold3.csv���J���܂���\nFile_name : %s", math_name10); exit(1); }

}

void Rvector_size(){
//////////////////////Rvector�̑傫�������߂Ă���(cos�ɗp����)//////////////////////////////////////////////////////////////////////////////////////////////////////////////
   if(direction_number==8){
	   for(i=1;i<=8;i++){
		Rvector_square_sum[i]=Rvector_square[i][1]+Rvector_square[i][2]+Rvector_square[i][3]+Rvector_square[i][4]+Rvector_square[i][5]+Rvector_square[i][6]+Rvector_square[i][7]+Rvector_square[i][8];
	   }
   }else{
	   if(direction_number==4){
		   for(i=1;i<=8;i++){
		   if(minor_flag==0)Rvector_square_sum[i]=Rvector_square[i][1]+Rvector_square[i][3]+Rvector_square[i][5]+Rvector_square[i][7];
		   if(minor_flag==1)Rvector_square_sum[i]=Rvector_square[i][1]+Rvector_square[i][2]+Rvector_square[i][3]+Rvector_square[i][4];//����
		   }
	   }else{
		   if(direction_number==2){
			   for(i=1;i<=8;i++){
			    if(minor_flag==0)Rvector_square_sum[i]=Rvector_square[i][1]+Rvector_square[i][3];
				if(minor_flag==1)Rvector_square_sum[i]=Rvector_square[i][1]+Rvector_square[i][2];
			   }
		   }else{
			   cout<<"direction_number�̐ݒ肪���������G���["<<endl;
			   exit(1);
		   }
	   }
   }
   for(i=1;i<=8;i++){
		Rvector_sqrt[i]=sqrt(Rvector_square_sum[i]);
   }
}


void Rvector_read(){
	/////////////////��x�N�g���̓ǂݍ���/////////////////////////////////////////////
	
	sprintf(Input_Rvectormagni_name,"%s%s",inputrvector_directory,Input_Rvectormagniname_s);
	printf("%s\n",Input_Rvectormagni_name);
	ifstream Rvector_magni;
	Rvector_magni.open(Input_Rvectormagni_name,ios::in);

	if(!Rvector_magni){cout<<"���̓G���[ Rvector_magni.csv������܂���";exit(1);}
	i=1,j=1;

	string str_Rvector_magni;
	while (getline(Rvector_magni,str_Rvector_magni)){
		string token_Rvector_magni;
		istringstream stream(str_Rvector_magni);
		while(getline(stream,token_Rvector_magni,',')){
			double temp_Rvector_magni=stof(token_Rvector_magni); //stof(string str) : string��float�ɕϊ�
			Rvectormagni[i]=temp_Rvector_magni;
			i++;
		}
	}

	//Rvector
	sprintf(Input_Rvectorname1,"%s%s",inputrvector_directory,Input_Rvectorname1_s);
	sprintf(Input_Rvectorname2,"%s%s",inputrvector_directory,Input_Rvectorname2_s);
	sprintf(Input_Rvectorname3,"%s%s",inputrvector_directory,Input_Rvectorname3_s);
	sprintf(Input_Rvectorname4,"%s%s",inputrvector_directory,Input_Rvectorname4_s);
	sprintf(Input_Rvectorname5,"%s%s",inputrvector_directory,Input_Rvectorname5_s);
	sprintf(Input_Rvectorname6,"%s%s",inputrvector_directory,Input_Rvectorname6_s);
	sprintf(Input_Rvectorname7,"%s%s",inputrvector_directory,Input_Rvectorname7_s);
	sprintf(Input_Rvectorname8,"%s%s",inputrvector_directory,Input_Rvectorname8_s);

	//printf("%s\n",Input_Rvectorname1);
	//Rvector

	printf("inputrvector_directory=%s,\nInput_Rvectorname4_s=%s\n",inputrvector_directory,Input_Rvectorname4_s);
	printf("Input_Rvectorname4=%s\n",Input_Rvectorname4);
	
	ifstream Rvector_0(Input_Rvectorname1);
	ifstream Rvector_45(Input_Rvectorname2);
	ifstream Rvector_90(Input_Rvectorname3);
	ifstream Rvector_135(Input_Rvectorname4);
	ifstream Rvector_180(Input_Rvectorname5);
	ifstream Rvector_225(Input_Rvectorname6);
	ifstream Rvector_270(Input_Rvectorname7);
	ifstream Rvector_315(Input_Rvectorname8);

	printf("R0:%s\nR45:%s\nR90:%s\nR135:%s\nR180:%s\nR225:%s\nR270:%s\nR315:%s\n",Input_Rvectorname1,Input_Rvectorname2,Input_Rvectorname3,Input_Rvectorname4,Input_Rvectorname5,Input_Rvectorname6,Input_Rvectorname7,Input_Rvectorname8);

	//if(!Rvector_magni){cout<<"���̓G���[ Rvector_magni.csv������܂���";exit(1);}
    if(!Rvector_0){cout<<"���̓G���[ Rvector0.csv������܂���";exit(1);}
	if(!Rvector_45){cout<<"���̓G���[ Rvector45.csv������܂���";exit(1);}
	if(!Rvector_90){cout<<"���̓G���[ Rvector90.csv������܂���";exit(1);}
	if(!Rvector_135){cout<<"���̓G���[ Rvector135.csv������܂���";exit(1);}
	if(!Rvector_180){cout<<"���̓G���[ Rvector180.csv������܂���";exit(1);}
	if(!Rvector_225){cout<<"���̓G���[ Rvector225.csv������܂���";exit(1);}
	if(!Rvector_270){cout<<"���̓G���[ Rvector270.csv������܂���";exit(1);}
	if(!Rvector_315){cout<<"���̓G���[ Rvector315.csv������܂���";exit(1);}

	i=1,j=1;
	string str_Rvector0,str_Rvector45,str_Rvector90,str_Rvector135,str_Rvector180,str_Rvector225,str_Rvector270,str_Rvector315;
	while(getline(Rvector_0,str_Rvector0)){
        string token_Rvector0;
        istringstream stream(str_Rvector0);

		getline(Rvector_45,str_Rvector45);	string token_Rvector45;	istringstream stream_Rvector45(str_Rvector45);
		getline(Rvector_90,str_Rvector90);	string token_Rvector90;	istringstream stream_Rvector90(str_Rvector90);
		getline(Rvector_135,str_Rvector135);	string token_Rvector135;	istringstream stream_Rvector135(str_Rvector135);
		getline(Rvector_180,str_Rvector180);	string token_Rvector180;	istringstream stream_Rvector180(str_Rvector180);
		getline(Rvector_225,str_Rvector225);	string token_Rvector225;	istringstream stream_Rvector225(str_Rvector225);
		getline(Rvector_270,str_Rvector270);	string token_Rvector270;	istringstream stream_Rvector270(str_Rvector270);
		getline(Rvector_315,str_Rvector315);	string token_Rvector315;	istringstream stream_Rvector315(str_Rvector315);

        //1�s�̂����A������ƃR���}�𕪊�����
        while(getline(stream,token_Rvector0,',')){
            //���ׂĕ�����Ƃ��ēǂݍ��܂�邽��
            //���l�͕ϊ����K�v
            double temp_Rvecrot0=stof(token_Rvector0);	//stof(string str) : string��float�ɕϊ�
			Rvector[i][j]=temp_Rvecrot0;				//0�x���͉摜�̎��̒l��ǂ�
			Rvector[i][j]=Rvectormagni[j]*Rvector[i][j];//�{����������
			Rvector_square[i][j]=pow(Rvector[i][j],2);	//��x�N�g����2�{�����߂�
			i++;										//i�͓��͂����摜�̊p�x��ԍ��ŁCj�͎擾�ʒu������

			getline(stream_Rvector45,token_Rvector45,',');
			double tmp_Rvector45=stof(token_Rvector45);
			Rvector[i][j]=tmp_Rvector45;
			Rvector[i][j]=Rvectormagni[j]*Rvector[i][j];
			Rvector_square[i][j]=pow(Rvector[i][j],2);
			i++;
        
			getline(stream_Rvector90,token_Rvector90,',');
			double tmp_Rvector90=stof(token_Rvector90);
			Rvector[i][j]=tmp_Rvector90;
			Rvector[i][j]=Rvectormagni[j]*Rvector[i][j];
			Rvector_square[i][j]=pow(Rvector[i][j],2);
			i++;

			getline(stream_Rvector135,token_Rvector135,',');
			double tmp_Rvector135=stof(token_Rvector135);
			Rvector[i][j]=tmp_Rvector135;
			Rvector[i][j]=Rvectormagni[j]*Rvector[i][j];
			Rvector_square[i][j]=pow(Rvector[i][j],2);
			i++;

			getline(stream_Rvector180,token_Rvector180,',');
			double tmp_Rvector180=stof(token_Rvector180);
			Rvector[i][j]=tmp_Rvector180;
			Rvector[i][j]=Rvectormagni[j]*Rvector[i][j];
			Rvector_square[i][j]=pow(Rvector[i][j],2);
			i++;

			getline(stream_Rvector225,token_Rvector225,',');
			double tmp_Rvector225=stof(token_Rvector225);
			Rvector[i][j]=tmp_Rvector225;
			Rvector[i][j]=Rvectormagni[j]*Rvector[i][j];
			Rvector_square[i][j]=pow(Rvector[i][j],2);
			i++;

			getline(stream_Rvector270,token_Rvector270,',');
			double tmp_Rvector270=stof(token_Rvector270);
			Rvector[i][j]=tmp_Rvector270;
			Rvector[i][j]=Rvectormagni[j]*Rvector[i][j];
			Rvector_square[i][j]=pow(Rvector[i][j],2);
			i++;

			getline(stream_Rvector315,token_Rvector315,',');
			double tmp_Rvector315=stof(token_Rvector315);
			Rvector[i][j]=tmp_Rvector315;
			Rvector[i][j]=Rvectormagni[j]*Rvector[i][j];
			Rvector_square[i][j]=pow(Rvector[i][j],2);
			i=1;
			j++;
           
        }
	}

   Rvector_0.close();
   Rvector_45.close();
   Rvector_90.close();
   Rvector_135.close();
   Rvector_180.close();
   Rvector_225.close();
   Rvector_270.close();
   Rvector_315.close();
   Rvector_magni.close();
 
	Rvector_size();
}

int local_connectivity(int image_x, int image_y, double *local_flag[],double *threshold_LC_number[]) {

	//Nrutil��p�����������̊m��
	double **threshold_LC_flag = matrix(0, image_x - 1, 0, image_y - 1);
	for (i = 0; i < image_y; i++) {
		for (j = 0; j < image_x; j++) {
			threshold_LC_flag[j][i] = 0;
		}
	}

	//4�ӂ�0�Ƃ���
	for (i = 1; i < image_y - 1; i++) {
		for (j = 1; j < image_x - 1; j++) {

			//�����̑������@�Ɍq���邱�Ƃ��l����
			//�G�b�W�����Ƃɗאڂ���������Ɓ}45���ȓ��ɑ��݂��Ȃ��ꍇ�Cflag��0��threshold3=0�Ƃ���
			//0���̎�
			if (local_flag[j][i] == 1) {
				if (local_flag[j][i - 1]>local_flag[j][i] + 1 && local_flag[j][i - 1] < local_flag[j][i] + 7 && local_flag[j][i + 1]>local_flag[j][i] + 1 &&  local_flag[j][i + 1]< local_flag[j][i] + 7) {
					threshold_LC_flag[j][i] = 0;
				}else {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
			}

			//45��,225���̎�
			if (local_flag[j][i] == 2 || local_flag[j][i] == 6) {
				if (local_flag[j - 1][i - 1] >= local_flag[j][i] - 1 && local_flag[j - 1][i - 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else if (local_flag[j + 1][i + 1] >= local_flag[j][i] - 1 && local_flag[j + 1][i + 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else {
					threshold_LC_flag[j][i] = 0;
				}
			}

			//90��,270���̎�
			if (local_flag[j][i] == 3 || local_flag[j][i] == 7) {
				if (local_flag[j - 1][i] >= local_flag[j][i] - 1 && local_flag[j - 1][i] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else if (local_flag[j + 1][i] >= local_flag[j][i] - 1 && local_flag[j + 1][i] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else {
					threshold_LC_flag[j][i] = 0;
				}
			}

			//135���̎�
			if (local_flag[j][i] == 4) {
				if (local_flag[j + 1][i - 1] >= local_flag[j][i] - 1 && local_flag[j + 1][i - 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else if (local_flag[j - 1][i + 1] >= local_flag[j][i] - 1 && local_flag[j - 1][i + 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else {
					threshold_LC_flag[j][i] = 0;
				}
			}
			
			//180���̎�
			if (local_flag[j][i] == 5) {
				if (local_flag[j][i - 1] >= local_flag[j][i] - 1 && local_flag[j][i - 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else if (local_flag[j][i + 1] >= local_flag[j][i] - 1 && local_flag[j][i + 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else {
					threshold_LC_flag[j][i] = 0;
				}
			}
			
			//315���̎�
			if (local_flag[j][i] == 8) {
				if (local_flag[j + 1][i - 1]>local_flag[j][i] - 7 && local_flag[j + 1][i - 1]  < local_flag[j][i] - 1 && local_flag[j - 1][i + 1]>local_flag[j][i] - 7 && local_flag[j - 1][i + 1]< local_flag[j][i] - 1) {
					threshold_LC_flag[j][i] = 0;
				}else {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
			}

/*
			//�����̒��������Ɍq���邱�Ƃ��l����
			//�G�b�W�����Ƃɗאڂ���������Ɓ}45���ȓ��ɑ��݂��Ȃ��ꍇ�Cflag��0��threshold3=0�Ƃ���
			//0���̎�
			if (local_flag[j][i] == 1) {
				if (local_flag[j - 1][i]>local_flag[j][i] + 1 && local_flag[j - 1][i] < local_flag[j][i] + 7 && local_flag[j + 1][i]>local_flag[j][i] + 1 && local_flag[j + 1][i]< local_flag[j][i] + 7) {
					threshold_LC_flag[j][i] = 0;
				}
				else {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
			}
			//45��,225���̎�
			if (local_flag[j][i] == 2 || local_flag[j][i] == 6) {
				if (local_flag[j + 1][i - 1] >= local_flag[j][i] - 1 && local_flag[j + 1][i - 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else if (local_flag[j - 1][i + 1] >= local_flag[j][i] - 1 && local_flag[j - 1][i + 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else {
					threshold_LC_flag[j][i] = 0;
				}
			}
			//90��,270���̎�
			if (local_flag[j][i] == 3 || local_flag[j][i] == 7) {
				if (local_flag[j][i - 1] >= local_flag[j][i] - 1 && local_flag[j][i - 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else if (local_flag[j][i + 1] >= local_flag[j][i] - 1 && local_flag[j][i + 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else {
					threshold_LC_flag[j][i] = 0;
				}
			}
			//135���̎�
			if (local_flag[j][i] == 4) {
				if (local_flag[j - 1][i - 1] >= local_flag[j][i] - 1 && local_flag[j - 1][i - 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else if (local_flag[j + 1][i + 1] >= local_flag[j][i] - 1 && local_flag[j + 1][i + 1] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else {
					threshold_LC_flag[j][i] = 0;
				}
			}
			//180���̎�
			if (local_flag[j][i] == 5) {
				if (local_flag[j - 1][i] >= local_flag[j][i] - 1 && local_flag[j - 1][i] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else if (local_flag[j + 1][i] >= local_flag[j][i] - 1 && local_flag[j + 1][i] <= local_flag[j][i] + 1) {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
				else {
					threshold_LC_flag[j][i] = 0;
				}
			}
			//315���̎�
			if (local_flag[j][i] == 8) {
				if (local_flag[j - 1][i - 1]>local_flag[j][i] - 7 && local_flag[j - 1][i - 1]  < local_flag[j][i] - 1 && local_flag[j + 1][i + 1]>local_flag[j][i] - 7 && local_flag[j + 1][i + 1]< local_flag[j][i] - 1) {
					threshold_LC_flag[j][i] = 0;
				}
				else {
					threshold_LC_flag[j][i] = local_flag[j][i];
				}
			}

			//45�����炷
			if (threshold_LC_flag[j][i] != 0) {
				if (threshold_LC_flag[j][i] >= 3) {							//-90����p����
					threshold_LC_number[j][i] = threshold_LC_flag[j][i] - 2;		//90,135,180,225,270,315�̂Ƃ�
				}else {
					threshold_LC_number[j][i] = threshold_LC_flag[j][i] + 6;		//0,45�̂Ƃ�
				}
			}
*/
		}
	}

	free_matrix(threshold_LC_flag, 0, image_x - 1, 0, image_y - 1);
	return **threshold_LC_number;
}
 