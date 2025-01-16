#include "ex6.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define INT_BUFFER 128
//
// // ================================================
// // Basic struct definitions from ex6.h assumed:
// //   PokemonData { int id; char *name; PokemonType TYPE; int hp; int attack; EvolutionStatus CAN_EVOLVE; }
// //   PokemonNode { PokemonData* data; PokemonNode* left, *right; }
// //   OwnerNode   { char* ownerName; PokemonNode* pokedexRoot; OwnerNode *next, *prev; }
// //   OwnerNode* ownerHead;
// //   const PokemonData pokedex[];
// // ================================================
//
// // --------------------------------------------------------------
// // 1) Safe integer reading
// // --------------------------------------------------------------
//

void trimWhitespace(char *str)
{
    // Remove leading spaces/tabs/\r
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\r')
        start++;

    if (start > 0)
    {
        int idx = 0;
        while (str[start])
            str[idx++] = str[start++];
        str[idx] = '\0';
    }

    // Remove trailing spaces/tabs/\r
    int len = (int)strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\r'))
    {
        str[--len] = '\0';
    }
}

// char *myStrdup(const char *src)
// {
//     if (!src)
//         return NULL;
//     size_t len = strlen(src);
//     char *dest = (char *)malloc(len + 1);
//     if (!dest)
//     {
//         printf("Memory allocation failed in myStrdup.\n");
//         return NULL;
//     }
//     strcpy(dest, src);
//     return dest;
// }

int readIntSafe(const char *prompt)
{
    char buffer[INT_BUFFER];
    int value;
    int success = 0;

    while (!success)
    {
        printf("%s", prompt);

        // If we fail to read, treat it as invalid
        if (!fgets(buffer, sizeof(buffer), stdin))
        {
            printf("Invalid input.\n");
            clearerr(stdin);
            continue;
        }

        // 1) Strip any trailing \r or \n
        //    so "123\r\n" becomes "123"
        size_t len = strlen(buffer);
        if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r'))
            buffer[--len] = '\0';
        if (len > 0 && (buffer[len - 1] == '\r' || buffer[len - 1] == '\n'))
            buffer[--len] = '\0';

        // 2) Check if empty after stripping
        if (len == 0)
        {
            printf("Invalid input.\n");
            continue;
        }

        // 3) Attempt to parse integer with strtol
        char *endptr;
        value = (int)strtol(buffer, &endptr, 10);

        // If endptr didn't point to the end => leftover chars => invalid
        // or if buffer was something non-numeric
        if (*endptr != '\0')
        {
            printf("Invalid input.\n");
        }
        else
        {
            // We got a valid integer
            success = 1;
        }
    }
    return value;
}

// // --------------------------------------------------------------
// // 2) Utility: Get type name from enum
// // --------------------------------------------------------------
// const char *getTypeName(PokemonType type)
// {
//     switch (type)
//     {
//     case GRASS:
//         return "GRASS";
//     case FIRE:
//         return "FIRE";
//     case WATER:
//         return "WATER";
//     case BUG:
//         return "BUG";
//     case NORMAL:
//         return "NORMAL";
//     case POISON:
//         return "POISON";
//     case ELECTRIC:
//         return "ELECTRIC";
//     case GROUND:
//         return "GROUND";
//     case FAIRY:
//         return "FAIRY";
//     case FIGHTING:
//         return "FIGHTING";
//     case PSYCHIC:
//         return "PSYCHIC";
//     case ROCK:
//         return "ROCK";
//     case GHOST:
//         return "GHOST";
//     case DRAGON:
//         return "DRAGON";
//     case ICE:
//         return "ICE";
//     default:
//         return "UNKNOWN";
//     }
// }
//
// // --------------------------------------------------------------
// // Utility: getDynamicInput (for reading a line into malloc'd memory)
// // --------------------------------------------------------------
char *getDynamicInput()
{
    char *input = NULL;
    size_t size = 0, capacity = 1;
    input = (char *)malloc(capacity);
    if (!input)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        if (size + 1 >= capacity)
        {
            capacity *= 2;
            char *temp = (char *)realloc(input, capacity);
            if (!temp)
            {
                printf("Memory reallocation failed.\n");
                free(input);
                return NULL;
            }
            input = temp;
        }
        input[size++] = (char)c;
    }
    input[size] = '\0';

    // Trim any leading/trailing whitespace or carriage returns
    trimWhitespace(input);

    return input;
}


// // Function to print a single Pokemon node
// void printPokemonNode(PokemonNode *node)
// {
//     if (!node)
//         return;
//     printf("ID: %d, Name: %s, Type: %s, HP: %d, Attack: %d, Can Evolve: %s\n",
//            node->data->id,
//            node->data->name,
//            getTypeName(node->data->TYPE),
//            node->data->hp,
//            node->data->attack,
//            (node->data->CAN_EVOLVE == CAN_EVOLVE) ? "Yes" : "No");
// }
//
// // --------------------------------------------------------------
// // Display Menu
// // --------------------------------------------------------------
// void displayMenu(OwnerNode *owner)
// {
//     if (!owner->pokedexRoot)
//     {
//         printf("Pokedex is empty.\n");
//         return;
//     }
//
//     printf("Display:\n");
//     printf("1. BFS (Level-Order)\n");
//     printf("2. Pre-Order\n");
//     printf("3. In-Order\n");
//     printf("4. Post-Order\n");
//     printf("5. Alphabetical (by name)\n");
//
//     int choice = readIntSafe("Your choice: ");
//
//     switch (choice)
//     {
//     case 1:
//         displayBFS(owner->pokedexRoot);
//         break;
//     case 2:
//         preOrderTraversal(owner->pokedexRoot);
//         break;
//     case 3:
//         inOrderTraversal(owner->pokedexRoot);
//         break;
//     case 4:
//         postOrderTraversal(owner->pokedexRoot);
//         break;
//     case 5:
//         displayAlphabetical(owner->pokedexRoot);
//         break;
//     default:
//         printf("Invalid choice.\n");
//     }
// }
//
// // --------------------------------------------------------------
// // Sub-menu for existing Pokedex
// // --------------------------------------------------------------
void enterExistingPokedexMenu()
{
    // list owners
    // you need to implement a few things here :)
    OwnerNode* cur=printOwners();    // printf("\nEntering %s's Pokedex...\n", cur->ownerName);
    printf("Entering %s's Pokedex...", cur->ownerName);

    int subChoice;
    do
    {
        printf("\n-- %s's Pokedex Menu --\n", cur->ownerName);
        printf("1. Add Pokemon\n");
        printf("2. Display Pokedex\n");
        printf("3. Release Pokemon (by ID)\n");
        printf("4. Pokemon Fight!\n");
        printf("5. Evolve Pokemon\n");
        printf("6. Back to Main\n");

        subChoice = readIntSafe("Your choice: ");

        switch (subChoice)
        {
        case 1:
             addPokemon(cur);
            break;
        case 2:
            // displayMenu(cur);
            break;
        case 3:
            // freePokemon(cur);
            break;
        case 4:
            // pokemonFight(cur);
            break;
        case 5:
            // evolvePokemon(cur);
            break;
        case 6:
            // printf("Back to Main Menu.\n");
            break;
        default:
            // printf("Invalid choice.\n");
        }
    } while (subChoice != 6);
}

// // --------------------------------------------------------------
// // Main Menu
// // --------------------------------------------------------------
void mainMenu() {
    int choice;
    do {
        printf("\n=== Main Menu ===\n");
        printf("1. New Pokedex\n");
        printf("2. Existing Pokedex\n");
        printf("3. Delete a Pokedex\n");
        printf("4. Merge Pokedexes\n");
        printf("5. Sort Owners by Name\n");
        printf("6. Print Owners in a direction X times\n");
        printf("7. Exit\n");
        choice = readIntSafe("Your choice: ");

        switch (choice)
        {
            case 1:
                openPokedexMenu();
            break;
            case 2:
                enterExistingPokedexMenu();
                break;
            //         case 3:
            //             deletePokedex();
            //             break;
            //         case 4:
            //             mergePokedexMenu();
            //             break;
            //         case 5:
            //             sortOwners();
            //             break;
            //         case 6:
            //             printOwnersCircular();
            //             break;
            //         case 7:
            //             printf("Goodbye!\n");
            //             break;
            //         default:
            //             printf("Invalid.\n");
                }
            } while (choice != 7);
        }



int main()
{
    mainMenu();
    return 0;
}void openPokedexMenu(void) {
    OwnerNode *owner = (OwnerNode *)malloc(sizeof(OwnerNode));
    PokemonNode *ownerPokedex = (PokemonNode *)malloc(sizeof(PokemonNode));
    ownerPokedex->data = (PokemonData *)malloc(sizeof(PokemonData));

    if (!ownerPokedex->data) {
        fprintf(stderr, "Memory allocation failed for PokemonData.\n");
        free(ownerPokedex);
        free(owner);
        exit(1);
    }

    if (!owner) {
        fprintf(stderr, "Memory allocation failed for OwnerNode.\n");
        exit(1);
    }

    printf("Your name:\n");
    owner->ownerName = getDynamicInput();

    if(ownerHead == NULL) {
        // First owner - create circular list
        owner->next = owner;
        owner->prev = owner;
        ownerHead = owner;
    } else {
        // Add to existing circular list
        OwnerNode *tail = ownerHead->prev;
        tail->next = owner;
        owner->prev = tail;
        owner->next = ownerHead;
        ownerHead->prev = owner;
    }

    printf("Choose Starter:\n");
    printf("1. Bulbasaur\n");
    printf("2. Charmander\n");
    printf("3. Squirtle\n");
    int choice = readIntSafe("Your choice: ");

    switch (choice) {
        case 1:
            *ownerPokedex->data = pokedex[0];
        break;
        case 2:
            *ownerPokedex->data = pokedex[3];
        break;
        case 3:
            *ownerPokedex->data = pokedex[6];
        break;
        default:
            printf("Invalid choice.\n");
        free(ownerPokedex->data);
        free(ownerPokedex);
        free(owner);
        return;
    }

    owner->pokedexRoot = ownerPokedex;
    ownerPokedex->left = NULL;
    ownerPokedex->right = NULL;

    printf("New Pokedex created for %s with starter %s\n", owner->ownerName, owner->pokedexRoot->data->name);
}
OwnerNode *printOwners() {
    printf("\nExisting Pokedexes:\n");

    if (ownerHead == NULL) {
        printf("No existing Pokedexes.\n");
        return NULL;
    }

    // Iterate through the circular linked list
    OwnerNode *current = ownerHead;
    int i = 1; // Start index for display
    do {
        printf("%d. %s\n", i++, current->ownerName);
        current = current->next;
    } while (current != ownerHead); // Stop when we loop back to the head

    // Prompt the user to choose an owner
    int choice = readIntSafe("Enter the number of the Pokedex you want to access: ");

    // Validate the user's choice
    if (choice < 1 || choice >= i) {
        printf("Invalid choice. Returning to main menu.\n");
        return NULL;
    }

    // Find the selected owner based on the choice
    current = ownerHead;
    for (int j = 1; j < choice; j++) {
        current = current->next;
    }

    printf("You selected %s's Pokedex.\n", current->ownerName);
    return current;
}

void addPokemon(OwnerNode *owner) {
printf("Enter ID to add:\n");
    int id;
id=readIntSafe("");
    insertPokemon(&owner->pokedexRoot , id);
}
void insertPokemon(PokemonNode **root, int id) {
    if (*root == NULL) {
        // Create a new node
        PokemonNode *newNode = (PokemonNode *)malloc(sizeof(PokemonNode));
        if (!newNode) {
            printf("Memory allocation failed.\n");
            return;
        }
        newNode->data = (PokemonData *)malloc(sizeof(PokemonData));
        if (!newNode->data) {
            printf("Memory allocation failed for PokemonData.\n");
            free(newNode);
            return;
        }
        *newNode->data = pokedex[id - 1]; // Copy data from the global pokedex array
        newNode->left = NULL;
        newNode->right = NULL;
        *root = newNode;
        printf("Pokemon %s (ID %d) added.\n", newNode->data->name, id);
        return;
    }

    // Compare IDs to determine insertion position
    if (id < (*root)->data->id) {
        insertPokemon(&(*root)->left, id);
    } else if (id > (*root)->data->id) {
        insertPokemon(&(*root)->right, id);
    } else {
        printf("Pokemon with ID %d already exists in the Pokedex.\n", id);
    }
}