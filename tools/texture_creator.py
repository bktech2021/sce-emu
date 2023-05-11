def convert_to_texture_bin(array, name="texture.bin"):
    pixel = [0] * 256

    for i in range(256):
        pixel[i] = (
            (array[(i * 8) + 0] << 7)
            | (array[(i * 8) + 1] << 6)
            | (array[(i * 8) + 2] << 5)
            | (array[(i * 8) + 3] << 4)
            | (array[(i * 8) + 4] << 3)
            | (array[(i * 8) + 5] << 2)
            | (array[(i * 8) + 6] << 1)
            | (array[(i * 8) + 7])
        )
    f = open(name, "wb")
    arr = bytearray(pixel)
    f.write(arr)
    f.close()

# There is only 1 pixel array, 1D pixel array, since the CHIP-8 screen is 64*32, the array is needing to be length of 2048 pixels.
# 1 means pixels is on and 0 means off. No colours. 

#   16 
#32 ****************
#   ****************
#   ****************
#   ****************
#   ****************
#   ****************
#   ****************
#   ****************
# This is a 2D screen but the array is 1D, so

# **************** **************** ... array is going like this


# EXAMPLE USAGE FOR CREATING PAUSE TEXTURE

pixels = [0] * 2048

for i in range(13, 19):
    for j in range(2):
        pixels[(i * 64) + 30 + (j * 3)] = 0b1
        print((i, 30 + (j * 3)))

convert_to_texture_bin(pixels)
