#include <stdlib.h>
#include <string.h>
#include <allegro.h>

int main(int argc, char **argv)
{
    if((argc > 1 && argc < 7))  {		// check for valid # of args
	int i, y, PixPerCol, PixPerRow, offset, maxcolor;
	int Columns, Rows, seed = -1;
	long imgpos, bufpos;
    char *patdata, *outfile = "OUT.BMP";

	BITMAP *BackImg, *Image, *Stereogram;
	PALLETE Pal;

    allegro_init();				// init Allegro Graphics
    install_keyboard();

    if(set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0))  {	// 640x480x8
    	printf("***Error: VESA not detected!\n");
        readkey();
        return 0;
    }

	if(!(Image = load_bitmap(argv[1], Pal)))  {	// load stereo image
    	printf("Error opening file %s", argv[1]);
        readkey();
     	return 1;
	}

    if(argc >= 3)					// if there is an outfile
    	strcpy(outfile, argv[2]);

    if(argc >= 4)  {				// if there is a covering graphic, load it
        if(isalpha(argv[3][0]))  {
    		if(!(BackImg = load_bitmap(argv[3], Pal)))  {
    			printf("Error opening file %s", argv[3]);
                readkey();
        		return 1;
	    	}
        }
        else  {
            seed = atoi(argv[3]);		// if there is a number in the cmd line
            srand(seed);
        }
    }
    else						// otherwise, use 0 as the seed
    	seed = 0;

    if(argc >= 5)					// reset the columns to the cmd line vals
    	Columns = atoi(argv[4]);
    else
    	Columns = 12;

    if(argc == 6)					// reset the rows to the cmd line vals
    	Rows = atoi(argv[5]);
    else
    	Rows = 6;

	PixPerCol = (int)(Image->w / Columns);	// calc pixels/col and pixels/row
   	PixPerRow = (int)(Image->h / Rows);
    patdata = (char *)malloc(PixPerCol);

   	set_palette(Pal);
    blit(Image, screen, 0,0, 0,0, Image->w,Image->h);
    Stereogram = create_sub_bitmap(screen, 0, 0, Image->w + PixPerCol, Image->h);

    if(seed == -1)					// only if there is a foreground image
        for(i=0;i<Rows*PixPerRow;i+=PixPerRow)
        	stretch_blit(BackImg, Stereogram, 0,0, BackImg->w, BackImg->h, 0,i, PixPerCol, PixPerRow);

    for(y=0;y<Image->h;y++)  {			// draw the stereogram over the picture ...
        bufpos = 0, imgpos = 0;

        if(seed != -1)  {				// ... or the foreground picture data
            for(i=0;i<PixPerCol;i++)  {
            	patdata[i] = rand()%16;
        		putpixel(screen, bufpos++, y, patdata[i]);
            }
        }
        else
        	bufpos = PixPerCol;

		for(i=0;i<Image->w;i++)			// calc pixel offset + put it there
		{
			offset = getpixel(Image, imgpos++, y);
            putpixel(Stereogram, bufpos++, y, getpixel(Stereogram, bufpos - PixPerCol + offset, y));
		}
	}

    save_bitmap(outfile, Stereogram, Pal);	// save the stereogram
    readkey();
    }								// if args not right, show usage:
    else  {	
    printf("\nSyntax: sgrammer <Image> (Out File) (Repeated Image) (Columns) (Rows)\n\n");
    printf("<Image>: picture to stereogramalize (?!)\n");
    printf("(Out File) (opt.): filename to output stereogram as (default: OUT.BMP)\n");
    printf("(Repeated Image) (opt.): picture to repeat for foreground (or a num for a SIRD)\n");
    printf("(Columns) (opt.): number of columns (default: 12)\n");
    printf("(Rows) (opt.): number of rows (default: 6)\n\n");
    printf("-----------------------------------------------\n");
    printf("Stereogrammer v0.8 (freeware) by Daniel Wilhelm\n");
    }

    return 0;
}

	
	
	
	