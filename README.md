# Steganography in BMP Images

## Introduction
Steganography is the technique of hiding secret data within non-secret data, such as an image, in a way that makes the hidden data invisible to the naked eye. This project demonstrates how to hide a secret message inside a BMP image by manipulating the least significant bits (LSB) of its pixel data. The application also allows decoding an image to extract the secret message if it has been previously hidden using the same method.

## How Steganography Works
### Embedding Process
1. The BMP image file consists of pixel data where each pixel has three components: Red (R), Green (G), and Blue (B).
2. The secret message (file) is embedded by modifying the least significant bits (LSB) of the pixel data.
3. Metadata is embedded first to store information about the hidden file, followed by the actual data of the secret message.
4. A "magic string" (a special marker) is placed at the beginning of the embedded data to help detect whether the image contains hidden data.

### Decoding Process
1. The decoder reads the "magic string" from the BMP image.
2. If the string matches, it proceeds to extract the metadata and the actual secret message bit by bit.
3. The extracted message can then be saved to a new file.

## Requirements
- **GCC version 8.0 or higher** for compiling the program.
- Optionally, you can use `make` for building the project.
- **BMP Image Format**: The program only supports the `.bmp` format.

## Features
1. **Hide Secret Data**: Allows embedding a secret message inside a BMP image file.
2. **Extract Secret Data**: Extracts and reconstructs the hidden data from a steganographed BMP image.
3. **Magic String Detection**: Adds a special string to detect if a BMP image contains hidden data.
4. **Size Validation**: Ensures the BMP image is large enough to hold the secret message.

## Usage

### Embedding a Message
```bash
./steg_encoder input_image.bmp secret_message.txt output_image.bmp
