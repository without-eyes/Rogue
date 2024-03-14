#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

typedef struct Position {
    int x;
    int y;
    //TILE_TYPE tile;
} Position;

typedef struct Room {
    Position position;
    int height;
    int width;

    Position** doors;

    //Monster** monster;
    //Item** items;
} Room;

typedef struct Player {
    Position position;
    int health;
    //Room* room;
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
int connectDoors(const Position* doorOne, const Position* doorTwo);

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
    srand(time(NULL));
}

Room* mapSetUp() {
    Room** rooms;
    rooms = malloc(sizeof(Room*) * 6);

    rooms[0] = createRoom(13, 13, 6, 8);
    drawRoom(rooms[0]);

    rooms[1] = createRoom(2, 40, 6, 8);
    drawRoom(rooms[1]);

    rooms[2] = createRoom(10, 40, 6, 12);
    drawRoom(rooms[2]);

    connectDoors(rooms[0]->doors[3], rooms[2]->doors[1]);

    connectDoors(rooms[1]->doors[2], rooms[0]->doors[0]);

    return rooms;
}

Room* createRoom(int y, int x, int height, int width) {
    Room* newRoom;
    newRoom = malloc(sizeof(Room));

    newRoom->position.x = x;
    newRoom->position.y = y;
    newRoom->height = height;
    newRoom->width = width;

    newRoom->doors = malloc(sizeof(Position) * 4);

    // Top door
    newRoom->doors[0] = malloc(sizeof(Position));
    newRoom->doors[0]->x = newRoom->position.x + rand() % (width - 2) + 1;
    newRoom->doors[0]->y = newRoom->position.y;

    // Left door
    newRoom->doors[1] = malloc(sizeof(Position));
    newRoom->doors[1]->x = newRoom->position.x;
    newRoom->doors[1]->y = newRoom->position.y + rand() % (height - 2) + 1;

    // Bottom door
    newRoom->doors[2] = malloc(sizeof(Position));
    newRoom->doors[2]->x = newRoom->position.x + rand() % (width - 2) + 1;
    newRoom->doors[2]->y = newRoom->position.y + height - 1;

    // Right door
    newRoom->doors[3] = malloc(sizeof(Position));
    newRoom->doors[3]->x = newRoom->position.x + width - 1;
    newRoom->doors[3]->y = newRoom->position.y + rand() % (height - 2) + 1;

    return newRoom;
}

int drawRoom(Room* room) {
    // Draw top and bottom
    for (int x = room->position.x; x < room->position.x + room->width; x++) {
        mvprintw(room->position.y, x, "-"); // Top
        mvprintw(room->position.y + room->height - 1, x, "-"); // Bottom
    }

    // Draw floors and side walls
    for (int y = room->position.y + 1; y < room->position.y + room->height - 1; y++) {
        // Draw side walls
        mvprintw(y, room->position.x, "|");
        mvprintw(y, room->position.x + room->width - 1, "|");

        // Draw floors
        for (int x = room->position.x + 1; x < room->position.x + room->width - 1; x++) {
            mvprintw(y, x, ".");
        }
    }

    // Draw doors
    mvprintw(room->doors[0]->y, room->doors[0]->x, "+");
    mvprintw(room->doors[1]->y, room->doors[1]->x, "+");
    mvprintw(room->doors[2]->y, room->doors[2]->x, "+");
    mvprintw(room->doors[3]->y, room->doors[3]->x, "+");

    return 1;
}

int connectDoors(const Position* doorOne, const Position* doorTwo) {
    Position temp;
    Position previous;

    int count = 0;

    temp.x = doorOne->x;
    temp.y = doorOne->y;

    previous = temp;

    while (1) {
        // Step left
        if ((abs((temp.x - 1) - doorTwo->x) < abs(temp.x - doorTwo->x)) && (mvinch(temp.y, temp.x - 1) == ' ')) {
            previous.x = temp.x;
            temp.x = temp.x - 1;

        // Step right
        } else if ((abs((temp.x + 1) - doorTwo->x) < abs(temp.x - doorTwo->x)) && (mvinch(temp.y, temp.x + 1) == ' ')) {
            previous.x = temp.x;
            temp.x = temp.x + 1;

        // Step down
        } else if ((abs((temp.y + 1) - doorTwo->y) < abs(temp.y - doorTwo->y)) && (mvinch(temp.y + 1, temp.x) == ' ')) {
            previous.y = temp.y;
            temp.y = temp.y + 1;

        // Step left
        } else if ((abs((temp.y - 1) - doorTwo->y) < abs(temp.y - doorTwo->y)) && (mvinch(temp.y - 1, temp.x) == ' ')) {
            previous.y = temp.y;
            temp.y = temp.y - 1;

        } else {
            if (count == 0) {
                temp = previous;
                count++;
                continue;
            } else {
                return 0;
            }
        }

        mvprintw(temp.y, temp.x, "#");
    }
}

Player* playerSetUp() {
    Player* newPlayer = malloc(sizeof(Player));

    newPlayer->position.x = 14;
    newPlayer->position.y = 14;
    newPlayer->health = 20;

    playerMove(newPlayer->position.y, newPlayer->position.x, newPlayer);

    return newPlayer;
}

int handleInput(int input, Player* user) {
    int newY;
    int newX;

    switch (input) {
        // Move up
        case 'w':
        case 'W':
            newY = user->position.y - 1;
            newX = user->position.x;
            break;

        // Move down
        case 's':
        case 'S':
            newY = user->position.y + 1;
            newX = user->position.x;
            break;

        // Move left
        case 'a':
        case 'A':
            newY = user->position.y;
            newX = user->position.x - 1;
            break;

        // Move right
        case 'd':
        case 'D':
            newY = user->position.y;
            newX = user->position.x + 1;
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
        case '#':
        case '+':
            playerMove(newY, newX, user);
            break;

        default:
            move(user->position.y, user->position.x);
            break;
    }
}

int playerMove(int y, int x, Player* user) {
    mvprintw(user->position.y, user->position.x, ".");

    user->position.y = y;
    user->position.x = x;

    mvprintw(user->position.y, user->position.x, "@");
    move(user->position.y, user->position.x);
}