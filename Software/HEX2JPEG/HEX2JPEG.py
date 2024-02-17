# Assuming hex_data is a string containing the hexadecimal representation of the JPEG image
hex_data=""

# Convert hexadecimal string to bytes
binary_data = bytes.fromhex(hex_data)

print("length of binary data: ", len(binary_data))

# Specify the output file name
output_file = "output.jpg"

# Write the binary data to a JPEG file
with open('output.jpg', "wb") as f:
    f.write(binary_data)

print(f"JPEG image saved as {output_file}")