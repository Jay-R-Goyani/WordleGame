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
#include <random>
#include <cctype>
#pragma once
#include "player.h"
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

// Define lifeline constants
const int LIFELINE_REVEAL = 1;
const int LIFELINE_ELIMINATE = 1 << 1; // 2
const int LIFELINE_EXTRA = 1 << 2;     // 4

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[2J\033[1;1H"; // ANSI escape code to clear screen
#endif
}

class WordleGame
{
private:
    string secretWord;
    int maxAttempts = 6;
    map<char, bool> keys;
    Player *p;
    int availableLifelines;
    int idxRevealChecked[5] = {0};

public:
    WordleGame(const string &word, Player *p) : secretWord(word), p(p)
    {
        for (char c = 'a'; c <= 'z'; c++)
        {
            keys[c] = true;
        }
        availableLifelines = LIFELINE_REVEAL | LIFELINE_ELIMINATE | LIFELINE_EXTRA;
    }

    int play()
    {
        clearScreen();
        cout << BOLD << GREEN << "Get ready to guess!" << RESET << endl;
        cout << "The word has 5 letters. You have " << maxAttempts << " tries." << endl;
        int attempts = 0;
        bool won = false;

        while (attempts < maxAttempts)
        {
            cout << CYAN << "\nAttempt " << attempts + 1 << " of " << maxAttempts << ": " << RESET;
            cout << endl;

            displayKeyboard();

            cout << endl;
            cout << "Do you want to use any lifeline (if yes then type : lifeline)" << endl;

            string guess;
            cin >> guess;

            if (guess == "lifeline")
            {
                showLifeline();
                continue;
            }

            attempts++;
            transform(guess.begin(), guess.end(), guess.begin(), ::tolower);
            if (!isValidGuess(guess))
            {
                cout << RED << "Invalid input. Please enter a 5-letter word." << RESET;
                continue;
            }

            giveFeedback(guess);
            if (guess == secretWord)
            {
                won = true;
                break;
            }
        }

        int score = won ? (maxAttempts - attempts + 1) * 1000 : 0;
        cout << BOLD;
        if (won)
        {
            cout << GREEN << "\n*** Congrats! You guessed it in " << attempts << " attempts! ***" << RESET << endl;
        }
        else
        {
            cout << RED << "\n*** Game Over! The word was: " << secretWord << " ***" << RESET << endl;
        }
        cout << RESET;

        return score;
    }

private:
    void showLifeline()
    {
        if (availableLifelines == 0)
        {
            cout << RED << "No lifelines left this game.\n"
                 << RESET;
            return;
        }

        int score = p->gettotscore();
        cout << BOLD << "\nAvailable Lifelines (Score: "
             << score << ")\n"
             << RESET;

        if ((availableLifelines & LIFELINE_REVEAL) && score >= 100)
            cout << "- reveal     (cost 100)\n";
        if ((availableLifelines & LIFELINE_ELIMINATE) && score >= 200)
            cout << "- eliminate  (cost 200)\n";
        if ((availableLifelines & LIFELINE_EXTRA) && score >= 300)
            cout << "- extra      (cost 300)\n";
        cout << "Type a lifeline name or 'back': ";

        string cmd;
        cin>>cmd;

        if (cmd == "back")
            return;

        if (cmd == "reveal" && (availableLifelines & LIFELINE_REVEAL) && score >= 100)
        {
            p->setscore(-100);
            int idx;
            vector<int> unrevealed;
            for (int i = 0; i < 5; ++i)
            {
                if (!idxRevealChecked[i])
                    unrevealed.push_back(i);
            }

            if (unrevealed.empty())
            {
                cout << RED << "All letters already revealed.\n"
                     << RESET;
                return;
            }

            int _idx = unrevealed[rand() % unrevealed.size()];
            idxRevealChecked[_idx] = true;

            cout << GREEN << "Letter " << (_idx + 1) << " is '" << secretWord[_idx] << "'\n"
                 << RESET;

            availableLifelines &= ~LIFELINE_REVEAL;
        }
        else if (cmd == "eliminate" && (availableLifelines & LIFELINE_ELIMINATE) && score >= 200)
        {
            p->setscore(-200);
            vector<char> pool;
            for (char c = 'a'; c <= 'z'; ++c)
                if (keys[c] && secretWord.find(c) == string::npos)
                    pool.push_back(c);
            shuffle(pool.begin(), pool.end(), mt19937{random_device{}()});
            cout << YELLOW << "Eliminated:";
            for (int i = 0; i < 5 && i < pool.size(); ++i)
            {
                keys[pool[i]] = false;
                cout << ' ' << pool[i];
            }
            cout << RESET << "\n";
            availableLifelines &= ~LIFELINE_ELIMINATE;
        }
        else if (cmd == "extra" && (availableLifelines & LIFELINE_EXTRA) && score >= 300)
        {
            p->setscore(-300);
            maxAttempts++;
            cout << CYAN << "One extra attempt granted!\n"
                 << RESET;
            availableLifelines &= ~LIFELINE_EXTRA;
        }
        else
        {
            cout << RED
                 << "Cannot use '" << cmd
                 << "': unavailable or insufficient score.\n"
                 << RESET;
        }
    }

    void displayKeyboard()
    {
        string row1 = "qwertyuiop";
        string row2 = "asdfghjkl";
        string row3 = "zxcvbnm";

        cout << "\n";
        for (char c : row1)
        {
            if (keys[c])
                cout << "  " << c << "  " << RESET;
            else
                cout << "  _  ";
        }

        cout << "\n ";
        cout << " ";
        for (char c : row2)
        {
            if (keys[c])
                cout << "  " << c << "  " << RESET;
            else
                cout << "  _  ";
        }

        cout << "\n  ";
        cout << "  ";
        for (char c : row3)
        {
            if (keys[c])
                cout << "  " << c << "  " << RESET;
            else
                cout << "  _  ";
        }
        cout << "\n";
    }

    bool isValidGuess(const string &g)
    {
        if (g.size() != 5)
            return false;
        return all_of(g.begin(), g.end(), ::isalpha);
    }
    void giveFeedback(const string &guess)
    {
        vector<string> colors(5, "gray");
        vector<bool> secretUsed(5, false);

        // First pass: exact matches (green)
        for (int i = 0; i < 5; i++)
        {
            if (guess[i] == secretWord[i])
            {
                colors[i] = "green";
                secretUsed[i] = true;
            }
        }

        // Second pass: present but wrong position (yellow)
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                if (!secretUsed[j] && guess[i] == secretWord[j])
                {
                    colors[i] = "yellow";
                    secretUsed[j] = true;
                    break;
                }
            }
        }

        // Output feedback and update keys
        for (int i = 0; i < 5; i++)
        {
            if (colors[i] == "green")
            {
                idxRevealChecked[i] = 1;
                cout << GREEN << guess[i] << " " << RESET;
            }
            else if (colors[i] == "yellow")
            {
                cout << YELLOW << guess[i] << " " << RESET;
            }
            else
            {
                cout << WHITE << guess[i] << " " << RESET;
                keys[guess[i]] = false;
            }
        }
        cout << endl;
    }
};