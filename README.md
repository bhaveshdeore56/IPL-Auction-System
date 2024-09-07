# IPL-Auction-System

## Introduction

This project implements an **Auction System** where teams bid for players. The system uses **AVL Trees** to manage and organize player data efficiently. Each player has a base price, and teams take turns bidding until a player is either sold or marked as unsold. The auction ensures fair and efficient management of players, with the help of AVL trees to maintain balance and quick lookups.

## Features

- **Player Management Using AVL Trees**: Players are stored and managed using an AVL tree, ensuring balanced and efficient insertion, deletion, and lookup operations.
- **Bidding System**: Multiple teams can place bids for players. The highest valid bid wins the player, and the system tracks team budgets to ensure no team overspends.
- **File I/O for Auction Results**:
  - **Input**: Player details are read from a `player_data.txt` file.
  - **Output**: The auction results are written to two files:
    - `sold_players.txt` (for players who were sold)
    - `unsold_players.txt` (for players who remained unsold)
- **Team Budget Management**: Teams have fixed budgets at the start, and each team's remaining budget is updated dynamically as bids are placed.

## Requirements

- **C Compiler**: A standard C compiler is required to compile and run the program.
- **Player Data File**: The player data must be provided in a file called `player_data.txt`, containing the names and base prices of players.

## How to Run

1. **Prepare the Player Data**

   
   Ensure that a `player_data.txt` file exists in the same directory as the program. This file should contain player information, formatted as:
   <number_of_players>
   <Player_name> <base_price><br/>
   **Example:**<br/>
   3<br/>
   Player1 500<br/>
   Player2 200<br/>
   Player3 350<br/>

   
2. **Compile the Program**


Compile the C code using a standard C compiler:

```bash
gcc -o auction auction.c
```
3. **Run the Program**

After compiling, run the program:

```bash
./auction
```

## Auction Process

- Teams will take turns placing bids on players.
- Each team has the option to place a bid or skip.
- The player is sold to the team with the highest bid, and the system records the result.

##  Output Files

After the auction concludes:

- `sold_players.txt`: Contains the details of the players sold, including the team that won the bid and the final bid amount.
- `unsold_players.txt`: Lists the players who were not sold during the auction.

## Team Budget

- RCB: Starting budget of 2000
- MI: Starting budget of 2000
- CSK: Starting budget of 2000
- KKR: Starting budget of 2000

## Player Status

- Players can either be sold or unsold.
- Players are sold if at least one valid bid is placed within the team's budget.
- Players remain unsold if no bids are placed or all teams skip bidding.

## File Structure

- `player_data.txt`: Contains the list of players with their base prices.
- `sold_players.txt`: Contains the list of players who were successfully auctioned.
- `unsold_players.txt`: Contains the list of players who were not auctioned.


