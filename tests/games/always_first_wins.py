import argparse

def main():
    # Set up the argument parser
    parser = argparse.ArgumentParser(description="Mock Game Simulator")
    parser.add_argument("--player1", type=str, help="Name of Player 1")
    parser.add_argument("--player2", type=str, help="Name of Player 2")

    # Parse the arguments
    args = parser.parse_args()

    print(args.player1)

if __name__ == "__main__":
    main()
