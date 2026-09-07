from PIL import Image
import sys
import os


def rgb888_to_rgb565(r, g, b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def get_transparent_index(img):
    transparency = img.info.get("transparency")

    if transparency is None:
        raise RuntimeError("Indexed PNG must have transparency")

    if isinstance(transparency, int):
        return transparency

    if isinstance(transparency, bytes):
        for i, alpha in enumerate(transparency):
            if alpha == 0:
                return i

    raise RuntimeError("Could not determine transparent index")


def main(png_path, name):

    img = Image.open(png_path)

    if img.mode != "P":
        raise RuntimeError("Image must be indexed (mode 'P')")

    width, height = img.size
    pixels = list(img.getdata())
    palette = img.getpalette()

    if palette is None:
        raise RuntimeError("Indexed image has no palette")

    transparent_index = get_transparent_index(img)

    if transparent_index != 0:
        raise RuntimeError(
            f"Transparent color must be palette index 0 (found {transparent_index})"
        )

    used_indices = set(pixels)

    if max(used_indices) > 15:
        raise RuntimeError(
            "Image uses palette indices above 15 (not valid 4-bpp)"
        )

    # -------------------------------------------------
    # Convert first 16 palette entries exactly as-is
    # -------------------------------------------------
    palette565 = []

    for i in range(16):
        r, g, b = palette[i * 3 : i * 3 + 3]
        palette565.append(rgb888_to_rgb565(r, g, b))

    # -------------------------------------------------
    # Pack pixels directly (no remapping!)
    # -------------------------------------------------
    packed = []

    for y in range(height):
        for x in range(0, width, 2):

            p0 = pixels[y * width + x]
            p1 = pixels[y * width + x + 1] if x + 1 < width else 0

            packed.append((p0 << 4) | (p1 & 0x0F))

    # -------------------------------------------------
    # Output
    # -------------------------------------------------
    base = name.lower()
    macro = base.upper()

    c_file = f"{base}.c"
    h_file = f"{base}.h"

    with open(h_file, "w") as h:
        h.write(f"// Generated from {os.path.basename(png_path)}\n")
        h.write("#pragma once\n\n")
        h.write("#include <stdint.h>\n\n")
        h.write(f"#define {macro}_W {width}\n")
        h.write(f"#define {macro}_H {height}\n\n")
        h.write(f"extern const uint16_t {base}_palette[16];\n")
        h.write(f"extern const uint8_t  {base}_data[];\n")

    with open(c_file, "w") as c:
        c.write(f"// Generated from {os.path.basename(png_path)}\n")
        c.write(f'#include "{base}.h"\n\n')

        c.write(f"const uint16_t {base}_palette[16] = {{\n")
        for color in palette565:
            c.write(f"    0x{color:04X},\n")
        c.write("};\n\n")

        c.write(f"const uint8_t {base}_data[] = {{\n")

        for i, b in enumerate(packed):
            if i % 12 == 0:
                c.write("    ")
            c.write(f"0x{b:02X}, ")
            if i % 12 == 11:
                c.write("\n")

        c.write("\n};\n")

    print(f"Wrote {c_file} and {h_file}")


if __name__ == "__main__":

    if len(sys.argv) != 3:
        print("Usage: python png_to_tile4.py input.png name")
        sys.exit(1)

    main(sys.argv[1], sys.argv[2])