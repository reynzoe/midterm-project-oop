#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>


using namespace std;

// Function to check if input is a valid numeric string (including decimals)
bool isValidNumericString(const string &input) {
    bool decimal = false;

    for (char ch : input) {
        if (ch == '.') {
            if (decimal)  // Only allow one decimal point
                return false;
            decimal = true;
        } else if (!isdigit(ch))  // Check if each character is a digit
            return false;
    }
    return !input.empty();  // Return false if the string is empty
}

string toUpperCase(string str) {
    for (char &c : str) {
        if (isalpha(c))
            c = toupper(c);
    }
    return str;
}

class Item {
private:
    string id, name;
    int quantity;
    double price;
    string category;

public:
    Item(string id, string name, int quantity, double price, string category)
            : id(id), name(name), quantity(quantity), price(price), category(category) {}

    string getId() const { return id; }
    string getName() const { return name; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }
    string getCategory() const { return category; }

    void setQuantity(int newQuantity) { quantity = newQuantity; }
    void setPrice(double newPrice) { price = newPrice; }

    void display() const {
        cout << left << setw(10) << id << setw(20) << name << setw(10) << quantity
             << setw(10) << price << setw(15) << category << endl;
    }
};

class Inventory {
protected:
    Item* items[100];  // Max 100 items in inventory
    int itemCount;     // Initialize itemCount
public:
    Inventory() : itemCount(0) {}  // Constructor to initialize itemCount
    virtual void displayAllItems() = 0;
    virtual void addItem() = 0;
    virtual void displayItemsByCategory() = 0;
    virtual void searchItem() = 0;
    virtual void sortItems() = 0;
    virtual void displayLowStockItems() = 0;
    virtual void updateItem() = 0;
    virtual void removeItems() = 0;
};

class ItemManager : public Inventory {
public:
    // Validates category in a case-insensitive manner
    bool isValidCategory(const string &category) {
        string lowerCategory = toUpperCase(category);
        return lowerCategory == "CLOTHING" || lowerCategory == "ELECTRONICS" || lowerCategory == "ENTERTAINMENT";
    }

    int findItemById(const string &id) {
        for (int i = 0; i < itemCount; ++i) {
            if (items[i]->getId() == id)
                return i;
        }
        return -1;
    }

    int findItemByName(const string &name) {
        string lowerName = toUpperCase(name);
        for (int i = 0; i < itemCount; ++i) {
            if (toUpperCase(items[i]->getName()) == lowerName)
                return i;
        }
        return -1;
    }

    void addItem() override {
        string id, name, category, quantityStr, priceStr;
        int quantity;
        double price;
        bool isDuplicate = true;

        // Input and validate category (case-insensitive)
        do {
            cout << "Enter Category (Clothing, Electronics, Entertainment): ";
            cin >> category;

            // Check if category is valid (case-insensitive check)
            if (!isValidCategory(category)) {
                cout << "Invalid category! Please enter 'Clothing', 'Electronics', or 'Entertainment'." << endl;
            }
        } while (!isValidCategory(category));

        // Check for duplicate IDs
        while (isDuplicate) {
            cout << "Enter Item ID: ";
            cin >> id;

            isDuplicate = false;  // Assume it's not a duplicate initially
            for (int i = 0; i < itemCount; i++) {  // Check all existing items
                if (items[i]->getId() == id) {  // If the ID already exists
                    isDuplicate = true;  // Mark as duplicate and prompt again
                    cout << "ERROR: An item already has that ID, please enter another ID.\n";
                    break;
                }
            }
        }

        cout << "Enter Item Name: ";
        cin.ignore();
        getline(cin, name);

        // Validate quantity input to ensure it's greater than 0
        do {
            cout << "Enter Quantity: ";
            cin >> quantityStr;
            if (!isValidNumericString(quantityStr) || stoi(quantityStr) <= 0) {
                cout << "Input a valid quantity! Quantity must be greater than 0." << endl;
            }
        } while (!isValidNumericString(quantityStr) || stoi(quantityStr) <= 0);
        quantity = stoi(quantityStr);

        // Validate price input to ensure it's greater than 0
        do {
            cout << "Enter Price: ";
            cin >> priceStr;
            if (!isValidNumericString(priceStr) || stod(priceStr) <= 0) {
                cout << "Input a valid price! Price must be greater than 0." << endl;
            }
        } while (!isValidNumericString(priceStr) || stod(priceStr) <= 0);
        price = stod(priceStr);

        // Add the item to the inventory
        items[itemCount++] = new Item(id, name, quantity, price, toUpperCase(category));
        cout << "Item added successfully!" << endl;
    }

    void updateItem() override {
        if (itemCount == 0) {
            cout << "No items available to update!" << endl;
            return;
        }

        string id, newQuantityStr, newPriceStr;
        cout << "Enter Item ID: ";
        cin >> id;

        int index = findItemById(id);
        if (index == -1) {
            cout << "Item not found!" << endl;
            return;
        }

        int choice;
        cout << "Update (1- Quantity, 2- Price): ";
        cin >> choice;

        if (choice == 1) {
            int newQuantity;
            do {
                cout << "Enter new Quantity: ";
                cin >> newQuantityStr;
                if (!isValidNumericString(newQuantityStr) || stoi(newQuantityStr) < 0) {
                    cout << "Invalid quantity! Please enter a valid positive number." << endl;
                }
            } while (!isValidNumericString(newQuantityStr) || stoi(newQuantityStr) < 0);
            newQuantity = stoi(newQuantityStr);
            items[index]->setQuantity(newQuantity);
            cout << "Quantity of Item " << items[index]->getName() << " is updated!" << endl;
        } else if (choice == 2) {
            double newPrice;
            do {
                cout << "Enter new Price: ";
                cin >> newPriceStr;
                if (!isValidNumericString(newPriceStr) || stod(newPriceStr) < 0) {
                    cout << "Invalid price! Please enter a valid positive number." << endl;
                }
            } while (!isValidNumericString(newPriceStr) || stod(newPriceStr) < 0);
            newPrice = stod(newPriceStr);
            items[index]->setPrice(newPrice);
            cout << "Price of Item " << items[index]->getName() << " is updated!" << endl;
        } else {
            cout << "Invalid option!" << endl;
        }
    }

    void removeItems() override {
        if (itemCount == 0) {
            cout << "There is nothing to remove!" << endl;
            return;
        }
        string id;
        cout << "Enter Item ID: ";
        cin >> id;
        toUpperCase(id);

        int index = findItemById(id);
        if (index == -1) {
            cout << "Item with ID " << id << " was not found." << endl;
            return;
        }

        cout << "Item " << items[index]->getName() << " has been removed from the inventory." << endl;

        // Shift the remaining items to fill the gap
        delete items[index];  // Free the memory
        for (int i = index; i < itemCount - 1; ++i) {
            items[i] = items[i + 1];
        }
        itemCount--;  // Decrease item count
    }

    void displayAllItems() override {
        if (itemCount == 0) {
            cout << "No items available!" << endl;
            return;
        }

        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity"
             << setw(10) << "Price" << setw(15) << "Category" << endl;

        for (int i = 0; i < itemCount; i++) {
            items[i]->display();
        }
    }

    void displayItemsByCategory() override {
        if (itemCount == 0) {
            cout << "No items available!" << endl;
            return;
        }

        string category;
        cout << "Enter Category (Clothing, Electronics, Entertainment): ";
        cin >> category;
        category = toUpperCase(category);

        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity"
             << setw(10) << "Price" << setw(15) << "Category" << endl;

        bool found = false;
        for (int i = 0; i < itemCount; i++) {
            if (items[i]->getCategory() == category) {
                items[i]->display();
                found = true;
            }
        }
        if (!found) {
            cout << "No items found in the " << category << " category!" << endl;
        }
    }

    void searchItem() override {
        if (itemCount == 0) {
            cout << "No items available!" << endl;
            return;
        }

        string name;
        cout << "Enter Item Name: ";
        cin.ignore();
        getline(cin, name);

        int index = findItemByName(name);
        if (index != -1) {
            cout << "Item found!" << endl;
            items[index]->display();
        } else {
            cout << "Item not found!" << endl;
        }
    }

    void sortItems() override
    {
        // Check if there are items to sort
        if (itemCount == 0)
        {
            cout << "There is nothing to sort." << endl;
            return;
        }

        string choice;
        bool ascending;

        // Prompt for sorting criteria
        cout << "Sort by: 1. Quantity 2. Price: ";
        cin >> choice;

        // Ask for sort order
        char orderChoice;
        cout << "Sort in ascending order? (Y/N): ";
        cin >> orderChoice;
        ascending = (toupper(orderChoice) == 'Y');

        // Bubble Sort
        for (int i = 0; i < itemCount - 1; ++i)
        {
            for (int j = 0; j < itemCount - i - 1; ++j)
            {
                bool swapNeeded = false;

                if (choice == "1") // Sort by Quantity
                {
                    if ((ascending && items[j]->getQuantity() > items[j + 1]->getQuantity()) ||
                        (!ascending && items[j]->getQuantity() < items[j + 1]->getQuantity()))
                    {
                        swapNeeded = true;
                    }
                }
                else if (choice == "2") // Sort by Price
                {
                    if ((ascending && items[j]->getPrice() > items[j + 1]->getPrice()) ||
                        (!ascending && items[j]->getPrice() < items[j + 1]->getPrice()))
                    {
                        swapNeeded = true;
                    }
                }

                if (swapNeeded)
                {
                    // Hardcode the swap logic
                    Item *temp = items[j];
                    items[j] = items[j + 1];
                    items[j + 1] = temp;
                }
            }
        }
    }


    void displayLowStockItems() override {
        if (itemCount == 0) {
            cout << "No items available!" << endl;
            return;
        }

        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity"
             << setw(10) << "Price" << setw(15) << "Category" << endl;

        bool found = false;
        for (int i = 0; i < itemCount; i++) {
            if (items[i]->getQuantity() <=5) {  // Assuming low stock is less than 5
                items[i]->display();
                found = true;
            }
        }
        if (!found) {
            cout << "No low stock items found!" << endl;
        }
    }
};

int main() {
    ItemManager manager;
    int choice;

    do {
        cout << "\nMenu " << endl;
        cout << "1. Add Item" << endl;
        cout << "2. Display All Items" << endl;
        cout << "3. Update Item" << endl;
        cout << "4. Remove Item" << endl;
        cout << "5. Display Items by Category" << endl;
        cout << "6. Search Item" << endl;
        cout << "7. Sort Items" << endl;
        cout << "8. Display Low Stock Items" << endl;
        cout << "9. Exit" << endl;
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                manager.addItem();
                break;
            case 2:
                manager.displayAllItems();
                break;
            case 3:
                manager.updateItem();
                break;
            case 4:
                manager.removeItems();
                break;
            case 5:
                manager.displayItemsByCategory();
                break;
            case 6:
                manager.searchItem();
                break;
            case 7:
                manager.sortItems();
                break;
            case 8:
                manager.displayLowStockItems();
                break;
            case 9:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 9);

    return 0;
}
