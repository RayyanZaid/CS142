import matplotlib.pyplot as plt

data_file = "data.txt"

n_values = []
time_1_thread = []
time_2_threads = []

with open(data_file, "r") as file:
    lines = file.readlines()
    for line in lines:
        parts = line.split(', ')
        time_1_thread.append(int(parts[0]))
        time_2_threads.append(int(parts[1]))
        n_values.append(int(parts[2]))

plt.plot(n_values, time_1_thread, label="1 thread", marker='o')
plt.plot(n_values, time_2_threads, label="2 threads", marker='x')

plt.xlabel("Number of Elements (n)")
plt.ylabel("Time (nanoseconds)")
plt.title("Execution Time for 1 Thread vs. 2 Threads")
plt.legend()
plt.grid(True)

# Save the plot to a file before displaying it
plt.savefig("version3TimeComparison.png")

# Display the plot
plt.show()
