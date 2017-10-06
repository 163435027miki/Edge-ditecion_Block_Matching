#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>
#include <tuple>

#include <opencv2/opencv.hpp>


void readCSVData(
  double** pArray,      /* CSV�f�[�^�i�[�p�z�� */
  int* pElems,          /* CSV�f�[�^�̗v�f�� */
  int* pRows,           /* CSV�f�[�^�̍s�� */
  int* pCols,           /* CSV�f�[�^�̗� */
  const char* filename  /* csv�t�@�C���̖��O */
);
char* readCSVFile(const char* filename);

std::tuple<int, int, std::vector<std::vector<double>>> read_csv(const char *filename)
{


  //const char *filename = "Pepper_sd10.csv";
  double *data = NULL; 
  int elems = 0;  /* �S�v�f�� */
  int rows = 0;   /* �s�̐� */
  int cols = 0;   /* ��̐� */
  int i, j;
 
  

  /* ��������csv�t�@�C���̖��O���擾 */
  /*
  if (argc >= 2) {
    filename = argv[1];
  }
  */
  /* CSV�f�[�^�̓Ǎ� */
  readCSVData(&data, &elems, &rows, &cols, filename);

  printf("\n");
  printf("filename = %s\n\n", filename);

  
 
  /* data[i*cols+j] = i�sj��̐��� */
  /*
  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols-1; j++) {
		if (j == cols - 1) {
			printf("%.3f\n\n\n", data[i*cols + j]);
			//make_imagea.at<uchar>(j, i) = data[i*cols + j];
		}
		else {
			//make_imagea.at<uchar>(j, i) = data[i*cols + j];
			printf("%.3f, ", data[i*cols + j]);
	  }
       
    }
  }
  printf("\n");
  */
  cols = cols - 1;
 
  
  std::vector<std::vector<double>>Output;
  Output.resize(rows);
  for (int i = 0; i<rows; ++i) {
	  Output[i].resize(cols);
  }

  for (i = 0; i < rows; i++) {
	  for (j = 0; j < cols ; j++) {
			  Output[j][i] = data[i*(cols+1) + j];
	  }
  }

  printf("�v�f�� = %d\n", elems);
  printf("�s�� = %d\n", rows);
  printf("�� = %d\n", cols);

  /* �������𓮓I�Ɋm�ۂ���̂ŉ�����K�v */
  free(data); data = NULL;

  //return rows,cols;
  return std::forward_as_tuple(rows,cols,Output);
}

void readCSVData(
  double** pArray,      /* CSV�f�[�^�i�[�p�z�� */
  int* pElems,          /* CSV�f�[�^�̗v�f�� */
  int* pRows,           /* CSV�f�[�^�̍s�� */
  int* pCols,           /* CSV�f�[�^�̗� */
  const char* filename  /* csv�t�@�C���̖��O */
) {
  char *str = NULL;
  char elem[15];     /* �v�f�𕶎���Ƃ��Ċi�[ */
  char *ep = NULL;
  int countNL = 0;   /* ���s�̐����J�E���g */
  int countSep = 0;  /* �J���}�̐����J�E���g */
  int cols = 0;      /* �e�s�̗񐔂��J�E���g */
  int i, j, k;
  double val;

  /* �e�L�X�g�t�@�C���̓ǂݍ��� */
  str = readCSVFile(filename);
  /* �v�f��, �s��, �񐔂̎擾 */
  printf("�摜�T�C�Y���擾���Ă��܂�\n");
  for (i = 0; i < strlen(str); i++) {
    switch (str[i]) {
      case ',': countSep++; cols++; break;
	  //case '\t': countSep++; cols++; break;
      case '\n': 
        countNL++; cols++;
        *pCols = (*pCols > cols) ? *pCols : cols;
        cols = 0;
        break;
    }
	printf("countNL=%d\n", countNL);
  }
  printf("�摜�T�C�Y�擾�ł���");
 
  *pRows = countNL;
  *pElems = countSep + countNL; 

  /* CSV�f�[�^�i�[�p�z��̐��� */
  *pArray = (double *)calloc(
    (*pRows) * (*pCols) * sizeof(double), sizeof(double));
  if (*pArray == NULL) {
    printf("can't allocate memory. '*pArray' is NULL. \n");
    free(str); str = NULL;
    exit(1);
  }

  /* CSV�f�[�^���i�[�p�z��փR�s�[ */
  i = j = 0; 
  k = 0; elem[0] = '0'; elem[1] = '\0';

  printf("�f�[�^���������Ɋi�[���܂�\n");

  while(j < (*pRows) * (*pCols)) {
    if (i < strlen(str)) {
      switch (str[i]) {
		//case '\t': case '\n':
        case ',': case '\n':
          val = strtod(elem, &ep);
          if(*ep != '\0') {
            printf("Warning (%d, %d): "
              "Conversion may be incorrect. \n", 
              j/(*pCols), j%(*pCols));
          }
          (*pArray)[j] = val; j++; 
          if (str[i] == '\n') {
            while(j % (*pCols) > 0) {
              (*pArray)[j] = 0.0; j++;
            }
          }
          k = 0; elem[0] = '0'; elem[1] = '\0';
          break;
        default:
          if (k+1 < sizeof(elem)) {
            elem[k] = str[i]; elem[k+1] = '\0'; k++;
          } 
          else if (k+1 == sizeof(elem)) {
            printf("Warning (%d, %d): "
              "Too many digits. \n", 
              j/(*pCols), j%(*pCols));
            k++;
          }
          break;
      }
      i++;
    }
    else {
      (*pArray)[j] = 0.0; j++;
    }
  }

  /* �������𓮓I�Ɋm�ۂ���̂ŉ�����K�v */
  free(str); str = NULL;

  return;
}

char* readCSVFile(const char* filename) 
{
  FILE *fp = NULL;
  char buf[256];
  char* str = NULL;
  int fsize;

  /* �t�@�C���̃I�[�v�� */
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Can't read '%s'.\n", filename);
    exit(1);
  };

  /* �t�@�C���T�C�Y�̎擾 */
  fseek(fp, 0L, SEEK_END);
  fsize = ftell(fp);

  /* �������̓��I�m�� */
  str = (char*)malloc(fsize * sizeof(char)); 
  if (str == NULL) {
    printf("Can't allocate memory. 'str' is NULL.\n");
    fclose(fp);
    exit(1);
  }

  /* �V�[�N�ʒu��擪�ɖ߂� */
  fseek(fp, 0L, SEEK_SET);

  /* �e�L�X�g�f�[�^�̓ǂݍ��� */
  str[0] = '\0';
  printf("csv��ǂݍ���ł��܂�\n");
  while (fgets(buf, sizeof(buf), fp) != NULL) {
    strncat(str, buf, strlen(buf) + 1);
  }

  /* �t�@�C���̃N���[�Y */
  fclose(fp);  

  return str;
}