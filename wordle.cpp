#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <functional>
#include <bits/stdc++.h>
#pragma once
#include "WordleGame.h"
#include "UserManager.h"

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

void printBanner()
{
    cout << "  __        __   _                                \n";
    cout << "  \\ \\      / /__| | ___ ___  _ __ ___   ___     \n";
    cout << "   \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _  \n";
    cout << "    \\ V  V /  __/ | (_| (_) | | | | | |  __/     \n";
    cout << "     \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___|\n";
    cout << RESET;
}

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));
    UserManager um;
    Player *current = nullptr;
    vector<string> wordList = {
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
    
    while (true)
    {
        clearScreen();
        printBanner();

        cout << BOLD << MAGENTA << "1." << " Login" << RESET << endl;
        cout << BOLD << MAGENTA << "2." << " Signup" << RESET << endl;
        cout << BOLD << MAGENTA << "3." << " Exit" << RESET << endl;
        cout << CYAN << "Select option: " << RESET;
        int opt;
        cin >> opt;

        if (opt == 3)
            break;

        if (opt == 1 || opt == 2)
        {
            string u, p, d, g;
            cout << "Username: ";
            cin >> u;
            cout << "Password: ";
            cin >> p;
            if (opt == 2)
            {
                cout << "DOB (YYYY-MM-DD): ";
                cin >> d;
                cout << "Gender: ";
                cin >> g;
                current = um.signup(u, p, d, g);
                if (current)
                    cout << GREEN << "Signup successful!" << RESET << endl;
            }
            else
            {
                current = um.login(u, p);
                if (!current)
                    cout << RED << "Invalid credentials." << RESET << endl;
            }
            if (!current)
            {
                cin.get();
                cin.get();
                continue;
            }

            while (true)
            {
                clearScreen();
                printBanner();
                cout << BOLD << YELLOW << "Welcome, " << current->getUsername() << "!" << RESET << endl;
                cout << BOLD << MAGENTA << "1." << " Play Game" << RESET << endl;
                cout << BOLD << MAGENTA << "2." << " View Stats" << RESET << endl;
                cout << BOLD << MAGENTA << "3." << " Logout" << RESET << endl;
                cout << CYAN << "Choose: " << RESET;
                int choice;
                cin >> choice;
                if (choice == 1)
                {
                    while (1)
                    {
                        string secret = wordList[rand() % wordList.size()];
                        WordleGame game(secret, current);
                        int score = game.play();
                        current->incrementGames();
                        current->updateScore(score);
                        current->showStats();
                        um.save();
                        int quitorcont;
                        cout << BOLD << BLUE << "1." << " Continue " << RESET << endl;
                        cout << BOLD << CYAN << "2." << " Quit " << RESET << endl;
                        cin >> quitorcont;
                        if (quitorcont == 2)
                        {
                            break;
                        }
                    }
                }
                else if (choice == 2)
                {
                    current->showStats();
                    cout << "Press Enter to return...";
                    cin.get();
                    cin.get();
                }
                else
                    break;
            }
        }
    }
    clearScreen();
    cout << BOLD << GREEN << "Thanks for playing! Goodbye!" << RESET << endl;
    return 0;
}
