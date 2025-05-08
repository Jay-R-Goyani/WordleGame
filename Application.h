#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <functional>
#include <limits>
#pragma once
#include "WordleGame.h"
#include "UserManager.h"
#include "Admin.h"

using namespace std;

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"

// Admin credentials (consider moving to a config or making them more secure later)
const string ADMIN_USERNAME = "admin";
const string ADMIN_SPECIAL_KEY = "admin";

class Application {
public:
    vector<string> wordList; // Moved wordList to be a member of Application

    Application() { // Constructor to initialize wordList
        wordList = {
            "apple", "world", "crate", "point", "table", "plant", "score", "piano", "magic", "light",
            "brush", "candy", "dream", "eagle", "field", "grape", "house", "jelly", "knife", "lemon",
            "mouse", "night", "ocean", "pride", "queen", "river", "stone", "tiger", "uncle", "vivid",
            "whale", "xenon", "youth", "zebra", "angel", "beach", "cloud", "drama", "earth", "flame",
            "globe", "happy", "image", "juice", "karma", "lucky", "medal", "noble", "orbit", "peace",
            "quick", "robot", "smile", "trust", "unity", "value", "wheat", "xylem", "zesty", "arise",
            "bloom", "clear", "daisy", "event", "fresh", "giant", "honor", "ideal", "jumpy", "kneel",
            "laugh", "match", "nerve", "offer", "party", "quest", "royal", "shade", "trend", "upper",
            "vocal", "worry", "xenon", "yield", "zippy", "acorn", "bliss", "charm", "doubt", "enter",
            "flare", "grasp", "hurry", "inbox", "jewel", "known", "loyal", "merry", "needy", "openy",
            "plain", "quiet", "rider", "speak", "tempo", "urban", "voter", "witty", "xerox", "young"
        };
        // TODO: Add loading wordList from a file here if persistence is desired
    }

    // TODO: Add a method to save wordList to a file if persistence is desired
    // void saveWordListToFile() { ... }

    void printBanner() {
    cout << "  __        __   _                                \n";
    cout << "  \\ \\      / /__| | ___ ___  _ __ ___   ___     \n";
    cout << "   \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ \n";
    cout << "    \\ V  V /  __/ | (_| (_) | | | | | |  __/     \n";
    cout << "     \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___|\n";
    cout<<endl;
    cout << RESET;
}

    // Helper function for robust integer input
    int getIntegerInput(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            cin >> value;
            if (cin.good()) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
                return value;
            } else {
                cout << RED << "Invalid input. Please enter a number." << RESET << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }

    void run() {
        srand(static_cast<unsigned>(time(nullptr)));
        UserManager um;
        Player *current = nullptr;
        Admin adminUser;

        // wordList is now a member, initialized in constructor

        if (wordList.empty()) {
            cerr << RED << "Error: Word list is empty. Cannot start the game." << RESET << endl;
            // Consider if admin should still be able to manage an empty word list
            // For now, exiting if critical for gameplay.
             // return; // If wordlist is critical for any operation including admin, then exit.
                      // If admin can manage an empty list, then this check needs refinement.
        }

        while (true)
        {
            clearScreen();
            printBanner();

            cout << BOLD << MAGENTA << "1." << " Login" << RESET << endl;
            cout << BOLD << MAGENTA << "2." << " Signup" << RESET << endl;
            cout << BOLD << MAGENTA << "3." << " Exit" << RESET << endl;
            int opt = getIntegerInput(string(CYAN) + "Select option: " + RESET);

            if (opt == 3)
                break;

            if (opt == 1) // Login Attempt
            {
                string u, p;
                cout << "Username: ";
                cin >> u;
                cout << "Password: ";
                cin >> p;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Consume newline after password input

                // Check for Admin Login
                if (u == ADMIN_USERNAME && p == ADMIN_SPECIAL_KEY) {
                    cout << GREEN << "Admin login successful!" << RESET << endl;
                    cout << "Press Enter to continue...";
                    cin.get();

                    bool adminLoggedIn = true;
                    while (adminLoggedIn) {
                        clearScreen();
                        printBanner(); // Or a specific admin banner
                        adminUser.displayMenu();
                        int adminChoice = adminUser.getAdminIntegerInput(string(CYAN) + "Admin option: " + RESET);

                        switch (adminChoice) {
                            case 1:
                                adminUser.handleUserManagement(um);
                                break;
                            case 2:
                                adminUser.handleWordListManagement(wordList);
                                break;
                            case 3:
                                adminUser.viewSystemStatistics(um);
                                break;
                            case 4:
                                adminLoggedIn = false;
                                cout << YELLOW << "Logging out from Admin Panel..." << RESET << endl;
                                break;
                            default:
                                cout << RED << "Invalid admin option." << RESET << endl;
                                break;
                        }
                        if (adminLoggedIn && adminChoice !=4) {
                            cout << "Press Enter to return to Admin Menu...";
                            cin.get();
                        }
                    }
                    current = nullptr; // Ensure player is not set after admin logout
                    continue; // Go back to the main menu after admin logs out
                }

                // Regular Player Login
                if (u.empty() || p.empty()) {
                    cout << RED << "Username and password cannot be empty." << RESET << endl;
                    cout << "Press Enter to continue...";
                    cin.get();
                    continue;
                }
                current = um.login(u, p);
                if (current) {
                    cout << GREEN << "Login successful!" << RESET << endl;
                } else {
                    cout << RED << "Invalid credentials." << RESET << endl;
                }
            }
            else if (opt == 2) // Signup
            {
                string u, p, d, g;
                cout << "Username: ";
                cin >> u;
                cout << "Password: ";
                cin >> p;
                cout << "DOB (YYYY-MM-DD): ";
                cin >> d;
                cout << "Gender: ";
                cin >> g;
                if (d.empty() || g.empty()) {
                    cout << RED << "DOB and Gender cannot be empty for signup." << RESET << endl;
                    cout << "Press Enter to continue...";
                    cin.get();
                    continue;
                }
                current = um.signup(u, p, d, g);
                if (current)
                    cout << GREEN << "Signup successful!" << RESET << endl;
                else
                    cout << RED << "Signup failed. Username might already exist." << RESET << endl;
            }

            // This block is for Player session, only if login/signup was successful and it wasn't admin
            if (current) // This ensures this part only runs if a regular player is logged in
            {
                bool playerSessionActive = true;
                while(playerSessionActive){
                    clearScreen();
                    printBanner();
                    cout << BOLD << YELLOW << "Welcome, " << current->getUsername() << "!" << RESET << endl;
                    cout << BOLD << MAGENTA << "1." << " Play Game" << RESET << endl;
                    cout << BOLD << MAGENTA << "2." << " View Stats" << RESET << endl;
                    cout << BOLD << MAGENTA << "3." << " Logout" << RESET << endl;
                    int choice = getIntegerInput(string(CYAN) + "Choose: " + RESET);

                    if (choice == 1) {
                        while (1)
                        {
                            string secret = wordList[rand() % wordList.size()];
                            WordleGame game(secret, current);
                            int score = game.play();
                            current->incrementGames();
                            current->updateScore(score);
                            current->showStats();
                            um.save();
                            
                            cout << BOLD << BLUE << "1." << " Continue " << RESET << endl;
                            cout << BOLD << CYAN << "2." << " Quit " << RESET << endl;
                            int quitorcont = getIntegerInput(string(CYAN) + "Enter choice (1 or 2): " + RESET);
                            
                            if (quitorcont == 2)
                            {
                                break;
                            }
                            else if (quitorcont != 1) {
                                cout << RED << "Invalid choice. Defaulting to continue." << RESET << endl;
                            }
                        }
                    } else if (choice == 2) {
                        current->showStats();
                        cout << "Press Enter to return...";
                        cin.get();
                    } else if (choice == 3) {
                        playerSessionActive = false; // Logout
                        current = nullptr; // Clear current player
                    } else {
                         cout << RED << "Invalid option. Please try again." << RESET << endl;
                         cout << "Press Enter to continue...";
                         cin.get();
                    }
                }
            }
            else if (opt != 1 && opt !=2 && opt !=3) // Catch invalid main menu options if not admin and not player login/signup failure that gets handled inside opt 1/2
            {
                cout << RED << "Invalid option. Please select 1, 2, or 3." << RESET << endl;
                cout << "Press Enter to continue...";
                cin.get();
            }
        }
        clearScreen();
        cout << BOLD << GREEN << "Thanks for playing! Goodbye!" << RESET << endl;
    }

    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            cout << "\033[2J\033[1;1H"; 
        #endif
    } 
}; 