
#ifndef _NR_UTILS_H_

#define _NR_UTILS_H_

/*
static int lminargi,lminarg2;
#define LMIN(a,b) (lminarg1=(a) , lminarg2=(b) , (lminarg1) < (lminarg2) ?\
	(lminarg1) : (lminarg2))
*/
static int imaxarg1,imaxarg2 ; 
#define IMAX(a,b) (imaxarg1=(a) ,imaxarg2=(b) , (imaxarg1) > (imaxarg2) ? (imaxarg1) : (imaxarg2))
static int iminargi,iminarg2;
#define IMIN(a,b) (iminarg1=(a) , iminarg2=(b) , (iminarg1) < (iminarg2) ? (iminarg1) : (iminarg2))

#define SIGN(a,b) ((b) >=0.0 ? fabs(a) : -fabs(a))

#if defined(__STDC__)  || defined(ANSI)  || defined(NRANSI) /* ANSI */ 

void nrerror(char error_text [] ) ; 
double *vector(long nl, long nh) ; 
int *ivector(long nl, long nh) ;
unsigned char *cvector(long nl, long nh) ; 
unsigned long *lvector(long nl, long nh) ; 
double *dvector(long nl, long nh) ; 
double **matrix(long nrl, long nrh, long ncl, long nch) ; 
double **dmatrix(long nrl, long nrh, long ncl, long nch) ; 
int **imatrix(long nrl, long nrh, long ncl, long nch) ; 
double **submatrix(double **a, long oldrl, long oldrh, long oldcl, long oldch, 
		long newrl, long newcl) ; 

//opencv‚ðŽg‚¤‚½‚ß‚É’Ç‰Á‚µ‚½
//void make_bmp(int argc, char** argv);
void make_bmp(char date_directory2[],char Filename1[],int &image_x,int &image_y,int make_image_repeat,int Save_image_flag[][9]);

void convolution(int &image_y,int &image_x,int fs, int hfs,double *output1[],double *spfil1[],double *input_bmp[],double magnification);
void write_file(char *Filename,int &image_x,int &image_y,double *output1[],int Rvector_create,int Rvector_pointX,int Rvector_pointY,double Rvector[],int Rvector_number);

void Rvector_size();






double **convert_matrix(double *a, long nrl, long nrh, long ncl, long nch); 
double ***f3tensor(long nrl, long nrh, long ncl, long nch, long ndl, long ndh) ; 
int ***i3tensor(long nrl, long nrh, long ncl, long nch, long ndl, long ndh) ; 

void free_vector(double *v, long nl, long nh) ; 
void free_ivector(int *v, long nl, long nh) ; 
void free_cvector(unsigned char *v, long nl, long nh) ; 
void free_lvector(unsigned long *v, long nl, long nh) ; 
void free_dvector(double *v, long nl, long nh) ; 
void free_matrix(double **m, long nrl, long nrh, long ncl, long nch) ; 
void free_dmatrix(double **m, long nrl, long nrh, long ncl, long nch) ; 
void free_imatrix(int **m, long nrl, long nrh, long ncl, long nch) ; 
void free_submatrix(double **b, long nrl, long nrh, long ncl, long nch) ; 
void free_convert_matrix(double **b, long nrl, long nrh, long ncl, long nch) ; 
void free_f3tensor(double ***t, long nrl, long nrh, long ncl, long nch, 
		long ndl , long ndn) ; 
void free_i3tensor(int ***t, long nrl, long nrh, long ncl, long nch, 
		long ndl , long ndn) ; 

#else /* ANSI */ 
/* traditional - K&R */ 

void nrerror() ; 
double *vector() ; 
double **matrix() ; 
double **submatrix() ; 
double **convert_matrix() ; 
double ***f3tensor() ; 
int ***i3tensor() ; 
double *dvector() ; 
double **dmatrix() ; 
int *ivector() ; 
int **imatrix() ; 
unsigned char *cvector() ; 
unsigned long *lvector() ; 
void free_vector() ; 
void free_dvector() ; 
void free_ivector() ; 
void free_cvector() ; 
void free_lvector() ; 
void free_matrix() ; 
void free_submatrix() ; 
void free_convert_matrix() ; 
void free_dmatrix() ; 
void free_imatrix() ; 
void free_f3tensor() ; 
void free_i3tensor() ; 

#endif /* ANSI */ 

#endif /* _NR_UTILS_H_ */ 