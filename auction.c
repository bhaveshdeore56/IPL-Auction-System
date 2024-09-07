#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Constants for maximum team name length and number of teams
#define MAX_TEAM_NAME_LENGTH 5
#define NUM_TEAMS 4

// Constants for file names
#define PLAYER_DATA_FILE "player_data.txt"
#define SOLD_PLAYERS_FILE "sold_players.txt"
#define UNSOLD_PLAYERS_FILE "unsold_players.txt"

// Player structure
typedef struct Player {
    char name[50];
    char team[MAX_TEAM_NAME_LENGTH + 1]; // +1 for null terminator
    int basePrice;
    int sold;
    int currentBid;
} Player;

// AVL tree node structure
typedef struct Node {
    Player player;
    struct Node *left;
    struct Node *right;
    int height;
} Node;

// Team structure
typedef struct Team {
    char name[MAX_TEAM_NAME_LENGTH + 1]; // +1 for null terminator
    int budget;
} Team;

// Function prototypes
int max(int a, int b);
int height(Node *node);
int getBalance(Node *node);
Node *rightRotate(Node *y);
Node *leftRotate(Node *x);
Node *insertPlayer(Node *root, Player player);
Node *deletePlayer(Node *root, char playerName[]);
Node *minValueNode(Node *node);
void displayInOrder(Node *temp);
void performAuction(Node *root, Team teams[], int numPlayers);
void displayMenu();
void displayTeamInformation(Team teams[]);
void displayRemainingPlayers(Node *root);
bool validateBid(int bid, int currentBid, int basePrice, int prevMaxBid);

// Main function
int main() {
    Node *root = NULL; // Initialize AVL tree root
    Team teams[NUM_TEAMS] = {{"RCB", 2000}, {"MI", 2000}, {"CSK", 2000}, {"KKR", 2000}};
    FILE *file = fopen(PLAYER_DATA_FILE, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }
    int numPlayers;
    fscanf(file, "%d", &numPlayers);
    Player *players = malloc(numPlayers * sizeof(Player));
    for (int i = 0; i < numPlayers; ++i) {
        fscanf(file, "%s %d", players[i].name, &players[i].basePrice);
        players[i].sold = 0;
        players[i].currentBid = 0;
        root = insertPlayer(root, players[i]);
    }
    Node *temp=root;
    fclose(file);
    displayRemainingPlayers(root);
    performAuction(root, teams, numPlayers);
    printf("\nFinal Teams Information:\n");
    displayTeamInformation(teams);



    // Display contents of sold_players.txt
printf("\nContents of sold_players.txt:\n");
FILE *soldFile = fopen(SOLD_PLAYERS_FILE, "r");
if (soldFile == NULL) {
    printf("Error opening sold_players.txt.\n");
} else {
    char playerName[50],teamname[4];
    int basePrice, currentBid;
    while (fscanf(soldFile, "%s %s %d %d", playerName, teamname,&basePrice,&currentBid) != EOF) {
        printf("Player: %s, Sold to Team: %s,  Base Price: %d, Current Bid: %d\n", playerName,teamname, basePrice, currentBid);
    }
    fclose(soldFile);
}

// Display contents of unsold_players.txt
printf("\nContents of unsold_players.txt:\n");
FILE *unsoldFile = fopen(UNSOLD_PLAYERS_FILE, "r");
if (unsoldFile == NULL) {
    printf("Error opening unsold_players.txt.\n");
} else {
    char playerName[50];
    int basePrice;
    while (fscanf(unsoldFile, "%s %d", playerName, &basePrice) != EOF) {
        printf("Player: %s, Base Price: %d\n", playerName, basePrice);
    }
    fclose(unsoldFile);
}


    free(players);
    return 0;
}

// Function to get maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Function to get the height of a node in the AVL tree
int height(Node *node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Function to rotate the subtree rooted with y to the right
Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

// Function to rotate the subtree rooted with x to the left
Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

// Get the balance factor of a node
int getBalance(Node *node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// Function to insert a player into the AVL tree
Node *insertPlayer(Node *root, Player player) {
    if (root == NULL) {
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->player = player;
        newNode->left = newNode->right = NULL;
        newNode->height = 1;
        return newNode;
    }
    if (strcmp(player.name, root->player.name) < 0)
        root->left = insertPlayer(root->left, player);
    else if (strcmp(player.name, root->player.name) > 0)
        root->right = insertPlayer(root->right, player);
    else
        return root;
    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);
    if (balance > 1 && strcmp(player.name, root->left->player.name) < 0)
        return rightRotate(root);
    if (balance < -1 && strcmp(player.name, root->right->player.name) > 0)
        return leftRotate(root);
    if (balance > 1 && strcmp(player.name, root->left->player.name) > 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && strcmp(player.name, root->right->player.name) < 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

// Function to delete a player from the AVL tree
Node *deletePlayer(Node *root, char playerName[]) {
    if (root == NULL)
        return root;
    if (strcmp(playerName, root->player.name) < 0)
        root->left = deletePlayer(root->left, playerName);
    else if (strcmp(playerName, root->player.name) > 0)
        root->right = deletePlayer(root->right, playerName);
    else {
        if (root->left == NULL || root->right == NULL) {
            Node *temp = root->left ? root->left : root->right;
            if (temp == NULL) {//
                temp = root;
                root = NULL;
            } 
            else//
                *root = *temp;
            free(temp);
        } 
        else {
            Node *temp = minValueNode(root->right);
            root->player = temp->player;
            root->right = deletePlayer(root->right, temp->player.name);
        }
    }
    if (root == NULL)
        return root;
    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

// Function to find the node with the minimum value in the AVL tree
Node *minValueNode(Node *node) {
    struct Node *current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// Function to display the AVL tree in-order
void displayInOrder(Node *temp) {
    if (temp != NULL) {
        displayInOrder(temp->left);
        printf("%s\t\t%d\t\t%d\n", temp->player.name, temp->player.basePrice, temp->player.currentBid);
        displayInOrder(temp->right);
    }
}

// Function to display team information
void displayTeamInformation(Team teams[]) {
    printf("Team\tBudget\n");
    printf("----------------\n");
    for (int i = 0; i < NUM_TEAMS; ++i) {
        printf("%s\t%d\n", teams[i].name, teams[i].budget);
    }
    printf("\n");
}

// Function to validate bid
bool validateBid(int bid, int currentBid, int basePrice, int prevMaxBid) {
    if (bid <= currentBid || bid < basePrice || bid <= prevMaxBid || bid <= 0) {
        return false;
    }
    return true;
}

// Function to display remaining players
void displayRemainingPlayers(Node *temp) {
    displayInOrder(temp);
}

// Function to perform auction process
void performAuction(Node *root, Team teams[], int numPlayers) {
    FILE *soldFile = fopen(SOLD_PLAYERS_FILE, "w");
    FILE *unsoldFile = fopen(UNSOLD_PLAYERS_FILE, "w");
    int prevMaxBid = 0;
    while (numPlayers > 0) {
        Node *minPlayerNode = minValueNode(root);
        Player minPlayer = minPlayerNode->player;
        int currentBid = (minPlayer.sold == 0) ? minPlayer.currentBid : minPlayer.basePrice;
        printf("\nPlayer: %s\nBase Price: %d\nCurrent Bid: %d\n", minPlayer.name, minPlayer.basePrice, currentBid);
        printf("Teams Information:\n");
        displayTeamInformation(teams);
        int maxBid = currentBid;
        int maxBidTeamIndex = -1;
        int teamsWithBids = 0;
        for (int j = 0; j < NUM_TEAMS; ++j) {
            int skipBid = 0;
            do {
                printf("Team %s,Press 1 to Bid or Press 0 to skip: \n", teams[j].name);
                scanf("%d", &skipBid);
                if (skipBid != 0 && skipBid != 1) {
                    printf("Invalid input. Please enter 0 to skip or 1 to bid.\n");
                }
            } while (skipBid != 0 && skipBid != 1);
            if (skipBid == 0) {
                printf("Team %s skipped.\n", teams[j].name);
            } else {
                int bid;
                do {
                    printf("Enter your bid: ");
                    scanf("%d", &bid);
                    if (!validateBid(bid, maxBid, minPlayer.basePrice, prevMaxBid)) {
                        printf("Invalid bid. Please enter a valid bid.\n");
                    }
                } while (!validateBid(bid, maxBid, minPlayer.basePrice, prevMaxBid));
                if (bid > maxBid && bid <= teams[j].budget) {
                    maxBid = bid;
                    maxBidTeamIndex = j;
                }
                teamsWithBids++;
            }
        }
        if (teamsWithBids > 1) {
            printf("\nPlayer %s will come back for the next bid round.\n", minPlayer.name);
            prevMaxBid = maxBid;
        } else if (maxBid == 0) {
            printf("Player %s skipped.\n", minPlayer.name);
            fprintf(unsoldFile, "%s %d\n", minPlayer.name, minPlayer.basePrice);
            root = deletePlayer(root, minPlayer.name);//not sure
            numPlayers--;
        } else {
            minPlayer.currentBid = maxBid;
            printf("Team %s successfully bid %d. Remaining budget: %d\n",
                   teams[maxBidTeamIndex].name, maxBid, teams[maxBidTeamIndex].budget);
            teams[maxBidTeamIndex].budget -= maxBid;
            minPlayer.sold = 1;
            fprintf(soldFile, "%s %s %d %d\n", minPlayer.name, teams[maxBidTeamIndex].name, minPlayer.basePrice, minPlayer.currentBid);
            printf("\nBid Status:\n");
            printf("SOLD TO:\n");
            printf("Team\tPlayer\t\tBid\n");
            printf("----------------------------------------\n");
            printf("%s\t%s\t\t%d\n", teams[maxBidTeamIndex].name, minPlayer.name, maxBid);
            printf("----------------------------------------\n");
            root = deletePlayer(root, minPlayer.name);//not sure
            numPlayers--;
            prevMaxBid=0;
        }
    }
    fclose(soldFile);
    fclose(unsoldFile);
}
