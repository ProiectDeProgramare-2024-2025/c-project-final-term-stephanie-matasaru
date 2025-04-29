#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MATCHES_FILE "matches.txt"

HANDLE hConsole;

typedef struct {
    int wins;
    int loses;
} Result;

typedef struct {
    char name[25];
    Result results;
} Player;

typedef struct {
    char name1[25];
    char name2[25];
    int points1;
    int points2;
} Match;

Match matches[100];
int matches_count = 0;
Player players[200];
int player_count = 0;

void setColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

void resetColor() {
    SetConsoleTextAttribute(hConsole, 7);
}

int find(const char *name, Player players[], int player_count) {
    for (int i = 0; i < player_count; i++) {
        if (strcmp(name, players[i].name) == 0)
            return i;
    }
    return -1;
}

void save_match(Match *m) {
    FILE *file = fopen(MATCHES_FILE, "a");
    if (!file) return;
    fprintf(file, "%s %d %s %d\n", m->name1, m->points1, m->name2, m->points2);
    fclose(file);
}

void info_in(Match matches[], int matches_count, Player players[], int *player_count) {
    int index1, index2;
    int i = matches_count - 1;
    index1 = find(matches[i].name1, players, *player_count);
    index2 = find(matches[i].name2, players, *player_count);
    if (index1 == -1) {
        index1 = *player_count;
        strcpy(players[index1].name, matches[i].name1);
        players[index1].results.wins = 0;
        players[index1].results.loses = 0;
        (*player_count)++;
    }
    if (index2 == -1) {
        index2 = *player_count;
        strcpy(players[index2].name, matches[i].name2);
        players[index2].results.wins = 0;
        players[index2].results.loses = 0;
        (*player_count)++;
    }
    if (matches[i].points1 == 21) {
        players[index1].results.wins++;
        players[index2].results.loses++;
    } else {
        players[index1].results.loses++;
        players[index2].results.wins++;
    }
}

int compare_players(const void *a, const void *b) {
    Player *playerA = (Player *)a;
    Player *playerB = (Player *)b;
    return playerB->results.wins - playerA->results.wins;
}

void header() {
    printf("--------------\n");
    printf(" ");
    setColor(13); printf("1"); resetColor(); printf(" - Add match\n");
    printf(" ");
    setColor(13); printf("2"); resetColor(); printf(" - Show ranking\n");
    printf(" ");
    setColor(13); printf("3"); resetColor(); printf(" - Show Player info\n");
    printf(" ");
    setColor(13); printf("4"); resetColor(); printf(" - Show match history\n");
    printf(" ");
    setColor(13); printf("5"); resetColor(); printf(" - Start a new tournament\n");
    printf(" ");
    setColor(13); printf("0"); resetColor(); printf(" - Exit\n");
    printf("--------------\n");
}

int is_valid_name(const char *name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i])) return 0;
    }
    return 1;
}

int is_valid_score(int score) {
    return score >= 1 && score <= 21;
}

int exactly_one_is_21(int s1, int s2) {
    return (s1 == 21 && s2 < 21) || (s2 == 21 && s1 < 21);
}

void add() {
    setColor(10);
    printf("Add match\n");
    resetColor();
    printf("(For all matches, 21 points are required for a win)\n\n");

    char name1[25], name2[25];
    int score1, score2;

    printf("Enter the name and score of Player 1 (name score): ");
    if (scanf("%24s %d", name1, &score1) != 2) {
        printf("Invalid input\n");
        return;
    }

    printf("Enter the name and score of Player 2 (name score): ");
    if (scanf("%24s %d", name2, &score2) != 2) {
        printf("Invalid input\n");
        return;
    }

    if (!is_valid_name(name1) || !is_valid_name(name2) ||
        !is_valid_score(score1) || !is_valid_score(score2) ||
        !exactly_one_is_21(score1, score2)) {
        printf("Invalid input\n");
        return;
    }

    Match *m = &matches[matches_count++];
    strcpy(m->name1, name1);
    strcpy(m->name2, name2);
    m->points1 = score1;
    m->points2 = score2;

    save_match(m);
    info_in(matches, matches_count, players, &player_count);
}

void ranking() {
    qsort(players, player_count, sizeof(Player), compare_players);
    setColor(11);
    printf("\nRanking:\n");
    resetColor();
    for (int i = 0; i < player_count; i++) {
        setColor(11);
        printf("%d.", i + 1);
        resetColor();
        printf(" %s: ", players[i].name);
        setColor(8);
        printf("%d wins\n", players[i].results.wins);
        resetColor();
    }
}

void player_info() {
    char player_name[25];
    printf("Enter player name ");
    setColor(8);
    printf("(name)");
    resetColor();
    printf(": ");
    scanf("%s", player_name);
    int index = find(player_name, players, player_count);
    if (index == -1) {
        setColor(12);
        printf("Player not found.\n");
        resetColor();
        return;
    }
    int option;
    do {
        printf("\n....................\n");
        printf("Player ");
        setColor(11); printf("%s", player_name); resetColor();
        printf(" Info:\n");
        printf(" ");
        setColor(13); printf("1"); resetColor(); printf(" - Show Matches\n");
        printf(" ");
        setColor(13); printf("2"); resetColor(); printf(" - Show Wins and Losses\n");
        printf(" ");
        setColor(13); printf("0"); resetColor(); printf(" - Back\n");
        printf("....................\n");
        printf("Enter option: ");
        scanf("%d", &option);

        system("cls");
        switch (option) {
            case 1:
                printf("Matches for ");
                setColor(11); printf("%s:\n", player_name); resetColor();
                for (int i = 0; i < matches_count; i++) {
                    if (!(strcmp(player_name, matches[i].name1)) || !(strcmp(player_name, matches[i].name2))) {
                        printf("%s ", matches[i].name1);
                        setColor(8);
                        printf("%d-%d ", matches[i].points1, matches[i].points2);
                        resetColor();
                        printf("%s\n", matches[i].name2);
                    }
                }
                break;
            case 2:
                printf("Wins and Losses for ");
                setColor(11); printf("%s:\n", player_name); resetColor();
                printf("Wins: ");
                setColor(10); printf("%d\n", players[index].results.wins); resetColor();
                printf("Losses: ");
                setColor(12); printf("%d\n", players[index].results.loses); resetColor();
                break;
            case 0:
                printf("Returning to main menu...\n");
                break;
            default:
                setColor(12);
                printf("Invalid option. Try again.\n");
                resetColor();
        }
    } while (option != 0);
}

void matches_history() {
    printf("\n");
    setColor(9);
    printf("Match History:\n");
    resetColor();
    for (int i = 0; i < matches_count; i++) {
        printf("%s ", matches[i].name1);
        setColor(9);
        printf("%d-%d ", matches[i].points1, matches[i].points2);
        resetColor();
        printf("%s\n", matches[i].name2);
    }
}

void clear_files() {
    FILE *file = fopen(MATCHES_FILE, "w");
    if (file) fclose(file);

    matches_count = 0;
    player_count = 0;

    setColor(10);
    printf("All data cleared!\n");
    resetColor();
}

void clear_menu() {
    clear_files();
    int option;
    do {
        printf("\n....................\n");
        printf(" ");
        setColor(13); printf("1"); resetColor(); printf(" - Back to menu\n");
        printf(" ");
        setColor(13); printf("0"); resetColor(); printf(" - Exit\n");
        printf("....................\n");
        printf("Enter option: ");
        scanf("%d", &option);
        system("cls");
        switch (option) {
            case 1:
                printf("Returning to main menu...\n");
                return;
            case 0:
                printf("EXIT!\n");
                exit(0);
            default:
                setColor(12);
                printf("Invalid option. Try again.\n");
                resetColor();
        }
    } while (1);
}

void load_matches() {
    FILE *file = fopen(MATCHES_FILE, "r");
    if (!file) return;
    while (fscanf(file, "%s %d %s %d", matches[matches_count].name1, &matches[matches_count].points1, matches[matches_count].name2, &matches[matches_count].points2) == 4) {
        info_in(matches, ++matches_count, players, &player_count);
    }
    fclose(file);
}

void menu(int op) {
    system("cls");
    switch (op) {
        case 1: add(); break;
        case 2: ranking(); break;
        case 3: player_info(); break;
        case 4: matches_history(); break;
        case 5: clear_menu(); break;
        default: printf("EXIT!\n");
    }
}

int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    load_matches();
    int option;
    do {
        header();
        printf("Enter option: ");
        scanf("%d", &option);
        menu(option);
    } while (option > 0 && option <= 5);

    return 0;
}
