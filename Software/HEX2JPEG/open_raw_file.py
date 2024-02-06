with open('tst.RAW', 'rb') as fobj:
    bytes_data = fobj.read()

    # Convert bytes to a list of hex strings
    hex_list = [hex(byte)[2:].zfill(2) for byte in bytes_data]

    # Convert the list of hex strings to a single string without spaces
    result_string = ''.join(hex_list)

    # Print the string without spaces
    print(result_string)