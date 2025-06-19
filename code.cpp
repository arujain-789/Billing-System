#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>

using namespace std;

// Product Class
class Stock {
private:
    string product_name;
    int input_quantity;
    int price;

public:
    Stock(const string& name, int quantity, int price)
        : product_name(name), input_quantity(quantity), price(price) {}

    void writeToCSV(const string& filename) const {
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << product_name << "," << input_quantity << "," << price << "\n";
            file.close();
            cout << "Product added: " << product_name << endl;
        } else {
            cerr << "Error opening file!" << endl;
        }
    }
    
    
};

// Billing Record Class
class Bill {
public:
    string consumer_name;
    long long mobile;
    int total;

    Bill(const string& name, long long mob, int total_amount)
        : consumer_name(name), mobile(mob), total(total_amount) {}

    void writeToCSV(const string& filename) const {
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << consumer_name << "," << mobile << "," << total << "\n";
            file.close();
            cout << "Bill generated for: " << consumer_name << endl;
        } else {
            cerr << "Error opening file!" << endl;
        }
    }
    

};

// Billing System
class Biller {
public:
    string consumer_name;
    long long mobile;

    void generateBill(const string& filename) {
        cout << "Enter consumer name: ";
        cin >> consumer_name;
        cout << "Enter mobile number: ";
        cin >> mobile;

        string items[50];
        int prices[50];
        int quantities[50];
        int product_count = 0;

        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening stock file!" << endl;
            return;
        }

        string line;
        while (getline(file, line) && product_count < 50) {
            stringstream ss(line);
            string name, quantity, price;
            getline(ss, name, ',');
            getline(ss, quantity, ',');
            getline(ss, price, ',');
            items[product_count] = name;
            quantities[product_count] = stoi(quantity);
            prices[product_count] = stoi(price);
            product_count++;
        }
        file.close();

        string selected;
        int order_qty;
        int total = 0;

        cout << "\nAvailable Products:\n";
        for (int i = 0; i < product_count; ++i) {
            cout << items[i] << " - Rs." << prices[i] << " (" << quantities[i] << " in stock)" << endl;
        }

        cout << "\nStart billing (type 0 to finish):\n";
        while (true) {
            cout << "Enter product name to add: ";
            cin >> selected;
            if (selected == "0") break;

            int found = -1;
            for (int i = 0; i < product_count; ++i) {
                if (items[i] == selected) {
                    found = i;
                    break;
                }
            }

            if (found == -1) {
                cout << "Product not found!\n";
                continue;
            }

            cout << "Enter quantity: ";
            cin >> order_qty;

            if (order_qty > quantities[found]) {
                cout << "Insufficient stock! Only " << quantities[found] << " available.\n";
                continue;
            }

            int cost = order_qty * prices[found];
            total += cost;
            quantities[found] -= order_qty;
            cout << "Added " << selected << " x" << order_qty << " = Rs." << cost << "\n";
        }

        cout << "\n======= BILL =======\n";
        cout << "Customer: " << consumer_name << "\n";
        cout << "Mobile: " << mobile << "\n";
        cout << "Total Amount: Rs." << total << "\n";   
        cout << "====================\n";



        // Save to CSV
        Bill bill(consumer_name, mobile, total);
        bill.writeToCSV("bill.csv");
                ofstream out("stock.csv", ios::trunc); // Open in truncate mode to overwrite
if (out.is_open()) {
    for (int i = 0; i < product_count; ++i) {
        out << items[i] << "," << quantities[i] << "," << prices[i] << "\n";
    }
    out.close();
    cout << "Stock updated.\n";
} else {
    cerr << "Error writing updated stock!\n";
}
    }
};

int main() {
    int main_choice;
    bool exit = false;

    while (!exit) {
        cout << "\n========== MAIN MENU ==========\n";
        cout << "1. Billing\n2. Stock Management\n3. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> main_choice)) {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (main_choice) {
        case 1: {
            Biller b;
            b.generateBill("stock.csv");
            break;
        }
        case 2: {
            int stock_choice;
            cout << "\n--- STOCK MANAGEMENT ---\n";
            cout << "1. Add New Product\n2. Restock Product (Not implemented)\n0. Back to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> stock_choice;

            switch (stock_choice) {
            case 1: {
                string product_name;
                int quantity;
                int price;

                cout << "Enter product name: ";
                cin >> product_name;
                cout << "Enter quantity: ";
                cin >> quantity;
                cout << "Enter price: ";
                cin >> price;

                Stock s(product_name, quantity, price);
                s.writeToCSV("stock.csv");
                break;
            }
            case 2:
                cout << "Restock feature not implemented.\n";
                break;
            case 0:
                break;
            default:
                cout << "Invalid input. Try again.\n";
                break;
            }
            break;
        }
        case 3:
            exit = true;
            break;
        default:
            cout << "Invalid choice! Try again.\n";
            break;
        }
    }

    cout << "Program ended.\n";
    return 0;
}
