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


void luminosity(int theImageArray[8192][8192]) {
    char lum[10];
    double perc;
    double start, end;
    double time_spent;
    printf("Do you want to lighten or darken the image? (Lighten, Darken) \n");
    scanf("%s", lum);
    lowerletter(lum);

    printf("Enter a percentage between 0 and 100 (without the %%) of how much you want\nthe luminosity to change by. \n");
    scanf("%lf", &perc);
    perc = 0.01* perc;

    start =  omp_get_wtime();
    if(strcmp(lum, "lighten") == 0){
        perc = 1.0 + perc;

        for (int i = 0; i < MAX_HEIGHT; i++) {
            for (int j = 0; j < MAX_WIDTH; j++) {
                if(perc*theImageArray[i][j]<= 255){
                    theImageArray[i][j] = perc*theImageArray[i][j];
                }
                else{
                    theImageArray[i][j] = 255;
                }
            }
        }
    }

    if(strcmp(lum, "darken") == 0){
        perc = 1.0 - perc;

        for (int i = 0; i < MAX_HEIGHT; i++) {
            for (int j = 0; j < MAX_WIDTH; j++) {
                theImageArray[i][j] = perc*theImageArray[i][j];

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
    char* pgmOutFileName = "./images/irv_out.pgm" ;

    
    read_pgm_file_into_array(theImageArray, &height, &width, &maxPixel, pgmInFileName ) ;
 
    luminosity(theImageArray);

    write_pgm_file_from_array ( pgmOutFileName, theImageArray, "# JR test file", 8192, 8192,
                                maxPixel ) ;

    return 0 ;
}