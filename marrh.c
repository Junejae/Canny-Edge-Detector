// Original code is from Dr. Niels Da Vitoria Lobo
// Modified by Junejae Kim
// 9/06/2020

#include <stdio.h>                  /*  Marr-Hildreth.c  (or marrh.c) */
#include <math.h>
#include <stdlib.h> // added to use atoi()

#define  PICSIZE 256
#define  MAXMASK 100

         int    pic[PICSIZE][PICSIZE];
         double outpic1[PICSIZE][PICSIZE];
         double outpic2[PICSIZE][PICSIZE];
         double outpicx[PICSIZE][PICSIZE];
         double outpicy[PICSIZE][PICSIZE];
         int    edgeflag[PICSIZE][PICSIZE];
         double mask[MAXMASK][MAXMASK]; // need to be doubled for x and y
         double maskx[MAXMASK][MAXMASK];
         double masky[MAXMASK][MAXMASK];
         double conv[PICSIZE][PICSIZE];

int main(argc,argv)
int argc;
char **argv;
{
        int     i,j,p,q,mr,centx,centy;
        double  maskval,sumx,sumy,sig,maxival;
        FILE    *fo1, *fo2, *fo3, *fp1, *fopen();
        char    *foobar;

        argc--; argv++;
        foobar = *argv;
        fp1=fopen(foobar,"rb"); // for original image

        argc--; argv++;
        foobar = *argv;
        fo1=fopen(foobar,"wb"); // for magnitude image

        /* 
        argc--; argv++;
        foobar = *argv;
        fo2=fopen(foobar,"wb"); // for peak image
         */

        /* 
        argc--; argv++;
        foobar = *argv;
        fo2=fopen(foobar,"wb"); // for final image
         */

        sig = 1.0; // sig should be 1 in this assignment

        mr = (int)(sig * 3);
        centx = (MAXMASK / 2);
        centy = (MAXMASK / 2);

        // Load the image
        for (i=0;i<15;i++) // cut off the header
          getc(fp1);
        
        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
                {
                  pic[i][j]  =  getc (fp1);
                  pic[i][j]  &= 0377; // clean the data with 1111 1111
                }
        }

        printf("Gaussian calculation...\n");
        // change derivative to first derivative
        // also, double it for x and y mask
        // done
        for (p=-mr;p<=mr;p++)
        {  for (q=-mr;q<=mr;q++)
           {
              maskval = p*(exp(-1*(((p*p)+(q*q))/(2*(sig*sig)))));
              maskx[p+centy][q+centx] = maskval;

              maskval = q*(exp(-1*(((p*p)+(q*q))/(2*(sig*sig)))));
              masky[p+centy][q+centx] = maskval;
           }
        }
        
        printf("Convolution...\n");
        // also, double it up for x and y mask
        // done
        for (i=mr;i<=255-mr;i++)
        { for (j=mr;j<=255-mr;j++)
          {
             sumx = 0;
             sumy = 0;
             for (p=-mr;p<=mr;p++)
             {
                for (q=-mr;q<=mr;q++)
                {
                   sumx += pic[i+p][j+q] * maskx[p+centy][q+centx];
                   sumy += pic[i+p][j+q] * masky[p+centy][q+centx];
                }
             }
             outpicx[i][j] = sumx;
             outpicy[i][j] = sumy;
          }
        }

        // bring the sqrt code from sobel.c to calculat the magnitude
        maxival = 0;
        for (i=mr;i<256-mr;i++)
        { for (j=mr;j<256-mr;j++)
          {
             conv[i][j]=sqrt((double)((outpicx[i][j]*outpicx[i][j]) +
                                      (outpicy[i][j]*outpicy[i][j])));
             if (conv[i][j] > maxival)
                maxival = conv[i][j];
           }
        }

        // Add header to see it on image viewer
        fprintf(fo1, "P5\n");
        fprintf(fo1, "%d %d\n", PICSIZE, PICSIZE);
        fprintf(fo1, "%d\n", PICSIZE-1);

        printf("Producing magnitude image...\n");
        // produce mag image
        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
          {
            conv[i][j] = (conv[i][j] / maxival) * 255;
            fprintf(fo1,"%c",(char)((int)(conv[i][j])));
          }
        }

        fclose(fp1);
        fclose(fo1);
        
        return 0;
}

