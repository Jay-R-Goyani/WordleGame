#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <limits> // For numeric_limits in getIntegerInput if we move it here or use a similar utility
#include <iomanip> // For setw in viewAllUsers
#include <algorithm> // For std::sort for word list

#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

// Forward declaration to avoid circular dependency if Admin needs UserManager and vice-versa
class UserManager; 

using namespace std;

class Admin {
public:
    Admin() {}

    void clearScreen() { // Assuming this is the canonical clearScreen for Admin
        #ifdef _WIN32
            system("cls");
        #else
            // Consider using an alternative that doesn't require a system call if problematic
            // For example, printing enough newlines, or more advanced terminal manipulation libraries.
            std::cout << "\033[2J\033[1;1H"; // Clears screen and moves cursor to top-left
            std::cout.flush(); // Ensure the command is sent immediately
        #endif
    }

    int getAdminIntegerInput(const string& prompt) {
        int value;
        while (true) {
            cout << prompt; // Prompt usually already colored by caller
            cin >> value;
            if (cin.good()) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            } else {
                cout << RED << BOLD << "Invalid input. Please enter a number." << RESET << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }

    void displayMenu() {
        cout << BOLD << MAGENTA << "\n=== Admin Panel ===" << RESET << endl;
        cout << YELLOW << "1. Manage Users" << RESET << endl;
        cout << YELLOW << "2. Manage Word List" << RESET << endl;
        cout << YELLOW << "3. View System Statistics" << RESET << endl;
        cout << YELLOW << "4. Logout" << RESET << endl;
    }

    // --- User Management Sub-Menu ---
    void displayUserManagementMenu() {
        cout << BOLD << CYAN << "\n--- Manage Users ---" << RESET << endl;
        cout << GREEN << "1. View All Users" << RESET << endl;
        cout << GREEN << "2. View Specific User Details" << RESET << endl;
        cout << RED   << "3. Delete User" << RESET << endl;
        cout << YELLOW << "4. Back to Admin Menu" << RESET << endl;
    }

    void handleUserManagement(UserManager& um) {
        bool userManagementActive = true;
        while (userManagementActive) {
            clearScreen(); 
            displayUserManagementMenu();
            int choice = getAdminIntegerInput(string(CYAN) + BOLD + "User Mgt Option: " + RESET);
            switch (choice) {
                case 1:
                    clearScreen();
                    viewAllUsers(um);
                    break;
                case 2:
                    clearScreen();
                    viewSpecificUserDetails(um);
                    break;
                case 3:
                    clearScreen();
                    deleteUser(um);
                    break;
                case 4:
                    userManagementActive = false;
                    break;
                default:
                    cout << RED << BOLD << "Invalid option. Please try again." << RESET << endl;
                    break;
            }
            if (userManagementActive) {
                cout << YELLOW << "\nPress Enter to return to User Management Menu..." << RESET;
                cin.get();
            }
        }
    }

    void viewAllUsers(UserManager& um) {
        cout << BOLD << BLUE << "\n--- All Registered Users ---" << RESET << endl;
        const auto& players = um.getAllPlayers();
        if (players.empty()) {
            cout << YELLOW << "No users registered in the system." << RESET << endl;
            return;
        }
        cout << BOLD << left << setw(20) << "Username" 
             << setw(10) << "Gender" 
             << setw(15) << "Total Games" 
             << setw(15) << "Max Score" << RESET << endl;
        cout << string(60, '-') << endl;
        for (const auto& pair : players) {
            const Player& p = pair.second;
            cout << left << setw(20) << p.getUsername()
                 << setw(10) << p.getGender()
                 << setw(15) << p.getTotalGames()
                 << setw(15) << p.getMaxScore() << endl;
        }
    }

    void viewSpecificUserDetails(UserManager& um) {
        cout << BOLD << BLUE << "\n--- View Specific User Details ---" << RESET << endl;
        string username;
        cout << CYAN << "Enter username to view details: " << RESET;
        // getline(cin, username); // cin.ignore() from getAdminIntegerInput handles newline usually
        // If issues persist with getline after integer input, a manual cin.ignore() might be needed before it.
        cin >> username; 
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear rest of the line including newline

        if (username.empty()) {
            cout << RED << BOLD << "Username cannot be empty." << RESET << endl;
            return;
        }
        const auto& players = um.getAllPlayers();
        auto it = players.find(username);
        if (it != players.end()) {
            it->second.showStats(); 
        } else {
            cout << RED << BOLD << "User '" << username << "' not found." << RESET << endl;
        }
    }

    void deleteUser(UserManager& um) {
        cout << BOLD << RED << "\n--- Delete User ---" << RESET << endl;
        string usernameToDelete;
        cout << CYAN << "Enter username of the player to delete: " << RESET;
        // getline(cin, usernameToDelete); 
        cin >> usernameToDelete;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (usernameToDelete.empty()) {
            cout << RED << BOLD << "Username cannot be empty." << RESET << endl;
            return;
        }

        if (um.deletePlayer(usernameToDelete)) {
            cout << GREEN << BOLD << "Player '" << usernameToDelete << "' has been deleted successfully." << RESET << endl;
        } else {
            cout << RED << BOLD << "Could not delete player '" << usernameToDelete << "'. User not found." << RESET << endl;
        }
    }

    // --- Word List Management Sub-Menu ---
    void displayWordListManagementMenu() {
        cout << BOLD << CYAN << "\n--- Manage Word List ---" << RESET << endl;
        cout << GREEN << "1. List All Words (Sorted)" << RESET << endl;
        cout << GREEN << "2. Add Word" << RESET << endl;
        cout << RED   << "3. Remove Word" << RESET << endl;
        cout << YELLOW << "4. Back to Admin Menu" << RESET << endl;
    }

    void handleWordListManagement(vector<string>& wordList) {
         bool wordListManagementActive = true;
        while (wordListManagementActive) {
            clearScreen();
            displayWordListManagementMenu();
            int choice = getAdminIntegerInput(string(CYAN) + BOLD + "Word List Mgt Option: " + RESET);
            switch (choice) {
                case 1:
                    clearScreen();
                    listAllWords(wordList);
                    break;
                case 2:
                    clearScreen();
                    addWord(wordList);
                    break;
                case 3:
                    clearScreen();
                    removeWord(wordList);
                    break;
                case 4:
                    wordListManagementActive = false;
                    break;
                default:
                    cout << RED << BOLD << "Invalid option. Please try again." << RESET << endl;
                    break;
            }
            if (wordListManagementActive) {
                cout << YELLOW << "\nPress Enter to return to Word List Management Menu..." << RESET;
                cin.get();
            }
        }
    }

    void listAllWords(vector<string>& wordList) {
        cout << BOLD << BLUE << "\n--- All Words in List ---" << RESET << endl;
        if (wordList.empty()) {
            cout << YELLOW << "The word list is currently empty." << RESET << endl;
            return;
        }
        sort(wordList.begin(), wordList.end());
        cout << GREEN; // Color for the list items
        for (size_t i = 0; i < wordList.size(); ++i) {
            cout << (i + 1) << ". " << wordList[i] << endl;
        }
        cout << RESET;
    }

    void addWord(vector<string>& wordList) {
        cout << BOLD << GREEN << "\n--- Add Word to List ---" << RESET << endl;
        string newWord;
        cout << CYAN << "Enter new 5-letter word to add: " << RESET;
        cin >> newWord;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        transform(newWord.begin(), newWord.end(), newWord.begin(), ::tolower);

        if (newWord.length() != 5 || !all_of(newWord.begin(), newWord.end(), ::isalpha)) {
            cout << RED << BOLD << "Invalid word. Must be 5 alphabetic characters." << RESET << endl;
            return;
        }
        
        if (find(wordList.begin(), wordList.end(), newWord) != wordList.end()) {
            cout << YELLOW << BOLD << "Word '" << newWord << "' already exists in the list." << RESET << endl;
        } else {
            wordList.push_back(newWord);
            cout << GREEN << BOLD << "Word '" << newWord << "' added successfully." << RESET << endl;
        }
    }

    void removeWord(vector<string>& wordList) {
        cout << BOLD << RED << "\n--- Remove Word from List ---" << RESET << endl;
        string wordToRemove;
        cout << CYAN << "Enter word to remove: " << RESET;
        cin >> wordToRemove;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        transform(wordToRemove.begin(), wordToRemove.end(), wordToRemove.begin(), ::tolower);

        auto it = find(wordList.begin(), wordList.end(), wordToRemove);
        if (it != wordList.end()) {
            wordList.erase(it);
            cout << GREEN << BOLD << "Word '" << wordToRemove << "' removed successfully." << RESET << endl;
        } else {
            cout << YELLOW << BOLD << "Word '" << wordToRemove << "' not found in the list." << RESET << endl;
        }
    }

    void viewSystemStatistics(UserManager& um) {
        // This function already has good formatting and coloring from previous implementation
        // Minor additions for consistency if any
        cout << BOLD << CYAN << "\n--- Wordle Game System Statistics ---" << RESET << endl;
        SystemStats stats = um.getSystemStatistics();
        
        cout << string(50, '=') << endl;
        cout << BOLD << YELLOW << left << setw(35) << "Metric" << setw(15) << "Value" << RESET << endl;
        cout << string(50, '-') << endl;

        cout << left << setw(35) << "Total Registered Users:" << setw(15) << stats.totalRegisteredUsers << endl;
        cout << left << setw(35) << "Total Games Played by All Users:" << setw(15) << stats.totalGamesPlayedByAll << endl;
        cout << string(50, '-') << endl;
        cout << BOLD << MAGENTA << "User Demographics:" << RESET << endl;
        cout << left << setw(35) << "  Total Male Members:" << setw(15) << stats.totalMaleMembers << endl;
        cout << left << setw(35) << "  Total Female Members:" << setw(15) << stats.totalFemaleMembers << endl;
        if (stats.totalOtherGenderMembers > 0) {
            cout << left << setw(35) << "  Total Other Gender Members:" << setw(15) << stats.totalOtherGenderMembers << endl;
        }
        cout << string(50, '-') << endl;
        cout << BOLD << MAGENTA << "Score Achievements:" << RESET << endl;
        cout << left << setw(35) << "  Highest Score by a Male Player:" << setw(15) << (stats.highestMaleScore > 0 ? to_string(stats.highestMaleScore) : "N/A") << endl;
        cout << left << setw(35) << "  Highest Score by a Female Player:" << setw(15) << (stats.highestFemaleScore > 0 ? to_string(stats.highestFemaleScore) : "N/A") << endl;
        cout << string(50, '=') << endl;
    }
}; 