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
    int totalscore;

public:
    Player() : User(), totalGames(0), maxScore(0), totalscore(0) {}
    Player(const string &uname, const string &pwd, const string &dob, const string &gen)
        : User(uname, pwd, dob, gen), totalGames(0), maxScore(0), totalscore(0) {}

    void updateScore(int score)
    {
        totalscore += score;
        if (totalscore > maxScore)
            maxScore = totalscore;
    }

    int gettotscore(){
        return totalscore;
    }

    int setscore(int sco){
        totalscore += sco;
    }

    void incrementGames() { totalGames++; }

    void showStats() const
    {
        cout << BOLD << BLUE << "\n=== Player Stats ===" << RESET << endl;
        cout << "Total Games Played: " << totalGames << endl;
        cout << "Highest Score: " << maxScore << endl;
        cout << "Current Total Score: " << totalscore << endl;
    }

    // Serialization for file storage
    string serialize() const
    {
        ostringstream oss;
        oss << username << "|" << passwordHash << "|" << DOB << "|" << gender
            << "|" << totalGames << "|" << maxScore;
        return oss.str();
    }

    // Deserialize from file line
    static Player deserialize(const string &line)
    {
        istringstream iss(line);
        vector<string> parts;
        string token;

        while (getline(iss, token, '|'))
            parts.push_back(token);

        Player p;
        if (parts.size() == 6)
        {
            p.username = parts[0];
            p.passwordHash = stoull(parts[1]);
            p.DOB = parts[2];
            p.gender = parts[3];
            p.totalGames = stoi(parts[4]);
            p.maxScore = stoi(parts[5]);
        }
        return p;
    }
};