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
char *EdBM_name2_s = "threshold2.csv";

char *EdBM_name1t_s = "Anglet.csv";
char *EdBM_name2t_s = "threshold2t.csv";

char inputAngle_directory[255];

char inputAngle_deta[128];
char inputAnglet_deta[128];
char inputthreshold2_deta[128];
char inputthreshold2t_deta[128];

//std::tuple<int, int, std::vector<std::vector<double>>>read_csv(const char *filename);

int Edge_detection_Block_Matching(char date_directory[], int &image_x, int &image_y, int &image_xt, int &image_yt, int paramerter[], int paramerter_count, int sd, char date[]) {
	printf("****************************************\n");
	printf("start�FEdge-detection_Block_Matching\n");
	printf("****************************************\n");

	//Nrutil��p�����������̊m��
	double **Angle = matrix(0, image_x - 1, 0, image_y - 1);
	double **threshold2 = matrix(0, image_x - 1, 0, image_y - 1);
	double **Anglet = matrix(0, image_xt - 1, 0, image_y - 1);
	double **threshold2t = matrix(0, image_xt - 1, 0, image_yt - 1);

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
		}
	}
	
	//input�f�[�^�̑I��(cossim�̂�)
	switch (paramerter[0]){
	case 3:
		sprintf(inputAngle_directory, "%s%dk_cossim_sd%d\\", date_directory, paramerter[paramerter_count], sd);
		break;
	case 4:
	case 5:
		sprintf(inputAngle_directory, "%s%d�~%dsobel_cossim_sd%d\\", date_directory, paramerter[paramerter_count], paramerter[paramerter_count], sd);
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



	//�������̉��
	free_matrix(Angle, 0, image_x - 1, 0, image_y - 1);
	free_matrix(threshold2, 0, image_x - 1, 0, image_y - 1);
	free_matrix(Anglet, 0, image_xt - 1, 0, image_yt - 1);
	free_matrix(threshold2t, 0, image_xt - 1, 0, image_yt - 1);

	return 0;
}