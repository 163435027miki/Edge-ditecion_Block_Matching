/**************************************************************************
 *    3次スプライン関数による補間                                          *
 *        ファイル名: Spline.c                                             *
 *                                          programed by K. Minemura       *
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//   マクロ定義

#define  NP      9     //  節点の数
#define  NINT    360    //  全区間の等分数( 全区間を NINT 等分した点で補間 )


//  プロトタイプ宣言
void S_Spline(double x[],double y[],int n,double xint[],double yint[],int no,int bcs, double yds, int bce, double yde,double ydash[]);

int S_TriDiag( double diag[], double upp[], double low[], double righ[], int n,int dec);


//  メイン関数
void Spline( double ydata[],double M[])
{
   double dx,xint[NINT+1],yint[NINT+1],ydash[NP];
   int i;
   //  ======= sin 関数を補間する場合 ======
   //  境界条件の挿入
   int  bcs = 0;        //  始点は自然条件
   int  bce = 0;        //  終点は自然条件
   double yds = 0.0, yde = -1.0;   // 端末条件が自由条件につき、この値はダミー

   double xdata[NP]={0,1,2,3,4,5,6,7,8};	// x座標
 //  double ydata[NP]={M1,M2,M3,M4,M5,M6,M7,M8,M9,M10,M11};					// y座標
  
   //  =================================
   //  全区間を NINT 等分して補間点を設定
   dx=(xdata[NP-1]-xdata[0])/NINT;
   xint[0]=xdata[0];

   for( i=1; i<=NINT;  i++) 
	   xint[i]=xdata[0]+dx*i;  //  補間点のx座標

   //  3次スプライン補間により、補間点y座標を求める
   S_Spline(xdata,ydata,NP,xint,yint,NINT+1,bcs,yds,bce,yde,ydash);

   for(i=0;i<NINT+1;i++)
		M[i]=yint[i];
}

/*
void Spline_MP(double M1,double M2,double M3,double M4,double M5,double M6,double M7,double M[601])
{
   double dx,xint[NINT_MP+1],yint[NINT_MP+1],ydash[NP_MP];
   int i;
   //  ======= sin 関数を補間する場合 ======
   //  境界条件の挿入
   int  bcs = 0;        //  始点は自然条件
   int  bce = 0;        //  終点は自然条件
   double yds = 0.0, yde = -1.0;   // 端末条件が自由条件につき、この値はダミー
  
   double xdata[NP_MP]={0.0,0.25,0.5,0.75,1.0,1.25,1.5};	// x座標
   double ydata[NP_MP]={M1,M2,M3,M4,M5,M6,M7};				// y座標

   //  =================================
   //  全区間を NINT_MP 等分して補間点を設定
   dx=(xdata[NP_MP-1]-xdata[0])/NINT_MP;
   xint[0]=xdata[0];

   for( i=1; i<=NINT_MP;  i++) 
	   xint[i]=xdata[0]+dx*i;  //  補間点のx座標

   //  3次スプライン補間により、補間点y座標を求める
   S_Spline(xdata,ydata,NP_MP,xint,yint,NINT_MP+1,bcs,yds,bce,yde,ydash);

   for(i=0;i<NINT_MP+1;i++)
		M[i]=yint[i];

}




/*******  3次スプライン補間関数      ************************************/
/*   入力  bcs,bce;  始点・終点での境界条件(0 = 自然条件,  1 = 固定条件)*/
/*         yds,yde;  始点・終点での固定条件挿入時の１次微係数           */
/*         x[],y[];  節点の座標 ( 要素数： n )                          */
/*         xint[];   補間点のx座標 ( 要素数： no )                      */
/*   出力  yint[];   補間点のy座標,  ydash[];  節点の１次微係数         */
/************************************* by K.Minemura ********************/


void S_Spline(double x[],double y[],int n,double xint[],double yint[],
      int no,int bcs, double yds, int bce, double yde,double ydash[]){
    int i,j,status;
    double ai,bi,ci,di,dx,*a,*b,*c,*h;
    a=(double *)calloc(n, sizeof(double));    // 配列の動的確保
    b=(double *)calloc(n, sizeof(double));
    c=(double *)calloc(n, sizeof(double));
    h=(double *)calloc(n, sizeof(double));  


    for(i=0; i<n-1; i++)  h[i]=x[i+1]-x[i];
    //  始点における境界条件の挿入
    a[0] = 0.0;
    if(bcs == 0)  // 自然条件の場合
    {  
        b[0] = 2.0*h[0]; 
        c[0] = h[0]; 
        ydash[0] = 3.0*(y[1]-y[0]);
    }
    if(bcs==1)    // 固定条件の場合
    { 
        b[0] = 1.0;    
        c[0] = 0.0;  
        ydash[0] = yds; 
    }
    //  中間の節点での係数    
    for(i=1; i<n-1; i++)
    {
        a[i] = h[i];   
        b[i] = 2.0*(h[i-1]+h[i]);   
        c[i] = h[i-1];
        ydash[i]=3.0*((y[i]-y[i-1])*h[i]/h[i-1]+(y[i+1]-y[i])*h[i-1]/h[i]);
    }
    //   終点における境界条件の挿入
    c[n-1] = 0.0; 
    if(bce==0)   //  自然条件の挿入
    { 
        a[n-1] = h[n-2];   
        b[n-1] = 2.0*h[n-2];
        ydash[n-1] = 3.0*(y[n-1]-y[n-2]);
    }
    if(bce==1)   //  固定条件の挿入
    {
        a[n-1] = 0.0; 
        b[n-1] = 1.0; 
        ydash[n-1] = yde;
    }   
    //   ３重対角係数行列をもつ連立方程式の解析
    status = S_TriDiag( b, c, a, ydash, n, 0 );
    if( status == 0 ){
        //  for(i=0;i<n;i++) printf(" %d %f \n",i,ydash[i]); 
        //   補間値の計算
        for(j=0, i=0; i<n-1; i++){
            ai=(2.0*(y[i]-y[i+1])/h[i]+ydash[i]+ydash[i+1])/(h[i]*h[i]);
            bi=(3.0*(y[i+1]-y[i])/h[i]-2.0*ydash[i]-ydash[i+1])/h[i];
            ci=ydash[i];
            di=y[i]; 
	    while( xint[j] < x[i+1] )
            {
                dx=xint[j]-x[i];                   // 補間されるx座標
                yint[j]=dx*(dx*(dx*ai+bi)+ci)+di;  // 補間されたy座標
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

/*********  3重対角行列を直接法(LU分解法)により解析する関数 ********************/
/*  リターンコード：  0=正常,  9=異常                                          */
/*  入力  ：diag[]=対角要素，upp[]=上側要素,low[]=下側要素 （low[0]はダミー）  */
/*          righ[]=右辺べクトル， n=配列 diag,upp,low,righ の要素数            */
/*          dec=0 (LU分解する), dec != 0(LU分解せず、既存のLUを用いて解析する) */
/*  出力  ：righ[]=解                                                          */
/************************************* by K.Minemura ***************************/
int S_TriDiag( double diag[], double upp[], double low[], double righ[], int n, 
     int dec)
{
    int i;
    double Tiny=1.0e-8;
    for(i=0; i<n; i++){
       if( diag[i] < Tiny ){ return 9;  }
    }

    if( dec == 0 ){   //   LU-分解  
        upp[0] /= diag[0];
        for(i=1; i<n-1; i++){
            diag[i] -= upp[i-1]*low[i];      
            upp[i] /= diag[i];
        }
        diag[n-1] -= upp[n-2]*low[n-1];
    }
    //    前進代入
    righ[0] /= diag[0];  
    for(i=1; i<n; i++)  righ[i] = (righ[i]-low[i]*righ[i-1])/diag[i]; 
    //    後退代入
    for(i=n-2; i>=0; i--)  righ[i] -= righ[i+1]*upp[i]; 
    return 0;
}

