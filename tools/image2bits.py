import io
from PIL import Image

print('The image must be in png format and 1bpp!')

img = Image.open("wallpaper.png", mode='r')

bin = ''

for y in range(img.height):
    for x in range(img.width):
        pixel = img.getpixel((x, y))
        bin += str(pixel)

carr = ""

for i in range(0, len(bin), 8): 
    carr += f'{hex(int(bin[i+8:i:-1], 2))}, '
carr = carr[0:-2]

print(carr)