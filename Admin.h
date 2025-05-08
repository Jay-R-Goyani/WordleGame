#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <limits> // For numeric_limits in getIntegerInput if we move it here or use a similar utility
#include <iomanip> // For setw in viewAllUsers
#include <algorithm> // For std::sort for word list

// Forward declaration if UserManager methods are used directly in Admin methods
// #include "UserManager.h" // Or forward declare: class UserManager;

// Assuming color codes and clearScreen might be used by admin panel too.
// You might want a shared "utils.h" for these if they are used in many places.
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

    // Placeholder for a helper function for robust integer input for admin menu
    int getAdminIntegerInput(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            cin >> value;
            if (cin.good()) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            } else {
                cout << RED << "Invalid input. Please enter a number." << RESET << endl;
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
        cout << "1. View All Users" << endl;
        cout << "2. View Specific User Details" << endl;
        cout << "3. Delete User" << endl;
        cout << "4. Back to Admin Menu" << endl;
    }

    void handleUserManagement(UserManager& um) {
        bool userManagementActive = true;
        while (userManagementActive) {
            displayUserManagementMenu();
            int choice = getAdminIntegerInput(string(CYAN) + "User Mgt Option: " + RESET);
            switch (choice) {
                case 1:
                    viewAllUsers(um);
                    break;
                case 2:
                    viewSpecificUserDetails(um);
                    break;
                case 3:
                    deleteUser(um);
                    break;
                case 4:
                    userManagementActive = false;
                    break;
                default:
                    cout << RED << "Invalid option." << RESET << endl;
                    break;
            }
            if (userManagementActive) {
                cout << "Press Enter to continue...";
                cin.get();
                clearScreen(); // Or keep on same screen depending on preference
            }
        }
    }

    void viewAllUsers(UserManager& um) {
        cout << BOLD << BLUE << "\n--- All Registered Users ---" << RESET << endl;
        const auto& players = um.getAllPlayers();
        if (players.empty()) {
            cout << "No users registered in the system." << endl;
            return;
        }
        cout << BOLD << left << setw(20) << "Username" 
             << setw(10) << "Gender" 
             << setw(15) << "Total Games" 
             << setw(15) << "Max Score" << RESET << endl;
        cout << string(60, '-') << endl; // Separator line
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
        cout << "Enter username to view details: ";
        getline(cin, username);
        if (username.empty()) {
            cout << RED << "Username cannot be empty." << RESET << endl;
            return;
        }
        const auto& players = um.getAllPlayers();
        auto it = players.find(username);
        if (it != players.end()) {
            it->second.showStats(); // Use the enhanced showStats from Player
        } else {
            cout << RED << "User '" << username << "' not found." << RESET << endl;
        }
    }

    void deleteUser(UserManager& um) {
        cout << BOLD << RED << "\n--- Delete User ---" << RESET << endl;
        string usernameToDelete;
        cout << "Enter username of the player to delete: ";
        // cin.ignore(); // Clear potential newline if previous input was integer
        getline(cin, usernameToDelete); // Use getline to allow usernames with spaces, though current signup might not

        if (usernameToDelete.empty()) {
            cout << RED << "Username cannot be empty." << RESET << endl;
            return;
        }

        if (um.deletePlayer(usernameToDelete)) {
            cout << GREEN << "Player '" << usernameToDelete << "' has been deleted successfully." << RESET << endl;
        } else {
            cout << RED << "Could not delete player '" << usernameToDelete << "'. User not found." << RESET << endl;
        }
    }

    // --- Word List Management Sub-Menu ---
    void displayWordListManagementMenu() {
        cout << BOLD << CYAN << "\n--- Manage Word List ---" << RESET << endl;
        cout << "1. List All Words (Sorted)" << endl;
        cout << "2. Add Word" << endl;
        cout << "3. Remove Word" << endl;
        cout << "4. Back to Admin Menu" << endl;
    }

    // Word list will be passed by reference from Application class
    void handleWordListManagement(vector<string>& wordList) {
         bool wordListManagementActive = true;
        while (wordListManagementActive) {
            displayWordListManagementMenu();
            int choice = getAdminIntegerInput(string(CYAN) + "Word List Mgt Option: " + RESET);
            switch (choice) {
                case 1:
                    listAllWords(wordList);
                    break;
                case 2:
                    addWord(wordList);
                    break;
                case 3:
                    removeWord(wordList);
                    break;
                case 4:
                    wordListManagementActive = false;
                    break;
                default:
                    cout << RED << "Invalid option." << RESET << endl;
                    break;
            }
            if (wordListManagementActive) {
                cout << "Press Enter to continue...";
                cin.get();
                // clearScreen(); 
            }
        }
    }

    void listAllWords(vector<string>& wordList) {
        cout << BOLD << BLUE << "\n--- All Words in List ---" << RESET << endl;
        if (wordList.empty()) {
            cout << "The word list is currently empty." << endl;
            return;
        }
        sort(wordList.begin(), wordList.end());
        for (size_t i = 0; i < wordList.size(); ++i) {
            cout << (i + 1) << ". " << wordList[i] << endl;
        }
    }

    void addWord(vector<string>& wordList) {
        cout << BOLD << GREEN << "\n--- Add Word to List ---" << RESET << endl;
        string newWord;
        cout << "Enter new 5-letter word to add: ";
        cin >> newWord;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // consume newline

        transform(newWord.begin(), newWord.end(), newWord.begin(), ::tolower);

        if (newWord.length() != 5 || !all_of(newWord.begin(), newWord.end(), ::isalpha)) {
            cout << RED << "Invalid word. Must be 5 alphabetic characters." << RESET << endl;
            return;
        }
        
        if (find(wordList.begin(), wordList.end(), newWord) != wordList.end()) {
            cout << YELLOW << "Word '" << newWord << "' already exists in the list." << RESET << endl;
        } else {
            wordList.push_back(newWord);
            cout << GREEN << "Word '" << newWord << "' added successfully." << RESET << endl;
            // Note: This change is in memory. For persistence, Application would need to save the wordList.
        }
    }

    void removeWord(vector<string>& wordList) {
        cout << BOLD << RED << "\n--- Remove Word from List ---" << RESET << endl;
        string wordToRemove;
        cout << "Enter word to remove: ";
        cin >> wordToRemove;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // consume newline

        transform(wordToRemove.begin(), wordToRemove.end(), wordToRemove.begin(), ::tolower);

        auto it = find(wordList.begin(), wordList.end(), wordToRemove);
        if (it != wordList.end()) {
            wordList.erase(it);
            cout << GREEN << "Word '" << wordToRemove << "' removed successfully." << RESET << endl;
        } else {
            cout << YELLOW << "Word '" << wordToRemove << "' not found in the list." << RESET << endl;
        }
        // Note: This change is in memory. For persistence, Application would need to save the wordList.
    }

    void viewSystemStatistics(UserManager& um) {
        cout << BOLD << CYAN << "\n--- Wordle Game System Statistics ---" << RESET << endl;
        SystemStats stats = um.getSystemStatistics();
        
        cout << string(50, '=') << endl;
        cout << BOLD << YELLOW << left << setw(35) << "Metric" << setw(15) << "Value" << RESET << endl;
        cout << string(50, '-') << endl;

        cout << left << setw(35) << "Total Registered Users:" << setw(15) << stats.totalRegisteredUsers << endl;
        cout << left << setw(35) << "Total Games Played by All Users:" << setw(15) << stats.totalGamesPlayedByAll << endl;
        cout << string(50, '-') << endl;
        cout << BOLD << "User Demographics:" << RESET << endl;
        cout << left << setw(35) << "  Total Male Members:" << setw(15) << stats.totalMaleMembers << endl;
        cout << left << setw(35) << "  Total Female Members:" << setw(15) << stats.totalFemaleMembers << endl;
        if (stats.totalOtherGenderMembers > 0) {
            cout << left << setw(35) << "  Total Other Gender Members:" << setw(15) << stats.totalOtherGenderMembers << endl;
        }
        cout << string(50, '-') << endl;
        cout << BOLD << "Score Achievements:" << RESET << endl;
        cout << left << setw(35) << "  Highest Score by a Male Player:" << setw(15) << (stats.highestMaleScore > 0 ? to_string(stats.highestMaleScore) : "N/A") << endl;
        cout << left << setw(35) << "  Highest Score by a Female Player:" << setw(15) << (stats.highestFemaleScore > 0 ? to_string(stats.highestFemaleScore) : "N/A") << endl;
        cout << string(50, '=') << endl;
    }

    // Helper for clearScreen, if not globally available or defined in Application.h
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            cout << "\033[2J\033[1;1H";
        #endif
    }
}; 