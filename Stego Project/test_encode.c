/*
Name: Atharva kpthawade
Batch: ECEP 23036_018
*/
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"
#include <unistd.h>

int main(int argc,char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    if (argc == 1)
    {
	printf("./a.out: Encoding: ./a.out -e <.bmp file> <.txt file> [output file ]\n");
	printf("./a.out: Decoding: ./a.out -d <.bmp file> [output file]\n");
	return 0;
    }
    if (check_operation_type(argv) == e_encode)
    {
	if (argc >= 4)
	{
	    if (read_and_validate_encode_args(argv,&encInfo) == e_success)
	    {
		printf("----Selected Encoding----\n\n");
		sleep(1);
		printf("Success: Read and validate Encode argunments\n");
		if (do_encoding(&encInfo) == e_success)
		{
		    sleep(1);
		    printf("\n#### Encoding is Completed ####\n");
		}
		else
		{
		    printf("\n#### Failure: Encoding ####\n");
		    return e_failure;
		}
	    }
	    else
	    {
		printf("Failure: Read and validate encode argument\n");
		printf("./a.out: Encoding: ./a.out -e <.bmp file> <.txt file> [output file ]\n");
		return e_failure;
	    }
	}
	else
	    printf("./a.out: Encoding: ./a.out -e <.bmp file> <.txt file> [output file ]\n");
    }
    else if (check_operation_type(argv) == e_decode)
    {
	if (argc >= 3)
	{
	    if (read_and_validate_decode_args(argv,&decInfo) == e_success)
	    {
		printf("----Selected Decoding----\n\n");
		sleep(1);
		printf("Success: Read and validate Decode arguments\n");
		if (do_decoding(&decInfo) == e_success)
		{
		    sleep(1);
		    printf("\n#### Decoding is complited ####\n");
		}
		else 
		{
		    printf("\n#### Failure: Decoding ####\n");
		    return e_failure;
		}
	    }
	    else 
	    {
		printf("Failure: Read and validate Decode argument\n");
		printf("./a.out: Decoding: ./a.out -d <.bmp file> [output file]\n");
		return e_failure;
	    }
	}
	else
	    printf("./a.out: Decoding: ./a.out -d <.bmp file> [output file]\n");
    }
    else 
    {
	printf("Error: Nothing will be selected (Encoding/Decoding)\n");
	printf("Error: Please pass the valide arguments (-e/-d)\n");
    }
    return 0;
}
