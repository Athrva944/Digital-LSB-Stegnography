/*
Name: Atharva kothawade
Batch:ECEP 23036_018
*/
#include <stdio.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include "string.h"
#include <unistd.h>
//Function for read and validate decode argument
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (strstr(argv[2],".bmp") != 0)
        decInfo->img_file_name = argv[2];
    else
    {
	printf("Error: Invalide file name pass .bmp file\n");
        return e_failure;
    }
    if (argv[3] != NULL)
        strcpy(decInfo->out_file_name , argv[3]);
    else
        strcpy(decInfo->out_file_name , "output");
    return e_success;
}

//Function File open file for decode
Status open_files_decode(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->img_file_name, "r");

    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->img_file_name);
        return e_failure;
    }
    else 
    {
	printf("## Decoding procedure started ##\n");
	printf("Opening required files\n");
	printf("INFO: Opened %s\n",decInfo->img_file_name);
    }
    return e_success;
}

//Function for the decode magic string
Status decode_magic_string(const char *magic_str, DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    if (decode_magic_str_from_lsb(strlen(magic_str), decInfo) == e_success)
        return e_success;
    else
        return e_failure;
}

//Function for decode magic string from lsb
Status decode_magic_str_from_lsb(int size, DecodeInfo *decInfo)
{
    char output = 0x00;
    char mask = 0x01;
    char data[8];
    char str[2];
    int j = 0;
    while (size--)
    {
        for (int i = 0; i < 8; i++)
        {
            fread(data, 1, sizeof(char), decInfo->fptr_stego_image);
            unsigned char res = (*data) & mask;
            res = res << (7 - i);
            output = output | res;
        }
        decInfo->dec_string[j++] = output;
        output = 0x00;
    }
    decInfo->dec_string[j] = '\0';
    //printf("->Magic String : %s \n", decInfo->dec_string);
    return e_success;
}

//Function for decode secret file extention size
Status decode_secrect_file_ext_size(DecodeInfo *decInfo)
{
    if (decode_int_from_lsb(&decInfo->secret_file_extn_size, decInfo->fptr_stego_image)  == e_success)
	return e_success;
    else
	return e_failure;
}

//Function for decode int from lsb side
Status decode_int_from_lsb(int *data, FILE *fptr_stego)  
{
    int output = 0x00;
    char mask = 0x01;
    char img_data[32];
    for (int i = 0; i < 32; i++)
    {
        fread(img_data, 1, sizeof(char), fptr_stego);
        unsigned int res = (*img_data) & mask;
        res = res << (31 - i);
        output = output | res;
    }
    *data = output; 
    //printf("->Decoded Size = %d\n", output);
    return e_success;
}

//Function for decode secret file extention
Status decode_secrect_file_ext(DecodeInfo *decInfo)
{
    int size = decInfo->secret_file_extn_size;
    decode_data(size, decInfo->fptr_stego_image,decInfo->secret_file_extn );

    strcat(decInfo->out_file_name, decInfo->secret_file_extn);
    //printf("->File Name = %s\n",decInfo->out_file_name);
    return e_success;
}

//Function for decode the secret file data
Status decode_data(int size,  FILE *fptr_stego, char *str_ptr)
{
    *str_ptr = '\0';
    int size_bits = size*8;
    char output = 0x00;
    char mask = 0x01;
    char data[8];
    static int flag=1;
    while(size--)
    {
        for (int i = 0; i < 8; i++)
        {
            fread(data, 1, sizeof(char), fptr_stego);
            unsigned char res = (*data) & mask;
            res = res << (7 - i);
            output = output | res;
        }
        strncat(str_ptr,&output,1);
        output = 0;
    }
    if (flag)
    {
	//printf("->String: Nothing decoded\n->size = %ld\n",strlen(str_ptr));
	flag=0;
    }
    else
    {
	//printf("->String: %s->size = %ld\n",str_ptr,strlen(str_ptr));
    }
}

//Function for decode secret file size
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    if (decode_int_from_lsb(&decInfo->secret_file_size,decInfo->fptr_stego_image) == e_success)
	return e_success;
    else 
	return e_failure;
}

//Fuction for decode that secret file 
Status decode_secret_file(DecodeInfo *decInfo)
{
    decode_data(decInfo->secret_file_size, decInfo->fptr_stego_image, decInfo->secret_file);
    FILE *fp;
    fp = fopen(decInfo->out_file_name,"w");
    if ( fp != NULL)
        fwrite(decInfo->secret_file, 1,strlen(decInfo->secret_file), fp);
    else
    {
        printf("ERROR: opening file\n");
        return e_failure;
    }
    fclose(fp);
    return e_success;
}

//Fuction for do decoding 
Status do_decoding(DecodeInfo *decInfo)
{
    if (open_files_decode(decInfo) == e_success)
    {
        sleep(1);
        printf("Success: Files opened\n");
        if (decode_magic_string(MAGIC_STRING, decInfo) == e_success)
        {
            sleep(1);
            printf("Success: Magic string decoded\n");
            if (decode_secrect_file_ext_size(decInfo) == e_success)
            {
                sleep(1);
                printf("Success: File extn size decoded\n");
                if ( decode_secrect_file_ext(decInfo) == e_success)
                {
                    sleep(1);
                    printf("Success: Secret File Extension Decoded\n");
                    if(decode_secret_file_size(decInfo) == e_success)
                    {
                        sleep(1);
                        printf("Success: Secret File Size Decoded\n");
                        if ( decode_secret_file(decInfo) == e_success)
                        {
                            sleep(1);
                            printf("Success: Secret file Data Decoded\n");
                        }
                        else
                        {
                            printf("Failure: Secret file data decode\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("Failure: Secret File Size decoding\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("Failure: Decode secret file extension\n");
                    return e_failure;
                }
            }
            else
            {
                printf("Failure: Decode secret file extention size\n");
                return e_failure;
            }
        }
        else
        {
            printf("Failure: Decode magic string");
            return e_failure;
        }
    }
    else
    {
        printf("Failure: Open file\n");
        return e_failure;
    }
    return e_success;
}
