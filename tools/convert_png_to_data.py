"""
Given a PNG file, converts it to data usable in C++ (Copy & paste).
(For now), transparent = 0, black = 1
"""

from PIL import Image
import sys

if len(sys.argv) < 6:
    print("USAGE: convert /path/to/image.png tile_width tile_height num_tiles_x num_tiles_y")

image_path = sys.argv[1]
tile_width = int(sys.argv[2])
tile_height = int(sys.argv[3])
num_tiles_x = int(sys.argv[4])
num_tiles_y = int(sys.argv[5])

img = Image.open(image_path).convert("RGBA")
img_w, img_h = img.size
pixels = img.load()

def color_to_int(pixel):
    """
    Takes the RGBA pixel value and converts it to a color palette index.
    for now just black and transparent -> 1 and 0
    """
    r, g, b, a = pixel
    return int(a / 255)

def tile_to_data(tile_x, tile_y):
    data = []
    for y in range(tile_height):
        for x in range(tile_width):
            px = tile_x * tile_width + x
            py = tile_y * tile_height + y
            pixel = pixels[px, py]
            data.append(color_to_int(pixel))
    return data

# print for each tile
print("const std::array<std::array<Color, " + str(tile_width * tile_height) + ">, " + str(num_tiles_x * num_tiles_y) + "> texture = {{")
for y in range(num_tiles_y):
    for x in range(num_tiles_x):
        i = y * num_tiles_x + x
        data = tile_to_data(x, y)
        out = ", ".join(map(str, data))

        print("\t{ " + out + " },");
print("}};")
