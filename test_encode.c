#include <stdio.h>
#include "encode.h"
#include "types.h"
OperationType check_operation(char opt);
int main(int argc,char*argv[])
{
    EncodeInfo encInfo;
    if(argc<4)
    {
        printf("Usage: %s -e <beatiful.bmp> <secret.txt>[output.bmp]",argv[0]);
        
    }
    else{
        if(check_operation(argv[1][1])==e_encode)
        {
            read_and_validate_encode_args(argv,&encInfo);
        }
        
    }
    if(read_and_validate_encode_args(argv,&encInfo)==e_success)
    {
        if(do_encoding(&encInfo)==e_success)
    }
    return 0;
}
OperationType check_operation(char opt)
{
    if(opt=='e')
    {
        return e_encode;
    }
    else if(opt=='d')
    {
        return e_decode;
    }
    else
    {
        printf("enter e or d\n");
        return e_unsupported;
    }
} 
    // uint img_size;

    // // Fill with sample filenames
    // encInfo.src_image_fname = "beautiful.bmp";
    // encInfo.secret_fname = "secret.txt";
    // encInfo.stego_image_fname = "stego_img.bmp";

    // // Test open_files
    // if (open_files(&encInfo) == e_failure)
    // {
    // 	printf("ERROR: %s function failed\n", "open_files" );
    // 	return 1;
    // }
    // else
    // {
    // 	printf("SUCCESS: %s function completed\n", "open_files" );
    // }

    // // Test get_image_size_for_bmp
    // img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
    // printf("INFO: Image size = %u\n", img_size);

 
