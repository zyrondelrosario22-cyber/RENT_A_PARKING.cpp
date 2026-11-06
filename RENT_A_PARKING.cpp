#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <queue>
#include <stack>
using namespace std;

struct Vehicle {
    int type;
    double price;
    string typeName;
    string plate;
    string brand;
    string color;
    string targetSlot;
};

const int ROWS = 12;
const int COLS = 5;
bool lot[ROWS][COLS];
string logBook[60][5];
int logCount = 0;
double r2 = 0, r3 = 0, r4 = 0, rT = 0;

queue<Vehicle> waitingQueue;
stack<Vehicle> releasedStack;

void clearTab() {
#if defined _WIN32
    system("cls");
#elif defined (_LINUX_) || defined(_gnu_linux_) || defined(_linux_)
    system("clear");
#endif
}

void header(string title) {
    clearTab();
    cout << "====================================" << endl;
    cout << "      " << title << endl;
    cout << "====================================" << endl;
}

void pause() {
    int x;
    cout << "\n[0] Type 0 to continue: ";
    cin >> x;
}

void drawMap() {
    cout << "    1   2   3   4   5" << endl;
    for (int i = 0; i < ROWS; i++) {
        cout << char('A' + i) << " ";
        for (int j = 0; j < COLS; j++) {
            if (lot[i][j])
                cout << "[ ] ";
            else
                cout << "[X] ";
        }
        cout << endl;
    }
    cout << "\n[ ]=Free  [X]=Taken" << endl;
}

void viewRates() {
    header("REVENUE REPORT");
    cout << "1. Two Wheels   : PHP " << r2 << endl;
    cout << "2. Three Wheels : PHP " << r3 << endl;
    cout << "3. Four Wheels  : PHP " << r4 << endl;
    cout << "4. Truck        : PHP " << rT << endl;
    cout << "------------------------------------" << endl;
    cout << "GRAND TOTAL REVENUE: PHP " << (r2 + r3 + r4 + rT) << endl;
    pause();
}

void viewLogBook() {
    header("PARKER LOG BOOK");
    if (logCount == 0) {
        cout << "No records found." << endl;
    } else {
        cout << "SLOT | TYPE | PLATE | BRAND | COLOR" << endl;
        cout << "------------------------------------" << endl;
        for (int i = 0; i < logCount; i++) {
            cout << logBook[i][0] << " | "
                 << logBook[i][1] << " | "
                 << logBook[i][2] << " | "
                 << logBook[i][3] << " | "
                 << logBook[i][4] << endl;
        }
    }
    pause();
}

void viewQueue() {
    header("PARKING WAITING QUEUE");
    if (waitingQueue.empty()) {
        cout << "No vehicles in waiting queue." << endl;
    } else {
        queue<Vehicle> temp = waitingQueue;
        cout << "WAITING VEHICLES" << endl;
        cout << "------------------------------------" << endl;
        cout << "SLOT | TYPE | PLATE | BRAND | COLOR" << endl;
        cout << "------------------------------------" << endl;
        int count = 1;
        while (!temp.empty()) {
            Vehicle v = temp.front();
            cout << count << ". " << v.targetSlot << " | " << v.typeName << " | " << v.plate << " | " << v.brand << " | " << v.color << endl;
            temp.pop();
            count++;
        }
    }
    pause();
}

void viewStack() {
    header("RELEASED VEHICLES");
    if (releasedStack.empty()) {
        cout << "No released vehicles recorded." << endl;
    } else {
        stack<Vehicle> temp = releasedStack;
        cout << "RELEASE HISTORY" << endl;
        cout << "------------------------------------" << endl;
        cout << "SLOT | TYPE | PLATE | BRAND | COLOR" << endl;
        cout << "------------------------------------" << endl;
        int count = 1;
        while (!temp.empty()) {
            Vehicle v = temp.top();
            cout << count << ". " << v.targetSlot << " | " << v.typeName << " | " << v.plate << " | " << v.brand << " | " << v.color << endl;
            temp.pop();
            count++;
        }
    }
    pause();
}

void release() {
    header("RELEASE SPACE");
    drawMap();

    char row;
    int col;

    cout << "\nEnter Row (A-L) or 0 to back: ";
    cin >> row;

    if (row == '0') return;

    if (!((row >= 'A' && row <= 'Z') || (row >= 'a' && row <= 'z'))) {
        cout << "Invalid input.";
        pause();
        return;
    }

    int r = toupper(row) - 'A';
    if (r >= ROWS) {
        cout << "Invalid row.";
        pause();
        return;
    }

    cout << "Enter Col (1-5) or 0 to back: ";
    cin >> col;

    if (col == 0) return;

    if (col < 1 || col > 5) {
        cout << "Invalid input.";
        pause();
        return;
    }

    int c = col - 1;

    if (lot[r][c]) {
        cout << "Slot is already FREE.";
        pause();
        return;
    }

    string slotName;
    slotName += toupper(row);
    slotName += to_string(col);

    // Search the log book to find the vehicle details of the car leaving this slot
    Vehicle releasedVehicle;
    releasedVehicle.targetSlot = slotName;
    releasedVehicle.typeName = "Unknown";
    releasedVehicle.plate = "N/A";
    releasedVehicle.brand = "N/A";
    releasedVehicle.color = "N/A";

    for (int i = 0; i < logCount; i++) {
        if (logBook[i][0] == slotName) {
            releasedVehicle.typeName = logBook[i][1];
            releasedVehicle.plate = logBook[i][2];
            releasedVehicle.brand = logBook[i][3];
            releasedVehicle.color = logBook[i][4];
            break;
        }
    }

    lot[r][c] = true;
    releasedStack.push(releasedVehicle);

    cout << "\nSlot " << slotName << " is now FREE.";
    cout << "\nVehicle (" << releasedVehicle.plate << ") added to released history stack.";
    pause();
}

void assignQueueToSlot() {
    header("ASSIGN WAITING VEHICLE");
    if (waitingQueue.empty()) {
        cout << "No vehicles in the waiting queue." << endl;
        pause();
        return;
    }

    drawMap();
    char row;
    int col;

    cout << "\nEnter Row (A-L) for the waiting vehicle or 0 to back: ";
    cin >> row;

    if (row == '0') return;

    if (!((row >= 'A' && row <= 'Z') || (row >= 'a' && row <= 'z'))) {
        cout << "Invalid input.";
        pause();
        return;
    }

    int r = toupper(row) - 'A';
    if (r >= ROWS) {
        cout << "Invalid row.";
        pause();
        return;
    }

    cout << "Enter Col (1-5) or 0 to back: ";
    cin >> col;

    if (col == 0) return;

    if (col < 1 || col > 5) {
        cout << "Invalid input.";
        pause();
        return;
    }

    int c = col - 1;

    if (!lot[r][c]) {
        cout << "Selected slot is OCCUPIED. Choose a free slot [ ].";
        pause();
        return;
    }

    Vehicle v = waitingQueue.front();
    waitingQueue.pop();

    lot[r][c] = false;

    string slotName;
    slotName += toupper(row);
    slotName += to_string(col);

    if (v.type == 1) r2 += v.price;
    else if (v.type == 2) r3 += v.price;
    else if (v.type == 3) r4 += v.price;
    else rT += v.price;

    if (logCount < 60) {
        logBook[logCount][0] = slotName;
        logBook[logCount][1] = v.typeName;
        logBook[logCount][2] = v.plate;
        logBook[logCount][3] = v.brand;
        logBook[logCount][4] = v.color;
        logCount++;
    }

    cout << "\nVehicle from queue successfully assigned to slot " << slotName << "!";
    cout << "\nAssigned Plate: " << v.plate << " (" << v.typeName << ")";
    pause();
}

void rent() {
    header("STEP 1: VEHICLE TYPE");

    int type;

    cout << "0. GO BACK TO MENU" << endl;
    cout << "1. Two Wheels\n2. Three Wheels\n3. Four Wheels\n4. Truck\n\nSelection: ";
    cin >> type;

    if (type == 0) return;

    if (type != 1 && type != 2 && type != 3 && type != 4) {
        cout << "Invalid input.";
        pause();
        return;
    }

    double price;
    string typeName;

    if (type == 1) { price = 10; typeName = "2-Wheels"; }
    else if (type == 2) { price = 15; typeName = "3-Wheels"; }
    else if (type == 3) { price = 30; typeName = "4-Wheels"; }
    else { price = 50; typeName = "Truck"; }

    header("STEP 2: VEHICLE INFO");

    string plate, brand, color;
    cout << "Plate Number : "; cin >> plate;
    cout << "Brand/Make   : "; cin >> brand;
    cout << "Vehicle Color: "; cin >> color;

    header("STEP 3: CHOOSE SLOT");
    drawMap();

    char row;
    int col;

    cout << "Enter Row (A-L) or 0 to back: ";
    cin >> row;

    if (row == '0') return;

    if (!((row >= 'A' && row <= 'Z') || (row >= 'a' && row <= 'z'))) {
        cout << "Invalid input.";
        pause();
        return;
    }

    int r = toupper(row) - 'A';
    if (r >= ROWS) {
        cout << "Invalid row.";
        pause();
        return;
    }

    cout << "Enter Col (1-5) or 0 to back: ";
    cin >> col;

    if (col == 0) return;

    if (col < 1 || col > 5) {
        cout << "Invalid input.";
        pause();
        return;
    }

    int c = col - 1;

    string slotName;
    slotName += toupper(row);
    slotName += to_string(col);

    if (!lot[r][c]) {
        cout << "Slot occupied.";
        Vehicle v;
        v.type = type;
        v.price = price;
        v.typeName = typeName;
        v.plate = plate;
        v.brand = brand;
        v.color = color;
        v.targetSlot = slotName;
        waitingQueue.push(v);
        cout << "\nVehicle added to waiting queue for slot " << slotName << ".";
        pause();
        return;
    }

    header("STEP 4: PAYMENT");

    double cash;
    cout << "Amount Due: PHP " << price << endl;
    cout << "Insert Cash: ";
    cin >> cash;

    if (cash < price) {
        cout << "Insufficient payment.";
        pause();
        return;
    }

    lot[r][c] = false;

    if (type == 1) r2 += price;
    else if (type == 2) r3 += price;
    else if (type == 3) r4 += price;
    else rT += price;

    if (logCount < 60) {
        logBook[logCount][0] = slotName;
        logBook[logCount][1] = typeName;
        logBook[logCount][2] = plate;
        logBook[logCount][3] = brand;
        logBook[logCount][4] = color;
        logCount++;
    }

    header("STEP 5: PRINTING RECEIPT");
    cout << "====================================" << endl;
    cout << "        OFFICIAL RECEIPT            " << endl;
    cout << "====================================" << endl;
    cout << " SLOT ASSIGNED  : " << slotName << endl;
    cout << "------------------------------------" << endl;
    cout << " VEHICLE TYPE   : " << typeName << endl;
    cout << " PLATE NUMBER   : " << plate << endl;
    cout << " VEHICLE BRAND  : " << brand << endl;
    cout << " VEHICLE COLOR  : " << color << endl;
    cout << "------------------------------------" << endl;
    cout << " TOTAL PAID     : PHP " << price << endl;
    cout << " CASH TENDERED  : PHP " << cash << endl;
    cout << " CHANGE         : PHP " << (cash - price) << endl;
    cout << "====================================" << endl;
    cout << "      THANK YOU FOR PARKING!        " << endl;
    cout << "====================================" << endl;

    pause();
}

int main() {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            lot[i][j] = true;

    int choice;

    while (true) {
        header("PARKING SYSTEM");

        cout << "1. Rent Space\n"
             << "2. View Map\n"
             << "3. Revenue\n"
             << "4. Release Space\n"
             << "5. Log Book\n"
             << "6. View Waiting\n"
             << "7. View Released Vehicles\n"
             << "8. Assign Waiting Vehicle to Slot\n"
             << "9. Exit\n\n"
             << "Selection: ";

        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input.";
            pause();
            continue;
        }

        if (choice == 1) rent();
        else if (choice == 2) { header("MAP"); drawMap(); pause(); }
        else if (choice == 3) viewRates();
        else if (choice == 4) release();
        else if (choice == 5) viewLogBook();
        else if (choice == 6) viewQueue();
        else if (choice == 7) viewStack();
        else if (choice == 8) assignQueueToSlot();
        else if (choice == 9) break;
        else {
            cout << "Invalid input.";
            pause();
        }
    }

    return EXIT_SUCCESS;
}
