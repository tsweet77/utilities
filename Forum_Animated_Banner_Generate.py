from PIL import Image, ImageDraw, ImageFont
import numpy as np
import math

# --- Animation Settings ---
num_frames = 40      # More frames can make the sine wave twinkle smoother
duration_ms = 75     # Adjust speed (~13 FPS)
max_brightness = 255 # Peak brightness for a star (when fully visible)
# No min_brightness needed for background fade
# No rate variables needed as cycle length is now fixed by num_frames

# --- Banner Dimensions ---
width, height = 800, 200

# --- Star Data Storage ---
star_data = []
total_stars = 150
num_starry_shapes = 45
num_dots = total_stars - num_starry_shapes

print("Generating star data...")
# Generate positions, sizes, types, and phases (rate is not needed)
# Dots
for _ in range(num_dots):
    star_data.append({
        'x': np.random.randint(0, width),
        'y': np.random.randint(0, height),
        'size': np.random.randint(1, 4),
        'type': 'dot',
        'phase': np.random.uniform(0, 2 * math.pi) # Random starting point in twinkle cycle
    })
# Shapes
for _ in range(num_starry_shapes):
    star_data.append({
        'x': np.random.randint(0, width),
        'y': np.random.randint(0, height),
        'size': np.random.randint(3, 6),
        'type': 'shape',
        'phase': np.random.uniform(0, 2 * math.pi) # Random starting point
    })
print(f"Generated data for {len(star_data)} stars.")

# --- Function to calculate background color at a specific Y ---
def get_background_color(y, h):
    """Calculates the gradient color at a given y-coordinate."""
    y_clipped = max(0, min(y, h - 1))
    t = (y_clipped / h) ** 1.5
    r = int(20 + (60 - 20) * t)
    g = int(70 * (1 - t))
    b = int(180 - (180 - 80) * t)
    return (np.clip(r, 0, 255), np.clip(g, 0, 255), np.clip(b, 0, 255))

# --- Function to draw the static background and text ---
def create_base_banner(width, height):
    base_image = Image.new('RGB', (width, height))
    draw = ImageDraw.Draw(base_image)
    for y in range(height):
        draw.line((0, y, width, y), fill=get_background_color(y, height))

    text = "Intention Repeater &\nAvatar Field Forum"
    try:
        font = ImageFont.truetype("WinkyRough-Regular.ttf", 48)
    except IOError:
        try:
            font = ImageFont.truetype("arial.ttf", 40)
            print("WinkyRough-Regular.ttf not found or unreadable, using Arial font.")
        except IOError:
            font = ImageFont.load_default()
            print("WinkyRough-Regular.ttf and Arial not found, using default PIL font.")

    text_bbox = draw.textbbox((0, 0), text, font=font, align="center")
    text_width = text_bbox[2] - text_bbox[0]
    text_height = text_bbox[3] - text_bbox[1]
    text_x = (width - text_width) // 2
    text_y = 50 - 35 # 5 pixels from the top

    shadow_offset = 3
    draw.text((text_x + shadow_offset, text_y + shadow_offset), text, font=font, fill='black', align="center")
    gold_color = (255, 215, 0)
    draw.text((text_x, text_y), text, font=font, fill=gold_color, align="center")
    return base_image

# --- Generate Frames ---
frames = []
print("Generating base banner...")
base_banner = create_base_banner(width, height)
print("Generating animation frames...")

peak_star_color = (max_brightness, max_brightness, max_brightness)
# Define the angular length of one full cycle (used for sine wave)
cycle_length = 2 * math.pi

for frame_index in range(num_frames):
    frame_image = base_banner.copy()
    draw = ImageDraw.Draw(frame_image)

    # Calculate the base angle progression for this frame.
    # All stars use this same progression, ensuring synchronized cycle length.
    twinkle_angle_step = cycle_length / num_frames
    current_base_angle = frame_index * twinkle_angle_step

    # Draw stars, fading them into the background color smoothly
    for star in star_data:
        # 1. Calculate the effective angle for this star using its phase offset
        #    NO rate multiplication - cycle length is fixed.
        angle_for_star = current_base_angle + star['phase']

        # 2. Calculate brightness factor using Sine wave (smooth 0 to 1 oscillation)
        brightness_factor = (math.sin(angle_for_star) + 1) / 2

        # 3. Get the background color at the star's position
        bg_color = get_background_color(star['y'], height)

        # 4. Interpolate between background color and peak star color
        final_r = int(bg_color[0] * (1 - brightness_factor) + peak_star_color[0] * brightness_factor)
        final_g = int(bg_color[1] * (1 - brightness_factor) + peak_star_color[1] * brightness_factor)
        final_b = int(bg_color[2] * (1 - brightness_factor) + peak_star_color[2] * brightness_factor)

        final_color = (np.clip(final_r, 0, 255),
                       np.clip(final_g, 0, 255),
                       np.clip(final_b, 0, 255))

        # 5. Draw the star with the calculated interpolated color
        x, y, size = star['x'], star['y'], star['size']
        if star['type'] == 'dot':
            draw.ellipse((x, y, x + size, y + size), fill=final_color)
        else: # 'shape'
            diag_offset = size // 2
            line_width = 1
            draw.line((x - size, y, x + size, y), fill=final_color, width=line_width)
            draw.line((x, y - size, x, y + size), fill=final_color, width=line_width)
            if diag_offset > 0:
                draw.line((x - diag_offset, y - diag_offset, x + diag_offset, y + diag_offset), fill=final_color, width=line_width)
                draw.line((x - diag_offset, y + diag_offset, x + diag_offset, y - diag_offset), fill=final_color, width=line_width)

    frames.append(frame_image)
    if (frame_index + 1) % 5 == 0 or frame_index == num_frames - 1:
        print(f"Generated frame {frame_index + 1}/{num_frames}")

# --- Save as GIF ---
output_filename = "banner_smooth_fade_offset.gif" # New filename
print(f"Saving animation to '{output_filename}'...")
if frames:
    frames[0].save(
        output_filename,
        save_all=True,
        append_images=frames[1:],
        duration=duration_ms,
        loop=0
    )
    print("Animation saved successfully!")
else:
    print("Error: No frames generated.")