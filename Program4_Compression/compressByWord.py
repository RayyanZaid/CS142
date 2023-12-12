import heapq
import os
import json
import bitarray


class Node:
    def __init__(self, char, freq):
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq






def make_frequency_dict(text):
    frequency = {}
    words = text.split()  # Split text into words
    for word in words:
        if word not in frequency:
            frequency[word] = 0
        frequency[word] += 1
    return frequency


def make_huffman_tree(frequency):
    priority_queue = [Node(char, freq) for char, freq in frequency.items()]
    heapq.heapify(priority_queue)

    while len(priority_queue) > 1:
        left = heapq.heappop(priority_queue)
        right = heapq.heappop(priority_queue)

        merged = Node(None, left.freq + right.freq)
        merged.left = left
        merged.right = right

        heapq.heappush(priority_queue, merged)

    return priority_queue[0]


def make_codes(root, current_code, codes):
    if root is None:
        return

    if root.char is not None:
        codes[root.char] = current_code

    make_codes(root.left, current_code + "0", codes)
    make_codes(root.right, current_code + "1", codes)


def huffman_encoding(text):
    frequency = make_frequency_dict(text)
    root = make_huffman_tree(frequency)

    codes = {}
    make_codes(root, "", codes)

    words = text.split()
    encoded_text = ' '.join([codes[word] for word in words])  # Encode words
    return encoded_text, codes

def huffman_decoding(encoded_text, codes):
    reverse_codes = {v: k for k, v in codes.items()}
    decoded_text = ""
    current_code = ""

    for bit in encoded_text.split():
        if bit in reverse_codes:
            decoded_text += reverse_codes[bit] + " "  # Decode words

    return decoded_text.strip()


def compress(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        text = file.read()

    encoded_text, codes = huffman_encoding(text)
    b = bitarray.bitarray(encoded_text)

    compressed_file_path = file_path + '.bin'
    with open(compressed_file_path, 'wb') as file:
        b.tofile(file)

    code_file_path = file_path + '.codes'
    with open(code_file_path, 'w', encoding='utf-8') as file:
        json.dump(codes, file)

    print("Compressed file created:", compressed_file_path)
    print("Codes file created:", code_file_path)


def decompress(compressed_file_path, code_file_path):
    with open(code_file_path, 'r', encoding='utf-8') as file:
        codes = json.load(file)

    b = bitarray.bitarray()
    with open(compressed_file_path, 'rb') as file:
        b.fromfile(file)
    encoded_text = b.to01()

    decoded_text = huffman_decoding(encoded_text, codes)

    decompressed_file_path = compressed_file_path.replace(
        '.bin', '_decompressed.txt')
    with open(decompressed_file_path, 'w', encoding='utf-8') as file:
        file.write(decoded_text)

    print("Decompressed file created:", decompressed_file_path)

    

def count_bits_in_text_file(file_path):
    try:

        with open(file_path, 'rb') as file:
            content = file.read()

        # Calculate the number of bits (I assumed that each character is 1 byte)
        num_bits = len(content) * 8  # use OS sys.getsizeOf(content)

        print(f'The text file {file_path} contains {num_bits} bits.')
    except FileNotFoundError:
        print(f'The file {file_path} does not exist.')
    except Exception as e:
        print(f'An error occurred: {str(e)}')

    return num_bits


def test(inputFileName):

    compress(f'{inputFileName}')
    decompress(f'{inputFileName}.bin', f'{inputFileName}.codes')

    originalBits = count_bits_in_text_file(f"{inputFileName}")
    compressedBits = count_bits_in_text_file(f"{inputFileName}.bin")

    compressionRatio = originalBits / compressedBits
    print(f"Compression ratio is: {compressionRatio}")

    return originalBits, compressedBits, compressionRatio


# put text file name to test

if __name__ == "__main__":
    test("texts/pride.txt")
