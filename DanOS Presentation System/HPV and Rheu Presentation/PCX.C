typedef unsigned char * BUFFER;
typedef unsigned short USHORT;
typedef unsigned char UCHAR;

typedef struct PICBUF  {
    BUFFER buffer;
	USHORT width;
    USHORT height;
} PICBUF;

typedef struct PCXHEAD  {
	char	manufacturer		__attribute__ ((packed));
    char	encoding			__attribute__ ((packed));
    char	bits_per_pixel		__attribute__ ((packed));
    short	x, y				__attribute__ ((packed));
    short	width, height		__attribute__ ((packed));
    short	horiz_res			__attribute__ ((packed));
    short	vert_res			__attribute__ ((packed));
    char	ega_palette[48]		__attribute__ ((packed));
    char	reserved			__attribute__ ((packed));
    char	num_color_planes	__attribute__ ((packed));
    short	bytes_per_line		__attribute__ ((packed));
    short	palette_type		__attribute__ ((packed));
    char	padding[58]			__attribute__ ((packed));
} PCXHEAD;

typedef struct RGB  {
	UCHAR red        			__attribute__ ((packed));
	UCHAR green					__attribute__ ((packed));
	UCHAR blue					__attribute__ ((packed));
} RGB;

void Get_PCX(char *filename, PICBUF *bufpicture, RGB *palette);
void Remove_PCX(PICBUF *bufpicture);

void Remove_PCX(PICBUF *bufpicture)
{
	free(bufpicture->buffer);
}

void Get_PCX(char *filename, PICBUF *bufpicture, RGB *palette)
{
    short run, i, width, height;
    long curpix = 0;
    unsigned char data;
	FILE *stream;
    PCXHEAD header;

    stream = fopen(filename, "rb");
    fseek(stream, 8, SEEK_SET);

    fread(&width , sizeof(short), 1, stream);
    fread(&height, sizeof(short), 1, stream);
    bufpicture->width = width++;
    bufpicture->height = height++;

    bufpicture->buffer = (BUFFER)xmalloc(width * height);

    fseek(stream, -768, SEEK_END);				//GET THE PALETTE DATA
	for(i=0;i<256;i++)
	{
		palette[i].red   = getc(stream) >> 2;
		palette[i].green = getc(stream) >> 2;
		palette[i].blue  = getc(stream) >> 2;
	}

    fseek(stream, 128, SEEK_SET);
    while(curpix <= width * height)  {
    	data = getc(stream);

        if((data & 0xC0) == 0xC0)  {		// if data >= 192
        	run = data - 192;
            data = getc(stream);
            while(run-- > 0)
                bufpicture->buffer[curpix++] = data;
        }
        else
        	bufpicture->buffer[curpix++] = data;
    }
}
