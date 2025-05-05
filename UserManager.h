#include<bits/stdc++.h>
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

class UserManager
{
private:
    map<string, Player> users;
    const string file = "users.txt";

    void load()
    {
        ifstream in(file);
        string line;
        while (getline(in, line))
        {
            Player p = Player::deserialize(line);
            if (!p.getUsername().empty())
                users[p.getUsername()] = p;
        }
    }

public:
    UserManager() { load(); }

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
        for (auto &kv : users)
            out << kv.second.serialize() << "\n";
    }
};