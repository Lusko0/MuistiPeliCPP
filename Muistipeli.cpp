#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iterator>
#include <direct.h>
//#include "Functions/load.h"
//Couldn't get header file to work gave up
#include "Functions/load.cpp"
#include "Functions/game.cpp"
//atleast need most of these


//Save/Load files called from Functions/load.cpp
void Save(const std::string& filename, std::vector<char> charList, std::vector<int> showList);
std::vector<std::string> Load(const std::string& filename);
std::vector<int> GetShown(std::vector<std::string> temp);
std::vector<char> GetBoard(std::vector<std::string> temp);
bool FileExists(const std::string& filename);

//Files that make the game work called from Functions/game.cpp
void Printer(std::vector<char> charList, std::vector<int> showList);
std::vector<char> Shuffler(std::vector<char> items);
int Selector(std::vector<int> shown);
bool Checker(std::vector<char> charList, std::vector<int> showList);

int main() {
    std::vector<char> tst = {'T', 'T', '@', '@', '=', '=', '?', '?', '8', '8', 'W', 'W', 'X', 'X', 'O', 'O'};
    std::vector<int> shown;
    std::string filename = "./saves/save.txt";
    std::string useSave;
    char again = 'y';
    while (tolower(again) == 'y')
    {
        bool continueCheck = false;
        tst = Shuffler(tst);
        shown = {};
        //check for save
        if (FileExists(filename))
        {
            bool saveCheck = false;
            while (!saveCheck)
            {
                std::cout << "Save detected. Do you want to continue previous session? (Y/N) ";
                std::cin >> useSave;
                if (tolower(useSave[0]) == 'y')
                {
                    //loading save
                    std::vector<std::string> loadvec = Load(filename);
                    tst = GetBoard(loadvec);
                    shown = GetShown(loadvec);
                    saveCheck = true;
                }
                else if (tolower(useSave[0]) == 'n')
                {
                    //does not work with string
                    remove("./saves/save.txt");
                    saveCheck = true;
                }
                else
                {
                    std::cout << "Invalid input!\n";
                }
            }

        }
        int y = 0;
        //the loop for the game
        while (true)
        {
            int show;
            if (y == 0)
            {
                Printer(tst, shown);
                y++;
            }
            int selected = 0;
            //if the amount of selected coordinates is less than two this makes you pick a new coordinate
            while (selected < 2)
            {
                show = Selector(shown);
                //Only gets value 99 if player wants to quit
                if(show == 99){
                    return 0;
                }
                shown.push_back(show);
                std::cout << "\n";
                Printer(tst, shown);
                std::cout << "\n";
                selected++;
            }
            //checks the coordinates and if they are not the same removes the two newest coordinates
            if (Checker(tst, shown) == false)
            {
                shown.pop_back();
                shown.pop_back();
                std::cout << "\n";
                Printer(tst, shown);
                std::cout << "\n";
            }
            else
            {
                //if they are the same automatically makes a directory for saves and saves the game in said directory
                _mkdir("./saves");
                Save(filename, tst, shown);
            }
            //if the amount of shown spaces are equal to the boards spaces the game ends
            if (shown.size() == tst.size())
            {
                break;
            }
        }
        remove("./saves/save.txt");
        while (!continueCheck)
        {
            std::cout << "You win! Do you want to play another round? (Y/N): ";
            std::cin >> again;
            if (tolower(again) == 'y' || tolower(again) == 'n')
            {
                continueCheck = true;
            }
            else
            {
                std::cout << "Invalid input!\n";
            }
        }

    }

    return 0;
}
