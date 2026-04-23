#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// total rooms in hotel
#define TOTAL_ROOMS 10
// admin pass
#define ADMIN_PASSWORD "admin123"
// file names
#define ROOMS_FILE "rooms.txt"
#define BOOKINGS_FILE "bookings.txt"
// room structure
struct Room {
    int number;
    char type[10];   // single double suite
    float price;
    int is_booked;   // 0=free 1=booked
};

// booking structure
struct Booking {
    int id;
    int room_number;
    char name[50];
    char phone[15];
    int nights;
    float total;
    int active;  // 1=active 0=cancelled
};
// global arrays
struct Room rooms[TOTAL_ROOMS];
struct Booking bookings[100];
int num_rooms = 0;
int num_bookings = 0;
// function declarations
void setup_rooms();
void save_rooms();
void load_rooms();
void save_bookings();
void load_bookings();
void show_menu();
void check_available();
void book_room();
void cancel_booking();
void view_booking();
void admin_menu();
void admin_view_all();
void admin_update_price();
void admin_view_bookings();
void pressEnter();
// main function
int main()
{
    int ch;
    // load data from files
    load_rooms();
    load_bookings();

    // if first time, make default rooms
    if (num_rooms == 0) {
        setup_rooms();
        save_rooms();
        printf("First time setup done. Rooms created.\n");
        pressEnter();
    }
    while (1) {
        system("cls || clear");

        printf("\n");
        printf("  ================================\n");
        printf("      SUNSHINE HOTEL SYSTEM      \n");
        printf("  ================================\n");
        printf("  1. Check Available Rooms\n");
        printf("  2. Book a Room\n");
        printf("  3. Cancel Booking\n");
        printf("  4. View My Booking\n");
        printf("  5. Admin Panel\n");
        printf("  6. Exit\n");
        printf("  ================================\n");
        printf("  Enter choice: ");
        scanf("%d", &ch);
        if (ch == 1)
            check_available();
        else if (ch == 2) {
            book_room();
            save_rooms();
            save_bookings();
        }
        else if (ch == 3) {
            cancel_booking();
            save_rooms();
            save_bookings();
        }
        else if (ch == 4)
            view_booking();
        else if (ch == 5)
            admin_menu();
        else if (ch == 6) {
            printf("\n  Bye bye!\n\n");
            break;
        }
        else {
            printf("  Invalid choice.\n");
            pressEnter();
        }
    }
// setup default rooms - runs only once
void setup_rooms()
{
    int i;
    int s[] = {101, 102, 103, 104};   // single rooms
    int d[] = {201, 202, 203};        // double rooms
    int su[] = {301, 302, 303};       // suites

    num_rooms = 0;

    for (i = 0; i < 4; i++) {
        rooms[num_rooms].number = s[i];
        strcpy(rooms[num_rooms].type, "Single");
        rooms[num_rooms].price = 800.0;
        rooms[num_rooms].is_booked = 0;
        num_rooms++;
    }

    for (i = 0; i < 3; i++) {
        rooms[num_rooms].number = d[i];
        strcpy(rooms[num_rooms].type, "Double");
        rooms[num_rooms].price = 1500.0;
        rooms[num_rooms].is_booked = 0;
        num_rooms++;
    }

    for (i = 0; i < 3; i++) {
        rooms[num_rooms].number = su[i];
        strcpy(rooms[num_rooms].type, "Suite");
        rooms[num_rooms].price = 3000.0;
        rooms[num_rooms].is_booked = 0;
        num_rooms++;
    }
}

// save rooms to file
void save_rooms()
{
    int i;
    FILE *f = fopen(ROOMS_FILE, "w");
    if (f == NULL) {
        printf("  Could not save rooms!\n");
        return;
    }

    for (i = 0; i < num_rooms; i++) {
        fprintf(f, "%d %s %.2f %d\n", rooms[i].number, rooms[i].type, rooms[i].price, rooms[i].is_booked);
    }

    fclose(f);
}

// load rooms from file
void load_rooms()
{
    FILE *f = fopen(ROOMS_FILE, "r");
    if (f == NULL)
        return;  // file not found, skip

    num_rooms = 0;
    while (fscanf(f, "%d %s %f %d", &rooms[num_rooms].number, rooms[num_rooms].type,
                  &rooms[num_rooms].price, &rooms[num_rooms].is_booked) == 4) {
        num_rooms++;
    }

    fclose(f);
}
// save bookings to file
void save_bookings()
{
    int i;
    FILE *f = fopen(BOOKINGS_FILE, "w");
    if (f == NULL) {
        printf("  Could not save bookings!\n");
        return;
    }

    for (i = 0; i < num_bookings; i++) {
        fprintf(f, "%d %d %s %s %d %.2f %d\n",
            bookings[i].id, bookings[i].room_number,
            bookings[i].name, bookings[i].phone,
            bookings[i].nights, bookings[i].total, bookings[i].active);
    }

    fclose(f);
}

// load bookings from file
void load_bookings()
{
    FILE *f = fopen(BOOKINGS_FILE, "r");
    if (f == NULL)
        return;

    num_bookings = 0;
    while (fscanf(f, "%d %d %s %s %d %f %d",
                  &bookings[num_bookings].id, &bookings[num_bookings].room_number,
                  bookings[num_bookings].name, bookings[num_bookings].phone,
                  &bookings[num_bookings].nights, &bookings[num_bookings].total,
                  &bookings[num_bookings].active) == 7) {
        num_bookings++;
    }

    fclose(f);
}
    return 0;
}
