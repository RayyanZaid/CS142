import heapq
from collections import Counter

def build_huffman_tree(text):
    """
    Build the Huffman tree for the given text.
    Returns the root of the tree and the dictionary with character encodings.
    """
    frequency = Counter(text)
    heap = [[weight, [symbol, ""]] for symbol, weight in frequency.items()]
    heapq.heapify(heap)

    while len(heap) > 1:
        lo = heapq.heappop(heap)
        hi = heapq.heappop(heap)
        for pair in lo[1:]:
            pair[1] = '0' + pair[1]
        for pair in hi[1:]:
            pair[1] = '1' + pair[1]
        heapq.heappush(heap, [lo[0] + hi[0]] + lo[1:] + hi[1:])
    
    tree = sorted(heapq.heappop(heap)[1:], key=lambda p: (len(p[-1]), p))
    huffman_code = {symbol: code for symbol, code in tree}
    return huffman_code

def compress_huffman(text, huffman_code):
    """
    Compress the text using the Huffman coding.
    Returns the compressed binary string.
    """
    return ''.join(huffman_code[char] for char in text)

def decompress_huffman(compressed_text, huffman_code):
    """
    Decompress the compressed text using the Huffman coding.
    Returns the original text.
    """
    reverse_huffman_code = {v: k for k, v in huffman_code.items()}
    decoded_text = ''
    current_code = ''
    for bit in compressed_text:
        current_code += bit
        if current_code in reverse_huffman_code:
            decoded_text += reverse_huffman_code[current_code]
            current_code = ''
    return decoded_text

# Example usage
def read_text_from_file(file_path):
    try:
        with open(file_path, 'r') as file:
            return file.read()
    except FileNotFoundError:
        print(f"File '{file_path}' not found.")
        return None

def write_text_to_file(file_path, text):
    with open(file_path, 'w') as file:
        file.write(text)

# Read text from a file
file_path = "input.txt"  # Replace with the path to your text file
text = read_text_from_file(file_path)

if text is not None:
    # Perform Huffman coding operations on the text
    huffman_code = build_huffman_tree(text)
    compressed_text = compress_huffman(text, huffman_code)
    decompressed_text = decompress_huffman(compressed_text, huffman_code)

    # Print and save the results
    print("Original Text:", text)
    print("Compressed Text:", compressed_text)
    print("Decompressed Text:", decompressed_text)
    print("Huffman Code:", huffman_code)

    # Save the compressed text to a file if needed
    write_text_to_file("compressed_text.txt", compressed_text)
else:
    print("Unable to proceed due to missing input text.")


def count_bits_in_text_file(file_path):
    try:
        # Open the file in binary mode and read its content
        with open(file_path, 'rb') as file:
            content = file.read()

        # Calculate the number of bits
        num_bits = len(content) * 8

        print(f'The text file {file_path} contains {num_bits} bits.')
    except FileNotFoundError:
        print(f'The file {file_path} does not exist.')
    except Exception as e:
        print(f'An error occurred: {str(e)}')

# Example usage:
file_path = 'input.txt'
count_bits_in_text_file(file_path)
file_path = 'compressed.txt'
count_bits_in_text_file(file_path)