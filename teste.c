// 2 players Batlleship Classic Board Game

#include <stdio.h>
#include <stdlib.h>
#include <time.h>



#define S 5  // Total ships
#define D 6 // Board Size

typedef struct {

    char* name;
    int  key;
    int  size;

} info;

typedef struct {

    char name[10];
    int** board;
    int** play;
    int pts;

} data;


const info ships[] = {{"Aircraft Carrier", 1, 5},
                      {"Battleship", 2, 4},
                      {"Cruiser", 3, 3},
                      {"Submarine", 4, 3},
                      {"Destroyer", 5, 2}};


void start_game();
void clear_buffer();
void free_board(int** board);
void print_board(int** board);
void print_banner(int player);

int* gets_coord();
int** init_board();
int** set_ships(int opt);
int player_shot(int** board, int** enemy );
int gets_input(char* options, char first, char second);
int check_overlap(int** board, int display, int size, int row, int col);

data gets_data(int number);



int main() {

    int opt = 1;
    while(opt) {
        system("clear");
        start_game();

        /*** BANNER ***/
        printf("\n\n\t╔══╗╔═══╦════╦════╦╗──╔═══╦═══╦╗─╔╦══╦═══╗");
        printf("\n\t║╔╗║║╔═╗║╔╗╔╗║╔╗╔╗║║──║╔══╣╔═╗║║─║╠╣╠╣╔═╗║");
        printf("\n\t║╚╝╚╣║─║╠╝║║╚╩╝║║╚╣║──║╚══╣╚══╣╚═╝║║║║╚═╝║");
        printf("\n\t║╔═╗║╚═╝║─║║───║║─║║─╔╣╔══╩══╗║╔═╗║║║║╔══╝");
        printf("\n\t║╚═╝║╔═╗║─║║───║║─║╚═╝║╚══╣╚═╝║║─║╠╣╠╣║");
        printf("\n\t╚═══╩╝─╚╝─╚╝───╚╝─╚═══╩═══╩═══╩╝─╚╩══╩╝\n\n\n\n");
        /***       ***/

        opt = gets_input("\n\tSTART NEW GAME [Y/N]? ", 89, 78);
    }
}


/*** Function to start the game ***/
void start_game() {
    data player[2];
    int turn, shot;

    // Gets user informations
    for(int i = 0; i < 2; i++)
        player[i] = gets_data(i + 1);

    turn = 0;

    while(player[0].pts && player[1].pts) {
        system("clear");
        printf("\n\n\t--------------------------\t%s's ATTACK\t--------------------------\n\n", player[!turn].name);
        print_board(player[turn].board);

        printf("\n\n\t--------------------------\t%s's TURN\t--------------------------\n\n", player[turn].name);
        print_board(player[turn].play);

        shot = player_shot(player[turn].play, player[!turn].board);
        system("clear");

        if(shot)
            printf("\n\t\tTARGET HIT!");
        else
            printf("\n\t\tSHOT MISSED!");

        print_board(player[turn].play);

        printf("\n\n\t\tPRESS ENTER TO CONTINUE\n\n");
        clear_buffer();

        if(shot) {
            player[turn].pts -= 1;
            continue;
        }

        turn = !turn;
    }

    print_board(player[turn].play);
    printf("\n\t****************%s WIN!****************\n\n", player[turn].name);

    printf("\n\n\t\tPRESS ENTER TO CONTINUE\n\n");
    clear_buffer();

    for(size_t i = 0; i < 2; i++) {
        free(player[i].board);
        free(player[i].play);
    }

    system("clear");

    return;
}

/*** Function to gets the shot coordinates and check if it was valid.
Assign the corresponding value on the boards and return the shot value ***/
int player_shot(int** board, int** enemy ) {
    int *coord, shot;

    while(1) {
        coord = (int *)malloc(sizeof(int) * 2);
        coord = gets_coord();

        if(!board[coord[0]][coord[1]]) {
            shot = enemy[coord[0]][coord[1]];
            break;
        }

        printf("\n\tYou already shot there!");
        free(coord);
    }

    if(shot) {
        board[coord[0]][coord[1]] = shot;
        enemy[coord[0]][coord[1]] = -2;

    } else {
        board[coord[0]][coord[1]] = -1;
        enemy[coord[0]][coord[1]] = -1;

    }

    free(coord);

    return shot;
}


/*** Function to gets the player information and the option for the board display ***/
data gets_data(int number) {
    data user_data;
    int mode;

    user_data.play = init_board();
    print_banner(number);

    printf("\tPlayer %i name: ", (number));
    scanf("%s", &user_data.name);
    clear_buffer();

    printf("\tSet ships mode:\n");
    printf("\n\t1 - Player set ships");
    printf("\n\t2 - Random set ships");

    // Option 2 for random set ships and 1 to player set the ships
    mode = gets_input("", '2', '1');
    user_data.board = set_ships(mode);

    user_data.pts = 0;
    for(size_t i = 0; i < S; i++)
        user_data.pts += ships[i].size;

    system("clear");

    return user_data;

}


/*** Function to print the board with the corresponding char ***/
void print_board(int** board) {

    // ~ : Water, * : Shot Missed, X : Enemy Target Hit
    char opt[3] = {'~', '*', 'X'}, target;
    int index;

    puts("\n\n");
    putchar('\t');
    for(int i = 0; i < D; i++)
        printf("\t%i", i);
    putchar('\n');

    for(int i = 0; i < D; i++) {
        printf("\n\n\t%i\t", i);
        for(int j = 0; j < D; j++) {
            index = board[i][j];

            if(index < 1)
                // 0 for water, -1 for shot missed and -2 to enemy target hit
                target = opt[(index * -1)];
            else
                target = ships[(index - 1)].name[0];

            printf("%c\t", target);
        }
    }

    // Legends
    printf("\n\n\t~ : Water\t* : Shot missed\n\n\t");
    for(size_t i = 0; i < S; i++)
        printf("%c: 1 x %i\t", ships[i].name[0], ships[i].size);
    puts("\n\n");

    return;
}


/*** Function to check if the ship position is valid ***/
int check_overlap(int** board, int display, int size, int row, int col) {
    int empty, start, temp;

    empty = 0;

    // start = col if display is H and strat = row if display i V
    start = (display)? col : row;

    for(size_t i = start; i < D; i++) {

        // To get the cell value and check if it has a ship
        temp = (display)
                ? board[row][i]
                : board[i][col];
        if(temp)
            return 1;

        // To check if the ship fits in the empties cells
        empty += 1;
        if(empty >= size)
            return 0;
    }

    // The ship doesn't fit in the empties
    return 2;
}


/*** Function to display ships in the board.
opt = 0 to player set the ships and opt = 1 to random set ships ***/
int** set_ships(int opt) {

    int display, *coord, choice;
    int overlap, row, col;
    int** board = init_board();

    srand(time(NULL));

    for(size_t i = 0; i < S; i++) {
        overlap = 0;
        if(!opt)
            print_board(board);

        do {
            if (opt) {
                // 0 to Vertical and 1 to Horizontal display
                display = rand() % 2;

                row = rand() % D;
                col = rand() % D;
            }
            else {
                if(overlap)
                    printf("\n\tOverlap! Try again: \n");

                printf("\n\n\t%s display", ships[i].name);

                display = gets_input("Horizontal or Vertical [H/V]?  ", 'H', 'V');
                coord = gets_coord();
                row = coord[0];
                col = coord[1];
                overlap = 1;

                free(coord);

            }

        } while(check_overlap(board, display, ships[i].size, row, col));

        // Assign the key ship value to the baord
        for(int j = 0; j < ships[i].size; j++) {
            if(display)
                board[row][col + j] = ships[i].key;

            else
                board[row + j][col] = ships[i].key;
        }
    }

    print_board(board);
    choice = gets_input("Did you like the display? [Y/N]? ", 'Y', 'N');

    if(!choice) {
        free_board(board);
        board = set_ships(opt);
    }

    return board;
}


/*** Function to get the option chosen
Return 1 if it is equal to the first and 0 if it is equal to the second ***/
int gets_input(char* options, char first, char second) {
    char input;

    while(1) {
        printf("\n\t%s", options);
        scanf("%c", &input);
        clear_buffer();

        if (input == first || input == second)
            break;

        printf("\n\tOption not found! Try again!\n");
    }

    return (input == first);
}


/*** Function to get the coordinates and check if it is in the board size ***/
int* gets_coord() {
    int* coord = (int *)malloc(sizeof(int) * 2);

    while(1) {
        printf("\n\tRow: ");
        scanf("%i", &coord[0]);
        clear_buffer();

        if(coord[0] < 0 || coord[0] >= D) {
            printf("\n\tThe board is not that big!");
            continue;
        }


        printf("\tCol: ");
        scanf("%i", &coord[1]);
        clear_buffer();

        if(coord[1] < 0 || coord[1] >= D) {
            printf("\n\tThe board is not that big!");
            continue;
        }

        break;
    }

    return coord;
}


/*** Function to clear the buffer ***/
void clear_buffer() {
    char c;

    while((c = getchar()) != '\n' && c != EOF) {}

    return;
}


/*** Function to return a null matrix(DxD) ***/
int** init_board() {
    int** matrix = (int **) malloc(sizeof(int *) * D);

    for(size_t i = 0; i < D; i++)
        matrix[i] = (int *) calloc(D , sizeof(int));

    return matrix;
}


/*** Function to free the memory used by the boards ***/
void free_board(int** board) {
    for(size_t i = 0; i < D; i++)
        free(board[i]);

    free(board);

    return;
}



/**************************BANNERS**************************
**************************Made in FSymbols******************/


void print_banner(int player) {
    printf("\n\t╔═══╦╗──╔═══╦╗──╔╦═══╦═══╗");
    printf("\n\t║╔═╗║║──║╔═╗║╚╗╔╝║╔══╣╔═╗║");
    printf("\n\t║╚═╝║║──║║─║╠╗╚╝╔╣╚══╣╚═╝║");
    printf("\n\t║╔══╣║─╔╣╚═╝║╚╗╔╝║╔══╣╔╗╔╝");
    printf("\n\t║║──║╚═╝║╔═╗║─║║─║╚══╣║║╚╗");
    printf("\n\t╚╝──╚═══╩╝─╚╝─╚╝─╚═══╩╝╚═╝");

    if(player == 1) {

        printf("\n\n\t\t\t╔═══╦═╗─╔╦═══╗");
        printf("\n\t\t\t║╔═╗║║╚╗║║╔══╝");
        printf("\n\t\t\t║║─║║╔╗╚╝║╚══╗");
        printf("\n\t\t\t║║─║║║╚╗║║╔══╝");
        printf("\n\t\t\t║╚═╝║║─║║║╚══╗");
        printf("\n\t\t\t╚═══╩╝─╚═╩═══╝\n\n\n\n");
    }

    else {
        printf("\n\n\t\t\t╔════╦╗╔╗╔╦═══╗");
        printf("\n\t\t\t║╔╗╔╗║║║║║║╔═╗║");
        printf("\n\t\t\t╚╝║║╚╣║║║║║║─║║");
        printf("\n\t\t\t──║║─║╚╝╚╝║║─║║");
        printf("\n\t\t\t──║║─╚╗╔╗╔╣╚═╝║");
        printf("\n\t\t\t──╚╝──╚╝╚╝╚═══╝\n\n\n\n");
    }

}















