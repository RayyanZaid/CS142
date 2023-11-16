import matplotlib.pyplot as plt

def plot_from_file(filename):

    num_processes = []
    time_taken = []

    with open(filename, 'r') as file:
        for line in file:
            parts = line.split(',')
            if len(parts) == 2:
                # Extracting number of processes
                num_proc_str = parts[0].split(': ')[1]
                num_processes.append(int(num_proc_str))

                # Extracting time taken
                time_str = parts[1].split(' ')[3]
                time_taken.append(float(time_str))

    # Plotting the graph
    plt.figure(figsize=(10, 6))
    plt.plot(num_processes, time_taken, marker='o')

    if("processes" in filename):
        plt.title('Num Processes vs Speed (num elements held to 200 constant)')
        plt.xlabel('Number of Processes')
    else:
        plt.title('Num Elements vs Speed (processes held to 10 constant)')
        plt.xlabel('Number of Elements')
    plt.ylabel('Time Taken (seconds)')
    plt.xscale('log')
    plt.yscale('linear')
    plt.grid(True)
    plt.savefig(f'{filename}.png', format='png', dpi=300)

    plt.show()

# Example usage
plot_from_file('speed_increasing_processes.txt')
plot_from_file('speed_increasing_max_num_element.txt')
