import argparse

def main():
    # Set up the argument parser
    parser = argparse.ArgumentParser(description="Mock Game Simulator")
    parser.add_argument("--player1", type=str, help="Name of Player 1")
    parser.add_argument("--player2", type=str, help="Name of Player 2")
    parser.add_argument("--winner", type=str, help="Name of the winner (optional)")

    # Parse the arguments
    args = parser.parse_args()

    # Determine the output
    if args.winner:
        print(args.winner)
    else:
        print(f"{args.player1} {args.player2}")  # Print both player names in case of a draw

if __name__ == "__main__":
    main()
