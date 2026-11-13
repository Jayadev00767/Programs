#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <limits>
#include <stdexcept>
using namespace std;

// Quantity class with operator overloading
class Quantity {
    int value;

public:
    explicit Quantity(int v = 0) : value(v) {
        if (v < 0)
            throw invalid_argument("Quantity cannot be negative.");
    }

    int get() const {
        return value;
    }

    Quantity operator+(const Quantity& q) const {
        return Quantity(value + q.value);
    }

    Quantity operator-(const Quantity& q) const {
        if (q.value > value)
            throw underflow_error("Insufficient quantity.");

        return Quantity(value - q.value);
    }

    bool operator<(const Quantity& q) const {
        return value < q.value;
    }

    friend ostream& operator<<(ostream& out, const Quantity& q) {
        return out << q.value;
    }
};

// Money class with operator overloading
class Money {
    double value;

public:
    explicit Money(double v = 0.0) : value(v) {
        if (v < 0)
            throw invalid_argument("Price cannot be negative.");
    }

    double get() const {
        return value;
    }

    Money operator+(const Money& m) const {
        return Money(value + m.value);
    }

    Money operator*(int quantity) const {
        if (quantity < 0)
            throw invalid_argument("Quantity cannot be negative.");

        return Money(value * quantity);
    }

    friend ostream& operator<<(ostream& out, const Money& m) {
        return out << fixed << setprecision(2) << m.value;
    }
};

// Component class
class Component {
    string id, name, type;
    Quantity stock;
    Money price;

public:
    Component() : stock(0), price(0) {}

    Component(string i, string n, string t, int q, double p)
        : id(move(i)), name(move(n)), type(move(t)),
          stock(q), price(p) {

        if (id.empty() || name.empty() || type.empty())
            throw invalid_argument("Component fields cannot be empty.");
    }

    string getId() const {
        return id;
    }

    string getName() const {
        return name;
    }

    string getType() const {
        return type;
    }

    Quantity getStock() const {
        return stock;
    }

    Money getPrice() const {
        return price;
    }

    Money stockValue() const {
        return price * stock.get();
    }

    void setStock(Quantity q) {
        stock = q;
    }

    void display() const {
        cout << left << setw(10) << id
             << setw(18) << name
             << setw(14) << type
             << right << setw(8) << stock
             << setw(12) << price << '\n';
    }

    friend ostream& operator<<(ostream& out, const Component& c) {
        return out << c.id << " - " << c.name
                   << " (stock: " << c.stock << ")";
    }
};

// Inventory class
class Inventory {
    map<string, Component> items;

public:
    void add(const Component& c) {
        auto result = items.emplace(c.getId(), c);

        if (!result.second)
            throw runtime_error("Component ID already exists.");
    }

    void updateStock(const string& id, int change) {
        auto it = items.find(id);

        if (it == items.end())
            throw runtime_error("Component not found.");

        Quantity current = it->second.getStock();

        if (change >= 0)
            it->second.setStock(current + Quantity(change));
        else
            it->second.setStock(current - Quantity(-change));
    }

    void remove(const string& id) {
        if (items.erase(id) == 0)
            throw runtime_error("Component not found.");
    }

    const Component* find(const string& id) const {
        auto it = items.find(id);

        return (it == items.end()) ? nullptr : &it->second;
    }

    void display() const {
        cout << "\n"
             << left << setw(10) << "ID"
             << setw(18) << "NAME"
             << setw(14) << "TYPE"
             << right << setw(8) << "STOCK"
             << setw(12) << "PRICE" << '\n';

        cout << string(62, '-') << '\n';

        for (const auto& entry : items)
            entry.second.display();

        if (items.empty())
            cout << "Inventory is empty.\n";
    }

    Money totalValue() const {
        Money total;

        for (const auto& entry : items)
            total = total + entry.second.stockValue();

        return total;
    }

    // Save inventory to file
    void save(const string& filename) const {
        ofstream file(filename);

        if (!file)
            throw runtime_error("Unable to open inventory file for writing.");

        for (const auto& entry : items) {
            const Component& c = entry.second;

            file << c.getId() << '|'
                 << c.getName() << '|'
                 << c.getType() << '|'
                 << c.getStock().get() << '|'
                 << c.getPrice().get() << '\n';
        }
    }

    // Load inventory from file
    void load(const string& filename) {
        ifstream file(filename);

        if (!file)
            return;

        string line;

        while (getline(file, line)) {
            stringstream ss(line);

            string id, name, type, qtyText, priceText;

            if (getline(ss, id, '|') &&
                getline(ss, name, '|') &&
                getline(ss, type, '|') &&
                getline(ss, qtyText, '|') &&
                getline(ss, priceText)) {

                try {
                    Component c(id, name, type,
                                stoi(qtyText), stod(priceText));

                    items[id] = c;
                }
                catch (const exception&) {
                    cerr << "Skipped invalid record in inventory file.\n";
                }
            }
        }
    }
};

// Bill of Materials item
struct BOMItem {
    string componentId;
    Quantity required;

    BOMItem(string id, int q)
        : componentId(move(id)), required(q) {}
};

// BOM Validator class
class BOMValidator {
    vector<BOMItem> requirements;

public:
    void addRequirement(const string& id, int qty) {
        if (id.empty())
            throw invalid_argument("Component ID cannot be empty.");

        auto it = find_if(
            requirements.begin(),
            requirements.end(),
            [&](const BOMItem& item) {
                return item.componentId == id;
            }
        );

        if (it == requirements.end())
            requirements.emplace_back(id, qty);
        else
            it->required = it->required + Quantity(qty);
    }

    void validate(const Inventory& inventory) const {
        int shortages = 0;

        cout << "\n========== BOM VALIDATION ==========\n";

        for (const auto& item : requirements) {
            const Component* c = inventory.find(item.componentId);

            if (!c) {
                cout << item.componentId
                     << " : NOT IN INVENTORY (required "
                     << item.required << ")\n";

                shortages += item.required.get();
            }

            else if (c->getStock() < item.required) {
                int shortBy =
                    item.required.get() - c->getStock().get();

                cout << c->getName()
                     << " : SHORTAGE (" << shortBy
                     << " units)\n";

                shortages += shortBy;
            }

            else {
                cout << c->getName()
                     << " : AVAILABLE\n";
            }
        }

        cout << "------------------------------------\n";

        if (shortages == 0)
            cout << "BOM STATUS: READY FOR PRODUCTION\n";

        else {
            cout << "BOM STATUS: NOT READY FOR PRODUCTION\n";
            cout << "Total shortage: "
                 << shortages << " units\n";
        }

        cout << "====================================\n";
    }
};

// Input validation
int readInt(const string& prompt) {
    int value;

    cout << prompt;

    if (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        throw invalid_argument("Please enter a valid whole number.");
    }

    if (value < 0)
        throw invalid_argument("Value cannot be negative.");

    return value;
}

double readDouble(const string& prompt) {
    double value;

    cout << prompt;

    if (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        throw invalid_argument("Please enter a valid price.");
    }

    if (value < 0)
        throw invalid_argument("Price cannot be negative.");

    return value;
}

// Main function
int main() {
    Inventory inventory;

    inventory.load("inventory.txt");

    BOMValidator bom;

    int choice = 0;

    cout << "==============================================\n";
    cout << " PCB COMPONENT INVENTORY & BOM VALIDATOR\n";
    cout << "==============================================\n";

    do {
        cout << "\n1. Add component\n";
        cout << "2. View inventory\n";
        cout << "3. Search component\n";
        cout << "4. Update stock\n";
        cout << "5. Remove component\n";
        cout << "6. Inventory value\n";
        cout << "7. Add BOM requirement\n";
        cout << "8. Validate BOM\n";
        cout << "9. Save and exit\n";

        try {
            choice = readInt("Enter choice: ");

            switch (choice) {

            case 1: {
                string id, name, type;

                cout << "Component ID: ";
                cin >> id;

                cout << "Component name: ";
                cin >> ws;
                getline(cin, name);

                cout << "Component type: ";
                getline(cin, type);

                int qty = readInt("Opening quantity: ");

                double price = readDouble("Unit price: ");

                inventory.add(
                    Component(id, name, type, qty, price)
                );

                cout << "Component added successfully.\n";

                break;
            }

            case 2:
                inventory.display();
                break;

            case 3: {
                string id;

                cout << "Enter component ID: ";
                cin >> id;

                const Component* c = inventory.find(id);

                if (c)
                    cout << *c << '\n';
                else
                    cout << "Component not found.\n";

                break;
            }

            case 4: {
                string id;

                cout << "Enter component ID: ";
                cin >> id;

                int change;

                cout << "Stock change (+ to add, - to subtract): ";

                if (!(cin >> change)) {
                    cin.clear();
                    cin.ignore(
                        numeric_limits<streamsize>::max(), '\n'
                    );

                    throw invalid_argument(
                        "Please enter a valid whole number."
                    );
                }

                inventory.updateStock(id, change);

                cout << "Stock updated successfully.\n";

                break;
            }

            case 5: {
                string id;

                cout << "Enter component ID: ";
                cin >> id;

                inventory.remove(id);

                cout << "Component removed.\n";

                break;
            }

            case 6:
                cout << "Total inventory value: Rs. "
                     << inventory.totalValue() << '\n';

                break;

            case 7: {
                string id;

                cout << "Component ID required: ";
                cin >> id;

                int qty = readInt("Required quantity: ");

                bom.addRequirement(id, qty);

                cout << "BOM requirement added.\n";

                break;
            }

            case 8:
                bom.validate(inventory);
                break;

            case 9:
                inventory.save("inventory.txt");

                cout << "Inventory saved. Goodbye!\n";

                break;

            default:
                cout << "Invalid menu choice.\n";
            }
        }

        catch (const exception& e) {
            cout << "Error: " << e.what() << '\n';
        }

    } while (choice != 9);

    return 0;
}