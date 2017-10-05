#include<fstream>
#include<iostream>
#include<string>
#include<sstream> //�����X�g���[��
#include<math.h>
#include<time.h>//���Ԃ�p����
#include <direct.h>//�t�H���_���쐬��
#include<stdio.h>

int atan_eco_mode_flag = 1;

//�������m�ۂ��s�����߂̃w�b�_
#define ANSI				
#include "nrutil.h"	

#define PI 3.14159265

using namespace std;

int arctan(char date_directory[], int &image_x, int &image_y,int paramerter[],int paramerter_count,int sd,char date[]){

	char date_directory4[128];

	char *Input_Filename_atan1_s = "\\V(0).csv";			//���̓t�@�C�����̎w��
	char *Input_Filename_atan3_s = "\\V(90).csv";
	char inputdate_directory_atan[128];

	char *math_atan1_s = "\\arctan.csv";				//�o�͌��ʂ̃t�@�C�����̎w��
	char *math_atan5_s = "\\threshold_atan_high.csv";
	char *math_atan8_s = "\\threshold2.csv";
	char *math_atan9_s = "\\threshold_atan_low.csv";

	char Input_Filename_atan1[255];						//���̓t�@�C�����E���͌��̐ݒ�
	char Input_Filename_atan3[255];

	char math_atan1[128];							//����						
	char math_atan5[128];							//threshold_atan_high
	char math_atan8[128];							//2��臒l�����
	char math_atan9[128];							//threshold(use_Rvector_flag�̉����d��V�̑傫���j

	double threshold_atan_high=0;
	double threshold_atan_low=0;
	double threshold_atan_low_abs;
	double threshold_atan_high_abs;

	FILE *fp_arctan, *fp_threshold_atan_low, *fp_threshold_atan_high, *fp_atan_threshold2;

	printf("****************************************\n");
	printf("start�F atan\n");
	printf("****************************************\n");
	int i = 0, j = 0;
//////////////////��`///////////////////////////////////////////////////////////////////////////////////////////

//Nrutil��p�����������̊m��
	double **V0 = matrix(0, image_x - 1, 0, image_y - 1);
	double **V90 = matrix(0, image_x - 1, 0, image_y - 1);
	double **threshold_atan_high_flag = matrix(0, image_x - 1, 0, image_y - 1);
	double **threshold_atan_low_flag = matrix(0, image_x - 1, 0, image_y - 1);
	double **Angle = matrix(0, image_x - 1, 0, image_y - 1);
	
	for (i = 0; i < image_y; i++) {
		for (j = 0; j < image_x; j++) {
			V0[j][i] = 0;
			V90[j][i] = 0;
			threshold_atan_high_flag[j][i] = 0;
			threshold_atan_low_flag[j][i] = 0;
			Angle[j][i] = 0;
		}
	}
	
////////////////////////////���o�̓f�B���N�g���̍쐬//////////////////////////////////////////////////////////////////////////////
	int count_small = 0, count_large = 0;

	switch (paramerter[0]) {
		case 1: 
			sprintf(inputdate_directory_atan, "%s%d�~%dsobel_conv_sd%d", date_directory, paramerter[paramerter_count], paramerter[paramerter_count], sd);
			break;
		case 2:
			sprintf(inputdate_directory_atan, "%s%d�~%dsobel_conv_sd%d", date_directory, paramerter[paramerter_count], paramerter[paramerter_count], sd);
			break;
		case 3:
			sprintf(Input_Filename_atan1_s, "\\V(0)t.csv");			//���̓t�@�C�����̎w��
			sprintf(Input_Filename_atan3_s, "\\V(90)t.csv");
			sprintf(inputdate_directory_atan, "%s%d�~%dsobel_conv_sd%d", date_directory, paramerter[paramerter_count], paramerter[paramerter_count], sd);
			break;
		case 4:
			sprintf(Input_Filename_atan1_s, "\\V(0)t.csv");			//���̓t�@�C�����̎w��
			sprintf(Input_Filename_atan3_s, "\\V(90)t.csv");
			sprintf(inputdate_directory_atan, "%s%d�~%dsobel_conv_sd%d", date_directory, paramerter[paramerter_count], paramerter[paramerter_count], sd);
			break;
		case 5:
			sprintf(Input_Filename_atan1_s, "\\V(0)t.csv");			//���̓t�@�C�����̎w��
			sprintf(Input_Filename_atan3_s, "\\V(90)t.csv");
			sprintf(inputdate_directory_atan, "%s%d�~%dsobel_conv_sd%d", date_directory, paramerter[paramerter_count], paramerter[paramerter_count], sd);
			break;
		default :
			sprintf(inputdate_directory_atan, "%s%dk_conv_sd%d", date_directory, paramerter[paramerter_count], sd);
			break;
	}
	
	//Input�t�@�C���̃f�B���N�g���ݒ�
	printf("%s\n", inputdate_directory_atan);
	sprintf(Input_Filename_atan1, "%s%s", inputdate_directory_atan, Input_Filename_atan1_s);	
	sprintf(Input_Filename_atan3, "%s%s", inputdate_directory_atan, Input_Filename_atan3_s);

	printf("Input_Filename_atan1=%s\n", Input_Filename_atan1);

	//Output�f�B���N�g���̍쐬
	switch (paramerter[0]) {
	case 1:sprintf(date_directory4, "%s%d�~%dsobel_atan_sd%d", date_directory, paramerter[paramerter_count], paramerter[paramerter_count], sd); break;
	case 2:sprintf(date_directory4, "%s%d�~%dsobel_atan_sd%d", date_directory, paramerter[paramerter_count], paramerter[paramerter_count], sd); break;
	case 3:sprintf(date_directory4, "%s%dk_atan_sd%dt", date_directory, paramerter[paramerter_count], sd); break;
	case 4:sprintf(date_directory4, "%s%dk_atan_sd%dt", date_directory, paramerter[paramerter_count], sd); break;
	case 5:sprintf(date_directory4, "%s%dk_atan_sd%dt", date_directory, paramerter[paramerter_count], sd); break;
	default:sprintf(date_directory4, "%s%dk_atan_sd%d", date_directory, paramerter[paramerter_count], sd); break;
	}
	
	if (_mkdir(date_directory4) == 0) {
		printf("�t�H���_ %s ���쐬���܂���\n", date_directory4);
	}
	else {
		printf("�t�H���_�쐬�Ɏ��s���܂����B\n");
	}

	//Output�t�@�C���̃f�B���N�g���ݒ�
	sprintf(math_atan1, "%s%s", date_directory4, math_atan1_s);
	sprintf(math_atan5, "%s%s", date_directory4, math_atan5_s);
	sprintf(math_atan8, "%s%s", date_directory4, math_atan8_s);
	sprintf(math_atan9, "%s%s", date_directory4, math_atan9_s);

////////////////////////////�t�@�C���̓ǂݍ���//////////////////////////////////////////////////////////////////////////////

	//Input�t�@�C�����J��
	ifstream V_0(Input_Filename_atan1);
	ifstream V_90(Input_Filename_atan3);

////////////////////////�G���[�o��/////////////////////////////////////////////////////////////////////////////////////////////

	if (!V_0) { cout << "���̓G���[ V(0).csv������܂���"; return 1; }
	if (!V_90) { cout << "���̓G���[ V(90).csv������܂���"; return 1; }

/////////////////////////�o�̓t�@�C�����J��///////////////////////////////////////////////////////////////////////////////////
	if ((fp_arctan = fopen(math_atan1, "w")) == NULL) {printf("�o�̓t�@�C�� : arctan.csv���J���܂���\n�o�̓t�@�C���f�B���N�g���F%s\n",math_atan1); exit(1); }
	if ((fp_atan_threshold2 = fopen(math_atan8, "w")) == NULL) {printf("�o�̓t�@�C�� : threshold2.csv���J���܂���\n�o�̓t�@�C���f�B���N�g���F%s\n",math_atan8); exit(1); }

	if (atan_eco_mode_flag != 1) {
		if ((fp_threshold_atan_high = fopen(math_atan5, "w")) == NULL) { printf("�o�̓t�@�C�� : threshold_atan_high.csv���J���܂���\n�o�̓t�@�C���f�B���N�g���F%s\n", math_atan5); exit(1); }
		if ((fp_threshold_atan_low = fopen(math_atan9, "w")) == NULL) { printf("�o�̓t�@�C�� : threshold.csv���J���܂���\n�o�̓t�@�C���f�B���N�g���F%s\n", math_atan9); exit(1); }
	}

///////////////////////�����d����csv�̓ǂݍ���//////////////////////////////////////////////////////////////////////////////////////////
	i = 0;
	string str_0, str_90;
	count_large = 0;			//������
	while (getline(V_0, str_0)) {					//���̃��[�v���ł��ׂĂ��s��
		count_small = 0;		//������

		//���낢���`�D
		string token_V_0;
		istringstream stream_V_0(str_0);

		getline(V_90, str_90);	string token_V_90;	istringstream stream_V_90(str_90);

		//�z��ɑ��
		while (getline(stream_V_0, token_V_0, ',')) {	 			//��s�ǂݎ��DV0�̂݁C�J��Ԃ��͈͎̔w��ɗp����

			double tmp_V_0 = stof(token_V_0);//�����𐔎��ɕϊ� ���������ҁH
			V0[count_small][count_large] = tmp_V_0;				//�z��ɑ��

			getline(stream_V_90, token_V_90, ',');
			double tmp_V_90 = stof(token_V_90);
			V90[count_small][count_large] = tmp_V_90;

			++count_small;
		}
		++count_large;
	}

////////////////////�����d���̑召����(臒l��ݒ肷�邽��)///////////////////////////////////
	for (count_small = 0; count_small < image_x; ++count_small) {
		for (count_large = 0; count_large < image_y; ++count_large) {

			for (count_small = 0; count_small < image_x; ++count_small) {
				for (count_large = 0; count_large < image_y; ++count_large) {
					if (V0[count_small][count_large] >= V90[count_small][count_large]) {
						threshold_atan_high_flag[count_small][count_large] = 1;			//V�̑召��r
						threshold_atan_low_flag[count_small][count_large] = 3;
					}
					else {
						threshold_atan_high_flag[count_small][count_large] = 3;
						threshold_atan_low_flag[count_small][count_large] = 1;
					}

				}
			}
		}
	}

	////////////////////////////�v�Z����//////////////////////////////////////////////////////////////////////////////////////////////
	for (i = 0; i < image_y; ++i) {
		for (j = 0; j < image_x; ++j) {						//��s���v�Z����
			if (V0[j][i] == 0) {
				Angle[j][i] = 90;
			} else {
				Angle[j][i] = atan(V90[j][i] / V0[j][i]) * 180 / PI;
			}

			double Angle_flag;
			Angle_flag = Angle[j][i];
			if (Angle_flag < 0)Angle[j][i] = Angle[j][i] + 360;

			if (V0[j][i] < 0 && V90[j][i]>0)Angle[j][i] = Angle[j][i] - 180;
			if (V0[j][i] <= 0 && V90[j][i] <= 0)Angle[j][i] = Angle[j][i] + 180;

			if (V0[j][i] > 0 && V90[j][i] < 0) {
				if (Angle[j][i] < 20) {
					if (Angle[j][i] < 0)Angle[j][i] = Angle[j][i] * -1;
					Angle[j][i] = 360 - Angle[j][i];
				}
			}

			if (V0[j][i] >= 0 && V90[j][i] >= 0) {
				if (Angle[j][i] > 200) {
					Angle[j][i] = 360 - Angle[j][i];
				}
			}

			if (Angle[j][i] < 0)Angle[j][i] = Angle[j][i] + 360;	//�}�C�i�X�̊p�x�����݂��Ȃ��悤�ɂ��邽��
/////////////////////////�v�Z�I���/////////////////////////////////////////////////////////////////////////////////////////////

//////////////�t�@�C���ւ̏�������/////////////////////////////////////////////
			//�p�x
			fprintf(fp_arctan, "%lf,", Angle[j][i]);
			if (j == image_x - 1) { fprintf(fp_arctan, "\n"); }

			if (atan_eco_mode_flag != 1) {
				//���臒l
				if (threshold_atan_high_flag[j][i] == 1) { threshold_atan_high = V0[j][i]; fprintf(fp_threshold_atan_high, "%lf,", threshold_atan_high); }
				if (threshold_atan_high_flag[j][i] == 3) { threshold_atan_high = V90[j][i]; fprintf(fp_threshold_atan_high, "%lf,", threshold_atan_high); }
				if (j == image_x - 1) { fprintf(fp_threshold_atan_high, "\n"); }

				//����臒l
				if (threshold_atan_low_flag[j][i] == 1) { threshold_atan_low = V0[j][i]; fprintf(fp_threshold_atan_low, "%lf,", threshold_atan_low); }
				if (threshold_atan_low_flag[j][i] == 3) { threshold_atan_low = V90[j][i]; fprintf(fp_threshold_atan_low, "%lf,", threshold_atan_low); }
				if (j == image_x - 1) { fprintf(fp_threshold_atan_low, "\n"); }
			}
			else {
				//���臒l
				if (threshold_atan_high_flag[j][i] == 1) { threshold_atan_high = V0[j][i];}
				if (threshold_atan_high_flag[j][i] == 3) { threshold_atan_high = V90[j][i]; }
				

				//����臒l
				if (threshold_atan_low_flag[j][i] == 1) { threshold_atan_low = V0[j][i]; }
				if (threshold_atan_low_flag[j][i] == 3) { threshold_atan_low = V90[j][i];}
				

			}

			threshold_atan_high_abs = threshold_atan_high;
			threshold_atan_low_abs = threshold_atan_low;

			//��Βl��臒l
			if (threshold_atan_high < 0) { threshold_atan_high_abs = threshold_atan_high_abs*-1; }
			if (threshold_atan_low < 0) { threshold_atan_low_abs = threshold_atan_low_abs*-1; }
			if (threshold_atan_high_abs > threshold_atan_low_abs) {
				fprintf(fp_atan_threshold2, "%lf,", threshold_atan_high);
			} else {
				fprintf(fp_atan_threshold2, "%lf,", threshold_atan_low);
			}
			if (j == image_x - 1) { fprintf(fp_atan_threshold2, "\n"); }
		}
	}
///////////////////////////�������ݏI���/////////////////////////////////////////////////////////			

	//�t�@�C�������
	fclose(fp_arctan);
	fclose(fp_atan_threshold2);
	if (atan_eco_mode_flag != 1) {
		fclose(fp_threshold_atan_high);
		fclose(fp_threshold_atan_low);
	}
			
////////////////////////log�t�@�C���̍쐬//////////////////////////////////////////////////////////////////////////
	FILE *fp_date;
	char filename_log[128];

	sprintf(filename_log, "%s\\log.txt",date_directory4);	//log�t�@�C���쐬�̃f�B���N�g���w��
	if((fp_date=fopen(filename_log,"w"))==NULL){printf("log�t�@�C�����J���܂���");exit(1);}
	fprintf(fp_date,"Time       : %s\n",date);						//����
	fprintf(fp_date,"�g�p�f�[�^ : %s\n",inputdate_directory_atan);		//�g�p������ݍ��ݍς݃f�[�^
	fprintf(fp_date,"�ۑ���     : %s\n",date_directory);			//�ۑ���
	fclose(fp_date);

	printf("�g�p�f�[�^ : %s\n",inputdate_directory_atan);				//�g�p������ݍ��ݍς݃f�[�^
	printf("log�t�@�C�� %s ��ۑ����܂���\n",filename_log);

	//�������̊J��
	free_matrix(V0, 0, image_x - 1, 0, image_y - 1);
	free_matrix(V90, 0, image_x - 1, 0, image_y - 1);
	free_matrix(threshold_atan_high_flag, 0, image_x - 1, 0, image_y - 1);
	free_matrix(threshold_atan_low_flag, 0, image_x - 1, 0, image_y - 1);
	free_matrix(Angle, 0, image_x - 1, 0, image_y - 1);
	
	return 0;
}
