

#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/*Operation type
Description : check_operation_type
sample input : argv[]
sample output : encode, decode
return values : e_success,e_failure
*/
OperationType check_operation_type(char *argv[])
{
        if(strcmp(argv[1],"-e") == 0)            //check the argv[1] is -e or -d
        return e_encode;

        else if(strcmp(argv[1],"-d") == 0)
        return e_decode;

        else
        return e_unsupported;
}

/*Read and validation
Description : read validation of args
sample input : argv[],EncodeInfo *encInfo
sample output : gives extn type
return value : e_success,e_failure
*/
Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
        //For argv[2]
        if(strcmp(strstr(argv[2],"."),".bmp") == 0)         //if .bmp extension then gives
        {
                printf("argv[2] is .bmp extention\n");
                encInfo -> src_image_fname = argv[2];
        }
        else
        return e_failure;

        //For argv[3]
        if(strcmp(strstr(argv[3],"."),".txt") == 0)               //check weather argv[3] is .c .sh .txt
        {
                printf("argv[3] is .txt extension\n");
                encInfo -> secret_fname = argv[3];
        }
        else if(strcmp(strstr(argv[3],"."),".c") == 0)
        {
                printf("argv[3] is .c extension\n");
                encInfo -> secret_fname = argv[3];
        }

        else if(strcmp(strstr(argv[3],"."),".sh") == 0)
        {
                printf("argv[3] is .sh extension\n");
                encInfo -> secret_fname = argv[3];
        }

        else
        return e_failure;

        //For argv[4]
        if(argv[4] == NULL)
        {
                encInfo -> stego_image_fname = "default.bmp";          //if null then gives gives default
        }
        else
        {
                if(strcmp(strstr(argv[4],"."),".bmp") == 0)
                {
                        printf("argv[4] is .bmp extension\n");
                        encInfo -> stego_image_fname = argv[4];
                }
                else
                return e_failure;
        }

        return e_success;
}

/*do_encoding
Description : do_encoding part
sample input : EncodeInfo *encInfo
sample output : check_capacity,copy_bmp_header,magic_string,secret_file_extn_size,secret_file_extn,secret_file,secret_file_size,copy_remaining_img_data is sucess
 */
Status do_encoding(EncodeInfo *encInfo)
{
        if(open_files(encInfo) == e_success)
        {
                printf("opening of file is successfull\n");
                if(check_capacity(encInfo) == e_success)
                {
                        printf("checking capcity is successful\n");
                        if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
                        {
                                printf("Coping of header file is successful\n");
                                if(encode_magic_string(MAGIC_STRING,encInfo) == e_success)
                                {
                                        strcpy(encInfo->extn_secret_file, strstr(encInfo->secret_fname,"."));
                                        printf("Encoding of magic string is successfull\n");
                                        if(encode_secret_file_extn_size(encInfo->extn_secret_file,encInfo) == e_success)
                                        {
                                                printf("Encoding of secret file extension size is successful\n");
                                                if(encode_secret_file_extn(strstr(encInfo->secret_fname,"."),encInfo) == e_success)
                                                {
                                                        printf("Encoding of secrete file extention is successfull\n");
                                                        if(encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_success)
                                                        {
                                                                printf("Encoding of secret file size is successfull\n");
                                                                if(encode_secret_file_data(encInfo) == e_success)
                                                                {
                                                                        printf("Encoding of secret file data is successfull\n");
                                                                        if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
                                                                        {
                                                                                printf("Encoding of remaining data is successfull\n");
                                                                                return e_success;
                                                                        }
                                                                        else
                                                                        {
                                                                                printf("Encoding of remaing file is failure\n");
                                                                                return e_failure;
                                                                        }
                                                                }
                                                                else
                                                                {
                                                                        printf("Encoding of secret file data is failure\n");
                                                                        return e_failure;
                                                                }
                                                        }
                                                        else
                                                        {
                                                                printf("Encoding of secret file size is failure\n");
                                                                return e_failure;
                                                        }
                                                }
                                                else
                                                {
                                                        printf("Encoding of secretfile extention is failure\n");
                                                        return e_failure;
                                                }
                                        }
                                        else
                                        {
                                                printf("Encoding of secret file extension size is failure\n");
                                                return e_failure;
                                        }
                                }
                                else
                                {
                                        printf("Encoding of magic string is failure\n");
                                        return e_failure;
                                }
                        }
                        else
                        {
                                printf("Coping of headerfile is failure\n");
                                return e_failure;
                        }
                }
                else
                {
                        printf("checking capcity is failed\n");
                        return e_failure;
                }
        }
        else
        {
                printf("openting of files is failed\n");
                return e_failure;
        }
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer files
 * Return Value: e_success or e_failure, on file errors
 */

//Encoding openfile
Status open_files(EncodeInfo *encInfo)
{
    // Src image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // error
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // errror
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;

    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // error
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}


/*Encoding-check_capacity
Description : check capacity
sample input : EncodeInfo *encInfo
sample output : extrn lenght, size of secret file, size of image file
return value : e_success,e e_failure
*/
Status check_capacity(EncodeInfo *encInfo)
{
        // size of secret file extention
        int extrn_length=strlen(strstr(encInfo->secret_fname,"."));

        //size of secrete file
        encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);

        //size of image file
        encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);


        if((encInfo->image_capacity-54) >= 16+32+(extrn_length*8)+32+(encInfo->size_secret_file*8))
        {
                return e_success;
        }
        else
        {
                return e_failure;
        }

}

/* get_file_size*/
unsigned int get_file_size(FILE *fptr_secret)
{
        fseek(fptr_secret,0,SEEK_END);
        return ftell(fptr_secret);
}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
` * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

//Encoding-4)check_capacity-get_image_size(image file)
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

/*Encoding-copy_bmp_header
Description : copy_bmp_header
sample input : fptr_src_image,fptr_stego_image
sample output : check the stego image is 54
return value : e_success,e_failure
*/
Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_stego_image)
{
        char buffer[54];
        rewind(fptr_src_image);
       // rewind(fptr_stego_image);
        fread(buffer,54,1,fptr_src_image);
        fwrite(buffer,54,1,fptr_stego_image);

        //fseek(fptr_stego_image,0,SEEK_END);
        //if(ftell(fptr_stego_image) == 54)
        return e_success;
        //return e_failure;
}

/*Description: encode_magic_string
sample input : magic_string,EncodeInfo *encInfo
sample output : check the fptr_stego_image is 70
return : e_success
*/
Status encode_magic_string(const char *magic_string,EncodeInfo *encInfo)
{
        encode_data_to_image(magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image);

        fseek(encInfo->fptr_stego_image,0,SEEK_END);
        if(ftell(encInfo->fptr_stego_image) == 70)
        return e_success;
        return e_failure;
}

/*Description : encode_data_to_image
 sample input : data, size,fptr_src_image,fptr_stego_image
 sample output : convert dta to image
 return value : e_success,e_failure
 */
Status encode_data_to_image(const char *data,int size,FILE *fptr_src_image,FILE *fptr_stego_image)
{
        char buffer[8];
        //char d[size+1];
        for(int i=0;i<size;i++)
        {
                fread(buffer,8,1,fptr_src_image);
                encode_byte_to_lsb(data[i],buffer);
                fwrite(buffer,8,1,fptr_stego_image);
        }
        //d[size]='\0';
        return e_success;
}

/*Description : encode_byte_to_lsb
  sample input : d,image_buffer
  sample output : image_buffer
  return value : e_success,e_failure
*/
Status encode_byte_to_lsb(char d,char *Image_buffer)
{
        for(int i=0;i<8;i++)
        {
                Image_buffer[i]=(Image_buffer[i]&0xFE)|((d&(1<<i))>>i);
        }
        return e_success;

}

/*Description : Secret file extesion size
  sample input : file_extn
  sample output ; check stego image is 102
  return : e_success e_failure
  */
Status encode_secret_file_extn_size(char *file_extn,EncodeInfo *encInfo)
{
        encode_size_to_lsb(strlen(file_extn),encInfo);

       // fseek(encInfo->fptr_stego_image,0,SEEK_END);
        //if(ftell(encInfo->fptr_stego_image) == 102)
        return e_success;
       // return e_failure;
}

/*Description : encode_size_to_lsb
  sample input : file_extn_size
 sample output : biffer[i]
 return value : e_success,e_failure
*/
Status encode_size_to_lsb(int file_extn_size,EncodeInfo *encInfo)
{
        char buffer[32];

        fread(buffer,32,1,encInfo->fptr_src_image);
        for(int i=0;i<32;i++)
        {
                buffer[i]=buffer[i]&0xFE | ((file_extn_size&(1<<i)) >> i);
        }
        fwrite(buffer,32,1,encInfo->fptr_stego_image);
}

/*Description : encode_secret_file_extn
  sample input : file_extn
  sample output : vheck fptr_stego_image is 154 or not
  return value : e_success,e_failure
  */
Status encode_secret_file_extn(char *file_extn,EncodeInfo *encInfo)
{
        encode_data_to_image(file_extn,strlen(file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image);

        fseek(encInfo->fptr_stego_image,0,SEEK_END);

        if(ftell(encInfo->fptr_stego_image) == 134)
        return e_success;
        return e_failure;
}

/*Description : ecoding of secrete file size
sample input : file_size
sample output : check fptr_stego_image is 154
return value : e_success.e_failure
*/
Status encode_secret_file_size(long file_size,EncodeInfo *encInfo)
{
        encode_size_to_lsb(file_size,encInfo);

        fseek(encInfo->fptr_stego_image,0,SEEK_END);
        if(ftell(encInfo->fptr_stego_image) == 166)
        return e_success;
        return e_failure;
}

/*Description : Encode secrete file data
  sample input : EncodeInfo *encInfo
  sample output : check fptr_stego_image is 366
  return value : e_success,e_failure
  */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
        char buffer[encInfo->size_secret_file+1];
        fseek(encInfo->fptr_secret,0,SEEK_SET);

        fread(buffer,encInfo->size_secret_file,1,encInfo->fptr_secret);
        buffer[encInfo->size_secret_file]='\0';

        encode_data_to_image(buffer,encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image);
        fseek(encInfo->fptr_stego_image,0,SEEK_END);
        if(ftell(encInfo->fptr_stego_image) == 366)
        return e_success;
        return e_failure;
}

/*Description : Encoding of copy_remaining_img_data
  sample input : fptr_dest, fptr_src_image
  sample output value : check fptr_src and fptr_dest is equal
  return value : e_success,e_failure
  */
Status copy_remaining_img_data(FILE *fptr_src,FILE *fptr_dest)
{
        char buffer;
        while(fread(&buffer,1,1,fptr_src) > 0)
        fwrite(&buffer,1,1,fptr_dest);

        fseek(fptr_src,0,SEEK_END);
        fseek(fptr_dest,0,SEEK_END);
        if(ftell(fptr_src) == ftell(fptr_dest))
        return e_success;
        return e_failure;
}
