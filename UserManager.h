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
#include "Player.h"
using namespace std;

// Struct to hold system statistics
struct SystemStats
{
    int totalGamesPlayedByAll = 0;
    int highestFemaleScore = 0;
    int highestMaleScore = 0;
    int totalFemaleMembers = 0;
    int totalMaleMembers = 0;
    int totalOtherGenderMembers = 0; // For genders not strictly "male" or "female"
    int totalRegisteredUsers = 0;
};

class UserManager
{
private:
    map<string, Player> users;
    const string file = "users.txt";

    void load()
    {
        ifstream in(file);
        if (!in.is_open())
        {
            cerr << "Error: Could not open user data file for reading: " << file << endl;
            return; // Or handle more gracefully, e.g., allow program to run with no users
        }
        string line;
        while (getline(in, line))
        {
            Player p = Player::deserialize(line);
            // cout<<p.getUsername()<<endl;
            if (!p.getUsername().empty())
                users[p.getUsername()] = p;
        }
        // in.close(); // ifstream closes automatically when it goes out of scope
    }

public:
    UserManager()
    {
        // cout << GREEN << "Game Loading...." <<RESET<< endl;
        load();
    }

    // Method to get all player data (read-only access)
    const map<string, Player> &getAllPlayers() const
    {
        return users;
    }

    bool deletePlayer(const string &username)
    {
        auto it = users.find(username);
        if (it != users.end())
        {
            users.erase(it);
            save(); // Save changes to file
            return true;
        }
        return false; // User not found
    }

    SystemStats getSystemStatistics() const
    {
        SystemStats stats;
        stats.totalRegisteredUsers = users.size();

        for (const auto &pair : users)
        {
            const Player &player = pair.second;
            stats.totalGamesPlayedByAll += player.getTotalGames();

            string playerGender = player.getGender();
            std::transform(playerGender.begin(), playerGender.end(), playerGender.begin(), ::tolower);

            if (playerGender == "female" || playerGender == "Female")
            {
                stats.totalFemaleMembers++;
                if (player.getMaxScore() > stats.highestFemaleScore)
                {
                    stats.highestFemaleScore = player.getMaxScore();
                }
            }
            else if (playerGender == "male" || playerGender == "Male")
            {
                stats.totalMaleMembers++;
                if (player.getMaxScore() > stats.highestMaleScore)
                {
                    stats.highestMaleScore = player.getMaxScore();
                }
            }
            else
            {
                stats.totalOtherGenderMembers++;
            }
        }
        return stats;
    }

    Player *login(const string &u, const string &p)
    {
        auto it = users.find(u);
        if (it != users.end() && it->second.checkPassword(p))
            return &(it->second);
        return nullptr;
    }
    Player *signup(const string &u, const string &p, const string &d, const string &g)
    {
        if (users.count(u))
            return nullptr;
        Player player(u, p, d, g);
        users[u] = player;
        save();
        return &users[u];
    }
    void save()
    {
        ofstream out(file);
        if (!out.is_open())
        {
            cerr << "Error: Could not open user data file for writing: " << file << endl;
            return; // Failed to save
        }
        for (auto &kv : users)
            out << kv.second.serialize() << "\n";
        // out.close(); // ofstream closes automatically
    }
};