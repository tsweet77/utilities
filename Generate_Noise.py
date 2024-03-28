import numpy as np
import soundfile as sf
import random

def generate_white_noise(duration, sample_rate):
    return np.random.uniform(-1, 1, size=duration * sample_rate)

def generate_brown_noise(duration, sample_rate):
    noise = np.zeros(duration * sample_rate)
    noise[0] = random.uniform(-1, 1)
    for i in range(1, len(noise)):
        noise[i] = noise[i - 1] + random.gauss(0, 1)
    noise /= np.max(np.abs(noise))
    return noise

def generate_pink_noise(duration, sample_rate):
    noise = np.zeros(duration * sample_rate)
    X = np.random.uniform(-1, 1, size=duration * sample_rate)
    noise[0] = X[0]
    for i in range(1, len(noise)):
        noise[i] = 0.99886 * noise[i - 1] + X[i] * 0.0555179
    noise /= np.max(np.abs(noise))
    return noise

def generate_blue_noise(duration, sample_rate):
    noise = np.zeros(duration * sample_rate)
    X = np.random.uniform(-1, 1, size=duration * sample_rate)
    noise[0] = X[0]
    for i in range(1, len(noise)):
        noise[i] = 0.99765 * noise[i - 1] - X[i] * 0.099046
    noise /= np.max(np.abs(noise))
    return noise

# Prompt the user for noise type and duration
noise_type = input("Generate white, brown, pink, or blue noise: ")
duration = int(input("Enter the duration in seconds: "))

# Set the sample rate
sample_rate = 48000

# Generate the noise based on user input
if noise_type.lower() == "white":
    noise = generate_white_noise(duration, sample_rate)
elif noise_type.lower() == "brown":
    noise = generate_brown_noise(duration, sample_rate)
elif noise_type.lower() == "pink":
    noise = generate_pink_noise(duration, sample_rate)
elif noise_type.lower() == "blue":
    noise = generate_blue_noise(duration, sample_rate)
else:
    print("Invalid noise type. Please choose white, brown, pink, or blue.")
    exit(1)

# Save the noise as a WAV file
filename = f"{noise_type}_noise_{duration}s.wav"
sf.write(filename, noise, sample_rate, subtype='PCM_32')
print(f"Noise saved as {filename}")