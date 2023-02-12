#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_HEIGHT 8192
#define MAX_WIDTH 8192


#define MAX_PIXELS_PER_LINE 1000

#define MIN( x, y ) ( ( x ) < ( y ) ? ( x ) : ( y ) )



int theImageArray[8192][MAX_WIDTH];
int theImageArrayDup[8192][MAX_WIDTH];


FILE* read_pgm_file_info
(
    int* height,    
    int* width,    
    int* maxPixel,  
    char* pgmFname   
)
{
    static FILE* fp = 0L ;
    char pgmFormatFlag[3] =  { '\0' } ;

    char trash[80] = { '\0' } ;
    memset ( ( void * ) trash, '\0', 80 ) ;

    fp = fopen ( pgmFname, "r" ) ;

    if ( fp )
    {

        fscanf ( fp, "%2c\n", pgmFormatFlag ) ;
        if(1)
        {
            fscanf ( fp, "%i", width ) ;

            fscanf ( fp, "%i", height ) ;

            fscanf ( fp, "%i", maxPixel ) ;
        }
    }


    return fp ;
}


void write_pgm_file_from_array
(
    char* pgmOutFileName,     
    int imageArray[8192][MAX_WIDTH], 
    char* commentLine,      
    int height,          
    int width,          
    int maxPixel         
)
{
    int row = 0 ;
    int col = 0 ;
    FILE* fp = fopen ( pgmOutFileName, "w" ) ;
    if ( fp )
    {
        fprintf ( fp, "P2\n" ) ;
        fprintf ( fp, "%i %i\n", width, height ) ;
        fprintf ( fp, "%i\n", maxPixel ) ;

        for ( row = 0 ; row < height ; row ++ )
        {
            for ( col = 0 ; col < width ; col ++ )
            {

                fprintf ( fp, "%i", imageArray[row][col] ) ;
                if ( MAX_PIXELS_PER_LINE > 1 )
                {
                    fprintf ( fp, " " ) ;
                }
                if ( ( col % MAX_PIXELS_PER_LINE ) == 0 )
                {
                    fprintf ( fp, "\n" ) ;
                }
            }
            if ( col % MAX_PIXELS_PER_LINE )
            {
                fprintf ( fp, "\n" ) ;
            }
        }
        fclose ( fp ) ;
    }
    return ;
}


void read_pgm_file_into_array
(
    int imageArray[8192][MAX_WIDTH],          
    int* height,        
    int* width,        
    int* maxPixel,              
    char* pgmInFileName      
)
{
    int row = 0 ;
    int col = 0 ;
    FILE* fp = read_pgm_file_info( height, width, maxPixel, pgmInFileName );
    char trash = ' ';
    char yesThreshold = ' ';

    if ( fp )
    {
        printf ( "reading height=%d, width=%d\n", *height, *width ) ;
        for ( row = 0 ; row < MIN( MAX_HEIGHT - 1, *height ) ; row ++ )
        {
            for ( col = 0 ; col < MIN( MAX_WIDTH -1, *width ) ; col ++ )
            {
                fscanf ( fp, "%i", &imageArray[row][col]) ;
            }
        }
        fclose ( fp ) ;
    }

    return ;
}

void lowerletter(char* myString){
    for(int i = 0; i < strlen(myString); i++){
        myString[i] = tolower(myString[i]);
    }
}

void conMatrixMult(int theImageArray[8192][8192], int theImageArrayDup[8192][8192], int myMatrix[3][3], int i, int j, int divisor, int subtractor){
    theImageArray[i][j] = (theImageArray[i+1][j+1]*myMatrix[0][0] + theImageArray[i][j+1]*myMatrix[0][1]+ theImageArray[i-1][j+1]*myMatrix[0][2]+theImageArray[i+1][j]*myMatrix[1][0]+theImageArray[i][j]*myMatrix[1][1]+theImageArray[i-1][j]*myMatrix[1][2]+theImageArray[i+1][j-1]*myMatrix[2][0]+theImageArray[i][j+1]*myMatrix[2][1]+theImageArray[i-1][j-1]*myMatrix[2][2])/divisor - subtractor;
}


void convolve(int theImageArray[8192][8192], int theImageArrayDup[8192][8192], int height, int width){
    double start, end;
    double time_spent;
    int boxBlur[3][3] = {
        { 1, 1, 1 },
        { 1,  1, 1 },
        { 1, 1, 1 }
    };

    int gaussian[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
    };

     int edgeDetect[3][3] = {
        {-1, -1, -1},
        {-1,  5, -1},
        {-1, -1, -1}
    };

    int sharpen[3][3] = {
        {0, -1,  0},
        {-1, 5, -1},
        {0, -1,  0}
    };


    char convolution[30];
    printf("Would you like to run box blur, Gaussian blur, edge detection, or sharpen?\n(Box, Gaussian, Edge, Sharpen) \n");
    scanf("%s", convolution);
    lowerletter(convolution);


    start =  omp_get_wtime();
    if(strcmp(convolution, "box") == 0){
        #pragma omp parallel for
        for (int i = 1; i < MAX_HEIGHT; i++) {
            for (int j = 1; j < MAX_WIDTH; j++) {
                conMatrixMult(theImageArray, theImageArrayDup, boxBlur, i, j, 9, 0);

            }
        }
    }

    if(strcmp(convolution, "gaussian") == 0){
        #pragma omp parallel for
        for (int i = 1; i < MAX_HEIGHT; i++) {
            for (int j = 1; j < MAX_WIDTH; j++) {
                conMatrixMult(theImageArray, theImageArrayDup, gaussian, i, j, 16, 0);

            }
        }
    }
    if(strcmp(convolution, "edge") == 0){
        #pragma omp parallel for
        for (int i = 1; i < MAX_HEIGHT; i++) {
            for (int j = 1; j < MAX_WIDTH; j++) {
                conMatrixMult(theImageArray, theImageArrayDup, edgeDetect, i, j, 128, 0);

            }
        }
    }
    if(strcmp(convolution, "sharpen") == 0){
        #pragma omp parallel for
        for (int i = 1; i < MAX_HEIGHT; i++) {
            for (int j = 1; j < MAX_WIDTH; j++) {
                conMatrixMult(theImageArray, theImageArrayDup, sharpen, i, j, 8, -50);
                /*if((2*theImageArray[i][j])<= 255){
                    theImageArray[i][j] = (2*theImageArray[i][j]);
                }
                else{
                    theImageArray[i][j] = 255;
                }*/
                theImageArray[i][j] = 2*(theImageArray[i][j] -50) + 100;
                if (theImageArray[i][j] > 255) {
                    theImageArray[i][j] = 255;
                }
            }

        }
    printf("Image is washed out, sorry. Couldn't figure out a way around that. It is sharpened though.");
    }
    end =  omp_get_wtime();
    time_spent = end - start;
    printf("Time spent parallel: %f seconds\n", time_spent);
}




int main( void )
{
    int height  = 0 ;
    int width = 0 ;
    int maxPixel = 0 ;
    double time_spent;

    double time_parallel,time_seq;

    char* pgmInFileName = "./images/irv.pgm" ;
    char* pgmOutFileName = "./images/irv_out.pgm" ;

    
    read_pgm_file_into_array(theImageArray, &height, &width, &maxPixel, pgmInFileName ) ;

    for(int i = 0; i< 8192; i++){
        for(int j =0; j< 8192; j++){
            theImageArrayDup[i][j] = theImageArray[i][j];
        }
    }


    convolve(theImageArray, theImageArrayDup, height, width);


    write_pgm_file_from_array ( pgmOutFileName, theImageArray, "# JR test file", 8192, 8192,
                                maxPixel ) ;

    return 0 ;
}