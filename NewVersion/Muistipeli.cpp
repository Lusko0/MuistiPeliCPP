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
//atleast need most of these
//#include "Functions/load.h"
//Couldn't get header file to work gave up
#include "Functions/load.cpp"
#include "Functions/game.cpp"

enum MemCardState;

class MemCard;

//Save/Load files called from Functions/load.cpp
void Save(const std::string& filename, std::vector<char> charList, std::vector<int> showList);
std::vector<std::string> Load(const std::string& filename);
std::vector<int> GetShown(std::vector<std::string> temp);
std::vector<char> GetBoard(std::vector<std::string> temp);
bool FileExists(const std::string& filename);

//Files that make the game work called from Functions/game.cpp
std::vector<MemCard> CreateCards();
std::vector<MemCard> CreateCards(std::vector<char> characters, std::vector<int> cardS);
void Printer(std::vector<MemCard> cardList);
std::vector<MemCard> Shuffler(std::vector<MemCard> items);
std::vector<MemCard> Closer(std::vector<MemCard> cards);
int Selector(std::vector<MemCard> cards);
std::vector<MemCard> Checker(std::vector<MemCard> cardList);
std::vector<char> PrepSave(std::vector<MemCard> cardList);
std::vector<int> PrepSave(std::vector<MemCard> cardList, int x);

int main() {
    std::vector<MemCard> gameBoard;
    std::vector<char> saveChars;
    std::vector<int> saveInts;
    gameBoard = CreateCards();
    std::vector<char> boardLoad;
    std::vector<int> shown;
    std::string filename = "./saves/save.txt";
    std::string useSave;
    char again = 'y';
    while (tolower(again) == 'y')
    {
        bool continueCheck = false;
        gameBoard = Closer(gameBoard);
        gameBoard = Shuffler(gameBoard);
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
                    boardLoad = GetBoard(loadvec);
                    shown = GetShown(loadvec);
                    gameBoard = CreateCards(boardLoad, shown);
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
                Printer(gameBoard);
                y++;
            }
            int selected = 0;
            //if the amount of selected coordinates is less than two this makes you pick a new coordinate
            while (selected < 2)
            {
                show = Selector(gameBoard);
                //Only gets value 99 if player wants to quit
                if(show == 99){
                    /*if player chooses to quit and has at least one shown card
                    makes a directory for saves and saves the game in said directory*/
                    for(MemCard card : gameBoard)
                    {
                        if(card.CardState == Shown)
                        {
                            saveChars = PrepSave(gameBoard);
                            saveInts = PrepSave(gameBoard, 0);
                            _mkdir("./saves");
                            Save(filename, saveChars, saveInts);
                            break;
                        }
                    }
                    return 0;
                }
                //opens selected card
                gameBoard[show].CardState = Open;
                std::cout << "\n";
                Printer(gameBoard);
                std::cout << "\n";
                selected++;
            }
            //checks the coordinates and if they are not the same removes the two newest coordinates
            gameBoard = Checker(gameBoard);
            std::cout << "\n";
            Printer(gameBoard);
            std::cout << "\n";
            //if all of the cards are shown win
            if (WinCheck(gameBoard))
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
