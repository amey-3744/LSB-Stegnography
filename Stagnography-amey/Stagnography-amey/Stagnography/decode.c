#include<stdio.h>
#include"decode.h"
#include"types.h"

/* Description : read_and_validate_decode_args
   sample input : argv, DecodeInfo *decInfo
   sample output : check in rgv[2] .bmp is present or not argv[3] is null
   return values : e_success,e_failure
   */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
	if(strcmp(strstr(argv[2],"."),".bmp")==0)
	{
		printf("argv[2] having .bmp extension\n");
		decInfo -> output_image_fname = argv[2];
	}
	else
	{
		return e_failure;
	}

	if(argv[3]==NULL)
	{
		strcpy(decInfo->stego_msg_fname,"secret_data");
	}
        else
	{
		strcpy(decInfo->stego_msg_fname,argv[3]);
	
	}
	return e_success;
}

/* Description : do_decoding
sample input : DecodeInfo *decInfo
sample output : gives sucessfull open_files,skip_header,magic_string,file_extn_size,file_extn,decode_secret_file_data_size,decode_secret_file_data is passi 
return values : e_success,e_failure
 */
Status do_decoding(DecodeInfo *decInfo)
{
	if(open_files_decode(decInfo) == e_success)
	{
		printf("opening file is success\n");
		if(skip_header(decInfo->fptr_output_image)==e_success)
		{
			printf("skiping header is success\n");
			if(decode_magic_string(decInfo)==e_success)
			{
				printf("magic string is success\n");
				if(decode_file_extn_size(decInfo)==e_success)
				{
					printf("file extn size is success\n");
					if(decode_file_extn(decInfo)==e_success)
					{
						printf("file extn is success\n");
						
						if(decode_secret_file_data_size(decInfo)==e_success)
						{
							printf("decode of secret file data size is success\n");
							if(decode_secret_file_data(decInfo)==e_success)
							{
								printf("decode of secret file data is success\n");
								return e_success;
							}
							else
							{
								printf("decode of secret file data is fail\n");
							}
						}
						else
						{
							printf("decode of secret file data size is failure\n");
						}
						
					}
					else
					{
						printf("file extn is fail\n");
					}
				}
				else
				{
					printf("file extn size is fail\n");
				}
			}
			else
			{
				printf("magic string is fail\n");
			}
			
		}
		else
		{

			printf("skiping header is fail\n");
		}
         
	}
	else
	{
		printf("opening file is fail\n");
		return e_failure;
	}
}

 /* Description : open_files_decode
 sample input : DecodeInfo *decInfo
 sample output : opening of file
 return values : e_success,e_failure
  */
Status open_files_decode(DecodeInfo *decInfo)
{
	decInfo->fptr_output_image = fopen(decInfo->output_image_fname,"r");

	if(decInfo->fptr_output_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_image_fname);
		return e_failure;
	}

	return e_success;
}

 /* Description : skip_header
 sample input : fptr_output_image
 sample output : check fptr_ou_output_imags 54
 return values : e_success,e_failure
 */
Status skip_header(FILE *fptr_output_image)
{
	fseek(fptr_output_image,54,SEEK_SET);
	if(ftell(fptr_output_image) == 54)
	{
		return e_success;
	}
	else
	{
		return e_failure;
	}
}

 /* Description : decode_magic_string
 sample input : DecodeInfo *decInfo
 sample output :  check equal to #*
 return values : e_success,e_failure
 */
Status decode_magic_string(DecodeInfo *decInfo)
{

	char buffer[8];
	char magic_string[3];
	int i;
	for(i=0;i<2;i++)
	{
		fread(buffer,8,1,decInfo->fptr_output_image);
		magic_string[i]=decode_byte_to_lsb(buffer);
	}

	magic_string[i]='\0';
	
	if(strcmp(magic_string,"#*")==0)
	{
		return e_success;
	}
	else
	{
		return e_failure;
	}
}

 /* Description : decode_byte_to_lsb
 sample input : image_buffer
 sample output : fetch ch values
 return values : e_success,e_failure
 */
char decode_byte_to_lsb(char *image_buffer)
{
	char ch=0;
	int i;
	for(i=0;i<8;i++)
	{
		ch=((image_buffer[i] & 0x01 ) << i) | ch;
	}
	return ch;
}

 /* Description : decode_file_extn_size
 sample input : DecodeInfo *decInfo
 sample output : check fptr_output_image
 return values : e_success,e_failure
 */
Status decode_file_extn_size(DecodeInfo *decInfo)
{
	char buffer[32];
	fread(buffer,32,1,decInfo->fptr_output_image);
	decInfo->extn_size=decode_byte_to_lsb(buffer);
	//printf("%d",decInfo->extn_size);

	return e_success;
}

 /* Description : decode_size_to_lsb
 sample input : image_buffer
 sample output : fetch size
 return values : e_success,e_failure
 */
int decode_size_to_lsb(char *image_buffer)
{
	int size=0;
	int i;
	for(i=0;i<32;i++)
	{
		size=((image_buffer[i] & 0x01 ) << i) | size;
	}
	return size;
}

 /* Description : decode_file_extn
 sample input : DecodeInfo *decInfo
 sample output : ccopy the file and merge
 return values : e_success,e_failure
 */
Status decode_file_extn(DecodeInfo *decInfo)
{
	char buffer[8];
	char file_extn[decInfo->extn_size+1];
	int i;
	for(i=0;i<decInfo->extn_size;i++)
	{
		fread(buffer,1,8,decInfo->fptr_output_image);
		file_extn[i]=decode_byte_to_lsb(buffer);
	}
	file_extn[i]='\0';
	strcpy(decInfo->stego_msg_fname,strcat(decInfo->stego_msg_fname,file_extn));
	decInfo->fptr_stego_msg=fopen(decInfo->stego_msg_fname,"w");
	return e_success;
}

 /* Description : decode_secret_file_data_size
 sample input : DecodeInfo *decInfo
 sample output : store the values
 return values : e_success,e_failure
 */
Status decode_secret_file_data_size(DecodeInfo *decInfo)
{
	char buffer[32];
	fread(buffer,32,1,decInfo->fptr_output_image);
	decInfo->data_size=decode_size_to_lsb(buffer);

	return e_success;
}

 /* Description : decode_secret_file_data
 sample input : DecodeInfo *decInfo
 sample output : sore all into fptr_stego_msg and closing of file
 return values : e_success,e_failure
 */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
	char buffer[8];
	char secrete_data[decInfo->data_size+1];
        int i;
	for(i=0;i<decInfo->data_size;i++)
	{
		fread(buffer,8,1,decInfo->fptr_output_image);
		secrete_data[i]=decode_byte_to_lsb(buffer);
	}
	secrete_data[i]='\0';
	fwrite(secrete_data,1,decInfo->data_size,decInfo->fptr_stego_msg);
	fclose(decInfo->fptr_stego_msg);

	return e_success;
        
}





