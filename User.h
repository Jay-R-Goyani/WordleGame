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
using namespace std;

class User
{
protected:
    string username;
    size_t passwordHash;
    string DOB;
    string gender;

public:
    User() {}
    User(const string &uname, const string &pwd, const string &dob, const string &gen)
        : username(uname), passwordHash(hash<string>()(pwd)), DOB(dob), gender(gen) {}

    string getUsername() const { return username; }
    bool checkPassword(const string &pwd) const
    {
        return hash<string>()(pwd) == passwordHash;
    }
};