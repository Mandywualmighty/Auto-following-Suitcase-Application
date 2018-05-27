#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265358979323846
#define e  2.7182818284590452354

#define fabs(a) ((a)>0?(a):(-(a)))
#define maxf(a,b) ((a) > (b) ? (a) : (b))
#define minf(a,b) ((a) < (b) ? (a) : (b))
double pow1(double a,int n);
 double powf1(double a,double x);
 double sqrt1(double x);
 double cos1(double x);
double sin1(double x);
double tan1(double x);
 double simpson1(double a, double b,int flag);
double asr1(double a, double b, double eps, double A,int flag);
 double asr11(double a, double b, double eps,int flag);
 double acos1(double x);
 double asin1(double x);
 double atan1(double x);



double pow1(double a,int n)//ʵ�����������ݣ�������
{
    if(n<0) return 1/pow1(a,-n);//nΪ��ʱ�Ĵ���
    double res = 1.0;
    while(n)
    {
        if(n&1) res *= a;
        a *= a;
        n >>= 1;
    }
    return res;
}


double F11(double x)//Ϊ��ʹ�ö����ּ���lnx
{
    return 1/x;
}

double F21(double x)//Ϊ��ʹ�ö����ּ���arcsinx
{
    return 1/sqrt1(1-x*x);
}

double simpson1(double a, double b,int flag)//����ɭ���ֹ�ʽ
{
    double c = a + (b-a)/2;
    if(flag==1)
        return (F11(a)+4*F11(c)+F11(b))*(b-a)/6;
    if(flag==2)
        return (F21(a)+4*F21(c)+F21(b))*(b-a)/6;
}

double asr1(double a, double b, double eps, double A,int flag)//����ɭ���ֹ�ʽ������Ӧ����
{
    double c = a + (b-a)/2;
    double L = simpson1(a, c,flag), R = simpson1(c, b,flag);
    if(fabs(L+R-A) <= 15*eps) return L+R+(L+R-A)/15.0;
    return asr1(a, c, eps/2, L,flag) + asr1(c, b, eps/2, R,flag);
}

double asr11(double a, double b, double eps,int flag)//����ɭ�������庯��
{
    return asr1(a, b, eps, simpson1(a, b,flag),flag);
}

double sin1(double x)
{
    double fl = 1;
    if(x>2*PI || x<-2*PI) x -= (int)(x/(2*PI))*2*PI;
    if(x>PI) x -= 2*PI;
    if(x<-PI) x += 2*PI;
    if(x>PI/2)
    {
        x -= PI;
        fl *= -1;
    }
    if(x<-PI/2)
    {
        x += PI;
        fl *= -1;
    }
    if(x<0)
    {
        x*=-1;
        fl*=-1;
    }//��������
    if(x>PI/4) return cos1(PI/2-x);
    else return fl*(x - pow1(x,3)/6 + pow1(x,5)/120 - pow1(x,7)/5040 +pow1(x,9)/362880);//̩�չ�ʽ
}

double cos1(double x)
{
    double fl = 1;
    if(x>2*PI || x<-2*PI) x -= (int)(x/(2*PI))*2*PI;
    if(x>PI) x -= 2*PI;
    if(x<-PI) x += 2*PI;
    if(x>PI/2)
    {
        x -= PI;
        fl *= -1;
    }
    if(x<-PI/2)
    {
        x += PI;
        fl *= -1;
    }//��������
    if(x>PI/4) return sin1(PI/2-x);
    else return fl*(1 - pow1(x,2)/2 + pow1(x,4)/24 - pow1(x,6)/720 + pow1(x,8)/40320);//̩�չ�ʽ
}

double tan1(double x)
{
    if(x>PI || x<-PI) x -= (int)(x/(PI))*PI;
    if(x>PI/2) x -= PI;
    if(x<-PI/2) x += PI;//��������
    return x + pow1(x,3)/3 + pow1(x,5)*2/15 + pow1(x,7)*17/315 + pow1(x,9)*62/2835;//̩�չ�ʽ
}

double asin1(double x)
{
    if(fabs(x)>1) return -1;
    double fl = 1.0;
    if(x<0) {fl*=-1;x*=-1;}
    if(fabs(x-1)<1e-7) return PI/2;//xΪ����1ʱ����
    return (fl*asr11(0,x,1e-8,2));//������ֹ���
    //return x + pow(x,3)/6 + pow(x,5)*3/40 +pow(x,7)*5/112 + pow(x,9)*35/1152 + pow(x,11)*315/1408;
}

double acos1(double x)
{
    if(fabs(x)>1) return -1;
    return PI/2 - asin1(x);//�򵥹�ʽ��Ӧ��
}

double atan1(double x)
{
    if(x<0) return -atan1(-x);
    if(x>1) return PI/2 - atan1(1/x);
    if(x>1e-3) return 2*atan1((sqrt1(1+x*x)-1)/x);//���Ƶ���С�Ա�����ʹ֮�ӽ�0����֤̩�չ�ʽ�ľ���
    return x - pow1(x,3)/3 + pow1(x,5)/5 - pow1(x,7)/7 + pow1(x,9)/9;//̩�չ�ʽ
}


double sqrt1(double x)
{
    if(x>100) return 10.0*sqrt1(x/100);//��С�Ա���ֵ
    double t = x/8 + 0.5 + 2*x/(4+x);//��������֮��Ľ��
    int c = 10;//����ʮ��
    while(c--)
    {
        t = (t+x/t)/2;
    }
    return t;
}


