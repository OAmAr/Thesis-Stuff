#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define STEP 1e-3

double calc_L1(double Q, double L4){
    double L = 1 - 2*Q + L4;
    return L;
}
double calc_L2(double Q, double L4){
    double L = Q - L4;
    return L;
}
double calc_L3(double Q, double L4){
    double L = Q - L4;
    return L;
}

double calc_eAB(double L3, double L4){
    return L3+L4;
}

double calc_eN(double eAB, double N){
    double num = pow(eAB, N);
    double denom = num + pow((1-eAB), N);
    return num/denom;
}

double calc_Leq(double L1, double L2){
    double num   = L1-L2;
    double denom = L1+L2;
    return num/denom;
}
double calc_Ldiff(double L3, double L4){
    double num   = fabs(L3-L4);
    double denom = L3+L4;
    return num/denom;
}
    
double h_entropy(double x){
    double ret;
    if (fabs(x)<0.0000001 || fabs(x) > 1-0.00000001)
        ret = 0.0;
    else
        ret = -1*x*log2(x)-(1-x)*log2(1-x);
    return ret;
}
double I_entropy(double eN, double Leq, double Ldiff){
   double term1, term2,term3;
   term1 = 1-h_entropy(eN);
   term2 = (1-eN)*h_entropy( (1-Leq)/2);
   term3 = eN*h_entropy( (1-Ldiff)/2);
   return term1-term2-term3;
}

int main(int argc, char** argv){
    double Q, N, L1, L2, L3, L4, eAB, eN, Leq, Ldiff, res, min_res, best_L4;
    if(argc < 3){
        printf("Error, usage is %s Q N", argv[0]);
        return -1;
    }
    Q = atof(argv[1]);
    N = atof( argv[2]);
    min_res = 100;
    best_L4 = 100;

    for (L4 = 0; L4 <Q; L4+=STEP){

        L1 = calc_L1(Q, L4);
        L2 = calc_L2(Q, L4);
        L3 = calc_L3(Q, L4);
        Leq = calc_Leq(L1, L2);
        Ldiff = calc_Ldiff(L3, L4);
        //eAB = calc_eAB(L3, L4);
        eAB = Q;
        eN = calc_eN(eAB, N);
        res = I_entropy(eN, Leq, Ldiff); //-h_entropy(Q);
        //fprintf(stderr, "  [-] I(S:A) = %lf for L4 = %lf \n", res, L4);
        best_L4 = res < min_res ? L4  : best_L4;
        min_res = res < min_res ? res : min_res;

    }
    printf("[+] I(S:A) = %lf for L4 = %lf \n", min_res, best_L4);
    return 0;
}



