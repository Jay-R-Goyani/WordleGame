#include <bits/stdc++.h>
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
#pragma once
#include "User.h"
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

class Player : public User
{
private:
    int totalGames;
    int maxScore;
    int totalscore; // This will now be a session-only score, not persisted

public:
    Player() : User(), totalGames(0), maxScore(0), totalscore(300) {}
    Player(const string &uname, const string &pwd, const string &dob, const string &gen)
        : User(uname, pwd, dob, gen), totalGames(0), maxScore(0), totalscore(300) {}

    // Getters needed for Admin Panel
    string getGender() const { return gender; } // gender is a protected member of User
    int getMaxScore() const { return maxScore; }
    int getTotalGames() const { return totalGames; }

    void updateScore(int score)
    {
        totalscore += score;
        if (score > maxScore)
            maxScore = score;
    }

    int gettotscore()
    {
        return totalscore;
    }

    void setscore(int sco)
    {
        totalscore = sco; // Changed to set, not add, for explicit score setting like the 300 initial
    }

    void incrementGames() { totalGames++; }

    void showStats() const
    {
        cout << BOLD << BLUE << "\n=== Player Stats ===" << RESET << endl;
        cout << "Username: " << username << endl;
        cout << "Total Games Played: " << totalGames << endl;
        cout << "Highest Score Achieved: " << maxScore << endl; // maxScore is still persisted
        cout << "Current Session Score: " << totalscore << endl;
        cout << "Gender: " << gender << endl;
    }

    // Serialization for file storage
    string serialize() const
    {
        ostringstream oss;
        // Removed totalscore from serialization
        oss << username << "|" << passwordHash << "|" << DOB << "|" << gender
            << "|" << totalGames << "|" << maxScore;
        return oss.str();
    }

    // Deserialize from file line
    static Player deserialize(const string &line)
    {
        // If the line itself is empty, getline in UserManager::load might skip it,
        // but if it contains only whitespace, it might be processed here.
        cout<<line<<endl;
        if (line.empty() || std::all_of(line.begin(), line.end(), ::isspace)) {
            // Optionally print a message if you want to be notified of purely blank lines being processed
            // cerr << "Debug: Skipping empty or whitespace-only line in deserialize." << endl;
            return Player(); // Return default player for empty/whitespace lines
        }

        istringstream iss(line);
        vector<string> parts;
        string token;

        while (getline(iss, token, '|'))
            parts.push_back(token);

        Player p; 
        if (parts.size() == 6)
        {
            try {
                p.username = parts[0];
                // Basic validation for username - should not be empty if it's a key part
                if (p.username.empty()) {
                    cerr << "Error deserializing player data: Username field is empty. Line: " << line << endl;
                    return Player(); 
                }
                p.passwordHash = stoull(parts[1]); 
                p.DOB = parts[2];
                p.gender = parts[3];
                p.totalGames = stoi(parts[4]);     
                p.maxScore = stoi(parts[5]);       
            } catch (const std::invalid_argument& ia) {
                cerr << "Error deserializing player data (invalid argument): " << ia.what() << " for line: " << line << endl;
                return Player(); 
            } catch (const std::out_of_range& oor) {
                cerr << "Error deserializing player data (out of range): " << oor.what() << " for line: " << line << endl;
                return Player(); 
            }
        }
        else { // This will catch lines that don't have exactly 6 parts
            cerr << "Error deserializing player data: Incorrect number of fields (found " << parts.size() << ", expected 6) in line: [" << line << "]" << endl;
            return Player(); 
        }
        return p;
    }
};