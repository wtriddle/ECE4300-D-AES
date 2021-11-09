from aes import AES
import time
from PIL import Image

def align_bytes(img_size):
    aes_byte_aligner = 0
    if(img_size % 16 != 0):
        print("img_size not aligned with AES 16 byte requirements\n")
        print("total bytes = ", img_size)
        print("total 16 byte blocks = ", img_size/16.0)
        tmp = ((img_size/16.0) - ((int) (img_size/16)))
        print("aligner factor is ", tmp)
        tmp=16*tmp
        aes_byte_aligner = tmp
        print("Aligner offset to section off exact 16 block bytes is ", aes_byte_aligner)
    return int(aes_byte_aligner)

def run():
    key = b'\x2b\x7e\x15\x16\x28\xae\xd2\xa6\xab\xf7\x15\x88\x09\xcf\x4f\x3c'
    aes = AES(key)
    extension = "jpg"
    img = Image.open("Will." + extension)
    raw = img.tobytes()
    img_size = len(raw)
    channels = ''.join(img.getbands())
    print(channels)
    (dummy0,dummy1,x_dim,y_dim) = img.getbbox()
    aes_byte_aligner = align_bytes(img_size) if img_size % 16 != 0 else 0

    print("Testing image encryption...")
    encrypted_raw = bytearray(img_size*len(channels))
    t = time.time()
    for i in range(0, img_size - aes_byte_aligner, 16):
        encrypted_raw[i:i+16] = aes.encrypt_block(raw[i:i+16])
    print("Image encryption took", time.time() -t, "s")
    Image.frombytes(channels, (x_dim, y_dim), bytes(encrypted_raw)).save("Encrypted." + extension)

    print("Testing image decryption...")
    decrypted_raw = bytearray(img_size*len(channels))
    t = time.time()
    for i in range(0, img_size - aes_byte_aligner, 16):
        decrypted_raw[i:i+16] = aes.decrypt_block(encrypted_raw[i:i+16])
    print("Image decryption took", time.time() -t, "s")
    Image.frombytes(channels, (x_dim, y_dim), bytes(decrypted_raw)).save("Decrypted." + extension)
    
if __name__ == '__main__':
    run()
