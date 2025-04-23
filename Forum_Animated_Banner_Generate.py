from PIL import Image, ImageDraw, ImageFont
import numpy as np
import math # Import math for sine function

# --- Animation Settings ---
num_frames = 24      # Number of frames in one twinkle cycle
duration_ms = 100    # Duration each frame is displayed (milliseconds) -> ~10 FPS
min_brightness = 60  # Minimum brightness for a star (0-255)
max_brightness = 255 # Maximum brightness for a star (0-255)

# --- Banner Dimensions ---
width, height = 800, 200

# --- Star Data Storage ---
# Store star info so we can redraw them differently in each frame
star_data = []
total_stars = 150
num_starry_shapes = 45  # 30% of total stars
num_dots = total_stars - num_starry_shapes

# Generate positions, sizes, types, and random phase offsets for twinkling
# Dots
for _ in range(num_dots):
    star_data.append({
        'x': np.random.randint(0, width),
        'y': np.random.randint(0, height),
        'size': np.random.randint(1, 4), # Use this size for drawing
        'type': 'dot',
        'phase': np.random.uniform(0, 2 * math.pi) # Random starting point in twinkle cycle
    })
# Shapes
for _ in range(num_starry_shapes):
    star_data.append({
        'x': np.random.randint(0, width),
        'y': np.random.randint(0, height),
        'size': np.random.randint(3, 6),  # Use this size for drawing
        'type': 'shape',
        'phase': np.random.uniform(0, 2 * math.pi) # Random starting point
    })

# --- Function to draw the static background and text ---
def create_base_banner(width, height):
    # Create a new image with a gradient background (blue to dark violet)
    base_image = Image.new('RGB', (width, height))
    draw = ImageDraw.Draw(base_image)

    # Create a steeper night sky gradient
    for y in range(height):
        t = (y / height) ** 1.5
        r = int(20 + (60 - 20) * t)
        g = int(70 * (1 - t))
        b = int(180 - (180 - 80) * t)
        draw.line((0, y, width, y), fill=(r, g, b))

    # Text settings
    text = "Intention Repeater &\nAvatar Field Forum"
    try:
        font = ImageFont.truetype("WinkyRough-Regular.ttf", 48)
    except IOError: # More specific exception
        font = ImageFont.load_default()
        print("WinkyRough-Regular.ttf not found or unreadable, using default font.")

    # Calculate text size and position
    text_bbox = draw.textbbox((0, 0), text, font=font, align="center")
    text_width = text_bbox[2] - text_bbox[0]
    text_height = text_bbox[3] - text_bbox[1]
    text_x = (width - text_width) // 2
    text_y = 50 - 35 # 5 pixels from the top

    # Add drop shadow
    shadow_offset = 3
    draw.text((text_x + shadow_offset, text_y + shadow_offset), text, font=font, fill='black', align="center")

    # Add main text in gold
    gold_color = (255, 215, 0)
    draw.text((text_x, text_y), text, font=font, fill=gold_color, align="center")

    return base_image

# --- Generate Frames ---
frames = []
print("Generating base banner...")
base_banner = create_base_banner(width, height)
print("Generating animation frames...")

for frame_index in range(num_frames):
    # Start with a copy of the base banner for each frame
    frame_image = base_banner.copy()
    draw = ImageDraw.Draw(frame_image)

    # Calculate twinkle phase for this frame
    # A full sine cycle (2*pi) over num_frames
    twinkle_angle_step = (2 * math.pi) / num_frames
    current_angle = frame_index * twinkle_angle_step

    # Draw stars with varying brightness
    for star in star_data:
        # Calculate brightness using sine wave based on star's phase and current frame angle
        # sin range is -1 to 1. Shift and scale to map to brightness range.
        brightness_factor = (math.sin(current_angle + star['phase']) + 1) / 2 # Map sin(-1,1) -> (0,1)
        brightness = int(min_brightness + (max_brightness - min_brightness) * brightness_factor)
        brightness = np.clip(brightness, 0, 255) # Ensure brightness is within valid range

        star_color = (brightness, brightness, brightness) # White star, varying intensity

        # Draw the star based on its type
        x, y, size = star['x'], star['y'], star['size']
        if star['type'] == 'dot':
            draw.ellipse((x, y, x + size, y + size), fill=star_color)
        else: # 'shape'
            # Draw a small cross/asterisk
            # Use integer division for diagonal offsets
            diag_offset = size // 2
            # Thicker lines for shapes might look better when dimming
            line_width = 1 # Can increase this if needed
            draw.line((x - size, y, x + size, y), fill=star_color, width=line_width) # Horizontal
            draw.line((x, y - size, x, y + size), fill=star_color, width=line_width) # Vertical
            if diag_offset > 0: # Avoid zero-length lines for small sizes
                draw.line((x - diag_offset, y - diag_offset, x + diag_offset, y + diag_offset), fill=star_color, width=line_width) # \
                draw.line((x - diag_offset, y + diag_offset, x + diag_offset, y - diag_offset), fill=star_color, width=line_width) # /


    frames.append(frame_image)
    print(f"Generated frame {frame_index + 1}/{num_frames}")


# --- Save as GIF ---
output_filename = "banner_twinkle.gif"
print(f"Saving animation to '{output_filename}'...")
if frames:
    frames[0].save(
        output_filename,
        save_all=True,          # Save all frames
        append_images=frames[1:], # Add subsequent frames
        duration=duration_ms,   # Time per frame
        loop=0                  # Loop indefinitely (0 means infinite loop)
    )
    print("Animation saved successfully!")
else:
    print("Error: No frames generated.")