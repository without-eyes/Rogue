#include <ncurses.h>
#include <stdlib.h>

typedef struct Room {
    int xPosition;
    int yPosition;
    int height;
    int width;
    //Monster** monster;
    //Item** items;
} Room;

typedef struct Player {
    int xPosition;
    int yPosition;
    int health;
    Room* room;
} Player;

void screenSetUp();
Room* mapSetUp();
Player* playerSetUp();
int handleInput(int input, Player* user);
int checkPosition(int y, int x, Player* user);
int playerMove(int y, int x, Player* user);

// Room functions
Room* createRoom(int y, int x, int height, int width);
int drawRoom(Room* room);

int main() {
    Player* user;
    int userInput;

    screenSetUp();

    mapSetUp();

    user = playerSetUp();

    // Main game loop
    while ((userInput = getch()) != 'q') {
        handleInput(userInput, user);
    }

    endwin();

    free(user);
}

void screenSetUp() {
    initscr();
    noecho();
    refresh();
}

Room* mapSetUp() {
    Room** rooms;
    rooms = malloc(sizeof(Room*) * 6);

//    mvprintw(13, 13, "--------");
//    mvprintw(14, 13, "|......|");
//    mvprintw(15, 13, "|......|");
//    mvprintw(16, 13, "|......|");
//    mvprintw(17, 13, "|......|");
//    mvprintw(18, 13, "--------");

    rooms[0] = createRoom(13, 13, 6, 8);
    drawRoom(rooms[0]);

//    mvprintw(2, 40, "--------");
//    mvprintw(3, 40, "|......|");
//    mvprintw(4, 40, "|......|");
//    mvprintw(5, 40, "|......|");
//    mvprintw(6, 40, "|......|");
//    mvprintw(7, 40, "--------");

    rooms[1] = createRoom(2, 40, 6, 8);
    drawRoom(rooms[1]);

//    mvprintw(10, 40, "------------");
//    mvprintw(11, 40, "|..........|");
//    mvprintw(12, 40, "|..........|");
//    mvprintw(13, 40, "|..........|");
//    mvprintw(14, 40, "|..........|");
//    mvprintw(15, 40, "------------");

    rooms[2] = createRoom(10, 40, 6, 12);
    drawRoom(rooms[2]);

    return rooms;
}

Room* createRoom(int y, int x, int height, int width) {
    Room* newRoom;
    newRoom = malloc(sizeof(Room));

    newRoom->xPosition = x;
    newRoom->yPosition = y;
    newRoom->height = height;
    newRoom->width = width;

    return newRoom;
}

int drawRoom(Room* room) {
    int x;
    int y;

    // Draw top and bottom
    for (x = room->xPosition; x < room->xPosition + room->width; x++) {
        mvprintw(room->yPosition, x, "-"); // Top
        mvprintw(room->yPosition + room->height - 1, x, "-"); // Bottom
    }

    // Draw floors and side walls
    for (y = room->yPosition + 1; y < room->yPosition + room->height - 1; y++) {
        // Draw side walls
        mvprintw(y, room->xPosition, "|");
        mvprintw(y, room->xPosition + room->width - 1, "|");

        // Draw floors
        for (x = room->xPosition + 1; x < room->xPosition + room->width - 1; x++) {
            mvprintw(y, x, ".");
        }
    }

    return 1;
}

Player* playerSetUp() {
    Player* newPlayer = malloc(sizeof(Player));

    newPlayer->xPosition = 14;
    newPlayer->yPosition = 14;
    newPlayer->health = 20;

    playerMove(newPlayer->yPosition, newPlayer->yPosition, newPlayer);

    return newPlayer;
}

int handleInput(int input, Player* user) {
    int newY;
    int newX;

    switch (input) {
        // Move up
        case 'w':
        case 'W':
            newY = user->yPosition - 1;
            newX = user->xPosition;
            break;

        // Move down
        case 's':
        case 'S':
            newY = user->yPosition + 1;
            newX = user->xPosition;
            break;

        // Move left
        case 'a':
        case 'A':
            newY = user->yPosition;
            newX = user->xPosition - 1;
            break;

        // Move right
        case 'd':
        case 'D':
            newY = user->yPosition;
            newX = user->xPosition + 1;
            break;

        default:
            break;
    }

    checkPosition(newY, newX, user);
}

int checkPosition(int newY, int newX, Player* user) {
    int space;

    switch (mvinch(newY, newX)) {
        case '.':
            playerMove(newY, newX, user);
            break;

        default:
            move(user->yPosition, user->xPosition);
            break;
    }
}

int playerMove(int y, int x, Player* user) {
    mvprintw(user->yPosition, user->xPosition, ".");

    user->yPosition = y;
    user->xPosition = x;

    mvprintw(user->yPosition, user->xPosition, "@");
    move(user->yPosition, user->xPosition);
}