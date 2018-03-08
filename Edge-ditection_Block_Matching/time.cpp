#include<time.h>//時間を用いる
#include <direct.h>//フォルダを作成す
#include<sstream> //文字ストリーム
#include <windows.h>

int timeset(char date[]){
//実行日時の表示
	//実行時間の取得
	time_t now = time(NULL);
	struct tm *pnow = localtime(&now);
	
	sprintf(date, "%2d-%02d%02d-%02d%02d%02d", pnow->tm_year + 1900, pnow->tm_mon + 1, pnow->tm_mday, pnow->tm_hour, pnow->tm_min, pnow->tm_sec);
	printf(date);
	printf("\n");
	return *date;
	/*
	//結果を保存するフォルダの生成
	//フォルダ名は実行日時になる
	//char date_directory[128];
	sprintf(date_directory,"..\\output_data\\%2d-%02d%02d-%02d%02d%02d\\",pnow->tm_year+1900,pnow->tm_mon + 1,pnow->tm_mday,pnow->tm_hour,pnow->tm_min,pnow->tm_sec);
	 if(_mkdir(date_directory)==0){
        printf("フォルダ %s を作成しました\n",date_directory);
    }else{
        printf("フォルダ作成に失敗しました。\n");
    }
	 return date_directory[128];
	 */
}
/*
int log_create(char date_directory[],int cols,int rows,double sin_frequency,double degree,int step,int bold){
	FILE *fp_date;
	char log_directry[128];
	sprintf(log_directry,"%s\\log.txt",date_directory);
	if((fp_date=fopen(log_directry,"w"))==NULL){printf("logファイルが開けません");exit(1);}
	fprintf(fp_date,"Time       : %s\n",date_directory);						//時間
	fprintf(fp_date,"横幅：%d.pixel，縦幅：%d.pixel\n",cols,rows);
	fprintf(fp_date,"空間周波数f= %f\n回転角度 %d [°]\n角度刻みstep = %d\n太さ = %d\n",sin_frequency,degree,step,bold);

	fclose(fp_date);

	return 0;
}*/

int notimeset(char date[], int pixel[], int Togire[], int z2, int z) {
	//実行時間の取得
	time_t now = time(NULL);
	struct tm *pnow = localtime(&now);

	sprintf(date, "15-%dp-%dT", pixel[z2],Togire[z]);
	printf(date);
	printf("\n");
	return *date;
}

int msectime()
{
	int GetmsecTime;
	SYSTEMTIME st;
	GetSystemTime(&st);
	char szTime[25] = { 0 };
	// wHourを９時間足して、日本時間にする
	sprintf(szTime, "%02d%02d%02d%03d",

		st.wHour + 9, st.wMinute, st.wSecond, st.wMilliseconds);

	GetmsecTime = strtod(szTime, NULL);
	//printf("%s\n", szTime);
	//printf("%d\n", GetmsecTime);
	//return szTime;
	return GetmsecTime;
}