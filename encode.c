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
     printf("\nEncoding...\n");
    // copy bmp header
    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_failure)
    {
        printf("\nERROR : unable to copy BMP Header\n");
        return e_failure;
    }

    // encode magic string
    if(encode_magic_string(MAGIC_STRING , encInfo)==e_failure)
    {
        printf("\nERROR : unable to encode magic string\n");
        return e_failure;
    }

    // encode_secret_file_extention size
    if(encode_secret_file_extn_size(encInfo)==e_failure)
    {
        printf("\nERROR : unable to encode secret file extention size\n");
        return e_failure;
    }

    // encode_secret_file_extention 
    if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo)==e_failure)
    {
        printf("\nERROR : unable to encode secret file extention\n");
        return e_failure;
    }

    // encode_secret_file_size
    if(encode_secret_file_size(encInfo->size_secret_file, encInfo)==e_failure)
    {
        printf("\nERROR : unable to encode secret file size\n");
        return e_failure;
    }

    // encode_secret_file_data
    if(encode_secret_file_data(encInfo)==e_failure)
    {
        printf("\nERROR : unable to encode secret file data\n");
        return e_failure;
    }

    // copy remaining img data
    copy_remaining_img_data(encInfo->fptr_src_image ,encInfo->fptr_stego_image);


    //printf("\nEncoded successfuly.\n");
    return e_success;
}
Status check_capacity(EncodeInfo*encInfo)
{
    
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    

    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

    
    if(encInfo->image_capacity<((14+encInfo->size_secret_file)*8))
    {
        return e_failure;
    }

    printf("\n[ SUCCESS ] Sufficient image capacity available \n");
    return e_success;

}
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    fseek(fptr_image, 18, SEEK_SET);

    fread(&width, sizeof(int), 1, fptr_image);
   
    fread(&height, sizeof(int), 1, fptr_image);

    return width * height * 3;
}


uint get_file_size(FILE *fptr)
{
    
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    
    rewind(fptr_src_image);

    char buff[54];

    
    if(fread(buff,54,1,fptr_src_image)!=1)
    {
        printf("\nERROR : failed tp read BMP header\n");
        return e_failure;
    }

    
    if(fwrite(buff,54,1,fptr_dest_image)!=1)
    {
        printf("\nERROR : failed tp write BMP header\n");
        return e_failure;
    }

    printf("\n[ SUCCESS ] BMP header copied\n");
    return e_success;
}


Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    char image_buffer[8];

    for(int i=0;magic_string[i]!=0;i++)
    {
        
        if(fread(image_buffer,8,1,encInfo->fptr_src_image)!=1)
        {
            printf("\nERROR : Failed to read image data\n");
            return e_failure;
        }

        
        if(encode_byte_to_lsb(magic_string[i], image_buffer)==e_failure)
        {
            return e_failure;
        }

        
        if(fwrite(image_buffer,8,1,encInfo->fptr_stego_image)!=1)
        {
            printf("\nERROR : Failed to write image data\n");
            return e_failure;
        }
    }
    
    printf("\n[ SUCCESS ] Magic string encoded\n");
    return e_success;
}


Status encode_byte_to_lsb(unsigned char data, char *image_buffer)
{
    if (image_buffer == NULL)
    {
        printf("\nERROR : Invalid image buffer\n");
        return e_failure;
    }

    for(int i=7;i>=0;i--)
    {
        
        if((data>>i)&1) 
        {
            image_buffer[7-i]=image_buffer[7-i] | 1;
        }
        else 
        {
            image_buffer[7-i]=image_buffer[7-i] & ~1;
        }
    }
    return e_success;
}

//-------------------------------------------------------------------------------//

Status encode_secret_file_extn_size(EncodeInfo *encInfo)
{
    
    char *dot = strrchr(encInfo->secret_fname,'.');
    strcpy(encInfo->extn_secret_file,dot);

    char buffer[32];

    
    fread(buffer,32,1,encInfo->fptr_src_image);
    
    
    if(encode_size_to_lsb(strlen(encInfo->extn_secret_file),buffer)==e_failure)
    {
        return e_failure;
    }
    
    
    fwrite(buffer,32,1,encInfo->fptr_stego_image);

    printf("\n[ SUCCESS ] Secret file extention size encoded \n");
    return e_success;
}



Status encode_size_to_lsb(int size, char *image_buffer)
{
    if (image_buffer == NULL)
    {
        printf("\nERROR : Invalid image buffer\n");
        return e_failure;
    }

    for(int i=31;i>=0;i--)
    {
        
        if((size>>i)&1) 
        {
            image_buffer[31-i]= image_buffer[31-i] | 1;
        }
        else  
        {
            image_buffer[31-i]= image_buffer[31-i] & ~1;
        }
    }
      
    return e_success;
}


Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char buffer[8];

    for(int i=0;file_extn[i]!=0;i++)
    {
        if(fread(buffer,8,1,encInfo->fptr_src_image)!=1)
        {
            printf("\nERROR : Failed to read image data\n");
            return e_failure;   
        }

        if(encode_byte_to_lsb(file_extn[i],buffer)==e_failure)
        {
            return e_failure;
        }

        if(fwrite(buffer,8,1,encInfo->fptr_stego_image)!=1)
        {
            printf("\nERROR : Failed to write image data\n");
            return e_failure;
        }
    }  
    
    printf("\n[ SUCCESS ] Secret file extention encoded \n");
    return e_success;
}



Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    char buffer[32];

    if(fread(buffer,32,1,encInfo->fptr_src_image)!=1)
    {
        printf("\nERROR : Failed to read image data\n");
        return e_failure;
    }

    if(encode_size_to_lsb(file_size,buffer)==e_failure)
    {
        return e_failure;
    }

    if(fwrite(buffer,32,1,encInfo->fptr_stego_image)!=1)
    {
        printf("\nERROR : Failed to write image data\n");
        return e_failure;
    }
    
    printf("\n[ SUCCESS ] Secret file size encoded \n");
    return e_success;
}


Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char buffer[8];
    unsigned char data;
    rewind(encInfo->fptr_secret);
    while(fread(&data,1,1,encInfo->fptr_secret)==1)
    {
        if(fread(buffer,8,1,encInfo->fptr_src_image)!=1)
        {
            printf("\nERROR : Failed to read image data\n");
            return e_failure;
        }

        if(encode_byte_to_lsb(data,buffer)==e_failure)
        {
            return e_failure;
        }

        if(fwrite(buffer,8,1,encInfo->fptr_stego_image)!=1)
        {
            printf("\nERROR : Failed to write image data\n");
            return e_failure;
        }
    }

    printf("\n[ SUCCESS ] Secret file data encoded \n");
    return e_success;
}



Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char data;

    while(fread(&data,1,1,fptr_src)==1)
    {
        fwrite(&data,1,1,fptr_dest);
    }

    printf("\n[ SUCCESS ] Copied remaining img data \n");
}

