// #pragma once // Removed - not needed in .cpp file
#include "WordleGame.h"
#include "UserManager.h"
#include "Application.h"

using namespace std;

#define BG_CYAN "\033[46m"

int main()
{
    Application app;
    app.run();
    return 0;
}
