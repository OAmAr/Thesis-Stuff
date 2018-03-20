#include <stdio.h>
#include <math.h>
#include <string.h>
/*

#define P00 1.0-Q
#define P11 1.0-Q

#define P10 Q
#define P01 Q
#define PPM Q 

#define P0P .5
#define P1P .5
*/

#define STEP 1e-3
#define Q 0.021

double limit; 
double P00 = .5*(1.0-Q);
double P11 = .5*(1.0-Q);
double P10 = .5*Q;
double P01 = .5*Q;
double PPM = Q;
//Eve
double P0P = 1/2.0;
double P1P = 1/2.0;

double calc_blam1(double big_lambda2);
int check_blam1(double big_lambda1);
double calc_lambda1(double blamda1);
double calc_lambda2(double blamda2);
double h_entropy(double x);
double s_entropy(double blam2);

double calc_blam1(double big_lambda2){
    return 1.0 - 2.0*PPM - (P0P - 1/2.0) - (P1P - 1/2.0) - big_lambda2;
}

int check_blam1(double big_lambda1) { 
    return fabs(big_lambda1) <= 2*sqrt(P00*P11); 
}

double calc_lambda1(double blambda1){
    double determ, denom;
    determ = sqrt((P00-P11)*(P00-P11) + 4*blambda1*blambda1);
    denom = 4.0 * (P00+P11);

    double ret = 1/2.0 + determ/denom;
    return ret;

}
double calc_lambda2(double blambda2){
    double determ, denom;
    determ = sqrt((P01-P10)*(P01-P10) + 4*blambda2*blambda2);
    denom = 4.0 * (P01+P10);

    double ret = 1/2.0 + determ/denom;
    return ret;

}

double h_entropy(double x){
    double ret;
    if (fabs(x)<0.0000001 || fabs(x) > 1-0.00000001)
        ret = 0.0;
    else
        ret = -1*x*log2(x)-(1-x)*log2(1-x);
    return ret;
}


double s_entropy(double blam2){
    double blam1 = calc_blam1(blam2);
    if(!check_blam1(blam1)) {
       fprintf(stderr,"\n[-] Blam1 = %lf for Blam2 = %lf, erroring\n", blam1, blam2);
        return -1;
    }
    double stage1 = ((P00 + P11)) * (h_entropy(P00/(P00+P11)) - h_entropy(calc_lambda1(blam1)));
    double stage2 = ((P01 + P10)) * (h_entropy(P01/(P01+P10)) - h_entropy(calc_lambda2(blam2)));
    return stage1+stage2;
}
void print_params(){
    printf("    [-] P00 = %lf   P11 = %lf\n", P00, P11);
    printf("    [-] P01 = %lf   P10 = %lf   P+- = %lf\n", P01, P10, PPM);
    printf("    [-] P0+ = %lf   P1+ = %lf\n", P0P, P1P);
    printf("    [-] Testing Lambda_2 in [-%lf, %lf] with steps %lf\n", limit, limit, STEP);
    
}
void change_params(){
    printf("[+] Current Parameters:\n");
    print_params();
    printf("[+] End Parameters\n");

    printf("P00="); scanf("%lf", &P00);
    printf("P11="); scanf("%lf", &P11);
    printf("P10="); scanf("%lf", &P10);
    printf("P01="); scanf("%lf", &P01);
    printf("P+-="); scanf("%lf", &PPM);
    printf("P0+="); scanf("%lf", &P0P);
    printf("P1+="); scanf("%lf", &P1P);
    limit = sqrt(P10*P01);
    
    printf("[+ Enter New Parameters\n");
}
int main(int argc, char* argv[]){
    double min_ent = 1;
    double best_lam2, s_ent;
    limit = 2*sqrt(P01*P10);
    if (argc > 1){
        if (!strcmp(argv[1], "-c")){
            change_params();
        }else{
            printf("Usage:./%s -c(optional to change default params)\n", argv[0]);
            return -1;
        }
    }

    printf("[+] Start Parameters: \n");
    print_params();
    printf("[+] End Parameters \n\n");

    for (double big_lam2 = -1*limit; big_lam2 <= limit; big_lam2+=STEP){
        s_ent = s_entropy(big_lam2);
        if (s_ent> -.5 && s_ent < min_ent){
            min_ent = s_ent;
            best_lam2 = big_lam2;
            fprintf(stderr,"\n[+] Found new best Lambda2 %lf S(A|E) <= %lf\n", best_lam2, min_ent);
        }else{
            fprintf(stderr,"\n[-] Got S(A|E) <= %lf for Lambda2 = %lf\n", s_ent, big_lam2);
        }
    }
    printf("\n[+] Minimal Upper Bound on S(A|E) <= %lf with Lambda2= %lf\n",min_ent, best_lam2);
    printf("1-h(%lf) = %lf\n",Q, 1-h_entropy(Q));
    return 0;
}










