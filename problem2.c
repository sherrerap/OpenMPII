#include <omp.h>
#include <stdio.h>
#include <sys/time.h>

//  CONSTANTS
#define N1 900000
#define N2 4000
#define N3 2000
#define N4 5000
#define N5 5000
#define N6 2000000
#define N7 2000000
#define PIXMAX 10

// VARIABLES
struct timeval t0, t1;
int i, j, k;
double sum, x, sp1, sp2, sp3;
int histo[PIXMAX], image[N4][N5];
double A[N1], B[N1], C[N1], E[N1];
double D[N2][N3];
double H[N6], J[N6], N[N6];
double M[N7], P[N7], R[N7];


/*
* Procedure that displays the time (in milliseconds): pt1-pt0 together with the parameterized string: pText
*/
void time_track(char * pText, struct timeval *pt0, struct timeval *pt1)
{
    double tej;
    tej = (pt1->tv_sec - pt0->tv_sec) + (pt1->tv_usec - pt0->tv_usec) / 1e6;
    printf("%s = %10.3f\n",pText, tej*1000);
}


/*
* procedure to show the image -> global variable image[N4][N5]
*/
void show_image() {
    int i, j;
    for(i=0; i<10; i++) {
        for(j=0; j<10; j++) {
            printf("%1d",image[i][j]);
        }   
        printf("\n");
    }
    printf("\n");
}

/*
 * procedure to show vectors
 */ 
void show_vector(double *V, int L1, int L2) {
    int i;
    for (i=L1; i<L2; i++) {
        if(i%5==0) printf("\n");
        printf("%12.2f ",V[i]);
    }
    printf("\n");
}

/* 
* procedure to who the 2D matrix -> global variable D
*/
void show_matrix() {
    int i, j;
    for (i=0; i<10; i++) {
        for (j=0; j<10; j++) {
            if(j%5==0) printf("\n ");
            printf("%12.2f ",D[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

/*
* Function that calculates the scalar product of two vectors of the specified size
*/
double scalar_product (double *V1, double *V2, int lvec)
{
    int i;
    double pe = 0.0;
    for(i=0; i<lvec; i++)
    {
        pe = pe + V1[i] * V2[i];
    }
    return(pe);
}

/*
* MAIN PROGRAM
*/

int main ()
{

    /* 0. INITIALIZATION */
    // 1. vector initialization
    for(i=0; i<N1; i++) {
        A[i] = 0.0;
        B[i] = (double)(N1-i+2);
        C[i] = 1 / 10.0;
        E[i] = 0.0;
    }

    // 2. D: Initialization
    for(i=0; i<N2; i++)
        for(j=0; j<N3; j++) {
            D[i][j] = 6.0;
        }
    
    // 3. image initialization
    for(i=0; i<N4; i++)
        for(j=0; j<N5; j++) {
            if(i%3) image[i][j] = (i+j) % PIXMAX;
            else image[i][j]= (i+i*j) % PIXMAX;
        }
    
    // 4. HJN Initialization
    for(i=0; i<N6; i++) {
        H[i] = 1.0;
        J[i] = 6.0;
        N[i] = 3.0;
    }

    // 5. MPR Initialization
    for(i=0; i<N7; i++) {
        M[i] = 3.0;
        P[i] = 4.0;
        R[i] = 5.0;
    }

    /* 1. EXECUTION */

    // loop 1 
    for(i=1; i<(N1-1); i++) {
        x = B[i] / (B[i] + 1.0);
        A[i] = (x + B[i] + 1.0) / 1000.0;
        C[i] = (A[i] + C[i-1] + C[i+1]) / 3.0;
        E[i] = x * x / (x * x + 1.7);
    }
    
    // loop 2
    sum = 0.0;
    for(i=3; i<N2; i++)
        for(j=0; j<N3; j++) {
            D[i][j] = D[i-3][j] / 3.0 + x + E[i];
            if (D[i][j] < 6.5) sum = sum + D[i][j]/100.0;
        }

    // loop 3
    for(i=0; i<PIXMAX; i++) histo[i] = 0;
        for(i=0; i<N4; i++)
            for(j=0; j<N5; j++) {
                histo[image[i][j]] = histo[image[i][j]] + 1;
            }

    // loop 4
    for(i=2; i<N6; i++) {
        H[i] = 3.5 / (7.0/N[i-1] + 2.0/H[i]);
        N[i] = N[i] / (N[i]+2.5) + 3.5 / N[i];
        J[i] = (H[i-1]/N[i-2] + 3.5/H[i-1]) / (H[i-1] + N[i-2]);
    }

    // loop 5
    for(i=4; i<N7; i++) {
        M[i] = M[i] * 1.7 - P[i-4];
        R[i] = M[i-4] * 0.9 + R[i];
        P[i] = (R[i] - P[i]) / 2.0;
    }

    // functions
    sp1 = scalar_product(C,E,N1);
    sp2 = scalar_product(H,J,N6);
    sp3 = scalar_product(P,R,N7);

    /* 2. SHOW RESULTS */
    /*
    * final results: the first and last 10 elements are printed 
    * of each vector, the histogram of the image and the values of x, sum,
    * and scalar products
    */
 
    printf("A-> ");
    show_vector (A, 0, 10);
    show_vector (A, N1-10, N1);
    printf("C-> ");
    show_vector (C, 0, 10);
    show_vector (C, N1-10, N1);
    printf("E-> ");
    show_vector (E, 0, 10);
    show_vector (E, N1-10, N1);
    printf("H-> ");
    show_vector (H, 0, 10);
    show_vector (H, N6-10, N6);
    printf("J-> ");
    show_vector (J, 0, 10);
    show_vector (J, N6-10, N6);
    printf("N-> ");
    show_vector (N, 0, 10);
    show_vector (N, N6-10, N6);
    printf("M-> ");
    show_vector (M, 0, 10);
    show_vector (M, N7-10, N7);
    printf("P-> ");
    show_vector (P, 0, 10);
    show_vector (P, N7-10, N7);
    printf("R-> ");
    show_vector (R, 0, 10);
    show_vector (R, N7-10, N7);
    printf("D->\n");
    show_matrix();
    printf("El histograma de la imagen es:\n");
    for (k=0; k<PIXMAX; k++) printf("%9d", k);
    printf("\n");
    for (k=0; k<PIXMAX; k++) printf("%9d", histo[k]);
    printf("\n\n");
    printf("X = %18.2f\n", x);
    printf("SUM = %18.2f\n", sum);
    printf("C*E = %18.2f\n", sp1);
    printf("H*J = %18.2f\n", sp2);
    printf("P*R = %18.2f\n", sp3);

    return 0;
}