/*
Name: Atharva kothawade
Batch ECEP 23036_018
 */
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "common.h"
#include <unistd.h>
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
//open files 
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
	return e_failure;
    }
    else 
    {
	sleep(1);
	printf("INFO: Opening required files\n");
	printf("INFO: Opened %s\n",encInfo->src_image_fname);
    }
    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
	return e_failure;
    }
    else 
    {
	sleep(1);
	printf("INFO: Opened %s\n",encInfo->secret_fname);
    }
    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
	return e_failure;
    }
    else 
    {
	sleep(1);
	printf("INFO: Opened %s\n",encInfo->stego_image_fname);
    }
    // No failure return e_success
    return e_success;
}

//Function for check operations for encoding or decoding
OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1],"-e") == 0)
	return e_encode;
    else if (strcmp(argv[1],"-d") == 0)
	return e_decode;
    else
	return e_unsupported;
}

//Function for Read and validation of argunments
Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
    if (strstr(argv[2],".bmp") != 0)
    {
	encInfo->src_image_fname = argv[2];
	if (strstr(argv[3],".") != 0)
	    encInfo->secret_fname=argv[3];
	else 
	{
	    printf("Error: Invalide argument\n");
	    return e_failure;
	}
	if (argv[4]!=NULL)
	    if (strstr(argv[4],".bmp") != 0)
		encInfo->stego_image_fname=argv[4];
	    else
		printf("Error: Invalide Stego image file name\n");
	else
	    encInfo->stego_image_fname="stego_output.bmp";
	return e_success;
    }
    else
    {
	printf("Error: Invalide argument image file\n");
	return e_failure;
    }
}

//Function for Check the capacity of RGB data in bmp file & secret data
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

    if (((strlen(MAGIC_STRING) * 8) + 32 + (strlen(encInfo->extn_secret_file) * 8) + 32 + (encInfo->size_secret_file * 8)) < encInfo->image_capacity)
	return e_success;
    else
	return e_failure;
}

//Function for Get the file size 
uint get_file_size(FILE *fptr_secret)
{
    fseek (fptr_secret,0,SEEK_END);
    return ftell(fptr_secret);
}

//Function for Copy the bmp source file header to destination file header
Status copy_bmp_header(FILE *src_fptr_image,FILE *dest_fptr_image)
{
    char buffer[54];
    fseek(src_fptr_image,0,SEEK_SET);
    fread(buffer,1,54,src_fptr_image);
    fwrite(buffer,1,54,dest_fptr_image);
    return e_success;
}

//Function for encoding magic string
Status encode_magic_string(char *magic_string,EncodeInfo *encInfo)
{
    encode_data_to_image(magic_string,2,encInfo->fptr_src_image,encInfo->fptr_stego_image,encInfo);
    return e_success;
}

//Function for Encode data to image
Status encode_data_to_image(char *data,int size,FILE *fptr_src_image,FILE *fptr_stego_image,EncodeInfo *encInfo)
{
    for(int i=0;i<size;i++)
    {
	fread(encInfo->image_data,8,1,fptr_src_image);

	encode_byte_to_lsb(data[i],encInfo->image_data);

	fwrite(encInfo->image_data,8,1,fptr_stego_image);
    }
}

//Function for Encode byte to lsb
Status encode_byte_to_lsb(char data,char *image_buffer)
{
    for(int i=0;i<8;i++)
	image_buffer[i]=((data&(1<<(7 - i))) >> (7 - i)) | (image_buffer[i]&0XFE);
}

//Function for encode size to lsb
Status encode_size_to_lsb(int data,char *image_buffer)
{
    for(int i=0;i<32;i++)
	image_buffer[i]=((data&(1<<(31 - i))) >> (31 - i)) | (image_buffer[i]&0XFE);
}

//Function for encode secret file extention size
Status encode_secret_file_extn_size(int extn_size,EncodeInfo *encInfo)
{
    char arr[32];

    fread(arr,32,1,encInfo->fptr_src_image);

    encode_size_to_lsb(extn_size,arr);

    fwrite(arr,32,1,encInfo->fptr_stego_image);
    return e_success;
}

//Function for Encode secret file extention
Status encode_secret_file_extn(char *file_extn,EncodeInfo *encInfo)
{
    encode_data_to_image(file_extn,strlen(file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image,encInfo);
    return e_success;
}

//Function for Encode secret file size
Status encode_secret_file_size(long file_size,EncodeInfo *encInfo)
{
    char arr[32];

    fread(arr,32,1,encInfo->fptr_src_image);

    encode_size_to_lsb(file_size,arr);

    fwrite(arr,32,1,encInfo->fptr_stego_image);
    return e_success;
}

//Function for Encode secret file data
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    fseek(encInfo->fptr_secret,0,SEEK_SET);

    char str[encInfo->size_secret_file];

    fread(str , encInfo->size_secret_file,1,encInfo->fptr_secret);

    encode_data_to_image(str, strlen(str),encInfo->fptr_src_image,encInfo->fptr_stego_image,encInfo);
    return e_success;
}

//Function for copy remaining image data
Status copy_remaining_img_data(FILE *fptr_src,FILE *fptr_dest)
{
    char ch;

    while( (fread(&ch,1,1,fptr_src)) > 0)
	fwrite(&ch,1,1,fptr_dest);

    return e_success;
}
//Function for perform encoding
Status do_encoding(EncodeInfo *encInfo)
{
    if (open_files(encInfo) == e_success)
    {
	sleep(1);
	printf("Success: File open\n");
	printf("## Encoding procedure started ##\n");
	if (check_capacity(encInfo) == e_success)
	{
	    sleep(1);
	    printf("Success: Check capacity\n");
	    if (copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
	    {
		sleep(1);
		printf("Success: Copied bmp header\n");
		if (encode_magic_string(MAGIC_STRING,encInfo) == e_success)
		{
		    sleep(1);
		    printf("Success: Encoding magic string\n");
		    if (encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo) == e_success)
		    {
			sleep(1);
			printf("Success: Encode secret file extention size\n");
			if (encode_secret_file_extn(encInfo->extn_secret_file,encInfo) == e_success)
			{
			    sleep(1);
			    printf("Success: Encode secret file extention\n");
			    if (encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_success)
			    {
				sleep(1);
				printf("Success: Encode secret file size\n");
				if (encode_secret_file_data(encInfo) == e_success)
				{
				    sleep(1);
				    printf("Success: Encode secret file data\n");
				    if (copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
				    {
					sleep(1);
					printf("Success: Copy remaining data\n");
					return e_success;
				    }
				    else 
				    {
					printf("Failure: Copy remaining data\n");
					return e_failure;
				    }
				}
				else 
				{
				    printf("Failure: Encode secret file data\n");
				    return e_failure;
				}
			    }
			    else 
			    {
				printf("Failure: Encode secret file size\n");
				return e_failure;
			    }
			}
			else 
			{
			    printf("Failure: Encode secret file entention\n");
			    return e_failure;
			}
		    }
		    else 
		    {
			printf("Failure: Encode secret file extention\n");
			return e_failure;
		    }
		}
		else 
		{
		    printf("Failure: Encode magic string\n");
		    return e_failure;
		}
	    }
	    else 
	    {
		printf("Failure: Copy bmp header\n");
		return e_failure;
	    }
	}
	else
	{
	    printf("Failure: Check capacity\n");
	    return e_failure;
	}
    }
    else 
    {
	printf("Failure: Open file\n");
	return e_failure;
    }
}
