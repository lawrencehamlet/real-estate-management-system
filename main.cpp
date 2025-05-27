#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

struct Property {
  string id;
  string location;
  double price;
  string type;
  string status;
  string owner;
  string telephone;
};

vector<Property> properties;
int propertyCounter = 1000;

string generatePropertyID() { return "PT" + to_string(propertyCounter++); }

bool isValidNumber(const string &str) {
  return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}

bool isValidName(const string &str) {
  return !str.empty() && all_of(str.begin(), str.end(),
                                [](char c) { return isalpha(c) || c == ' '; });
}

void registerUser() {
  string username, password, role;
  cout << "Enter username: ";
  cin >> username;
  cout << "Enter password: ";
  cin >> password;
  cout << "Enter role (Agent/Buyer): ";
  cin >> role;
  if (role != "Agent" && role != "Buyer") {
    cout << "Invalid role!\n";
    return;
  }
  ifstream userFile("users.txt");
  string fileUser, filePass, fileRole;
  while (userFile >> fileUser >> filePass >> fileRole) {
    if (fileUser == username) {
      cout << "Username already exists!\n";
      return;
    }
  }
  userFile.close();
  ofstream file("users.txt", ios::app);
  file << username << " " << password << " " << role << endl;
  file.close();
  cout << "User registered successfully!\n";
}

bool loginUser(string &role) {
  ifstream file("users.txt");
  if (!file) {
    cout << "Error opening user file.\n";
    return false;
  }
  string username, password, fileUser, filePass, fileRole;
  cout << "Enter username: ";
  cin >> username;
  cout << "Enter password: ";
  cin >> password;
  while (file >> fileUser >> filePass >> fileRole) {
    if (fileUser == username && filePass == password) {
      role = fileRole;
      file.close();
      return true;
    }
  }
  file.close();
  cout << "Login failed! Incorrect username or password.\n";
  return false;
}

void addProperty() {
  Property p;
  p.id = generatePropertyID();
  cout << "Enter location: ";
  cin >> p.location;
  cout << "Enter price: ";
  cin >> p.price;
  cout << "Enter property type: ";
  cin >> p.type;
  p.status = "Available";
  cin.ignore();
  cout << "Enter owner's name: ";
  getline(cin, p.owner);
  cout << "Enter owner's telephone: ";
  cin >> p.telephone;

  ofstream file("properties.txt", ios::app);
  file << p.id << " " << p.location << " " << p.price << " " << p.type << " "
       << p.status << " " << p.owner << " " << p.telephone << endl;
  file.close();
  cout << "Property added successfully!\n";
}

void viewProperties() {
  ifstream file("properties.txt");
  if (!file) {
    cout << "No properties listed yet.\n";
    return;
  }
  Property p;
  cout << "ID | Location | Price (GHS) | Type | Status | Owner | Telephone\n";
  cout << "------------------------------------------------------------\n";
  while (file >> p.id >> p.location >> p.price >> p.type >> p.status >>
         p.owner >> p.telephone) {
    cout << p.id << " | " << p.location << " | " << p.price << " | " << p.type
         << " | " << p.status << " | " << p.owner << " | " << p.telephone
         << endl;
  }
  file.close();
}

void buyOrLeaseProperty() {
  string id, buyerName;
  cout << "Enter property ID to buy/lease: ";
  cin >> id;
  cin.ignore();
  cout << "Enter your name: ";
  getline(cin, buyerName);

  ifstream file("properties.txt");
  ofstream tempFile("temp.txt");
  Property p;
  bool found = false;

  while (file >> p.id >> p.location >> p.price >> p.type >> p.status >>
         p.owner >> p.telephone) {
    if (p.id == id) {
      found = true;
      cout << "Generating Agreement Form...\n";
      cout << "--------------------------------------\n";
      cout << "Property ID: " << p.id << "\n";
      cout << "Owner Name: " << p.owner << "\n";
      cout << "Owner Telephone: " << p.telephone << "\n";
      cout << "Location: " << p.location << "\n";
      cout << "Price: " << p.price << " GHS\n";
      cout << "Buyer Name: " << buyerName << "\n";
      cout << "--------------------------------------\n";
      p.status = "Sold";
    }
    tempFile << p.id << " " << p.location << " " << p.price << " " << p.type
             << " " << p.status << " " << p.owner << " " << p.telephone << endl;
  }
  file.close();
  tempFile.close();
  remove("properties.txt");
  rename("temp.txt", "properties.txt");

  if (!found) {
    cout << "Property ID not found.\n";
  }
}

int main() {
  int choice;
  string role;
  while (true) {
    cout << "Welcome to EstateFlow, Please sign up or Login let's get you "
            "started!\n";
    cout << "1. Sign Up\n2. Login\n3. Exit\nChoice: ";
    cin >> choice;
    if (choice == 1) {
      registerUser();
    } else if (choice == 2) {
      if (loginUser(role)) {
        if (role == "Agent") {
          while (true) {
            cout << "1. Add Property\n2. View Properties\n3. Modify "
                    "Property\n4. Logout\nChoice: ";
            cin >> choice;
            if (choice == 1)
              addProperty();
            else if (choice == 2)
              viewProperties();
            else
              break;
          }
        } else if (role == "Buyer") {
          while (true) {
            cout << "1. View Properties\n2. Buy or Lease Property\n3. "
                    "Logout\nChoice: ";
            cin >> choice;
            if (choice == 1)
              viewProperties();
            else if (choice == 2)
              buyOrLeaseProperty();
            else
              break;
          }
        }
      }
    } else if (choice == 3) {
      cout << "Exiting program.\n";
      break;
    }
  }
  return 0;
}
