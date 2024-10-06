
# Description
* The application accept an image file .bmp along with the a text file which
  contains the message to be steged.
* Analyze the size of the message file and the data part of the .bmp file to check
  whether the messsage could fit in the provided .bmp image
* Provide a option to steg a magic string which could be usefull to identify whether the
  image is steged or not
* The application decrypts a passed image and extracts the secrete data if the image is already steged.

# Compilation
* Gcc version > 8.0 .
* make can be used in optional.

# Processing
* Secrete file is bit by by bit encoded to lsb of each byte of the RGB data section of bmp image.
* We'll need the RGB data section of the bmp image to be ateast 8 times of the secrete data file bytes size (additional metadata are also required to encode).
* First 8 bytes contains Magic String Signature which can be any ascii special character.
* other meta data will be encoded in the following order File Extenstion Size -> file extension -> File Size (int) -> File Data




# References:
* https://en.wikipedia.org/wiki/Steganography
* https://en.wikipedia.org/wiki/BMP

# Steganography Project: Hiding Secret Messages in BMP Images

## Overview
This project demonstrates a simple implementation of steganography by embedding secret data within a BMP image file. The application allows users to encode and decode secret messages inside an image by manipulating the least significant bits (LSB) of the RGB data in the image.

## Features
1. **Image Embedding**:
   - Accepts an image file in `.bmp` format and a text file containing the secret message to hide.
   - Provides an option to add a magic string signature to identify whether an image contains hidden data.

2. **Image Decoding**:
   - Extracts hidden data from a `.bmp` image that has previously been embedded with a message.

## Steganographic Process
### Embedding:
- The secret message is encoded bit-by-bit into the least significant bits of the RGB pixel data in the BMP file.
- Before encoding the secret message, metadata such as file extension and file size is embedded.

### Decoding:
- The decoder extracts the hidden data by reading the magic string and metadata, reconstructing the original secret message.

## Requirements
- **Compiler**: `gcc` version 8.0 or higher.
- **Optional**: Use `make` for easy build management.

## How to Use

### Embedding a Message
```bash
./steg_encoder input_image.bmp secret_message.txt output_image.bmp
