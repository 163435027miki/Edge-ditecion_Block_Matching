#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>
#include <tuple>

#include <opencv2/opencv.hpp>


void readCSVData(
  double** pArray,      /* CSVデータ格納用配列 */
  int* pElems,          /* CSVデータの要素数 */
  int* pRows,           /* CSVデータの行数 */
  int* pCols,           /* CSVデータの列数 */
  const char* filename  /* csvファイルの名前 */
);
char* readCSVFile(const char* filename);

std::tuple<int, int, std::vector<std::vector<double>>> read_csv(const char *filename)
{


  //const char *filename = "Pepper_sd10.csv";
  double *data = NULL; 
  int elems = 0;  /* 全要素数 */
  int rows = 0;   /* 行の数 */
  int cols = 0;   /* 列の数 */
  int i, j;
 
  

  /* 引数からcsvファイルの名前を取得 */
  /*
  if (argc >= 2) {
    filename = argv[1];
  }
  */
  /* CSVデータの読込 */
  readCSVData(&data, &elems, &rows, &cols, filename);

  printf("\n");
  printf("filename = %s\n\n", filename);

  
 
  /* data[i*cols+j] = i行j列の成分 */
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

  printf("要素数 = %d\n", elems);
  printf("行数 = %d\n", rows);
  printf("列数 = %d\n", cols);

  /* メモリを動的に確保するので解放が必要 */
  free(data); data = NULL;

  //return rows,cols;
  return std::forward_as_tuple(rows,cols,Output);
}

void readCSVData(
  double** pArray,      /* CSVデータ格納用配列 */
  int* pElems,          /* CSVデータの要素数 */
  int* pRows,           /* CSVデータの行数 */
  int* pCols,           /* CSVデータの列数 */
  const char* filename  /* csvファイルの名前 */
) {
  char *str = NULL;
  char elem[15];     /* 要素を文字列として格納 */
  char *ep = NULL;
  int countNL = 0;   /* 改行の数をカウント */
  int countSep = 0;  /* カンマの数をカウント */
  int cols = 0;      /* 各行の列数をカウント */
  int i, j, k;
  double val;

  /* テキストファイルの読み込み */
  str = readCSVFile(filename);
  /* 要素数, 行数, 列数の取得 */
  printf("画像サイズを取得しています\n");
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
  printf("画像サイズ取得できた");
 
  *pRows = countNL;
  *pElems = countSep + countNL; 

  /* CSVデータ格納用配列の生成 */
  *pArray = (double *)calloc(
    (*pRows) * (*pCols) * sizeof(double), sizeof(double));
  if (*pArray == NULL) {
    printf("can't allocate memory. '*pArray' is NULL. \n");
    free(str); str = NULL;
    exit(1);
  }

  /* CSVデータを格納用配列へコピー */
  i = j = 0; 
  k = 0; elem[0] = '0'; elem[1] = '\0';

  printf("データをメモリに格納します\n");

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

  /* メモリを動的に確保するので解放が必要 */
  free(str); str = NULL;

  return;
}

char* readCSVFile(const char* filename) 
{
  FILE *fp = NULL;
  char buf[256];
  char* str = NULL;
  int fsize;

  /* ファイルのオープン */
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Can't read '%s'.\n", filename);
    exit(1);
  };

  /* ファイルサイズの取得 */
  fseek(fp, 0L, SEEK_END);
  fsize = ftell(fp);

  /* メモリの動的確保 */
  str = (char*)malloc(fsize * sizeof(char)); 
  if (str == NULL) {
    printf("Can't allocate memory. 'str' is NULL.\n");
    fclose(fp);
    exit(1);
  }

  /* シーク位置を先頭に戻す */
  fseek(fp, 0L, SEEK_SET);

  /* テキストデータの読み込み */
  str[0] = '\0';
  printf("csvを読み込んでいます\n");
  while (fgets(buf, sizeof(buf), fp) != NULL) {
    strncat(str, buf, strlen(buf) + 1);
  }

  /* ファイルのクローズ */
  fclose(fp);  

  return str;
}