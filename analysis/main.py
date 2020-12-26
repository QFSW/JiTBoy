from loader import load_data
from plot import scatter

data_path = "output/results.csv"

def main():
    data = load_data(data_path)
    scatter(data, 'hotness', 'mips', 'output/graphs/hotness.png', xscale='log', yscale='log')
    scatter(data, 'source block size', 'efficiency c', 'output/graphs/efficiency.png', xscale='log', yscale='linear')

if __name__ == "__main__":
    main()