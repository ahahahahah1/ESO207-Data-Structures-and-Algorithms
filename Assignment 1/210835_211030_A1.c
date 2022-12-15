#include <stdio.h>
#include <time.h>

//Fibonacci number using recursive algorithm
int RFib(long long int n)
{
    if(n==0)
    return 0;
    else if(n==1)
    return 1;
    else
    return ((RFib(n-1)+RFib(n-2))%2021);
}

//Fibonacci number using iterative algorithm
int IFib(long long int n)
{
    if(n==0)
    return 0;
    else if(n==1)
    return 1;
    else
    {
        int temp,a=0,b=1;
        for(int i=2;i<=n;i++)
        {
            temp=b;
            b=(a+b)%2021;
            a=temp;
        }
        return b;
    }
}

//function for multiplying 2X2 matrices
void matrixMultiplier(int F[2][2], int M[2][2])
{
int a = (F[0][0]*M[0][0] + F[0][1]*M[1][0])%2022;
int b = (F[0][0]*M[0][1] + F[0][1]*M[1][1])%2022;
int c = (F[1][0]*M[0][0] + F[1][1]*M[1][0])%2022;
int d = (F[1][0]*M[0][1] + F[1][1]*M[1][1])%2022;

F[0][0] = a;
F[0][1] = b;
F[1][0] = c;
F[1][1] = d;
}
 
//function for calculating power of matrix 
void powerMatrix(int F[2][2], long long int n)
{
if( n == 0 || n == 1)
    return;
int M[2][2] = {{1,1},{1,0}};
 
powerMatrix(F, n/2);
matrixMultiplier(F, F);
 
if (n%2 != 0)
    matrixMultiplier(F, M);
}
 
//Fibonacci number using matrix exponentiation 
int cleverFib(long long int n)
{
int F[2][2] = {{1,1},{1,0}};
if (n == 0)
    return 0;
powerMatrix(F, n-1);
return F[0][0];
}


int main()
{
    // freopen("input.txt","r",stdin);
    // freopen("output.txt", "w", stdout);

    clock_t start_t, end_t;
    double total_t;
    long long int n;
    int val;
    scanf("%lld",&n);

    // //executing RFib
    // start_t = clock();
    // val = RFib(n);
    // end_t = clock();
    // printf("%d\n",val);
    // total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    // // CLOCKS_PER_SEC is a constant defined in time.h and its value is 10^6.
    // printf("Total time taken by RFib: %f\n", total_t  );

    // //executing IFib
    // start_t = clock();
    // val = IFib(n);
    // end_t = clock();
    // printf("%d\n",val);
    // total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    // printf("Total time taken by IFib: %f\n", total_t  );

    //executing cleverFib
    start_t = clock();
    val = cleverFib(n);
    end_t = clock();
    printf("%d\n",val);
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    // CLOCKS_PER_SEC is a constant defined in time.h and its value is 10^6.
    printf("Total time taken by cleverFib: %f\n", total_t  );



    return 0;
}