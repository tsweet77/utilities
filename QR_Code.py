import qrcode

# Function to generate QR code from text and save as JPG
def generate_qr_code(text, filename="qrcode.jpg"):
    # Create a QR Code instance
    qr = qrcode.QRCode(
        version=1,  # version parameter controls the size of the QR Code
        error_correction=qrcode.constants.ERROR_CORRECT_L,  # Error correction level
        box_size=10,  # Size of each box in the QR code grid
        border=4,  # Thickness of the border (minimum is 4)
    )
    
    # Add data to the QR code
    qr.add_data(text)
    qr.make(fit=True)

    # Create an image from the QR code instance
    img = qr.make_image(fill_color="black", back_color="white")

    # Save the image as a JPG file
    img.save(filename)
    print(f"QR Code saved as {filename}")

# Example usage
text_input = input("Enter the text to encode in the QR code: ")
generate_qr_code(text_input)
