#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TOTAL_ROOMS 10
#define ADMIN_PASSWORD "admin123"
#define ROOMS_FILE "rooms.txt"
#define BOOKINGS_FILE "bookings.txt"

struct Room {
    int number;
    char type[10];
    float price;
    int is_booked;
};

struct Booking {
    int id;
    int room_number;
    char name[50];
    char phone[15];
    int nights;
    float total;
    int active;  // 1=active 0=cancelled
};

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

int main()
{
    int ch;
    load_rooms();
    load_bookings();
    if (num_rooms == 0) {
        setup_rooms();
        save_rooms();
        printf("First time setup done. Rooms created.\n");
        pressEnter();
    }
    while (1) {
        system("cls || clear");

        printf("\n");
        printf("  ==========================================\n");
        printf("           AAA_GEHU HOTEL WELCOMES YOU !!!    \n");
        printf("  ===========================================\n");
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
        else if (ch == 2){
            book_room();
            save_rooms();
            save_bookings();
        }
        else if (ch == 3) {
            cancel_booking();
            save_rooms();
            save_bookings();
        }
        else if (ch == 4){
            view_booking();
        }
        else if (ch == 5)
            admin_menu();
        else if (ch == 6) {
            printf("\n  Glad you liked it, have a great day ahead!!\n\n");
            break;
        }
        else {
            printf("  Invalid choice.\n");
            pressEnter();
        }
    }
    return 0;
}

void setup_rooms()
{
    int i;
    int s[] = {101,102,103,104}; 
    int d[] = {201,202, 203};      
    int su[] = {301, 302,303};     

    num_rooms = 0;

    for (i = 0; i < 4; i++) {
        rooms[num_rooms].number=s[i];
        strcpy(rooms[num_rooms].type, "Single");
        rooms[num_rooms].price=800.0;
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


void load_rooms()
{
    FILE *f = fopen(ROOMS_FILE, "r");
    if (f == NULL)
        printf("file not found\n");
        return;

    num_rooms = 0;
    while (fscanf(f, "%d %s %f %d", &rooms[num_rooms].number, rooms[num_rooms].type,&rooms[num_rooms].price, &rooms[num_rooms].is_booked) == 4) {
        num_rooms++;
    }

    fclose(f);
}

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
        printf("error opening file \n");
        return;

    num_bookings = 0;
    while (fscanf(f, "%d %d %s %s %d %f %d", &bookings[num_bookings].id, &bookings[num_bookings].room_number,bookings[num_bookings].name, bookings[num_bookings].phone,
&bookings[num_bookings].nights, &bookings[num_bookings].total,&bookings[num_bookings].active) == 7) {
        num_bookings++;
    }

    fclose(f);
}

// show rooms that are free
void check_available()
{
    int i;
    int found = 0;

    system("cls || clear");
    printf("\n  === AVAILABLE ROOMS ===\n\n");
    printf("  %-10s %-10s %-12s\n", "Room No.", "Type", "Price/Night");
    printf("  ------------------------------------\n");
for (i = 0; i < num_rooms; i++) {
        if (rooms[i].is_booked == 0) {
            printf("  %-10d %-10s Rs. %.2f\n", rooms[i].number, rooms[i].type, rooms[i].price);
            found = 1;
        }
    }

    if (found == 0)
        printf("  No rooms free right now.\n");

    printf("  ------------------------------------\n");
    pressEnter();
}

// book a room
void book_room()
{
    int i, roomNo, nights;
    int idx = -1;  // index of room
    char cname[50], ph[15];

    system("cls || clear");
    printf("\n  === BOOK A ROOM ===\n\n");

    // show available rooms first
    check_available();

    printf("\n  Enter room number: ");
    scanf("%d", &roomNo);

    // find room in array
    for (i = 0; i < num_rooms; i++) {
        if (rooms[i].number == roomNo) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("  Room not found.\n");
        pressEnter();
        return;
    }

    if (rooms[idx].is_booked == 1) {
        printf("  This room is already booked!\n");
        pressEnter();
        return;
    }

    // take customer details
    printf("  Your name: ");
    scanf(" %[^\n]", cname);
    printf("  Phone no: ");
    scanf("%s", ph);
    printf("  No. of nights: ");
    scanf("%d", &nights);

    if (nights < 1) {
        printf("  Nights should be at least 1.\n");
        pressEnter();
        return;
    }

    // store booking
    bookings[num_bookings].id = 1000 + num_bookings + 1;
    bookings[num_bookings].room_number = roomNo;
    strncpy(bookings[num_bookings].name, cname, 49);
    strncpy(bookings[num_bookings].phone, ph, 14);
    bookings[num_bookings].nights = nights;
    bookings[num_bookings].total = nights * rooms[idx].price;
    bookings[num_bookings].active = 1;

    // mark room as booked
    rooms[idx].is_booked = 1;

    num_bookings++;

    printf("\n  Booking Done!\n");
    printf("  Booking ID : %d\n", bookings[num_bookings-1].id);
    printf("  Room       : %d (%s)\n", roomNo, rooms[idx].type);
    printf("  Name       : %s\n", cname);
    printf("  Nights     : %d\n", nights);
    printf("  Total Cost : Rs. %.2f\n", bookings[num_bookings-1].total);
    printf("\n  Please note your Booking ID = %d\n", bookings[num_bookings-1].id);

    pressEnter();
}

// cancel booking
void cancel_booking()
{
    int i, j;
    int bid, confirm;
    int idx = -1;

    system("cls || clear");
    printf("\n  === CANCEL BOOKING ===\n\n");
    printf("  Enter Booking ID: ");
    scanf("%d", &bid);

    for (i = 0; i < num_bookings; i++) {
        if (bookings[i].id == bid && bookings[i].active == 1) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("  Booking ID not found or already cancelled.\n");
        pressEnter();
        return;
    }

    printf("\n  Name   : %s\n", bookings[idx].name);
    printf("  Room   : %d\n", bookings[idx].room_number);
    printf("  Nights : %d\n", bookings[idx].nights);
    printf("  Total  : Rs. %.2f\n", bookings[idx].total);
    printf("\n  Are you sure? (1=Yes, 2=No): ");
    scanf("%d", &confirm);

    if (confirm == 1) {
        bookings[idx].active = 0;

        // free the room
        for (j = 0; j < num_rooms; j++) {
            if (rooms[j].number == bookings[idx].room_number) {
                rooms[j].is_booked = 0;
                break;
            }
        }

        printf("\n  Booking cancelled.\n");
    }
    else {
        printf("\n  Okay, not cancelled.\n");
    }

    pressEnter();
}

// view booking details
void view_booking()
{
    int i, bid;
    int idx = -1;

    system("cls || clear");
    printf("\n  === VIEW BOOKING ===\n\n");
    printf("  Enter Booking ID: ");
    scanf("%d", &bid);

    for (i = 0; i < num_bookings; i++) {
        if (bookings[i].id == bid) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("  Booking not found.\n");
        pressEnter();
        return;
    }

    printf("\n  --------------------------\n");
    printf("  Booking ID : %d\n", bookings[idx].id);
    printf("  Name       : %s\n", bookings[idx].name);
    printf("  Phone      : %s\n", bookings[idx].phone);
    printf("  Room No.   : %d\n", bookings[idx].room_number);
    printf("  Nights     : %d\n", bookings[idx].nights);
    printf("  Total Cost : Rs. %.2f\n", bookings[idx].total);
    printf("  Status     : %s\n", bookings[idx].active ? "ACTIVE" : "CANCELLED");
    printf("  --------------------------\n");

    pressEnter();
}

// admin login and menu
void admin_menu()
{
    char pass[30];
    int ch, tries = 0;

    system("cls || clear");
    printf("\n  === ADMIN LOGIN ===\n\n");

    while (tries < 3) {
        printf("  Enter Password: ");
        scanf("%s", pass);

        if (strcmp(pass, ADMIN_PASSWORD) == 0)
            break;

        tries++;
        printf("  Wrong password! (%d/3)\n", tries);
    }

    if (tries == 3) {
        printf("  Access denied.\n");
        pressEnter();
        return;
    }

    // admin menu loop
    while (1) {
        system("cls || clear");
        printf("\n  === ADMIN PANEL ===\n\n");
        printf("  1. View All Rooms\n");
        printf("  2. View All Bookings\n");
        printf("  3. Update Room Price\n");
        printf("  4. Back\n");
        printf("\n  Choice: ");
        scanf("%d", &ch);

        if (ch == 1)
            admin_view_all();
        else if (ch == 2)
            admin_view_bookings();
        else if (ch == 3) {
            admin_update_price();
            save_rooms();
        }
        else if (ch == 4)
            return;
        else {
            printf("  Invalid.\n");
            pressEnter();
        }
    }
}


// show all rooms with status
void admin_view_all()
{
    int i;

    system("cls || clear");
    printf("\n  === ALL ROOMS ===\n\n");
    printf("  %-10s %-10s %-12s %-10s\n", "Room No.", "Type", "Price/Night", "Status");
    printf("  ------------------------------------------\n");

    for (i = 0; i < num_rooms; i++) {
        printf("  %-10d %-10s Rs. %-8.2f %s\n",
            rooms[i].number, rooms[i].type,
            rooms[i].price,
            rooms[i].is_booked ? "Booked" : "Free");
    }
        printf("  ------------------------------------------\n");
    printf("  Total rooms: %d\n", num_rooms);
    pressEnter();
}


// show all bookings
void admin_view_bookings()
{
    int i;

    system("cls || clear");
    printf("\n  === ALL BOOKINGS ===\n\n");

    if (num_bookings == 0) {
        printf("  No bookings yet.\n");
        pressEnter();
        return;
    }

    for (i = 0; i < num_bookings; i++) {
        printf("  ID: %-5d Room: %-4d Name: %-20s Nights: %-3d Total: Rs.%-8.2f [%s]\n",
            bookings[i].id, bookings[i].room_number,
            bookings[i].name, bookings[i].nights,
            bookings[i].total,
            bookings[i].active ? "Active" : "Cancelled");
    }
    printf("\n  Total: %d bookings\n", num_bookings);
    pressEnter();
}

// update price of a room
void admin_update_price()
{
    int i, roomNo;
    int idx = -1;
    float newPrice;

    system("cls || clear");
    admin_view_all();

    printf("  Enter room number to change price: ");
    scanf("%d", &roomNo);

    for (i = 0; i < num_rooms; i++) {
        if (rooms[i].number == roomNo) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("  Room not found.\n");
        pressEnter();
        return;
    }

    printf("  Current price: Rs. %.2f\n", rooms[idx].price);
    printf("  New price: Rs. ");
    scanf("%f", &newPrice);

    if (newPrice <= 0) {
        printf("  Price should be more than 0.\n");
        pressEnter();
        return;
    }

    rooms[idx].price = newPrice;
    printf("  Done! Price updated.\n");
    pressEnter();
}

// wait for enter key
void pressEnter()
{
    printf("\n  Press Enter to continue...");
    while (getchar() != '\n');
    getchar();
}
