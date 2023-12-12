import os
import matplotlib.pyplot as plt

import compressByChar
import compressByWord

def barGraphComparingWordVsChar(directory):
    # Lists to store data for plotting
    filenames = []
    char_original_bits = []
    char_compressed_bits = []
    word_original_bits = []
    word_compressed_bits = []
    char_compression_ratios = []
    word_compression_ratios = []

    # Iterate over each text file in the directory
    for eachText in os.listdir(directory):
        if eachText.endswith(".txt"):  # Ensure it's a text file
            print(f"Processing {eachText}")
            filenames.append(eachText)

            pathToText = os.path.join(directory, eachText)

            # Get data from character-based compression
            char_original, char_compressed, char_ratio = compressByChar.test(pathToText)
            char_original_bits.append(char_original)
            char_compressed_bits.append(char_compressed)
            char_compression_ratios.append(char_ratio)

            # Get data from word-based compression
            word_original, word_compressed, word_ratio = compressByWord.test(pathToText)
            word_original_bits.append(word_original)
            word_compressed_bits.append(word_compressed)
            word_compression_ratios.append(word_ratio)

    # Plotting
    x = range(len(filenames))  # x-coordinates for the plots

    # Graph 1: Character-based original vs compressed
    plt.figure(figsize=(10, 6))
    plt.bar(x, char_original_bits, width=0.4, label='Original Bits', align='center')
    plt.bar(x, char_compressed_bits, width=0.4, label='Compressed Bits', align='edge')
    plt.xlabel('Text Files')
    plt.ylabel('Bits')
    plt.title('Character-Based Compression')
    plt.xticks(x, filenames, rotation=45)
    plt.legend()
    plt.tight_layout()
    plt.savefig("Graphs/Characters.png")
    plt.show()
    
    # Graph 2: Word-based original vs compressed
    plt.figure(figsize=(10, 6))
    plt.bar(x, word_original_bits, width=0.4, label='Original Bits', align='center')
    plt.bar(x, word_compressed_bits, width=0.4, label='Compressed Bits', align='edge')
    plt.xlabel('Text Files')
    plt.ylabel('Bits')
    plt.title('Word-Based Compression')
    plt.xticks(x, filenames, rotation=45)
    plt.legend()
    plt.tight_layout()
    plt.savefig("Graphs/Words.png")
    plt.show()

    # Graph 3: Compression Ratios
    plt.figure(figsize=(10, 6))
    plt.bar(x, char_compression_ratios, width=0.4, label='Char Compression Ratio', align='center')
    plt.bar(x, word_compression_ratios, width=0.4, label='Word Compression Ratio', align='edge')
    plt.xlabel('Text Files')
    plt.ylabel('Compression Ratio')
    plt.title('Compression Ratios of Word and Character')
    plt.xticks(x, filenames, rotation=45)
    plt.legend()
    plt.tight_layout()
    plt.savefig("Graphs/Comparison.png")
    plt.show()
    
# Call the function with the directory containing the text files
barGraphComparingWordVsChar("texts")
