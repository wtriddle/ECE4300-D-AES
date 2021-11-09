// Load image using the stb_image library, convert the image to gray and sepia, write it back to disk
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

// Enable ECB, CTR and CBC mode. Note this can be done before including aes.h or at compile-time.
// E.g. with GCC by using the -D flag: gcc -c aes.c -DCBC=0 -DCTR=1 -DECB=1
#define CBC 1
#define CTR 1
#define ECB 1

#include "aes.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"


int main(void) {
    int width, height, channels;
    clock_t t;                  // Clock object for timer
    double exec_time;           // Execution time measurement
    unsigned char *img = stbi_load("Will.jpg", &width, &height, &channels, 0);

    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);


    size_t img_size = width * height * channels;

    // Align image with AES 16 byte input
    int aes_byte_aligner = 0;
    if(img_size%16 !=0)
    {
        printf("img_size not aligned with AES 16 byte requirements\n");
        printf("total bytes = %lld\n", img_size);
        printf("total 16 byte blocks = %f\n", img_size/16.0);
        float tmp = ((img_size/16.0) - ((int) img_size/16));
        printf("aligner factor is %f\n", tmp);
        tmp=16*tmp;
        aes_byte_aligner = tmp;
        printf("Aligner offset to section off exact 16 block bytes is %i\n", aes_byte_aligner);
    }

    // Initialize AES context
    uint8_t key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    struct AES_ctx ctx;
    AES_init_ctx(&ctx, key);

    // Image Encryption
    printf("Evaluate performance of %dx%d image encryption\n", width, height);
    printf("Encrypting image...\n");
    
    t = clock();        // Begin timer for eval

    for(unsigned char *p = img; p != img + img_size - aes_byte_aligner; p += 16) {     // 16 bytes per encryption
        AES_ECB_encrypt(&ctx, p);
    }
    t = clock() - t;   // Take difference for exec time
    exec_time = ((double)t)/CLOCKS_PER_SEC;
    
    printf("Done in %f sec\n", exec_time);
    stbi_write_jpg("Encrypted_Image.jpg", width, height, channels, img, width * channels);

    // Image Decryption
    printf("Evaluate performance of %dx%d image decryption\n", width, height);
    printf("Decrypting image...\n");

    t = clock();        // Begin timer for eval

    for(unsigned char *p = img; p != img + img_size - aes_byte_aligner; p += 16) {     // 16 bytes per decryption
        AES_ECB_decrypt(&ctx, p);
    }

    t = clock() - t;   // Take difference for exec time
    exec_time = ((double)t)/CLOCKS_PER_SEC;
    
    printf("Done in %f sec\n", exec_time);
    stbi_write_jpg("Decrypted_Image.jpg", width, height, channels, img, width * channels);
  
}

