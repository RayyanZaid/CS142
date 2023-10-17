import random

# Generate 500 random positive decimal numbers
random_numbers = [random.uniform(0, 1000) for _ in range(500)]

# Save the numbers to a text file
with open('random_numbers.txt', 'w') as file:
    for number in random_numbers:
        file.write(f"{number}\n")

print("Random numbers saved to 'random_numbers.txt'")