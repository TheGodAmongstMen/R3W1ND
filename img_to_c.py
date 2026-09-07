from PIL import Image

img = Image.open("mainscreen.png").convert("RGB")
w, h = img.size
pixels = img.load()
with open('output.txt', 'w') as f:
    print(f"// {w}x{h} RGB565",file=f)
    print(f"const uint16_t ghostline_img[{w*h}] = {{",file=f)

    for y in range(h):
        for x in range(w):
            r, g, b = pixels[x, y]
            rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
            print(f"0x{rgb565:04X},", end="", file=f)
        print("",file=f)

    print("};",file=f)
