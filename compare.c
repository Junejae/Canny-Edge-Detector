// Comparing two images
// Junejae Kim
// 9/06/2020

#include <stdio.h>
#include <math.h>

#define  PICSIZE 256

int main(int argc, char **argv)
{
    int     i,j,counter,percentage;
    FILE    *f1, *f2, *fopen();
    char    *foobar;

    int     pic1[PICSIZE][PICSIZE];
    int     pic2[PICSIZE][PICSIZE];
    
    argc--; argv++;
    foobar = *argv;
    f1=fopen(foobar,"rb");

    argc--; argv++;
    foobar = *argv;
    f2=fopen(foobar,"rb");

    // Load the image
    for (i=0;i<15;i++)
    {
        getc(f1);
        getc(f2);
    } // cut off the header
    
    for (i=0;i<256;i++)
    {   for (j=0;j<256;j++)
            {
                pic1[i][j]  =  getc (f1);
                pic1[i][j]  &= 0377; // clean the data with 1111 1111
                pic2[i][j]  =  getc (f2);
                pic2[i][j]  &= 0377; // clean the data with 1111 1111
            }
    }

    fclose(f1);
    fclose(f2);

    // comparing
    counter = 0;
    for (i=0;i<PICSIZE;i++)
    {
        for (j=0;j<PICSIZE;j++)
        {
            if (pic1[i][j] != pic2[i][j]) counter++;
        }
    }

    if (counter == 0)
    {
        printf("They are identical!\n");
    }
    else
    {
        percentage = (int)( 100*((double)counter / (PICSIZE*PICSIZE)));

        printf("Difference: %d ; They are %d percent different!\n", counter, percentage);
    }

    return 0;
}