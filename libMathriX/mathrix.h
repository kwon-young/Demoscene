#ifndef MATHRIX___
#define MATHRIX___

#define MATHRIX_ICON    10101010 //Win32 mathrix icon included in lib

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef mathrix_inline
#define mathrix_inline
#endif


#ifdef __cplusplus
extern "C" {
#endif

///Rounding
int roundi(double n);
unsigned int roundu(double n);
//long long int roundil(long double n);
//long long unsigned int roundul(long double n);
int roundif(float n);


///Angle
#define PI  3.1415926536
#define PI2 6.2831853072

double deg_to_rad(double deg);
int deg_to_radi(int deg);
float deg_to_radf(float deg);
//long double deg_to_radl(long double deg);

double rad_to_deg(double rad);
int rad_to_degi(int rad);
float rad_to_degf(float rad);
//long double rad_to_degl(long double rad);


///Divisions
unsigned int gcd(unsigned int a,unsigned int b);


///STD Random
void init_random();
double random(double a, double b);
float frandom(float a, float b);
int irandom(int a, int b);

///Numbers
#define SQRT2 = 1.4142136;


///Vectors
//Vector 2
typedef struct
{
	int x;
	int y;
} vec2i;

typedef struct
{
	float x;
	float y;
} vec2f;

typedef struct
{
	double x;
	double y;
} vec2;

vec2i add_vec2i(vec2i a, vec2i b);
vec2f add_vec2f(vec2f a, vec2f b);
vec2 add_vec2(vec2 a, vec2 b);

//Vector 3

typedef struct
{
	int x;
	int y;
	int z;
} vec3i;

typedef struct
{
	float x;
	float y;
	float z;
} vec3f;

typedef struct
{
	double x;
	double y;
	double z;
} vec3;

vec3i add_vec3i(vec3i a, vec3i b,vec3i c);
vec3f add_vec3f(vec3f a, vec3f b, vec3f c);
vec3 add_vec3(vec3 a, vec3 b, vec3f c);


///STR Crypt
void first_init_str_crypt();
void begin_str_crypt(const char* key);
char get_current_crypt_char();
char next_crypt_char();
void free_str_crypt();

//Special BDE NE666 decrypt ;)
void begin_ne666_str_crypt();
char decrypt_ne666_char(char in);
char encrypt_ne666_char(char in);
void encrypt_ne666_str(char* str,unsigned int len);
void decrypt_ne666_str(char* str,unsigned int len);


///Matrix
typedef struct {
	unsigned int m;
	unsigned int n;
	float* mat; 
} matrix;


//M lines N column (lines by lines)
matrix* initMatrix();
void printMatrix(const matrix* m);
void setDimMatrix(matrix* mt,unsigned int m, unsigned int n);
void scanMatrix(matrix* mt, const char * fileName);
void zerosMatrix(matrix *mt);
void onesMatrix(matrix *mt);
matrix* sumMatrix(const matrix *m1, const matrix* m2);
matrix* extDotMatrix(float lambda ,const matrix *mt);
matrix* dotMatrix(const matrix *m1, const matrix* m2);
matrix* transposeMatrix(const matrix* mt);
matrix* downMatrix(const matrix* mt, unsigned int k, unsigned int l);
void freeMatrix(matrix* m);
double detMatrix(const matrix* mt);
matrix* comMatrix(const matrix* mt);
matrix* inverseMatrix(const matrix* mt);


///Interpolation
double linear_interpolation(double x, double a,double b);
float linear_interpolationf(float x, float a,float b);
vec2 linear_interpolation2D(double x, vec2 a, vec2 b);
vec2f linear_interpolation2Df(float x, vec2f a, vec2f b);
unsigned char gate(double x, double a, double b);
unsigned char gatef(float x, float a, float b);
unsigned char gatei(int x, int a, int b);


///STR Numbers Operations
//String Number Operator Type  (a, b , base)
typedef char* (*rstr_op) (const char*, const char*, const unsigned int); 
//perform your wanted operation without "pre-reversing" your string
char* unreversed_string_operation(rstr_op op ,const char* a, const char *b, unsigned int base);

//Addition on reversed strings
char* reversed_string_operator_add(const char* a,const char *b,const unsigned int base);
//Binary base optimised 
char* binary_reversed_string_operator_add(const char* a, const char *b);



//Reverse your string number
char* get_reversed_string(const char* str);
void reverse_string(char* str);

///About 
void printf_about_mathrix(void);

#ifdef __cplusplus
} //extern "C"
#endif
#endif