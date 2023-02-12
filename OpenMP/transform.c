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

void transformation(int theImageArray[8192][8192], int theImageArrayDup[8192][8192], int width, int height){
    char tfm[10];
    int rotation;
    double start, end, time_spent;
    printf("Do you want to rotate the image or reflect the image? (Rotate, Reflect) \n");
    scanf("%s", tfm);
    lowerletter(tfm);

    if (strcmp(tfm, "reflect") == 0) {
        printf("Do you want the image to be reflected vertically or horizontally?\n(Vertical, Horizontal)\n");
        char direction[15];
        scanf("%s", direction);
        lowerletter(direction);
        start = omp_get_wtime();
        if(strcmp(direction, "vertical") == 0){

            #pragma omp parallel for
            for(int i = 0; i < width; i++){
                for(int j = 0; j < height; j++){
                    theImageArrayDup[i][j] = theImageArray[width-i][j];
                }
            }

            #pragma omp parallel for
            for(int i = 0; i < width; i++){
                for(int j = 0; j < height; j++){
                    theImageArray[i][j] = theImageArrayDup[i][j];
                }
            }
        }

        if(strcmp(direction,"horizontal") == 0){

            #pragma omp parallel for
            for(int i = 0; i < width; i++){
                for(int j = 0; j < height; j++){
                    theImageArrayDup[i][j] = theImageArray[i][height-j];
                }
            }

            #pragma omp parallel for
            for(int i = 0; i < width; i++){
                for(int j = 0; j < height; j++){
                    theImageArray[i][j] = theImageArrayDup[i][j];
                }
            }
        }
        end = omp_get_wtime();
    }

    if(strcmp(tfm, "rotate") == 0) {
        printf("Do you want the image rotated 90, 180, or 270 degrees clockwise? ");
        scanf("%d", &rotation);

        start = omp_get_wtime();
        if (rotation == 90){
            
            #pragma omp parallel for
            for(int i = 0; i < MAX_WIDTH; i++){
                for(int j = 0; j < MAX_HEIGHT; j++){
                    theImageArrayDup[i][j] = theImageArray[j][i];
                }
            }

            #pragma omp parallel for
            for(int i = 0; i < MAX_WIDTH; i++){
                for(int j = 0; j < MAX_HEIGHT; j++){
                    theImageArray[i][j] = theImageArrayDup[i][j];
                }
            }
        }

         if(rotation ==  180) {
            #pragma omp parallel for
            for(int i = 0; i < width; i++){
                for(int j = 0; j < height; j++){
                    theImageArrayDup[i][j] = theImageArray[width-i][height-j];
                }
            }

            #pragma omp parallel for
            for(int i = 0; i < width; i++){
                for(int j = 0; j < height; j++){
                    theImageArray[i][j] = theImageArrayDup[i][j];
                }
            }
        }

        if (rotation == 270){
            #pragma omp parallel for
            for(int i = 0; i < width; i++){
                for(int j = 0; j < height; j++){
                    theImageArrayDup[i][j] = theImageArray[height-j][i];
                }
            }

            #pragma omp parallel for
            for(int i = 0; i < width; i++){
                for(int j = 0; j < height; j++){
                    theImageArray[i][j] = theImageArrayDup[i][j];
                }
            }
        }
        end = omp_get_wtime();
    }
    time_spent = end - start;
    printf("Time spent parallel: %f\n", time_spent);
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
 
    transformation(theImageArray, theImageArrayDup, width, height);

    write_pgm_file_from_array ( pgmOutFileName, theImageArray, "# JR test file", 8192, 8192,
                                maxPixel ) ;

    return 0 ;
}