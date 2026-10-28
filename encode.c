#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
// Status open_files(EncodeInfo *encInfo)
// {
//     // Src Image file
//     encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
//     // Do Error handling
//     if (encInfo->fptr_src_image == NULL)
//     {
//     	perror("fopen");
//     	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

//     	return e_failure;
//     }

//     // Secret file
//     encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
//     // Do Error handling
//     if (encInfo->fptr_secret == NULL)
//     {
//     	perror("fopen");
//     	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

//     	return e_failure;
//     }

//     // Stego Image file
//     encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
//     // Do Error handling
//     if (encInfo->fptr_stego_image == NULL)
//     {
//     	perror("fopen");
//     	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

//     	return e_failure;
//     }

//     // No failure return e_success
//     return e_success;
// }
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    int size=strlen(argv[2]);
    if(size<4||strcmp(argv[2]+size-4,".bmp")!=0)
    {
        printf("Soruce image file have .bmp at last\n");
        return e_failure;
    }
    
        encInfo->src_image_fname=argv[2];
    
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    if(encInfo->fptr_src_image==NULL)
    {
        printf("image file does not exist\n");
        return e_failure;
    }
    fclose(encInfo->fptr_src_image);

    // secret file
    encInfo->secret_fname=argv[3];
    encInfo->fptr_secret=fopen(encInfo->secret_fname,"r");
    if(encInfo->fptr_secret==NULL)
    {
        printf("secret file doesnot exist");
        return e_failure;
    }
    fclose(encInfo->fptr_secret);
    // argument 4
    if (argv[4] != NULL)
    {
        size = strlen(argv[4]);

        if (size < 4 || strcmp(argv[4] + size - 4, ".bmp") != 0)
        {
            printf("Output image must have .bmp extension\n");
            return e_failure;
        }

        encInfo->stego_image_fname = argv[4];
    }
    else
    {
        encInfo->stego_image_fname = "beautiful.bmp";
    }
    encInfo->fptr_stego_image=fopen(encInfo->stego_image_fname,"w");

    
    return e_success;
    /*
    -check the argv[2] must be having ".bmp" extension(last 4 character)
        -if yes,//encInfo->src_image_fname=argv[2]
            -open the file(sceret.txt)in read mode argv[2]
            //encInfo->fptr_src_image=fopen()
            -if the file is not present,return failure

        -if no,return e_failure

    -check the argv[3] is valid file
    //encInfo->screct_fname=argv[3]
    is the file is present or not(read)
        -if present,encInfo->fptr-secret=fopen()
        -fi not,return e_failure

    -check the argv[4] is NULL
    if not null,enInfo->stego_image-fname=argv[4]
    if yes,enInfo->stego_image-fname="beatiful.bmp"

        -check the file extension is only of ".bmp"
            open the file in write mode,and store the file pointer to structe=re member
    */
    
}
Status do_encoding(EncodeInfo*encInfo)
{
    if(check_capacity(encInfo)==e_failure)
    {
        printf("Error!");
        return e_failure;
    }
}
Status check_capacity(EncodeInfo*encInfo)
{
    get_image_size_for_bmp(encInfo->fptr_src_image);
    if((get_file_size(encInfo->fptr_secret)+14)*8)
    {
        return e_success;

    }

}
uint get_file_size(FILE*fptr)
{
    fptr=fopen("beautiful.bmp","r");
    fseek(fptr,SEEK_END);
    return ftell(fptr);
}
