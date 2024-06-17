#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//https://youtu.be/BCVEAP_NHR0
char colors[5] = {'A', 'B', 'C', 'D', 'E'};
char **board;
int N;
// tahtay� olu�turan ve harfleri i�ine rastgele koyan fonksiyon
void initializeBoard() {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if ((i == N / 2 || i == N / 2 - 1) && (j == N / 2 || j == N / 2 - 1)) {
                board[i][j] = ' ';
            } else {
                board[i][j] = colors[rand() % 5];
            }
        }
    }
}
//tahtay� ekrana bas�yor
void printBoard() {
    int i, j;
    printf("  ");
    for (i = 0; i < N; i++) {
        printf(" %d", i);
    }
    printf("\n");

    for (i = 0; i < N; i++) {
        printf("%d ", i);
        for (j = 0; j < N; j++) {
            printf(" %c", board[i][j]);
        }
        printf("\n");
    }
}
// se�ilen hamlenin m�mk�n olup olmad���n kontrol ediyor
int isValidMove(int x1, int y1, int x2, int y2) {
    if (x1 == x2 && abs(y1 - y2) == 2) {
        if (board[x1][(y1 + y2) / 2] != ' ' && board[x1][y2] == ' ') return 1;
    } else if (y1 == y2 && abs(x1 - x2) == 2) {
        if (board[(x1 + x2) / 2][y1] != ' ' && board[x2][y1] == ' ') return 1;
    }
    return 0;
}
// hamleyi yap�p tahtay� g�ncelliyor
void makeMove(int x1, int y1, int x2, int y2, int *playerScore) {
    char captured = board[(x1 + x2) / 2][(y1 + y2) / 2];
    for (int i = 0; i < 5; i++) {
        if (captured == colors[i]) {
            playerScore[i]++;
        }
    }
    board[x2][y2] = board[x1][y1];
    board[x1][y1] = ' ';
    board[(x1 + x2) / 2][(y1 + y2) / 2] = ' ';
}
//tahtay� temizliyor
void freeBoard() {
    for (int i = 0; i < N; i++) {
        free(board[i]);
    }
    free(board);
}
//scorelar� yaz�yor
void printScores(char *player1, int *score1, char *player2, int *score2) {
    printf("Scores:\n");
    printf("%s: ", player1);
    for (int i = 0; i < 5; i++) {
        printf("%c: %d ", colors[i], score1[i]);
    }
    printf("\n");
    printf("%s: ", player2);
    for (int i = 0; i < 5; i++) {
        printf("%c: %d ", colors[i], score2[i]);
    }
    printf("\n");
}
//oyunu kaydeden fonksiyon
void saveGame(char *player1, char *player2, int *score1, int *score2, char *currentPlayer, int mode) {
    FILE *file = fopen("savegame.txt", "w");
    if (!file) {
        printf("Error saving game!\n");
        return;
    }
    fprintf(file, "%d\n", N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(file, "%c", board[i][j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "%s\n", player1);
    fprintf(file, "%s\n", player2);
    for (int i = 0; i < 5; i++) {
        fprintf(file, "%d ", score1[i]);
    }
    fprintf(file, "\n");
    for (int i = 0; i < 5; i++) {
        fprintf(file, "%d ", score2[i]);
    }
    fprintf(file, "\n");
    fprintf(file, "%s\n", currentPlayer);
    fprintf(file, "%d\n", mode); 
    fclose(file);
    printf("Game saved successfully.\n");
}
//oyunu geri y�kleyen fonksiyon
void loadGame(char *player1, char *player2, int *score1, int *score2, char **currentPlayer, int *mode) {
    FILE *file = fopen("savegame.txt", "r");
    if (!file) {
        printf("Error loading game!\n");
        return;
    }
    fscanf(file, "%d\n", &N);
    board = (char **)malloc(N * sizeof(char *));
    for (int i = 0; i < N; i++) {
        board[i] = (char *)malloc(N * sizeof(char));
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fscanf(file, "%c", &board[i][j]);
        }
        fgetc(file); // scanfden sonra girdi kalmamas�n� sa�l�yor
    }
    fscanf(file, "%s\n", player1);
    fscanf(file, "%s\n", player2);
    for (int i = 0; i < 5; i++) {
        fscanf(file, "%d", &score1[i]);
    }
    for (int i = 0; i < 5; i++) {
        fscanf(file, "%d", &score2[i]);
    }
    char currentPlayerName[50];
    fscanf(file, "%s\n", currentPlayerName);
    if (strcmp(currentPlayerName, player1) == 0) {
        *currentPlayer = player1;
    } else {
        *currentPlayer = player2;
    }
    fscanf(file, "%d\n", mode); 
    fclose(file);
    printf("Game loaded successfully.\n");
}
//bilgisayar�n hamlesini yap�yor algoritmay� bulamad�m bu y�zden rasgele hamle se�ip uyup uymad���na g�re hamleyi yap�yor
void computerMove(int *playerScore) {
    int x1, y1, x2, y2;
    do {
        x1 = rand() % N;
        y1 = rand() % N;
        if (rand() % 2) {
            x2 = x1;
            y2 = y1 + (rand() % 2 ? 2 : -2);
        } else {
            y2 = y1;
            x2 = x1 + (rand() % 2 ? 2 : -2);
        }
    } while (!isValidMove(x1, y1, x2, y2));
    printf("Computer moves from (%d, %d) to (%d, %d)\n", x1, y1, x2, y2);
    makeMove(x1, y1, x2, y2, playerScore);
}

int main() {
    srand(time(0));

    char player1[50], player2[50];
    int score1[5] = {0};
    int score2[5] = {0};
    char *currentPlayer = player1;
    int *currentScore = score1;
    int x1, y1, x2, y2;
    int mode; // 0 for two-player, 1 for computer

    printf("Would you like to load a saved game? (yes/no): ");
    char response[4];
    scanf("%s", response);
    if (strcmp(response, "yes") == 0) {
        loadGame(player1, player2, score1, score2, &currentPlayer, &mode);
        if (currentPlayer == player1) {
            currentScore = score1;
        } else {
            currentScore = score2;
        }
    } else {
        printf("Enter 0 for two-player mode, 1 to play against the computer: ");
        scanf("%d", &mode);

        do {
            printf("Enter the size of the board (even number between 4 and 20): ");
            scanf("%d", &N);
        } while (N < 4 || N > 20 || N % 2 != 0);

        board = (char **)malloc(N * sizeof(char *));
        for (int i = 0; i < N; i++) {
            board[i] = (char *)malloc(N * sizeof(char));
        }

        initializeBoard();

        printf("Enter the name of Player 1: ");
        scanf("%s", player1);
        if (mode == 0) {
            printf("Enter the name of Player 2: ");
            scanf("%s", player2);
        } else {
            strcpy(player2, "Computer");
        }

        currentPlayer = player1;
        currentScore = score1;
    }

    while (1) {
        printBoard();
        if ((mode == 0) || (mode == 1 && strcmp(currentPlayer, player1) == 0)) {
            printf("%s's turn. Enter your move (x1 y1 x2 y2) or -1 to save and exit: ", currentPlayer);
            scanf("%d", &x1);
            if (x1 == -1) {
                saveGame(player1, player2, score1, score2, currentPlayer, mode);
                break;
            }
            scanf("%d %d %d", &y1, &x2, &y2);
            if (isValidMove(x1, y1, x2, y2)) {
                makeMove(x1, y1, x2, y2, currentScore);
                printScores(player1, score1, player2, score2);
                if (currentPlayer == player1) {
                    currentPlayer = player2;
                    currentScore = score2;
                } else {
                    currentPlayer = player1;
                    currentScore = score1;
                }
            } else {
                printf("Invalid move. Try again.\n");
            }
        } else {
            computerMove(currentScore);
            printScores(player1, score1, player2, score2);
            currentPlayer = player1;
            currentScore = score1;
        }
    }

    freeBoard();

    return 0;
}

