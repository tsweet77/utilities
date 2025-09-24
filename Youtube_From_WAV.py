#!/usr/bin/env python3
import subprocess
import sys
import argparse
import os
import shutil

IMAGE_EXTENSIONS = ('.png', '.jpg', '.jpeg', '.bmp', '.webp')
AUDIO_EXTENSIONS = ('.wav', '.mp3', '.m4a', '.flac', '.aac', '.ogg')

def check_ffmpeg_installed():
    """Checks if ffmpeg is installed and available in the system's PATH."""
    if not shutil.which("ffmpeg"):
        print("Error: ffmpeg is not installed or not found in your system's PATH.", file=sys.stderr)
        print("Please install ffmpeg to use this script.", file=sys.stderr)
        print("You can download it from https://ffmpeg.org/download.html", file=sys.stderr)
        sys.exit(1)

def run_ffmpeg_with_progress(command):
    """
    Executes an FFmpeg command and displays a clean, single-line progress indicator.
    
    Args:
        command (list): A list of strings representing the FFmpeg command.
    
    Returns:
        bool: True if the command was successful, False otherwise.
    """
    print("\nRunning FFmpeg command...")
    try:
        process = subprocess.Popen(
            command, 
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            encoding='utf-8'
        )

        for line in process.stderr:
            # FFmpeg progress lines start with 'frame='
            line_stripped = line.strip()
            if line_stripped.startswith("frame="):
                # Use carriage return '\r' to overwrite the line
                print(f"\rProcessing... {line_stripped}", end="", flush=True)
            else:
                # Print other ffmpeg info/warnings that don't belong on the progress line
                print(line_stripped)
        
        process.wait()
        
        # Print a final newline to move off the progress line
        print()

        if process.returncode == 0:
            return True
        else:
            print(f"\nFFmpeg failed with exit code {process.returncode}", file=sys.stderr)
            return False

    except FileNotFoundError:
        print("Error: 'ffmpeg' command not found. Is FFmpeg installed and in your PATH?", file=sys.stderr)
        return False
    except Exception as e:
        print(f"An unexpected error occurred: {e}", file=sys.stderr)
        return False

def create_video_from_audio_and_image(audio_path, image_path, output_path):
    """
    Creates a 1920x1080 video by combining a static image and an audio file.
    """
    # Pre-flight checks are unchanged...
    if not os.path.exists(audio_path): sys.exit(f"Error: Input audio file not found at '{audio_path}'")
    if not os.path.exists(image_path): sys.exit(f"Error: Input image file not found at '{image_path}'")
    if not image_path.lower().endswith(IMAGE_EXTENSIONS): sys.exit(f"Error: Invalid image file type for '{os.path.basename(image_path)}'. Use {IMAGE_EXTENSIONS}")
    if not audio_path.lower().endswith(AUDIO_EXTENSIONS): sys.exit(f"Error: Invalid audio file type for '{os.path.basename(audio_path)}'. Use {AUDIO_EXTENSIONS}")

    print(f"Audio Input:  {audio_path}")
    print(f"Image Input:  {image_path}")
    print(f"Video Output: {output_path}")

    video_filter = "scale=1920:1080:force_original_aspect_ratio=decrease,pad=1920:1080:(ow-iw)/2:(oh-ih)/2:color=black"
    
    # --- THIS IS THE CORRECTED COMMAND ---
    command = [
        'ffmpeg',
        '-y',
        '-loop', '1',
        '-i', image_path,
        '-i', audio_path,
        '-vf', video_filter,
        '-c:v', 'libx264',
        '-tune', 'stillimage',
        '-c:a', 'aac',
        '-b:a', '192k',
        '-pix_fmt', 'yuv420p',
        '-shortest',
        # --- KEY CHANGES ---
        '-hide_banner',  # Hides the version and build info block
        '-stats',        # Explicitly tells FFmpeg to print stats (progress info)
        output_path
    ]
    
    success = run_ffmpeg_with_progress(command)

    if success:
        print(f"Successfully created 1920x1080 video: {output_path}")
    else:
        print("Video creation failed.")
        sys.exit(1)

def main():
    # main() function is unchanged...
    check_ffmpeg_installed()
    parser = argparse.ArgumentParser(
        description="Create a 1920x1080 video from a static image and an audio file.",
        formatter_class=argparse.RawTextHelpFormatter
    )
    parser.add_argument('-a', '--audio', required=True, help="Path to the input audio file (e.g., sound.wav, music.mp3)")
    parser.add_argument('-i', '--image', required=True, help="Path to the background image file (e.g., background.png, cover.jpg).\nIt will be automatically scaled and padded to fit a 1920x1080 frame.")
    parser.add_argument('-o', '--output', help="Path for the output video file (e.g., video.mp4).\nIf not specified, it will be derived from the audio filename.")
    args = parser.parse_args()
    output_file = args.output
    if not output_file:
        base_name = os.path.splitext(args.audio)[0]
        output_file = f"{base_name}.mp4"
    create_video_from_audio_and_image(args.audio, args.image, output_file)

if __name__ == "__main__":
    main()