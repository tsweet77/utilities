# conda create -n mp3toyoutube python=3.7.4
# conda activate mp3toyoutube
# pip install ffmpeg
# pip install Pillow==9.5.0

from PIL import Image, ImageDraw, ImageFont
import subprocess
import sys

def create_background_image(filename):
    text    = "I am Love and Bliss"
    textfontsize = 128
    subtext = "(174Hz) 33.333% Smoothing"
    subtextfontsize = 100
    backgroundcolor = "#301934"
    textcolor = "yellow"
    subtextcolor = "yellow"
    textyoffset = -80
    subtextyoffset = 120

    # Create an image with a specific background color
    image = Image.new('RGB', (1920, 1080), color = backgroundcolor)
    draw = ImageDraw.Draw(image)

    # Define a font
    textfont = ImageFont.truetype('times.ttf', textfontsize)

    # Use textsize to get the width and height of the text
    text_width, text_height = draw.textsize(text, font=textfont)

    # Now you can use text_width and text_height to position your text
    # For example, to center the text:
    image_width, image_height = image.size
    x = (image_width - text_width) / 2
    y = (image_height - text_height) / 2 + textyoffset

    # Draw the text
    draw.text((x, y), text, fill=textcolor, font=textfont)

    # Define a font
    subtextfont = ImageFont.truetype('times.ttf', subtextfontsize)
    
    subtext_width, subtext_height = draw.textsize(subtext, font=subtextfont)

    # Now you can use text_width and text_height to position your text
    # For example, to center the text:
    image_width, image_height = image.size
    x2 = (image_width - subtext_width) / 2
    y2 = (image_height - subtext_height) / 2 + subtextyoffset

    # Draw the text
    draw.text((x2, y2), subtext, fill=subtextcolor, font=subtextfont)

    # Save the image
    image.save('background.png')

def convert_to_mp4(audio_file):
    # Create the background image
    create_background_image(audio_file)
    
    # Prepare output filename
    output_file = audio_file.rsplit('.', 1)[0] + '.mp4'
    
    # Use FFmpeg to combine the image and audio into an MP4
    command = [
        'ffmpeg',
        '-loop', '1',
        '-i', 'background.png',
        '-i', audio_file,
        '-c:v', 'libx264',
        '-tune', 'stillimage',
        '-c:a', 'aac',
        '-b:a', '192k',
        '-pix_fmt', 'yuv420p',
        '-shortest',
        output_file
    ]
    subprocess.run(command)

if __name__ == "__main__":
    # Take the audio file name from the command line argument
    audio_file = sys.argv[1]
    convert_to_mp4(audio_file)
