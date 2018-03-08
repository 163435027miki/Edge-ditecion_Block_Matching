/**************************************************************************
 *    3���X�v���C���֐��ɂ����                                          *
 *        �t�@�C����: Spline.c                                             *
 *                                          programed by K. Minemura       *
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//   �}�N����`

#define  NP      9     //  �ߓ_�̐�
#define  NINT    360    //  �S��Ԃ̓�����( �S��Ԃ� NINT ���������_�ŕ�� )


//  �v���g�^�C�v�錾
void S_Spline(double x[],double y[],int n,double xint[],double yint[],int no,int bcs, double yds, int bce, double yde,double ydash[]);

int S_TriDiag( double diag[], double upp[], double low[], double righ[], int n,int dec);


//  ���C���֐�
void Spline( double ydata[],double M[])
{
   double dx,xint[NINT+1],yint[NINT+1],ydash[NP];
   int i;
   //  ======= sin �֐����Ԃ���ꍇ ======
   //  ���E�����̑}��
   int  bcs = 0;        //  �n�_�͎��R����
   int  bce = 0;        //  �I�_�͎��R����
   double yds = 0.0, yde = -1.0;   // �[�����������R�����ɂ��A���̒l�̓_�~�[

   double xdata[NP]={0,1,2,3,4,5,6,7,8};	// x���W
 //  double ydata[NP]={M1,M2,M3,M4,M5,M6,M7,M8,M9,M10,M11};					// y���W
  
   //  =================================
   //  �S��Ԃ� NINT �������ĕ�ԓ_��ݒ�
   dx=(xdata[NP-1]-xdata[0])/NINT;
   xint[0]=xdata[0];

   for( i=1; i<=NINT;  i++) 
	   xint[i]=xdata[0]+dx*i;  //  ��ԓ_��x���W

   //  3���X�v���C����Ԃɂ��A��ԓ_y���W�����߂�
   S_Spline(xdata,ydata,NP,xint,yint,NINT+1,bcs,yds,bce,yde,ydash);

   for(i=0;i<NINT+1;i++)
		M[i]=yint[i];
}

/*
void Spline_MP(double M1,double M2,double M3,double M4,double M5,double M6,double M7,double M[601])
{
   double dx,xint[NINT_MP+1],yint[NINT_MP+1],ydash[NP_MP];
   int i;
   //  ======= sin �֐����Ԃ���ꍇ ======
   //  ���E�����̑}��
   int  bcs = 0;        //  �n�_�͎��R����
   int  bce = 0;        //  �I�_�͎��R����
   double yds = 0.0, yde = -1.0;   // �[�����������R�����ɂ��A���̒l�̓_�~�[
  
   double xdata[NP_MP]={0.0,0.25,0.5,0.75,1.0,1.25,1.5};	// x���W
   double ydata[NP_MP]={M1,M2,M3,M4,M5,M6,M7};				// y���W

   //  =================================
   //  �S��Ԃ� NINT_MP �������ĕ�ԓ_��ݒ�
   dx=(xdata[NP_MP-1]-xdata[0])/NINT_MP;
   xint[0]=xdata[0];

   for( i=1; i<=NINT_MP;  i++) 
	   xint[i]=xdata[0]+dx*i;  //  ��ԓ_��x���W

   //  3���X�v���C����Ԃɂ��A��ԓ_y���W�����߂�
   S_Spline(xdata,ydata,NP_MP,xint,yint,NINT_MP+1,bcs,yds,bce,yde,ydash);

   for(i=0;i<NINT_MP+1;i++)
		M[i]=yint[i];

}




/*******  3���X�v���C����Ԋ֐�      ************************************/
/*   ����  bcs,bce;  �n�_�E�I�_�ł̋��E����(0 = ���R����,  1 = �Œ����)*/
/*         yds,yde;  �n�_�E�I�_�ł̌Œ�����}�����̂P�����W��           */
/*         x[],y[];  �ߓ_�̍��W ( �v�f���F n )                          */
/*         xint[];   ��ԓ_��x���W ( �v�f���F no )                      */
/*   �o��  yint[];   ��ԓ_��y���W,  ydash[];  �ߓ_�̂P�����W��         */
/************************************* by K.Minemura ********************/


void S_Spline(double x[],double y[],int n,double xint[],double yint[],
      int no,int bcs, double yds, int bce, double yde,double ydash[]){
    int i,j,status;
    double ai,bi,ci,di,dx,*a,*b,*c,*h;
    a=(double *)calloc(n, sizeof(double));    // �z��̓��I�m��
    b=(double *)calloc(n, sizeof(double));
    c=(double *)calloc(n, sizeof(double));
    h=(double *)calloc(n, sizeof(double));  


    for(i=0; i<n-1; i++)  h[i]=x[i+1]-x[i];
    //  �n�_�ɂ����鋫�E�����̑}��
    a[0] = 0.0;
    if(bcs == 0)  // ���R�����̏ꍇ
    {  
        b[0] = 2.0*h[0]; 
        c[0] = h[0]; 
        ydash[0] = 3.0*(y[1]-y[0]);
    }
    if(bcs==1)    // �Œ�����̏ꍇ
    { 
        b[0] = 1.0;    
        c[0] = 0.0;  
        ydash[0] = yds; 
    }
    //  ���Ԃ̐ߓ_�ł̌W��    
    for(i=1; i<n-1; i++)
    {
        a[i] = h[i];   
        b[i] = 2.0*(h[i-1]+h[i]);   
        c[i] = h[i-1];
        ydash[i]=3.0*((y[i]-y[i-1])*h[i]/h[i-1]+(y[i+1]-y[i])*h[i-1]/h[i]);
    }
    //   �I�_�ɂ����鋫�E�����̑}��
    c[n-1] = 0.0; 
    if(bce==0)   //  ���R�����̑}��
    { 
        a[n-1] = h[n-2];   
        b[n-1] = 2.0*h[n-2];
        ydash[n-1] = 3.0*(y[n-1]-y[n-2]);
    }
    if(bce==1)   //  �Œ�����̑}��
    {
        a[n-1] = 0.0; 
        b[n-1] = 1.0; 
        ydash[n-1] = yde;
    }   
    //   �R�d�Ίp�W���s������A���������̉��
    status = S_TriDiag( b, c, a, ydash, n, 0 );
    if( status == 0 ){
        //  for(i=0;i<n;i++) printf(" %d %f \n",i,ydash[i]); 
        //   ��Ԓl�̌v�Z
        for(j=0, i=0; i<n-1; i++){
            ai=(2.0*(y[i]-y[i+1])/h[i]+ydash[i]+ydash[i+1])/(h[i]*h[i]);
            bi=(3.0*(y[i+1]-y[i])/h[i]-2.0*ydash[i]-ydash[i+1])/h[i];
            ci=ydash[i];
            di=y[i]; 
	    while( xint[j] < x[i+1] )
            {
                dx=xint[j]-x[i];                   // ��Ԃ����x���W
                yint[j]=dx*(dx*(dx*ai+bi)+ci)+di;  // ��Ԃ��ꂽy���W
                if( j < no ){ j++;}
	    }
	    yint[no-1] = y[n-1]; 
	    //     printf(" %d %d %f \n",j,i,yint[j]);  
       }
    }
 if (a) {free(a), a = NULL;}
  if (b) {free(b), b = NULL;}
   if (c) {free(c), c = NULL;}
    if (h) {free(h), h = NULL;}



}

/*********  3�d�Ίp�s��𒼐ږ@(LU����@)�ɂ���͂���֐� ********************/
/*  ���^�[���R�[�h�F  0=����,  9=�ُ�                                          */
/*  ����  �Fdiag[]=�Ίp�v�f�Cupp[]=�㑤�v�f,low[]=�����v�f �ilow[0]�̓_�~�[�j  */
/*          righ[]=�E�ӂ׃N�g���C n=�z�� diag,upp,low,righ �̗v�f��            */
/*          dec=0 (LU��������), dec != 0(LU���������A������LU��p���ĉ�͂���) */
/*  �o��  �Frigh[]=��                                                          */
/************************************* by K.Minemura ***************************/
int S_TriDiag( double diag[], double upp[], double low[], double righ[], int n, 
     int dec)
{
    int i;
    double Tiny=1.0e-8;
    for(i=0; i<n; i++){
       if( diag[i] < Tiny ){ return 9;  }
    }

    if( dec == 0 ){   //   LU-����  
        upp[0] /= diag[0];
        for(i=1; i<n-1; i++){
            diag[i] -= upp[i-1]*low[i];      
            upp[i] /= diag[i];
        }
        diag[n-1] -= upp[n-2]*low[n-1];
    }
    //    �O�i���
    righ[0] /= diag[0];  
    for(i=1; i<n; i++)  righ[i] = (righ[i]-low[i]*righ[i-1])/diag[i]; 
    //    ��ޑ��
    for(i=n-2; i>=0; i--)  righ[i] -= righ[i+1]*upp[i]; 
    return 0;
}

