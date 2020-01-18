#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

void readfile();
void* multiplyC(void* );
void MulCbC();
void MulRbR();
void* multiplyR(int );

int **Arrp1, **Arrp2,**Arrp3;
int column1,row1,column2,row2;

int main()
{
    clock_t strtC, endC,strtR, endR;


    readfile();
    strtC =clock();
    MulCbC();
    endC =clock();
    printf("\n Time taken to execute Cell by Cell MUll as a whole %f ms\n",difftime(endC,strtC));


    strtR =clock();

    MulRbR();
    endR =clock();
    printf("\n Time taken to execute Row by Row MUll as a whole %f ms\n",difftime(endR,strtR));

}

void *multiplyC(void* boundries)
{
    int* q=(int*) boundries;


    for (int i = 0; i < column1; i++)
        Arrp3[q[0]][q[1]]+=Arrp1[q[0]][i]*Arrp2[i][q[1]];

}
void readfile()
{
    FILE *fp; // creation of file pointer

    fp = fopen("input.txt", "r"); // openning file on reading only

    if (fp == NULL) // checking if file is found or not
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //                                 reading first Matrix                                          //
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    fscanf(fp,"%d %d\n",&row1,&column1);  //scanning matrix size from file
    Arrp1= (int**) malloc(row1*sizeof(int*)); // allocating the size of the initalized Matrix to the Array size
    printf("The size of first matrix is %d rows and %d columns.\n", row1,column1);

    for(int i=0; i<row1; i++) // scanning the matrix from the file
    {
        Arrp1[i]= (int*) malloc(column1*sizeof(int)); // allocating the size of the initalized Matrix to the Array size
        for(int j=0; j<column1 ; j++)
        {
            if (!fscanf(fp, "%d", &Arrp1[i][j]))
                break;
        }
        fscanf(fp,"\n");

    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //                                 reading second Matrix                                          //
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    fscanf(fp,"%d %d\n",&row2,&column2);  //scanning matrix size from file
    Arrp2= (int**) malloc(row2*sizeof(int*)); // allocating the size of the initalized Matrix to the Array size

    printf("The size of second matrix is %d rows and %d columns.\n", row2,column2);

    for(int i=0; i<row2; i++) // scanning the matrix from the file
    {
        Arrp2[i]= (int*) malloc(column2*sizeof(int)); // allocating the size of the initalized Matrix to the Array size
        for(int j=0; j<column2 ; j++)
        {
            if (!fscanf(fp, "%d", &Arrp2[i][j]))
                break;
        }
        fscanf(fp,"\n");

    }
    fclose(fp);
    /////////////////////////////////////////////////////////////////////////////////////////////////
    printf("The first input Matrix is :\n"); // printing the input Matrix.
    for(int i=0; i<row1; i++)
    {
        for(int j=0; j<column1; j++)
            printf("%d ",Arrp1[i][j]);
        printf("\n");


    }
    printf("//////////////////////////////////////////\n");
    printf("The second input Matrix is :\n"); // printing the input Matrix.
    for(int i=0; i<row2; i++)
    {
        for(int j=0; j<column2; j++)
            printf("%d ",Arrp2[i][j]);
        printf("\n");


    }

    if (column1 == row2)
    {
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        //                                 creating resulting Matrix                                          //
        ///////////////////////////////////////////////////////////////////////////////////////////////////

        Arrp3= (int**) malloc(row1*sizeof(int*)); // allocating the size of the initalized Matrix to the Array size

        for(int i=0 ; i <row1; i++)
        {
            Arrp3[i]= (int*) malloc(column2*sizeof(int)); // allocating the size of the initalized Matrix to the Array size

        }
        for(int i=0; i<row1; i++)
            for(int q=0; q<column2; q++)
                Arrp3[i][q]=0;
    }
    else
        return;
}

void MulCbC()
{
    time_t srtC, endC;
    int numThreads = row1*column2, trvrs=0;
    pthread_t thrdArr[numThreads];
    srtC = clock();
    for(int i=0; i<row1; i++)
    {
        for(int j=0; j<column2; j++)
        {
            int* exceeds=(int*)malloc(2*sizeof(int));
            exceeds[0]=i;
            exceeds[1]=j;

            pthread_create(&thrdArr[trvrs++], NULL, multiplyC, exceeds);

        }

    }
    for(int i=0; i<numThreads; i++)
        pthread_join(thrdArr[i],NULL);

    endC = clock();

    printf("\nThe  output Matrix of Cell by Cell is :\n"); // printing the output Matrix.
    for(int i=0; i<row1; i++)
    {
        for(int j=0; j<column2; j++)
            printf("%d ",Arrp3[i][j]);
        printf("\n");
    }
    FILE *fp; // creation of file pointer

    fp = fopen("output.txt", "w"); // openning file on write

    if (fp == NULL) // checking if file is found or not
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //                                 printing Cell by Cell Multiplication Matrix                   //                       //
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    for(int i=0; i<row1; i++)
    {
        for(int j=0; j<column2; j++)
        {
            fprintf(fp,"%d ",Arrp3[i][j]);
        }
        fprintf(fp,"\n");
    }
    fprintf(fp,"\n END1	[elapsed time of procedure 1= %f ms]\n",difftime(endC,srtC));

    fclose(fp);
}

void MulRbR()
{
    for(int i = 0; i < row1; i++)
        for(int j=0 ; j<column2; j++)
            Arrp3[i][j]=0;


    time_t srtR, endR;
    int numThreads = row1, trvrs=0, r[row1];
    pthread_t thrdArr[numThreads];

    srtR=clock();
    for(int i=0; i<row1; i++ )
    {
        r[i]=i;
        pthread_create(&thrdArr[i], NULL, multiplyR,r[i]);
    }
    for(int i=0; i<numThreads; i++)
        pthread_join(thrdArr[i],NULL);
    endR=clock();

    printf("\nThe output of Row by Row Multiplication:\n");
    for(int i = 0; i < row1; i++)
    {
        for(int j=0 ; j<column2; j++)
            printf("%d ",Arrp3[i][j]);

        printf("\n");
    }
FILE *fp; // creation of file pointer

    fp = fopen("output.txt", "a"); // openning file on write

    if (fp == NULL) // checking if file is found or not
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //                                 printing Cell by Cell Multiplication Matrix                   //                       //
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    for(int i=0; i<row1; i++)
    {
        for(int j=0; j<column2; j++)
        {
            fprintf(fp,"%d ",Arrp3[i][j]);
        }
        fprintf(fp,"\n");
    }
    fprintf(fp,"\n END2	[elapsed time of procedure 2= %f ms]",difftime(endR,srtR));

    fclose(fp);
}

void *multiplyR(int boundries)
{
    for (int i = 0; i < column2; i++)
        for(int j=0 ; j<row2; j++)
            Arrp3[boundries][i]+=Arrp1[boundries][j]*Arrp2[j][i];
}

