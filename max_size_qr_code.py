import qrcode

# Function to generate a string that fits the max size for a high correction QR code
def generate_max_qr_string(input_text):
    # Max size for high error correction QR code (Version 40) in bytes
    max_bytes = 1274

    # Initialize the result string
    result_string = ""

    # Keep adding the input text until it reaches the max size
    while len(result_string.encode('utf-8')) + len(input_text.encode('utf-8')) <= max_bytes:
        result_string += input_text

    return result_string

# Get input from the user
input_text = input("Enter Text: ")

# Generate the repeated string
max_qr_string = generate_max_qr_string(input_text)

# Print the result
print(f"Generated QR Code String ({len(max_qr_string.encode('utf-8'))} bytes):")
print(max_qr_string)

# Optionally, create and save the QR code image
qr = qrcode.QRCode(
    version=40,
    error_correction=qrcode.constants.ERROR_CORRECT_H,
    box_size=10,
    border=4,
)
qr.add_data(max_qr_string)
qr.make(fit=True)

img = qr.make_image(fill="black", back_color="white")
img.save("max_size_qr_code.png")
print("QR code image saved as 'max_size_qr_code.png'.")
input("Press Enter to continue...")
