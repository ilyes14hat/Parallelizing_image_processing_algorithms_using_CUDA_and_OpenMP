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



void threshold(int theImageArray[8192][8192]) {
    int n;
    double start, end;
    double time_spent;
    printf("Enter the number of levels you wish to threshold by. \n(Integer between 2-6 inclusive). \n");
    scanf("%d", &n);

    start =  omp_get_wtime();
    for (int i = 0; i < MAX_HEIGHT; i++) {
        for (int j = 0; j < MAX_WIDTH; j++) {
            switch (n){

            case 1:
                printf("Invalid response. Please start over. /n");
                break;

            case 2:
                if (theImageArray[i][j] > (n-1)*255/n) {
                    theImageArray[i][j] = 255;
                }
                else {
                    theImageArray[i][j] = 0;
                }
                break;
            case 3:
                if (theImageArray[i][j] > (n-1)*255/n) {
                    theImageArray[i][j] = 255;
                }
                else if (theImageArray[i][j] > (n-2)*(255/n)){
                    theImageArray[i][j] = (n-2)*(255/n);
                }
                else {
                    theImageArray[i][j] = 0;
                }
                break;
            case 4:
                if (theImageArray[i][j] > (n-1)*255/n) {
                    theImageArray[i][j] = 255;
                }
                else if (theImageArray[i][j] > (n-2)*(255/n)){
                    theImageArray[i][j] = (n-2)*(255/n);
                }
                else if (theImageArray[i][j] > (n-3)*(255/n)){
                    theImageArray[i][j] = (n-3)*(255/n);
                }
                else {
                    theImageArray[i][j] = 0;
                }
                break;
            case 5:
                if (theImageArray[i][j] > (n-1)*255/n) {
                    theImageArray[i][j] = 255;
                }
                else if (theImageArray[i][j] > (n-2)*(255/n)){
                    theImageArray[i][j] = (n-2)*(255/n);
                }
                else if (theImageArray[i][j] > (n-3)*(255/n)){
                    theImageArray[i][j] = (n-3)*(255/n);
                }
                else if (theImageArray[i][j] > (n-4)*(255/n)){
                    theImageArray[i][j] = (n-4)*(255/n);
                }
                else {
                    theImageArray[i][j] = 0;
                }
                break;
            case 6:
                if (theImageArray[i][j] > (n-1)*255/n) {
                    theImageArray[i][j] = 255;
                }
                else if (theImageArray[i][j] > (n-2)*(255/n)){
                    theImageArray[i][j] = (n-2)*(255/n);
                }
                else if (theImageArray[i][j] > (n-3)*(255/n)){
                    theImageArray[i][j] = (n-3)*(255/n);
                }
                else if (theImageArray[i][j] > (n-4)*(255/n)){
                    theImageArray[i][j] = (n-4)*(255/n);
                }
                else if (theImageArray[i][j] > (n-5)*(255/n)){
                    theImageArray[i][j] = (n-5)*(255/n);
                }
                else {
                    theImageArray[i][j] = 0;
                }
                break;
            default:
                printf("Invalid response. Please start over. /n");
                break;
            }
        }
    }
    end =  omp_get_wtime();
    time_spent = end - start;
    printf("Time spent sequential: %f seconds\n", time_spent);
}



int main( void )
{
    int height  = 0 ;
    int width = 0 ;
    int maxPixel = 0 ;
    double time_spent;

    double time_parallel,time_seq;

    char* pgmInFileName = "./images/irv.pgm" ;
    char* pgmOutFileName = "./images/irv_out.pgm";

    
    read_pgm_file_into_array(theImageArray, &height, &width, &maxPixel, pgmInFileName ) ;
 
    threshold(theImageArray);

    write_pgm_file_from_array ( pgmOutFileName, theImageArray, "# JR test file", 8192, 8192,
                                maxPixel ) ;

    return 0 ;
}