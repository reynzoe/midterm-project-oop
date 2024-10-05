#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>  // For isdigit and tolower

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
    Item* items[100]; // Max 100 items in inventory
    int itemCount;

    // Custom  function for sorting two Item pointers
    void sorts(Item* &item1, Item* &item2)
    {
        Item* temp = item1;
        item1 = item2;
        item2 = temp;
    }

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

    // Function to find an item by name (case-insensitive)
    int findItemByName(const string &name)
    {
        string lowerName = toLowerCase(name);
        for (int i = 0; i < itemCount; ++i)
        {
            if (toLowerCase(items[i]->getName()) == lowerName)
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
        bool isDuplicate = true;

        // Input and validate category (case-insensitive, no numbers allowed)
        do
        {
            cout << "Enter Category (Clothing, Electronics, Entertainment): ";
            cin >> category;

            // Check if category is valid (case-insensitive check and no numeric or special characters allowed)
            if (!isValidCategory(category) || !isAlphaString(category))
            {
                cout << "Invalid category! Please enter 'Clothing', 'Electronics', or 'Entertainment' without any numbers or special characters." << endl;
            }
        } while (!isValidCategory(category) || !isAlphaString(category));

        // Check for duplicate IDs
        while (isDuplicate)
        {
            cout << "Enter Item ID: ";
            cin >> id;

            isDuplicate = false;  // Assume it's not a duplicate initially
            for (int i = 0; i < itemCount; i++)  // Check all existing items
            {
                if (items[i]->getId() == id)  // If the ID already exists
                {
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
        do
        {
            cout << "Enter Quantity: ";
            cin >> quantityStr;
            if (!isValidNumericString(quantityStr) || stoi(quantityStr) <= 0)
            {
                cout << "Input a valid quantity! Quantity must be greater than 0." << endl;
            }
        } while (!isValidNumericString(quantityStr) || stoi(quantityStr) <= 0);
        quantity = stoi(quantityStr);

        // Validate price input to ensure it's greater than 0
        do
        {
            cout << "Enter Price: ";
            cin >> priceStr;
            if (!isValidNumericString(priceStr) || stod(priceStr) <= 0)
            {
                cout << "Input a valid price! Price must be greater than 0." << endl;
            }
        } while (!isValidNumericString(priceStr) || stod(priceStr) <= 0);
        price = stod(priceStr);

        // Add the item to the inventory
        items[itemCount++] = new Item(id, name, quantity, price, toLowerCase(category));
        cout << "Item added successfully!" << endl;
    }

// Helper function to check if a string contains only alphabetic characters
    bool isAlphaString(const string &str)
    {
        for (char ch : str)
        {
            if (!isalpha(ch))  // Check if character is not alphabetic
            {
                return false;
            }
        }
        return true;
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
            cout << "No items in inventory to check!" << endl;
            return;
        }

        bool found = false;
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity"
             << setw(10) << "Price" << setw(15) << "Category" << endl;

        for (int i = 0; i < itemCount; ++i)
        {
            if (items[i]->getQuantity() < 5)
            {
                items[i]->display();
                found = true;
            }
        }

        if (!found)
        {
            cout << "There are no low stock items!" << endl;
        }
    }

    void sortItems()
    {
        if (itemCount == 0)  // Fix: Check if the item count is 0, not if the array is empty
        {
            cout << "\nNo items to sort.\n";
            return;
        }

        string choice;
        string order;

        cout << "\nSort by: \n1 - Quantity\n2 - Price\n";
        cin >> choice;

        while (choice != "1" && choice != "2")
        {
            cout << "ERROR: input 1 or 2 only \n";
            cout << "\nSort by: \n1 - Quantity\n2 - Price\n";
            cin >> choice;
        }

        cout << "\nSort order: \n1 - Ascending\n2 - Descending\n";
        cin >> order;

        while (order != "1" && order != "2")
        {
            cout << "ERROR: input 1 or 2 only \n";
            cout << "\nSort order: \n1 - Ascending\n2 - Descending\n";
            cin >> order;
        }

        bool ascending = (order == "1");

        // Bubble Sort
        for (int i = 0; i < itemCount - 1; ++i)
        {
            for (int j = 0; j < itemCount - i - 1; ++j)
            {
                bool swapNeeded = false;

                if (choice == "1")
                { // Sort by Quantity
                    if ((ascending && items[j]->getQuantity() > items[j + 1]->getQuantity()) ||
                        (!ascending && items[j]->getQuantity() < items[j + 1]->getQuantity()))
                    {
                        swapNeeded = true;
                    }
                }
                else if (choice == "2")
                { // Sort by Price
                    if ((ascending && items[j]->getPrice() > items[j + 1]->getPrice()) ||
                        (!ascending && items[j]->getPrice() < items[j + 1]->getPrice()))
                    {
                        swapNeeded = true;
                    }
                }

                if (swapNeeded)
                {
                    swap(items[j], items[j + 1]);  // Use the custom swap function to swap the pointers
                }
            }
        }

        // After sorting, display the sorted items
        displayAllItems();
    }


    void displaySortedItems()
    {
        cout << "\nSorted Items:\n";
        cout << "------------------------------------------------------------\n";
        cout << left << setw(10) << "ID"
             << left << setw(40) << "Name"
             << right << setw(10) << "Quantity"
             << right << setw(10) << "Price"
             << right << setw(20) << "Category" << endl;

        cout << "------------------------------------------------------------\n";

        for (const auto &item : items)
        {
            cout << left << setw(10) << item->getId()
                 << left << setw(40) << item->getName()
                 << right << setw(10) << item->getQuantity()
                 << right << setw(10) << fixed << setprecision(2) << item->getPrice()
                 << right << setw(20) << item->getCategory() << endl;
        }

        cout << "------------------------------------------------------------\n";
    }

    // Function to search for an item by ID or name
    void searchItem()
    {
        if (itemCount == 0)
        {
            cout << "No items to search for!" << endl;
            return;
        }

        int searchChoice;
        cout << "Search by (1- ID, 2- Name): ";
        cin >> searchChoice;

        if (searchChoice == 1)
        {
            string id;
            cout << "Enter Item ID: ";
            cin >> id;

            int index = findItemById(id);
            if (index == -1)
            {
                cout << "Item with ID " << id << " not found!" << endl;
            }
            else
            {
                cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity"
                     << setw(10) << "Price" << setw(15) << "Category" << endl;
                items[index]->display();
            }
        }
        else if (searchChoice == 2)
        {
            string name;
            cout << "Enter Item Name: ";
            cin.ignore(); // Clear input buffer
            getline(cin, name);

            int index = findItemByName(name);
            if (index == -1)
            {
                cout << "Item with name \"" << name << "\" not found!" << endl;
            }
            else
            {
                cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity"
                     << setw(10) << "Price" << setw(15) << "Category" << endl;
                items[index]->display();
            }
        }
        else
        {
            cout << "Invalid search option!" << endl;
        }
    }
};

int main()
{
    Inventory inventory;
    bool running = true;
    int choice;

    while (running)
    {
        cout << "\nMenu\n";
        cout << "1 - Add Item\n";
        cout << "2 - Update Item\n";
        cout << "3 - Remove Item\n";
        cout << "4 - Display Items by Category\n";
        cout << "5 - Display All Items\n";
        cout << "6 - Search Item\n";
        cout << "7 - Sort Items\n";
        cout << "8 - Display Low Stock Items\n";
        cout << "9 - Exit\n";
        cout << "Enter your choice: ";

        cin >> choice;

        if (cin.fail())  // Check if the input was invalid
        {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid choice! Please enter a valid number." << endl;
            continue;  // Go back to the start of the loop
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
            case 6:
                inventory.searchItem();
                break;
            case 7:
                inventory.sortItems();
                break;
            case 8:
                inventory.displayLowStockItems();
                break;
            case 9:
                cout << "Exiting..." << endl;
                running = false;  // Set running to false to exit the loop
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }

    return 0;
}

