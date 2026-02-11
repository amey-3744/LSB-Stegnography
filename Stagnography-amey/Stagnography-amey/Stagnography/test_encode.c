

#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

/* main function 
Description : main function is starting of program
sample input : argc, argv[]
sample output : return e_success, e_failure; 
*/

int main(int argc, char* argv[])
{
	if(argc<3 && argc<4)           //check argc must be 4 or 3 to encode decode
	{
		printf("Please pass CLA to select encoding: ./a.out -e <source_file_.bmp> <secret_file.txt>\n");
		printf("Please pass CLA to select decoding: ./a.out -d <source_file_.bmp>\n");

	       return e_failure;    //less than that gives error
	}
	else
	{

        if(check_operation_type(argv)==e_encode)         //then check operation encode
	{
			printf("user choice is encoding\n");
			EncodeInfo encInfo;
		if(read_and_validate_encode_args(argv,&encInfo)==e_success)    //read & validate part
		{
				printf("read and validation is success\n");
				if(do_encoding(&encInfo)==e_success)              //do_encoding part
				{
					printf("___ENCODING IS SUCCESSFULLY COMPLETED___\n");  //success msg
				}
		}
		else
		{
			printf("read and validation is failure\n");       
			return e_failure;
		}
	}
	
	
	else if(check_operation_type(argv)==e_decode)        //check opertion is for decode
	{
		printf("user choice is decoding\n");
		DecodeInfo decInfo;
		if(read_and_validate_decode_args ( argv, &decInfo )==e_success)    //read & validate part
		{
			printf("read and validation is success\n");
			if(do_decoding(&decInfo)==e_success)
			{
				printf("___DECODING IS SUCCESSFULLY COMPLETED___\n");		//decoding part
				//return e_success;
			}
		}
		else
		{
			printf("read and validation is failure\n");
	         	return e_failure;
         	}
         }

	else
	{
		printf("you entered rather than -e / -d\n");
		return e_failure;
	}
	}

 
}

	

