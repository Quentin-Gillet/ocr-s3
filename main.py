import random
import csv
from PIL import Image, ImageDraw, ImageFont
import uuid
import os
from pathlib import Path
import shutil


# Set all font you want to use (.ttf)
# If you get "OSError: cannot open resource" it's because font is not on your computer
# Caused by bad path or bad font name
font_names = ['ARIAL.ttf']


def main(image_count: int) -> None:
    dirpath = Path('images')
    if dirpath.exists() and dirpath.is_dir():
        shutil.rmtree(dirpath)
    try:
        os.mkdir('images')
    except OSError as e:
        pass
    with open('images_link.csv', 'w', newline='') as csv_file:
        csv_writer = csv.writer(csv_file)
        #csv_writer.writerow(['image_name', 'expected_result'])
        for i in range(image_count):
            number = random.randint(0, 9)
            image_name = str(uuid.uuid1())
            generate_image(number, image_name)
            csv_writer.writerow([f'{image_name}.png', number])


def generate_image(number: int, image_name: str) -> None:
    img = Image.new('RGB', (25, 25), color='white')

    # Replace path with local path to fonts
    fnt = ImageFont.truetype(f'C:\FG\Font/{random.choice(font_names)}', random.randint(15, 18))
    d = ImageDraw.Draw(img)
    d.text((7, 3), f"{number}", font=fnt, fill=(0, 0, 0))

    img.save(f'images/{image_name}.png')


if __name__ == '__main__':
    main(image_count=5)
