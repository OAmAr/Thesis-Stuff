#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double calc_E1(double Qx,  double E2, double E3, double E4){
    return 1 - 2*Qx - E2 - E3 - E4;
}
double calc_E2(double Qf, double Qr){
    return 1*Qf*(1-Qr);
}
double calc_E3(double Qf, double Qr){
    return 1*Qr*(1-Qf);
}
//should E2 and E3 be negative or positive. minimal or maximal? I think postive and maximal so E1 is minimal 

double calc_M(double Qf, double Qr){
    return 2*(1-Qf)*(1-Qr)+2*Qf*Qr;
}

double calc_lambda1(double E1, double Qf, double Qr){
    double term = fabs(E1)/((1-Qf)*(1-Qr));
    return (1+term)/2.0;
}

double calc_lambda2(double E4, double Qf, double Qr){
    double term = fabs(E4)/(Qf*Qr);
    return (1+term)/2.0;
}

double h_entropy(double x){
    double ret;
    if (fabs(x)<0.0000001 || fabs(x) > 1-0.00000001)
        ret = 0.0;
    else
        ret = -1*x*log2(x)-(1-x)*log2(1-x);
    return ret;
}

double S_entropy(double M, double Qf, double Qr, double lam1, double lam2){
    double term1 = (2/M)*(1-Qf)*(1-Qr)*(1-h_entropy(lam1));
    double term2 = (2/M)*Qf*Qr*(1-h_entropy(lam2));
    return term1+term2;
}
double H_part(double x){
    return -1*x*log2(x);
}
double H_AB(double M, double Qf, double Qr){
    double summand1,summand2,summand3,summand4;
    summand1=(1-Qf)*(1-Qr)/M;
    summand2=(1-Qf)*(1-Qr)/M;
    summand3=Qr*Qf/M;
    summand4=Qr*Qf/M;
    return H_part(summand1)+H_part(summand2)+H_part(summand3)+H_part(summand4)-1.0;
}

int main(int argc, char** argv){
    double Qf, Qr, Qx, E1,E2, E3, E4, lam1, lam2, res, M, min_E4, min_res;
    if(argc <4){
        printf("Error, usage is %s Qf Qr Qx\n", argv[0]);
        return -1;
    }
    Qf = atof(argv[1]);
    Qr = atof(argv[2]);
    Qx = atof(argv[3]);
    min_E4  = INFINITY;
    min_res = INFINITY;
    double STEP = 1.0E-3;

    for(E4 = -1*Qf*Qr; E4 <= Qf*Qr; E4+=STEP){
       E3 = calc_E3(Qf, Qr); 
       E2 = calc_E2(Qf, Qr); 
       E1 = calc_E1(Qx, E2, E3, E4);
       lam1 = calc_lambda1(E1, Qf, Qr);
       lam2 = calc_lambda2(E4, Qf, Qr);
       M = calc_M(Qf, Qr);
       res = S_entropy(M, Qf, Qr, lam1, lam2) - H_AB(M, Qf, Qr); //res = res *M;
       if (min_res > res){
        //printf("E1: %lf E2: %lf E3: %lf E4: %lf M: %lf\n lam1=%lf, lam2=%lf\nS(A|B) = %lf H(AB) = %lf\n res = %lf\n", E1, E2, E3, E4, M, lam1, lam2, S_entropy(M, Qf, Qr, lam1, lam2), H_AB(M,Qf,Qr), res);
       }
       min_E4  = min_res > res ? E4 : min_E4;
       min_res = min_res > res ? res : min_res;
    }

    printf("[+] S(A|E) - H(A|B) = %lf for E4 = %lf \n", min_res, min_E4);
    return 0;

    
}


