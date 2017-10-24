#include <string>
#include<sstream> //�����X�g���[��
#include<fstream>
#include<iostream>
#include <windows.h>
#include <stdio.h>
#include <vector>

#include <tuple>
#include <direct.h>//�t�H���_���쐬����

using namespace std;

//�������m�ۂ��s�����߂̃w�b�_
#define ANSI				
#include "nrutil.h"	

char *EdBM_name1_s = "Angle.csv";
//char *EdBM_name2_s = "threshold2.csv";
char *EdBM_name2_s = "edge_st.csv";

char *EdBM_name1t_s = "Anglet.csv";
//char *EdBM_name2t_s = "threshold2t.csv";
char *EdBM_name2t_s = "edge_st_t.csv";

char inputAngle_directory[255];

char inputAngle_deta[128];
char inputAnglet_deta[128];
char inputthreshold2_deta[128];
char inputthreshold2t_deta[128];

//std::tuple<int, int, std::vector<std::vector<double>>>read_csv(const char *filename);
int write_frame(char date_directory[], char Inputiamge[], int max_x, int max_y, int image_xt, int image_yt);

int Edge_detection_Block_Matching(char date_directory[], int &image_x, int &image_y, int &image_xt, int &image_yt, int paramerter[], int paramerter_count, int sd, char date[],int Bs, double threshold_EdBM, char Inputiamge[], double &threshold_otsu) {
	printf("\n****************************************\n");
	printf("start�FEdge-detection_Block_Matching\n");
	printf("****************************************\n");
	
	//Nrutil��p�����������̊m��
	double **Angle = matrix(0, image_x - 1, 0, image_y - 1);
	double **threshold2 = matrix(0, image_x - 1, 0, image_y - 1);
	double **Anglet = matrix(0, image_xt - 1, 0, image_y - 1);
	double **threshold2t = matrix(0, image_xt - 1, 0, image_yt - 1);

	double **threshold_tMAP = matrix(0, image_xt - 1, 0, image_yt - 1);

	//�u���b�N�̐�
	int M, N;
	M = image_xt / Bs;
	N = image_yt / Bs;

	int bm = 0;
	int bn = 0;

	double **CB = matrix(0, image_x - image_xt- 1, 0, image_y - image_yt - 1);
	double **V = matrix(0, image_x - image_xt - 1, 0, image_y - image_yt - 1);

	double min_CB;
	int min_x, min_y;
	

	//�m�ۂ���������������������
	for (int i = 0; i < image_y; i++) {
		for (int j = 0; j < image_x; j++) {
			Angle[j][i] = 0;
			threshold2[j][i] = 0;
		}
	}
	for (int i = 0; i < image_yt; i++) {
		for (int j = 0; j < image_xt; j++) {
			Anglet[j][i] = 0;
			threshold2t[j][i] = 0;
			threshold_tMAP[j][i] = 0;
		}
	}

	for (int i = 0; i < image_y - image_yt; i++) {
		for (int j = 0; j < image_x - image_xt; j++) {
			CB[j][i] = 0;
			V[j][i] = 0;
		}
	}

	
	
	//input�f�[�^�̑I��(cossim�̂�)
	switch (paramerter[0]){
	case 3:
		sprintf(inputAngle_directory, "%s%dk_cossim_sd%d\\", date_directory, paramerter[paramerter_count], sd);
		break;
	case 4:
	case 5:
		sprintf(inputAngle_directory, "%s%d�~%dsobel_atan_sd%d\\", date_directory, paramerter[paramerter_count], paramerter[paramerter_count], sd);
		break;
	}

	//Input�t�@�C���̃f�B���N�g���ݒ�
	sprintf(inputAngle_deta, "%s%s", inputAngle_directory, EdBM_name1_s);
	sprintf(inputAnglet_deta, "%s%s", inputAngle_directory, EdBM_name1t_s);
	sprintf(inputthreshold2_deta, "%s%s", inputAngle_directory, EdBM_name2_s);
	sprintf(inputthreshold2t_deta, "%s%s", inputAngle_directory, EdBM_name2t_s);

	//Input�t�@�C�����J��
	ifstream Angle_f(inputAngle_deta);
	ifstream Anglet_f(inputAnglet_deta);
	ifstream threshold2_f(inputthreshold2_deta);
	ifstream threshold2t_f(inputthreshold2t_deta);

	if (!Angle_f) { printf("���̓G���[ Angle.csv������܂���_Edge-detection_Block_Matching_filename=%s", inputAngle_deta); return 1; }
	if (!Anglet_f) { printf("���̓G���[ Anglet.csv������܂���_Edge-detection_Block_Matching_filename=%s", inputAnglet_deta); return 1; }
	if (!threshold2_f) { printf("���̓G���[ threshold2.csv������܂���_Edge-detection_Block_Matching_filename=%s", inputthreshold2_deta); return 1; }
	if (!threshold2t_f) { printf("���̓G���[ threshold2t.csv������܂���_Edge-detection_Block_Matching_filename=%s", inputthreshold2t_deta); return 1; }
	
	int count_large = 0;
	int count_small = 0;
/////////�f�[�^�̓ǂݍ���(�Ώۉ摜)////////////////////////////////////////////////////////////////////////
	int i = 1;
	printf("�Ώۉ摜�̌��ʂ�ǂݎ��܂�\n");
	string str_Angle, str_threshold2;
	count_large = 0;
	while (getline(Angle_f, str_Angle)) {					//���̃��[�v���ł��ׂĂ��s��
		count_small = 0;			//������
///////////////���낢���`�D�����ł��Ȃ��Ƃ����Ȃ�///////////////////////////////////////////////////////////////////////////
		string token_Angle_f;
		istringstream stream_Angle(str_Angle);

		getline(threshold2_f, str_threshold2);	string token_threshold2_f;	istringstream stream_threshold2(str_threshold2);

//////////////////////////�z��ɑ��//////////////////////////////////////////////////////////////////////////////////////////////////
		while (getline(stream_Angle, token_Angle_f, ',')) {	//��s�ǂݎ��DV0�̂݁C�J��Ԃ��͈͎̔w��ɗp����
			double tmp_Angle = stof(token_Angle_f);			//�����𐔎��ɕϊ�
			Angle[count_small][count_large] = tmp_Angle;				//�z��ɑ��

			getline(stream_threshold2, token_threshold2_f, ',');
			double tmp_threshold2 = stof(token_threshold2_f);			//�����𐔎��ɕϊ�
			threshold2[count_small][count_large] = tmp_threshold2;				//�z��ɑ��
			++count_small;
			}
			++count_large;
		}
/////////�f�[�^�̓ǂݍ���(�e���v���[�g�摜)////////////////////////////////////////////////////////////////////////
	i = 1;
	printf("�e���v���[�g�摜�̌��ʂ�ǂݎ��܂�\n");
	string str_Anglet, str_threshold2t;
	count_large = 0;
	while (getline(Anglet_f, str_Anglet)) {					//���̃��[�v���ł��ׂĂ��s��
		count_small = 0;			//������
									///////////////���낢���`�D�����ł��Ȃ��Ƃ����Ȃ�///////////////////////////////////////////////////////////////////////////
		string token_Anglet_f;
		istringstream stream_Anglet(str_Anglet);

		getline(threshold2t_f, str_threshold2t);	string token_threshold2t_f;	istringstream stream_threshold2t(str_threshold2t);

//////////////////////////�z��ɑ��//////////////////////////////////////////////////////////////////////////////////////////////////
		while (getline(stream_Anglet, token_Anglet_f, ',')) {	//��s�ǂݎ��DV0�̂݁C�J��Ԃ��͈͎̔w��ɗp����
			double tmp_Anglet = stof(token_Anglet_f);			//�����𐔎��ɕϊ�
			Anglet[count_small][count_large] = tmp_Anglet;				//�z��ɑ��

			getline(stream_threshold2t, token_threshold2t_f, ',');
			double tmp_threshold2t = stof(token_threshold2t_f);			//�����𐔎��ɕϊ�
			threshold2t[count_small][count_large] = tmp_threshold2t;				//�z��ɑ��
			++count_small;
		}
		++count_large;
	}

///////////////Edge-detection_Block_Matching//////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	//arctan�̏ꍇ�͔��ʕ��͖@�ŋ��߂�臒l��p����
	//if(paramerter[0]==4 || paramerter[0] == 5)threshold_EdBM = threshold_otsu;

	threshold_EdBM = threshold_otsu;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//threshold��flag
	for (int i = 0; i < image_yt; i++) {
		for (int j = 0; j < image_xt; j++) {
			if (threshold2t[j][i] >= threshold_EdBM) {

				threshold_tMAP[j][i] = 1;

				if(j>0 && i>0)threshold_tMAP[j - 1][i - 1] = 1;
				if (i>0)threshold_tMAP[j][i - 1] = 1;
				if (j<image_xt - 1 && i>0)threshold_tMAP[j + 1][i - 1] = 1;

				if(j>0)threshold_tMAP[j - 1][i] = 1;
				if (j<image_xt - 1)threshold_tMAP[j + 1][i] = 1;

				if(j>0 && i<image_yt-1)threshold_tMAP[j - 1][i + 1] = 1;
				if (i < image_yt - 1)threshold_tMAP[j][i + 1] = 1;
				if(j<image_xt - 1 && i<image_yt - 1)threshold_tMAP[j + 1][i + 1] = 1;

			}
		}
	}

//CB
	
	//����̃u���b�N�ɂ���
	for (int n = 0; n < N; n++) {
		for (int m = 0; m < M; m++) {

			bm = Bs*m;
			bn = Bs*n;
			min_CB = 0;
			min_x = 0;
			min_y = 0;
			for (int y = 0; y < image_y - image_yt; y++) {
				for (int x = 0; x < image_x - image_xt; x++) {
					CB[x][y] = 0;
				}
			}


			//���Wx,y���ׂ�
			for (int y = 0; y < image_y - image_yt; y++) {
				for (int x = 0; x < image_x - image_xt; x++) {

					for (int k = 0; k < Bs; k++) {
						for (int l = 0; l < Bs; l++) {
							if (threshold_tMAP[bm + l][bn + k] = 1) {
								CB[x][y] += abs(Angle[x + bm + l][y + bn + k] - Anglet[bm + l][bn + k]);
							}
							else {
								CB[x][y] += 0;
							}

						}
					}

				}
			}
			
			//m,n�ɂ���CB���ŏ��ƂȂ�x,y�����߂�
			min_CB = CB[0][0];
			for (int y = 0;  y< image_y - image_yt; y++) {
				for (int x = 0; x < image_x - image_xt; x++) {
					if (CB[x][y] < min_CB) {
						min_CB = CB[x][y];
						min_x = x;
						min_y = y;
					}
				}
			}
			//printf("x=%d,y=%d,%f\n", min_x, min_y,min_CB);
			V[min_x][min_y] += 1;
			
		}
	}

	double max_V;
	int max_x=0, max_y=0;
	max_V = V[0][0];
	for (int y = 0; y < image_y - image_yt; y++) {
		for (int x = 0; x < image_x - image_xt; x++) {
			
			if (V[x][y] > max_V) {
				max_V = V[x][y];
				max_x = x;
				max_y = y;
				//printf("V[%d][%d]=%f,", x, y, V[x][y]);
			}

		}
	}

	printf("max_x=%d,max_y=%d\n", max_x, max_y);
	

	write_frame(date_directory, Inputiamge, max_x, max_y, image_xt, image_yt);

//////////////////////////////log�̍쐬///////////////////////////////////////////////////////////////////////////////////
	FILE *fp_date_Matching;
	char filename_log_Matching[128];
	//sprintf(filename_log, "..\\log\\log-%2d-%02d%02d-%02d%02d%02d.txt",pnow->tm_year+1900,pnow->tm_mon + 1,pnow->tm_mday,pnow->tm_hour,pnow->tm_min,pnow->tm_sec);	//log�t�@�C���쐬�̃f�B���N�g���w��
	sprintf(filename_log_Matching, "%s\\log_Matching.txt", date_directory);	//log�t�@�C���쐬�̃f�B���N�g���w��
	if ((fp_date_Matching = fopen(filename_log_Matching, "w")) == NULL) { printf("log�t�@�C�����J���܂���"); exit(1); }
	fprintf(fp_date_Matching, "�̈�̍��W�F(x,y)=(%d,%d),(%d,%d)\n", max_x, max_y, max_x+ image_xt, max_y+ image_yt);
	fprintf(fp_date_Matching, "�u���b�N�T�C�Y�Fx=%d,y=%d\n", image_xt, image_yt); 
	fprintf(fp_date_Matching, "threshold_otsu=%lf\n", threshold_otsu);
	if (threshold_EdBM == threshold_otsu) {
		fprintf(fp_date_Matching, "���ʕ��͖@��p�����Fthreshold_otsu=%lf\n", threshold_otsu);
	}
	else {
		fprintf(fp_date_Matching, "�����Őݒ肵��臒l��p�����Fthreshold_EdBM=%lf\n", threshold_EdBM);
	}
	
	fclose(fp_date_Matching);
	printf("log�t�@�C�� %s ��ۑ����܂���\n", filename_log_Matching);


	//�������̉��
	free_matrix(Angle, 0, image_x - 1, 0, image_y - 1);
	free_matrix(threshold2, 0, image_x - 1, 0, image_y - 1);
	free_matrix(Anglet, 0, image_xt - 1, 0, image_yt - 1);
	free_matrix(threshold2t, 0, image_xt - 1, 0, image_yt - 1);
	free_matrix(threshold_tMAP, 0, image_xt - 1, 0, image_yt - 1);
	free_matrix(CB, 0, image_x - image_xt - 1, 0, image_y - image_yt - 1);
	free_matrix(V, 0, image_x - image_xt - 1, 0, image_y - image_yt - 1);

	
	return 0;
}