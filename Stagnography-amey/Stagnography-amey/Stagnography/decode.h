#ifndef DECODE_H
#define DECODE_H
#include<string.h>

#include "types.h" // Contains user defined types

/*
 * Structure to store information required for
 * decoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* output Image info */
    char *output_image_fname;
    FILE *fptr_output_image;
    int extn_size;

    /*output msg info*/
    int data_size;
    char stego_msg_fname[20];
    FILE *fptr_stego_msg;

} DecodeInfo;

/* decoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the do_decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_decode(DecodeInfo *decInfo);

/* skiping header */
Status skip_header(FILE *fptr_output_image);

/* Store Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/*Encode function, which does the real encoding */
char decode_byte_to_lsb(char *image_buffer);
 
/*file_extn_size */
Status decode_file_extn_size(DecodeInfo *decInfo);

/*size_to_lsb */
int decode_size_to_lsb(char *image_buffer);

/* file_extn */
Status decode_file_extn(DecodeInfo *decInfo);

/*secret file data size */
Status decode_secret_file_data_size(DecodeInfo *decInfo);

/*secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

#endif
