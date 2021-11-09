# ECE 4300 Group D AES Performance Evaluation

## C Image Encryption
aes_image_encryption_performance_eval.c takes in an image and tests the encryption and decryption speeds. To compile with gcc, the following command can be used <br>
`gcc -std=c17 -Wall -pedantic aes_image_encryption_performance_eval.c aes.c -o aes_image_encryption_performance_eval -lm`

You can test with various image inputs by changing and recompling the code. Some images wont be loaded so it wont work, but all loaded images should work. <br>

## Python Image Encryption
tests.py tests image encryption and decryption with pillow python library for image loader and an open source aes module. Run tests.py in a python environment with the packages available 


## Credits
Python AES: https://github.com/boppreh/aes <br>
C AES: https://github.com/kokke/tiny-AES-c
