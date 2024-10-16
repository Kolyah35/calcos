import io
from PIL import Image

img = Image.open("wallpaper.png", mode='r')

result = ''

# roi_img.save(img_byte_arr, format='PNG')

# for y in range(img.height):
#     for x in range(img.width):
#         pixel = img.getpixel((x, y))
#         result += str(pixel)
    
        # print(1 if (pixel[0] + pixel[1] + pixel[2]) < 765 else 0)

for i in range(img.width * img.height):
    result += str(img.tobytes()[i])

carr = ""

for i in range(0, len(result), 8): 
    carr += f'{hex(int(result[i:i+8], 2))}, '
carr = carr[0:-2]

print(carr)