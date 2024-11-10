import os
from PIL import Image

def cut_image_piece(image: Image.Image, x: int, y: int, w: int, h: int) -> Image.Image:
    return image.crop((x, y, x + w, y + h))

def cut_image(image: Image.Image, w: int, h: int) -> dict[int, Image.Image]:
    res = {}
    for low in range(0, 16):
        for high in range(0, 16):
            res[low + (high << 4)] = cut_image_piece(image, low * w, high * h, w, h)
    return res

def image_to_binary_array(image: Image.Image) -> list[int]:
    image = image.convert("RGBA")
    binary_array = []
    for y in range(image.height):
        row_bits = 0
        for x in range(image.width):
            r, g, b, a = image.getpixel((x, y))
            is_black = (r, g, b) == (0, 0, 0) and a != 0
            row_bits = (row_bits << 1) | (1 if is_black else 0)
        binary_array.append(row_bits)
    return binary_array

def main():
    w = 20
    h = 30
    file = os.path.dirname(os.path.abspath(__file__)) + "/font.png"
    img = Image.open(file)
    chars = cut_image(img, w, h)
    res = ""
    for i in range(256):
        bin_repr = image_to_binary_array(chars[i])
        bin_str = ", ".join(f"0x{byte:0{w // 8}X}" for byte in bin_repr)
        res += f"  {{{bin_str}}},\n"

    output = f"u32 font[256][{h}] = {{\n{res}}};"
    with open(os.path.dirname(os.path.abspath(__file__)) + "/font.c", "w") as file:
        file.write(output)

if __name__ == "__main__":
    main()
