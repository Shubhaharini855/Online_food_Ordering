#include <iostream>
#include <vector>
#include <iomanip>
#include <regex>
using namespace std;

// User Module
class User {
public:
    string username;
    string address;
    string mobile;

    User(string uname, string addr, string mob) : username(uname), address(addr), mobile(mob) {}

    void display() {
        cout << "\nUser: " << username << ", Address: " << address << ", Mobile: " << mobile << endl;
    }

    static bool isValidName(const string& name) {
        return regex_match(name, regex("^[A-Za-z ]+$"));
    }

    static bool isValidMobile(const string& mobile) {
        return regex_match(mobile, regex("^[0-9]{10}$"));
    }
};

// Menu Item
struct MenuItem {
    string name;
    double price;
};

// Cart Item
struct CartItem {
    MenuItem item;
    int quantity;
};

// Restaurant Module
class Restaurant {
public:
    string name;
    vector<MenuItem> menu;

    Restaurant(string rname) : name(rname) {
        menu.push_back(MenuItem{"Burger", 149.00});
        menu.push_back(MenuItem{"Pizza", 299.00});
        menu.push_back(MenuItem{"Pasta", 199.00});
        menu.push_back(MenuItem{"French Fries", 299.00});
        menu.push_back(MenuItem{"Veg Noodles", 150.00});
        menu.push_back(MenuItem{"Fried Rice", 239.00});
        menu.push_back(MenuItem{"Shawarma", 129.00});
        menu.push_back(MenuItem{"Chicken Noodles", 159.00});
        menu.push_back(MenuItem{"Veg Biriyani", 49.00});
        menu.push_back(MenuItem{"Chicken Biriyani", 49.00});
    }

    void displayMenu() {
        cout << "\n--- Menu for " << name << " ---\n";
        cout << left << setw(5) << "No." << setw(18) << "Item" << "Price (INR)\n";
        for (int i = 0; i < menu.size(); i++) {
            cout << setw(5) << i + 1 << setw(18) << menu[i].name << "₹" << fixed << setprecision(2) << menu[i].price << endl;
        }
    }

    MenuItem getItem(int index) {
        if (index >= 0 && index < menu.size())
            return menu[index];
        return {"Invalid", 0.0};
    }

    bool isValidChoice(int index) {
        return index >= 0 && index < menu.size();
    }
};

// Cart Module
class Cart {
public:
    vector<CartItem> items;

    void addItem(MenuItem item, int quantity) {
        if (item.name != "Invalid" && quantity > 0) {
            items.push_back(CartItem{item, quantity});
            cout << item.name << " × " << quantity << " added to cart.\n";
        }
    }

    void showCart() {
        cout << "\n--- Cart Items ---\n";
        double total = 0.0;
        for (const auto& cartItem : items) {
            double subtotal = cartItem.item.price * cartItem.quantity;
            cout << "- " << cartItem.item.name << " × " << cartItem.quantity
                 << ": ₹" << fixed << setprecision(2) << subtotal << endl;
            total += subtotal;
        }
        cout << "Total: ₹" << fixed << setprecision(2) << total << endl;
    }

    double getTotal() {
        double total = 0.0;
        for (const auto& cartItem : items)
            total += cartItem.item.price * cartItem.quantity;
        return total;
    }

    bool isEmpty() {
        return items.empty();
    }

    void clearCart() {
        items.clear();
    }
};

// Order Module
class Order {
public:
    User user;
    vector<CartItem> items;
    string status;

    Order(User u, vector<CartItem> i) : user(u), items(i), status("Placed") {}

    void displayOrder() {
        cout << "\n--- Order Summary ---\n";
        cout << "Customer: " << user.username << "\n";
        cout << "Mobile: " << user.mobile << "\n";
        cout << "Address: " << user.address << "\n";
        cout << "Items:\n";
        double total = 0;
        for (const auto& cartItem : items) {
            double subtotal = cartItem.item.price * cartItem.quantity;
            cout << "- " << cartItem.item.name << " × " << cartItem.quantity
                 << ": ₹" << fixed << setprecision(2) << subtotal << endl;
            total += subtotal;
        }
        cout << "Total: ₹" << fixed << setprecision(2) << total << endl;
        cout << "Order Status: " << status << endl;
    }
};

// Payment Module
class Payment {
public:
    static bool processPayment(double amount, string method, double& balance) {
        if (method == "Card" || method == "card") {
            cout << "\nProcessing payment of ₹" << fixed << setprecision(2) << amount << " via Card...\n";
            if (balance >= amount) {
                balance -= amount;
                cout << "Payment Successful!\n";
                cout << "Remaining Balance: ₹" << fixed << setprecision(2) << balance << endl;
                return true;
            } else {
                cout << "Payment Failed: Insufficient Balance.\n";
                return false;
            }
        } else if (method == "Cash" || method == "cash") {
            cout << "\nPayment of ₹" << fixed << setprecision(2) << amount << " will be made in Cash on Delivery.\n";
            cout << "Payment Marked as Successful.\n";
            return true;
        } else {
            cout << "Invalid payment method.\n";
            return false;
        }
    }
};

// Main Program
int main() {
    cout << "=== Welcome to the Food Ordering App ===\n";
    string name, address, mobile;

    // Name input with validation
    do {
        cout << "Enter your name: ";
        getline(cin, name);
        if (!User::isValidName(name)) {
            cout << "Invalid name. Please enter alphabets only.\n";
        }
    } while (!User::isValidName(name));

    // Address input
    cout << "Enter your address: ";
    getline(cin, address);

    // Mobile input with validation
    do {
        cout << "Enter your mobile number: ";
        getline(cin, mobile);
        if (!User::isValidMobile(mobile)) {
            cout << "Invalid mobile number. Please enter exactly 10 numeric digits.\n";
        }
    } while (!User::isValidMobile(mobile));

    User user(name, address, mobile);
    Restaurant rest("Foodies Corner");
    Cart cart;

    user.display();
    rest.displayMenu();

    int choice;
    cout << "\nEnter item number to add to cart (0 to finish): ";
    while (cin >> choice && choice != 0) {
        if (rest.isValidChoice(choice - 1)) {
            int quantity;
            cout << "Enter quantity: ";
            cin >> quantity;
            if (quantity <= 0) {
                cout << "Quantity must be greater than 0.\n";
                continue;
            }
            cart.addItem(rest.getItem(choice - 1), quantity);
        } else {
            cout << "Enter a valid choice.\n";
        }
        cout << "Add another item (0 to finish): ";
    }

    if (!cart.isEmpty()) {
        cart.showCart();

        string paymentMethod;
        double balance = 0.0;

        cout << "\nEnter your payment method (Card/Cash): ";
        cin >> paymentMethod;

        if (paymentMethod == "Card" || paymentMethod == "card") {
            cout << "Enter your account balance (INR): ₹";
            cin >> balance;
        }

        double totalAmount = cart.getTotal();
        bool paid = Payment::processPayment(totalAmount, paymentMethod, balance);

        if (paid) {
            Order order(user, cart.items);
            order.displayOrder();
            cart.clearCart();
        } else {
            cout << "Order Cancelled due to failed payment.\n";
        }
    } else {
        cout << "\nCart is empty. No order placed.\n";
    }

    cout << "\nThank you for using our Food Ordering App!\n";
    return 0;
}

