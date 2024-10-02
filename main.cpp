#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>  // For isdigit and tolower
#include <algorithm> // For transform

using namespace std;

// Function to check if input is a valid numeric string (including decimals)
bool isValidNumericString(const string &input)
{
    bool decimal = false;

    for (char ch : input)
    {
        if (ch == '.')
        {
            if (decimal) // Only allow one decimal point
            {
                return false;
            }
            decimal = true;
        }
        else if (!isdigit(ch)) // Check if each character is a digit
        {
            return false;
        }
    }
    return !input.empty(); // Return false if the string is empty
}

// Function to convert a string to lowercase for case-insensitive comparison
string toLowerCase(const string &str)
{
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

class Item
{
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

    void display() const
    {
        cout << left << setw(10) << id << setw(20) << name << setw(10) << quantity
             << setw(10) << price << setw(15) << category << endl;
    }
};

class Inventory
{
private:
    Item *items[100]; // Max 100 items in inventory
    int itemCount;

    // Validates category in a case-insensitive manner
    bool isValidCategory(const string &category)
    {
        string lowerCategory = toLowerCase(category);
        return lowerCategory == "clothing" || lowerCategory == "electronics" || lowerCategory == "entertainment";
    }

    int findItemById(const string &id)
    {
        for (int i = 0; i < itemCount; ++i)
        {
            if (items[i]->getId() == id)
                return i;
        }
        return -1;
    }

public:
    Inventory() : itemCount(0) {}

    void addItem()
    {
        string id, name, category, quantityStr, priceStr;
        int quantity;
        double price;

        // Input and validate category (case insensitive)
        cout << "Enter Category (Clothing, Electronics, Entertainment): ";
        cin >> category;
        if (!isValidCategory(category))
        {
            cout << "Category " << category << " does not exist!" << endl;
            return;
        }

        cout << "Enter Item ID: ";
        cin >> id;
        cout << "Enter Item Name: ";
        cin.ignore();
        getline(cin, name);

        // Validate quantity input
        do
        {
            cout << "Enter Quantity: ";
            cin >> quantityStr;
            if (!isValidNumericString(quantityStr) || stoi(quantityStr) < 0)
            {
                cout << "Invalid quantity! Please enter a valid positive number." << endl;
            }
        } while (!isValidNumericString(quantityStr) || stoi(quantityStr) < 0);
        quantity = stoi(quantityStr);

        // Validate price input
        do
        {
            cout << "Enter Price: ";
            cin >> priceStr;
            if (!isValidNumericString(priceStr) || stod(priceStr) < 0)
            {
                cout << "Invalid price! Please enter a valid positive number." << endl;
            }
        } while (!isValidNumericString(priceStr) || stod(priceStr) < 0);
        price = stod(priceStr);

        items[itemCount++] = new Item(id, name, quantity, price, toLowerCase(category));
        cout << "Item added successfully!" << endl;
    }

    void updateItem()
    {
        if (itemCount == 0)
        {
            cout << "No items available to update!" << endl;
            return;
        }

        string id, newQuantityStr, newPriceStr;
        cout << "Enter Item ID: ";
        cin >> id;

        int index = findItemById(id);
        if (index == -1)
        {
            cout << "Item not found!" << endl;
            return;
        }

        int choice;
        cout << "Update (1- Quantity, 2- Price): ";
        cin >> choice;

        if (choice == 1)
        {
            int newQuantity;
            do
            {
                cout << "Enter new Quantity: ";
                cin >> newQuantityStr;
                if (!isValidNumericString(newQuantityStr) || stoi(newQuantityStr) < 0)
                {
                    cout << "Invalid quantity! Please enter a valid positive number." << endl;
                }
            } while (!isValidNumericString(newQuantityStr) || stoi(newQuantityStr) < 0);
            newQuantity = stoi(newQuantityStr);
            items[index]->setQuantity(newQuantity);
            cout << "Quantity of Item " << items[index]->getName() << " is updated!" << endl;
        }
        else if (choice == 2)
        {
            double newPrice;
            do
            {
                cout << "Enter new Price: ";
                cin >> newPriceStr;
                if (!isValidNumericString(newPriceStr) || stod(newPriceStr) < 0)
                {
                    cout << "Invalid price! Please enter a valid positive number." << endl;
                }
            } while (!isValidNumericString(newPriceStr) || stod(newPriceStr) < 0);
            newPrice = stod(newPriceStr);
            items[index]->setPrice(newPrice);
            cout << "Price of Item " << items[index]->getName() << " is updated!" << endl;
        }
        else
        {
            cout << "Invalid option!" << endl;
        }
    }

    void removeItem()
    {
        if (itemCount == 0)
        {
            cout << "There is nothing to remove!" << endl;
            return;
        }

        string id;
        cout << "Enter Item ID: ";
        cin >> id;

        int index = findItemById(id);
        if (index == -1)
        {
            cout << "Item not found!" << endl;
            return;
        }

        cout << "Item " << items[index]->getName() << " has been removed from the inventory." << endl;
        delete items[index];

        for (int i = index; i < itemCount - 1; ++i)
        {
            items[i] = items[i + 1];
        }
        itemCount--;
    }

    void displayItemsByCategory()
    {
        if (itemCount == 0)
        {
            cout << "There are no items to display!" << endl;
            return;
        }

        string category;
        cout << "Enter Category: ";
        cin >> category;
        if (!isValidCategory(category))
        {
            cout << "Category " << category << " does not exist!" << endl;
            return;
        }

        bool found = false;
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity"
             << setw(10) << "Price" << setw(15) << "Category" << endl;

        for (int i = 0; i < itemCount; ++i)
        {
            if (items[i]->getCategory() == toLowerCase(category))
            {
                items[i]->display();
                found = true;
            }
        }

        if (!found)
        {
            cout << "There are no items to display in the " << category << " category." << endl;
        }
    }

    void displayAllItems()
    {
        if (itemCount == 0)
        {
            cout << "There are no items in the inventory!" << endl;
            return;
        }

        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity"
             << setw(10) << "Price" << setw(15) << "Category" << endl;

        for (int i = 0; i < itemCount; ++i)
        {
            items[i]->display();
        }
    }

    void displayLowStockItems()
    {
        if (itemCount == 0)
        {
            cout << "There are no items in the inventory!" << endl;
            return;
        }

        bool found = false;
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity"
             << setw(10) << "Price" << setw(15) << "Category" << endl;

        for (int i = 0; i < itemCount; ++i)
        {
            if (items[i]->getQuantity() <= 5)
            {
                items[i]->display();
                found = true;
            }
        }

        if (!found)
        {
            cout << "No low-stock items to display." << endl;
        }
    }
};

// Main menu
int main()
{
    Inventory inventory;
    int choice;

    while (true)
    {
        cout << "\n--- Main Menu ---\n1 - Add Item\n2 - Update Item\n3 - Remove Item\n4 - Display Items by Category\n5 - Display All Items\n6 - Search Item\n7 - Sort Items\n8 - Display Low Stock Items\n9 - Exit\nEnter your choice: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }

        switch (choice)
        {
            case 1:
                inventory.addItem();
                break;
            case 2:
                inventory.updateItem();
                break;
            case 3:
                inventory.removeItem();
                break;
            case 4:
                inventory.displayItemsByCategory();
                break;
            case 5:
                inventory.displayAllItems();
                break;
            case 8:
                inventory.displayLowStockItems();
                break;
            case 9:
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid choice! Try again." << endl;
        }
    }

    return 0;
}
