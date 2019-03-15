//** Recovers JPG files from formatted SD card//**

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[1];

    // open input file
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    //declare buffer array
    unsigned char* buffer = malloc(513 * sizeof(unsigned char));

    //declare filename array
    char* filename = malloc(51 * sizeof(char));

    //declare image count
    int imgcount = 0;

    FILE* img = NULL;

    while (fread(buffer, 512, 1, inptr) != 0)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0 && imgcount == 0)
        {

            sprintf(filename, "%03i.jpg", imgcount);
            img = fopen(filename, "w");
            fwrite(buffer, 512, 1, img);
            imgcount++;
        }

        else if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            fclose(img);
            sprintf(filename, "%03i.jpg", imgcount);
            img = fopen(filename, "w");
            fwrite(buffer, 512, 1, img);
            imgcount++;
        }

        else if (imgcount > 0)
        {
            fwrite(buffer, 1, 512, img);
        }
    }
    free(filename);
    free(buffer);
    fclose(img);
    fclose(inptr);
    return 0;
}