import numpy as np
import matplotlib.image as mpimg
import matplotlib.pyplot as plt

# Helper Functions
class Methods:
    def convert_to_binary(self, n):
        """Converts a decimal number to an 8-bit binary array."""
        binary = [int(x) for x in bin(n)[2:]]
        while len(binary) < 8:
            binary.insert(0, 0)
        return binary

    def convert_to_decimal(self, binary):
        """Converts an 8-bit binary array to a decimal number."""
        return sum(val * (2 ** idx) for idx, val in enumerate(binary[::-1]))

# Grayscale Conversion
def rgb_to_gray(image):
    """Converts an RGB image to grayscale using the luminance formula."""
    gray = np.dot(image[..., :3], [0.2989, 0.5870, 0.1140])
    return np.round(gray * 255).astype(np.uint8)

# Cellular Automata Encryption
def cellular_automata_encrypt(image, rule_list, cycles):
    """Applies cellular automata rules to encrypt the image."""
    height, width = image.shape
    encrypted_image = np.copy(image)
    methods = Methods()

    for cycle in range(cycles):
        new_image = np.zeros_like(encrypted_image, dtype=int)
        for i in range(height):
            for j in range(width):
                pixel = encrypted_image[i][j]
                binary_pixel = methods.convert_to_binary(pixel)
                new_binary_pixel = []

                for k in range(8):
                    rule = rule_list[k]

                    if k == 0:
                        a, b, c = 0, binary_pixel[k], binary_pixel[k + 1]
                    elif k == 7:
                        a, b, c = binary_pixel[k - 1], binary_pixel[k], 0
                    else:
                        a, b, c = binary_pixel[k - 1], binary_pixel[k], binary_pixel[k + 1]

                    if rule == 51:
                        exp = int(not b)
                    elif rule == 85:
                        exp = int(not c)
                    elif rule == 86:
                        exp = int((a and not c) or (b and not c) or (c and not a and not b))
                    elif rule == 102:
                        exp = int((b and not c) or (c and not b))
                    elif rule == 105:
                        exp = int((a and b and not c) or (a and c and not b) or (b and c and not a) or (not a and not b and not c))
                    elif rule == 150:
                        exp = int((a and b and c) or (a and not b and not c) or (b and not a and not c) or (c and not a and not b))
                    elif rule == 153:
                        exp = int((b and c) or (not b and not c))
                    elif rule == 165:
                        exp = int((a and c) or (not a or not c))
                    elif rule == 240:
                        exp = int(a)

                    new_binary_pixel.append(exp)

                new_pixel = methods.convert_to_decimal(new_binary_pixel)
                new_image[i][j] = new_pixel

        encrypted_image = new_image

    # Apply modular transformation
    encrypted_image = (encrypted_image * 187) % 256
    return encrypted_image

# Cellular Automata Decryption
def cellular_automata_decrypt(image, rule_list, cycles):
    """Applies cellular automata rules to decrypt the image."""
    height, width = image.shape
    decrypted_image = (image * 115) % 256  # Reverse modular transformation
    methods = Methods()

    for cycle in range(cycles):
        new_image = np.zeros_like(decrypted_image, dtype=int)
        for i in range(height):
            for j in range(width):
                pixel = decrypted_image[i][j]
                binary_pixel = methods.convert_to_binary(pixel)
                new_binary_pixel = []

                for k in range(8):
                    rule = rule_list[k]

                    if k == 0:
                        a, b, c = 0, binary_pixel[k], binary_pixel[k + 1]
                    elif k == 7:
                        a, b, c = binary_pixel[k - 1], binary_pixel[k], 0
                    else:
                        a, b, c = binary_pixel[k - 1], binary_pixel[k], binary_pixel[k + 1]

                    if rule == 51:
                        exp = int(not b)
                    elif rule == 85:
                        exp = int(not c)
                    elif rule == 86:
                        exp = int((a and not c) or (b and not c) or (c and not a and not b))
                    elif rule == 102:
                        exp = int((b and not c) or (c and not b))
                    elif rule == 105:
                        exp = int((a and b and not c) or (a and c and not b) or (b and c and not a) or (not a and not b and not c))
                    elif rule == 150:
                        exp = int((a and b and c) or (a and not b and not c) or (b and not a and not c) or (c and not a and not b))
                    elif rule == 153:
                        exp = int((b and c) or (not b and not c))
                    elif rule == 165:
                        exp = int((a and c) or (not a or not c))
                    elif rule == 240:
                        exp = int(a)

                    new_binary_pixel.append(exp)

                new_pixel = methods.convert_to_decimal(new_binary_pixel)
                new_image[i][j] = new_pixel

        decrypted_image = new_image

    return decrypted_image

# Main Functionality
if __name__ == "__main__":
    # Load the image
    input_image = mpimg.imread("Lenna_(test_image).png")

    # Convert to grayscale
    gray_image = rgb_to_gray(input_image)
    plt.imsave("gray_image.png", gray_image, cmap='gray')

    # Encryption
    rule_list = [102, 102, 102, 102, 51, 86, 240, 153]
    encrypted_image = cellular_automata_encrypt(gray_image, rule_list, cycles=4)
    plt.imsave("encrypted_image.png", encrypted_image, cmap='gray')

    # Decryption
    decrypted_image = cellular_automata_decrypt(encrypted_image, rule_list, cycles=4)
    plt.imsave("decrypted_image.png", decrypted_image, cmap='gray')

    print("Images saved: gray_image.png, encrypted_image.png, decrypted_image.png")
