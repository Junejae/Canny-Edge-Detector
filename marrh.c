// Original code is from Dr. Niels Da Vitoria Lobo
// Modified by Junejae Kim
// 9/06/2020

#include <stdio.h>                  /*  Marr-Hildreth.c  (or marrh.c) */
#include <math.h>
#include <stdlib.h> // added to use atoi()

#define  PICSIZE 256
#define  MAXMASK 100
//#define  M_PI 3.14159265358979323846

         int    pic[PICSIZE][PICSIZE];
         double xconv[PICSIZE][PICSIZE];
         double yconv[PICSIZE][PICSIZE];
         int    edgeflag[PICSIZE][PICSIZE];
         int    peak[PICSIZE][PICSIZE];
         double maskx[MAXMASK][MAXMASK];
         double masky[MAXMASK][MAXMASK];
         double mag[PICSIZE][PICSIZE];

int main(argc,argv)
int argc;
char **argv;
{
        int     i,j,p,q,mr,centx,centy,hi,lo,moretodo;
        double  maskval,sumx,sumy,sig,maxival,slope;
        FILE    *fo1, *fo2, *fo3, *fp1, *fopen();
        char    *foobar;

        argc--; argv++;
        foobar = *argv;
        fp1=fopen(foobar,"rb"); // for original image

        argc--; argv++;
        foobar = *argv;
        fo1=fopen(foobar,"wb"); // for magnitude image

        argc--; argv++;
        foobar = *argv;
        fo2=fopen(foobar,"wb"); // for peak image

        argc--; argv++;
        foobar = *argv;
        fo3=fopen(foobar,"wb"); // for final image

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
              maskval = q*(exp(-1*(((p*p)+(q*q))/(2*(sig*sig)))));
              maskx[p+centy][q+centx] = maskval;

              maskval = p*(exp(-1*(((p*p)+(q*q))/(2*(sig*sig)))));
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
             xconv[i][j] = sumx;
             yconv[i][j] = sumy;
          }
        }

        // bring the sqrt code from sobel.c to calculate the magnitude
        maxival = 0.0;
        for (i=mr;i<256-mr;i++)
        { for (j=mr;j<256-mr;j++)
          {
             mag[i][j]=sqrt((double)((xconv[i][j]*xconv[i][j]) +
                                      (yconv[i][j]*yconv[i][j])));
             if (mag[i][j] > maxival)
                maxival = mag[i][j];
           }
        }

        // Add header to see it on image viewer
        fprintf(fo1, "P5\n");
        fprintf(fo1, "%d %d\n", PICSIZE, PICSIZE);
        fprintf(fo1, "%d\n", 255);

        fprintf(fo2, "P5\n");
        fprintf(fo2, "%d %d\n", PICSIZE, PICSIZE);
        fprintf(fo2, "%d\n", 255);

        fprintf(fo3, "P5\n");
        fprintf(fo3, "%d %d\n", PICSIZE, PICSIZE);
        fprintf(fo3, "%d\n", 255);

        printf("Producing magnitude image...\n");
        // produce mag image
        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
          {
            mag[i][j] = (mag[i][j] / maxival) * 255;
            fprintf(fo1,"%c",(char)((int)(mag[i][j])));
          }
        }

        printf("Producing peak image...\n");
        // produce peak image
        for(i=mr;i<256-mr;i++){
          for(j=mr;j<256-mr;j++){
            
              if((xconv[i][j]) == 0.0) xconv[i][j] = .00001;

              slope = yconv[i][j]/xconv[i][j];

              if( (slope <= .4142)&&(slope > -.4142))
              {
                if((mag[i][j] > mag[i][j-1])&&(mag[i][j] > mag[i][j+1]))
                {
                  peak[i][j] = 255;
                }
              }
              else if( (slope <= 2.4142)&&(slope > .4142))
              {
                if((mag[i][j] > mag[i-1][j-1])&&(mag[i][j] > mag[i+1][j+1]))
                {
                  peak[i][j] = 255;
                }
              }
              else if( (slope <= -.4142)&&(slope > -2.4142))
              {
                if((mag[i][j] > mag[i+1][j-1])&&(mag[i][j] > mag[i-1][j+1]))
                {
                  peak[i][j] = 255;
                }
              }
              else
              {
                if((mag[i][j] > mag[i-1][j])&&(mag[i][j] > mag[i+1][j]))
                {
                  peak[i][j] = 255;
                }
              }
            }
        }

        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
          {
            fprintf(fo2,"%c",(char) ((peak[i][j] == 255) ? 255 : 0));
          }
        }

        printf("Producing final image...\n");
        // produce final image
        hi = 100;
        lo = 35;

        for(i=0;i<256;i++)
        { for(j=0;j<256;j++)
          {
            if (peak[i][j] == 255)
            {
              if (mag[i][j] > hi)
              {
                peak[i][j] = 0;
                edgeflag[i][j] = 255;
              }
              else if (mag[i][j] < lo)
              {
                peak[i][j] = 0;
                edgeflag[i][j] = 0;
              }
            }
          }
        }
        
        moretodo = 1;
        while (moretodo == 1)
        {
          moretodo = 0;
          for(i=0;i<256;i++)
          { for(j=0;j<256;j++)
            {
              if (peak[i][j] == 255)
              {
                for (p=-1;p<=1;p++)
                { for (q=-1;q<=1;q++)
                  {
                    if (edgeflag[i+p][j+q] == 255)
                    {
                      peak[i][j] = 0;
                      edgeflag[i][j] = 255;
                      moretodo = 1;
                    }
                  }
                }
              }
            }
          }
        }
        
        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
          {
            fprintf(fo3,"%c",(char) ((edgeflag[i][j] == 255) ? 255 : 0));
          }
        }
        

        // close files
        fclose(fp1);
        fclose(fo1);
        fclose(fo2);
        fclose(fo3);
        
        return 0;
}

