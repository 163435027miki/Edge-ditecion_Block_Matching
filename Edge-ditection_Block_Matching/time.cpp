#include<time.h>//���Ԃ�p����
#include <direct.h>//�t�H���_���쐬��
#include<sstream> //�����X�g���[��


int timeset(char date[]){
//���s�����̕\��
	//���s���Ԃ̎擾
	time_t now = time(NULL);
	struct tm *pnow = localtime(&now);
	
	sprintf(date, "%2d-%02d%02d-%02d%02d%02d", pnow->tm_year + 1900, pnow->tm_mon + 1, pnow->tm_mday, pnow->tm_hour, pnow->tm_min, pnow->tm_sec);
	printf(date);
	printf("\n");
	return *date;
	/*
	//���ʂ�ۑ�����t�H���_�̐���
	//�t�H���_���͎��s�����ɂȂ�
	//char date_directory[128];
	sprintf(date_directory,"..\\output_data\\%2d-%02d%02d-%02d%02d%02d\\",pnow->tm_year+1900,pnow->tm_mon + 1,pnow->tm_mday,pnow->tm_hour,pnow->tm_min,pnow->tm_sec);
	 if(_mkdir(date_directory)==0){
        printf("�t�H���_ %s ���쐬���܂���\n",date_directory);
    }else{
        printf("�t�H���_�쐬�Ɏ��s���܂����B\n");
    }
	 return date_directory[128];
	 */
}
/*
int log_create(char date_directory[],int cols,int rows,double sin_frequency,double degree,int step,int bold){
	FILE *fp_date;
	char log_directry[128];
	sprintf(log_directry,"%s\\log.txt",date_directory);
	if((fp_date=fopen(log_directry,"w"))==NULL){printf("log�t�@�C�����J���܂���");exit(1);}
	fprintf(fp_date,"Time       : %s\n",date_directory);						//����
	fprintf(fp_date,"�����F%d.pixel�C�c���F%d.pixel\n",cols,rows);
	fprintf(fp_date,"��Ԏ��g��f= %f\n��]�p�x %d [��]\n�p�x����step = %d\n���� = %d\n",sin_frequency,degree,step,bold);

	fclose(fp_date);

	return 0;
}*/

int notimeset(char date[], int pixel[], int Togire[], int z2, int z) {
	//���s���Ԃ̎擾
	time_t now = time(NULL);
	struct tm *pnow = localtime(&now);

	sprintf(date, "15-%dp-%dT", pixel[z2],Togire[z]);
	printf(date);
	printf("\n");
	return *date;
}