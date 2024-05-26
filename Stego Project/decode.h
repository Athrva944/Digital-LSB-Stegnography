#ifndef DENCODE_H
#define DENCODE_H
#include "types.h"
#include "common.h"
#define MAGIC_STRING "#*"
typedef struct _DecodeInfo
{
/*stego file info */

 char *img_file_name;
 FILE *fptr_stego_image;
 char image_data[8];

 /*output file info */
 char out_file_name[MAX_FILE_NAME];
 FILE *fptr_output_file;

 /*Magic String*/
 char dec_string[2];

 //Secret FILE
 uint secret_file_extn_size;
 char secret_file_extn[MAX_EXT_SIZE];

 //char *secret_file_extn;
 uint secret_file_size;

 //char *secret_file;
 char secret_file[MAX_FILE_SIZE];  

}DecodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *encInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_decode(DecodeInfo *decInfo);

/* Func to decode magic string */
Status decode_magic_string(const char *magic_str, DecodeInfo *decInfo);

/* Decode the int data from image*/
Status decode_data_from_image(int size, DecodeInfo *decInfo);

/* Decode Magic String from lsb and compare*/
Status decode_magic_str_from_lsb(int size, DecodeInfo *decInfo);

/* Decode FIle extn size*/
Status decode_secrect_file_ext_size(DecodeInfo *decInfo);

/* decode an integer from lsb */
Status decode_int_from_lsb(int *data, FILE *fptr_stego) ;

/* Deocde the secret file extension */
Status decode_secrect_file_ext(DecodeInfo *decInfo);

/* Decode data of any size */
Status decode_data(int size,  FILE *fptr_stego, char *str_ptr);

/* Decode the secret file size*/
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode the data from secret file */
Status decode_secret_file(DecodeInfo *decInfo);

#endif
