from PIL import Image, ImageDraw, ImageFont
import numpy as np

# Banner dimensions
width, height = 800, 200

# Create a new image with a gradient background (blue to dark violet)
image = Image.new('RGB', (width, height))
draw = ImageDraw.Draw(image)

# Create a steeper night sky gradient (lighter blue at top, darker violet at horizon)
for y in range(height):
    # Steeper gradient: top is a lighter blue (20, 70, 180), bottom is a darker violet (60, 0, 80)
    t = (y / height) ** 1.5  # Exponent > 1 makes the gradient steeper toward the bottom
    r = int(20 + (60 - 20) * t)  # Red: 20 to 60
    g = int(70 * (1 - t))        # Green: 70 to 0
    b = int(180 - (180 - 80) * t)  # Blue: 180 to 80
    draw.line((0, y, width, y), fill=(r, g, b))

# Add stars: 150 total, 70% dots (105), 30% starry shapes (45)
total_stars = 150
num_starry_shapes = 45  # 30% of total stars
num_dots = total_stars - num_starry_shapes

# Add dot stars
for _ in range(num_dots):
    x = np.random.randint(0, width)
    y = np.random.randint(0, height)
    size = np.random.randint(1, 4)  # Small dots, range 1-3 pixels
    draw.ellipse((x, y, x + size, y + size), fill='white')

# Add starry shapes (simple cross/asterisk pattern)
for _ in range(num_starry_shapes):
    x = np.random.randint(0, width)
    y = np.random.randint(0, height)
    size = np.random.randint(3, 6)  # Slightly larger for visibility, 3-5 pixels
    # Draw a small cross/asterisk for a star shape
    draw.line((x - size, y, x + size, y), fill='white')  # Horizontal line
    draw.line((x, y - size, x, y + size), fill='white')  # Vertical line
    draw.line((x - size // 2, y - size // 2, x + size // 2, y + size // 2), fill='white')  # Diagonal \
    draw.line((x - size // 2, y + size // 2, x + size // 2, y - size // 2), fill='white')  # Diagonal /

# Text settings
text = "Intention Repeater &\nAvatar Field Forum"
try:
    # Use WinkyRough-Regular.ttf from the same folder
    font = ImageFont.truetype("WinkyRough-Regular.ttf", 48)
except:
    # Fallback to default font if WinkyRough-Regular.ttf isn't found
    font = ImageFont.load_default()
    print("WinkyRough-Regular.ttf not found, using default font.")

# Calculate text size and position
text_bbox = draw.textbbox((0, 0), text, font=font, align="center")
text_width = text_bbox[2] - text_bbox[0]
text_height = text_bbox[3] - text_bbox[1]
text_x = (width - text_width) // 2
# Move text up 35 pixels from current position (50 pixels from top)
text_y = 50 - 35  # Now 5 pixels from the top

# Add drop shadow (slightly offset black text)
shadow_offset = 3
draw.text((text_x + shadow_offset, text_y + shadow_offset), text, font=font, fill='black', align="center")

# Add main text in gold
gold_color = (255, 215, 0)  # Gold color
draw.text((text_x, text_y), text, font=font, fill=gold_color, align="center")

# Save the image
image.save("banner.png")
print("Banner saved as 'banner.png'!")