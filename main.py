from PIL import Image
import math

def extract_sprites(img, spacing=2, sprites_per_row=5):
    pixels = img.load()
    width, height = img.size

    # Detect sprite boundaries horizontally by transparent columns
    sprites = []
    inside_sprite = False
    start_x = 0

    def column_is_transparent(x):
        for y in range(height):
            if pixels[x, y][3] != 0:
                return False
        return True

    x = 0
    while x < width:
        if not column_is_transparent(x):
            if not inside_sprite:
                inside_sprite = True
                start_x = x
        else:
            if inside_sprite:
                inside_sprite = False
                sprites.append((start_x, x))
        x += 1
    if inside_sprite:
        sprites.append((start_x, width))

    # Crop sprites tightly vertically
    cropped_sprites = []
    max_w, max_h = 0, 0
    for (x0, x1) in sprites:
        box = (x0, 0, x1, height)
        sprite = img.crop(box)

        sprite_data = sprite.load()
        w, h = sprite.size

        # Crop transparent rows top to bottom
        top = 0
        for y in range(h):
            if all(sprite_data[x, y][3] == 0 for x in range(w)):
                top += 1
            else:
                break

        # Crop transparent rows bottom to top
        bottom = h
        for y in reversed(range(h)):
            if all(sprite_data[x, y][3] == 0 for x in range(w)):
                bottom -= 1
            else:
                break

        if bottom <= top:
            continue  # fully transparent

        sprite = sprite.crop((0, top, w, bottom))
        w, h = sprite.size

        if w > max_w: max_w = w
        if h > max_h: max_h = h

        cropped_sprites.append(sprite)

    # Pad sprites to max width and height
    pad_w = max_w + spacing
    pad_h = max_h + spacing

    padded_sprites = []
    for sprite in cropped_sprites:
        new_img = Image.new("RGBA", (pad_w, pad_h), (0, 0, 0, 0))
        offset_x = (pad_w - sprite.width) // 2
        offset_y = (pad_h - sprite.height) // 2
        new_img.paste(sprite, (offset_x, offset_y))
        padded_sprites.append(new_img)

    # Calculate rows and columns for packing
    total_sprites = len(padded_sprites)
    rows = math.ceil(total_sprites / sprites_per_row)

    sheet_width = pad_w * sprites_per_row
    sheet_height = pad_h * rows

    result = Image.new("RGBA", (sheet_width, sheet_height), (0, 0, 0, 0))

    for index, spr in enumerate(padded_sprites):
        row = index // sprites_per_row
        col = index % sprites_per_row
        x = col * pad_w
        y = row * pad_h
        result.paste(spr, (x, y))

    return result

if __name__ == "__main__":
    name = input()
    path = f'images/{name}'
    img = Image.open(path).convert("RGBA")
    output = extract_sprites(img, spacing=0, sprites_per_row=8)  # adjust sprites_per_row as needed
    output.save(f"images/packed_{name}")
