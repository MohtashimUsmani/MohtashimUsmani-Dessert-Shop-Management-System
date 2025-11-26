#include <iostream>
#include <fstream>
using namespace std;

class Dessert {
public:
    int id;
    string name;
    float price;

    Dessert(int id = 0, string name = "", float price = 0.0) {
        this->id = id;
        this->name = name;
        this->price = price;
    }

    virtual void display() {
        cout << "ID: " << id 
             << " | Name: " << name 
             << " | Price: " << price << endl;
    }
};
class IceCream : public Dessert {
public:
    IceCream(int id, string name, float price) : Dessert(id, name, price) {}
};

class Cake : public Dessert {
public:
    Cake(int id, string name, float price) : Dessert(id, name, price) {}
};

class Pastry : public Dessert {
public:
    Pastry(int id, string name, float price) : Dessert(id, name, price) {}
};
class Customer {
public:
    int customerId;
    string name;
    string phoneNumber;

    Customer(int id = 0, string name = "", string phone = "") {
        customerId = id;
        this->name = name;
        phoneNumber = phone;
    }

    void display() {
        cout << "Customer ID: " << customerId 
             << " | Name: " << name 
             << " | Phone: " << phoneNumber << endl;
    }
};
class Order {
public:
    int orderId;
    Customer customer;
    Dessert* desserts[50];  
    int dessertCount;
    float totalBill;

    Order(int id = 0, Customer c = Customer()) {
        orderId = id;
        customer = c;
        dessertCount = 0;
        totalBill = 0;
    }

    void addDessert(Dessert* d) {
        desserts[dessertCount] = d;
        dessertCount++;
    }

    void calculateBill() {
        totalBill = 0;
        for (int i = 0; i < dessertCount; i++) {
            totalBill += desserts[i]->price;
        }
    }

    void printReceipt() {
        cout << "\n===== ORDER RECEIPT =====\n";
        cout << "Order ID: " << orderId << endl;
        customer.display();

        cout << "\nItems:\n";
        for (int i = 0; i < dessertCount; i++) {
            desserts[i]->display();
        }

        calculateBill();
        cout << "--------------------------\n";
        cout << "Total Bill: " << totalBill << endl;
        cout << "==========================\n";
    }

    void saveToFile() {
        calculateBill();
        ofstream file("orders.txt", ios::app);

        file << "Order ID: " << orderId << "\n";
        file << "Customer: " << customer.name << "\n";
        file << "Phone: " << customer.phoneNumber << "\n";
        file << "Items:\n";

        for (int i = 0; i < dessertCount; i++) {
            file << desserts[i]->name << " - " << desserts[i]->price << "\n";
        }

        file << "Total: $ " << totalBill << "\n";
        file << "-----------------------------\n";

        file.close();
    }
};
void loadDessertsFromFile(Dessert* desserts[], int &dessertCount) {
    ifstream file("desserts.txt");
    if (!file) {
        cout << "No dessert file found. Starting fresh.\n";
        return;
    }

    int type, id;
    string name;
    float price;

    while (file >> type >> id >> name >> price) {
        if (type == 1)
            desserts[dessertCount] = new IceCream(id, name, price);
        else if (type == 2)
            desserts[dessertCount] = new Cake(id, name, price);
        else
            desserts[dessertCount] = new Pastry(id, name, price);

        dessertCount++;
    }

    file.close();
    cout << "Desserts Loaded Successfully!\n";
}
void saveDessertsToFile(Dessert* desserts[], int dessertCount) {
    ofstream file("desserts.txt");

    for (int i = 0; i < dessertCount; i++) {
        int type = 0;
        if (dynamic_cast<IceCream*>(desserts[i])) type = 1;
        else if (dynamic_cast<Cake*>(desserts[i])) type = 2;
        else if (dynamic_cast<Pastry*>(desserts[i])) type = 3;

        file << type << " "
             << desserts[i]->id << " "
             << desserts[i]->name << " "
             << desserts[i]->price << "\n";
    }

    file.close();
}
int main() {
    Dessert* desserts[100];
    int dessertCount = 0;

    Customer customers[100];
    int customerCount = 0;

    Order orders[100];
    int orderCount = 0;
    
    loadDessertsFromFile(desserts, dessertCount);

    int choice;

    while (true) {
        cout << "\n===== DESSERT SHOP MENU =====\n";
        cout << "1. Add Dessert\n";
        cout << "2. View All Desserts\n";
        cout << "3. Add Customer\n";
        cout << "4. Create Order\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        if (choice == 1) {
            int type, id;
            string name;
            float price;

            cout << "Choose Dessert Type:\n1. IceCream\n2. Cake\n3. Pastry\n";
            cin >> type;

            cout << "Enter ID: ";
            cin >> id;

            cout << "Enter Name: ";
            cin >> name;

            cout << "Enter Price: ";
            cin >> price;

            if (type == 1)
                desserts[dessertCount] = new IceCream(id, name, price);
            else if (type == 2)
                desserts[dessertCount] = new Cake(id, name, price);
            else
                desserts[dessertCount] = new Pastry(id, name, price);

            dessertCount++;
            saveDessertsToFile(desserts, dessertCount);
            cout << "Dessert Added Successfully!\n";
        }
        else if (choice == 2) {
            cout << "\n=== Available Desserts ===\n";
            for (int i = 0; i < dessertCount; i++) {
                desserts[i]->display();
            }
        }
        else if (choice == 3) {
            int id;
            string name, phone;

            cout << "Enter Customer ID: ";
            cin >> id;

            cout << "Enter Name: ";
            cin >> name;

            cout << "Enter Phone: ";
            cin >> phone;

            customers[customerCount] = Customer(id, name, phone);
            customerCount++;

            cout << "Customer Added!\n";
        }
        else if (choice == 4) {
            if (customerCount == 0 || dessertCount == 0) {
                cout << "Please add customers and desserts first!\n";
                continue;
            }

            int orderId, custId;
            cout << "Enter Order ID: ";
            cin >> orderId;

            cout << "Enter Customer ID: ";
            cin >> custId;

            Customer selectedCustomer;
            bool found = false;

            for (int i = 0; i < customerCount; i++) {
                if (customers[i].customerId == custId) {
                    selectedCustomer = customers[i];
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "Customer not found!\n";
                continue;
            }

            Order order(orderId, selectedCustomer);

            int addMore = 1;
            while (addMore == 1) {
                cout << "\nEnter Dessert ID to add: ";
                int did;
                cin >> did;

                bool dessertFound = false;

                for (int i = 0; i < dessertCount; i++) {
                    if (desserts[i]->id == did) {
                        order.addDessert(desserts[i]);
                        dessertFound = true;
                        break;
                    }
                }

                if (!dessertFound)
                    cout << "Dessert not found!\n";

                cout << "Add more desserts? (1=yes, 0=no): ";
                cin >> addMore;
            }

            order.calculateBill();
            order.printReceipt();
            order.saveToFile();

            orders[orderCount] = order;
            orderCount++;

            cout << "Order Saved Successfully!\n";
        }

        else if (choice == 5) {
            cout << "Exiting... Goodbye!\n";
            break;
        }

        else {
            cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}

