#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iterator>
//atleast need most of these

//Save/Load stuff probably subpar horrendously bad code
//Turns vectors into strings to save into a file
void Save(const std::string& filename, std::vector<char> charList, std::vector<int> showList){
    std::string board(charList.begin(), charList.end());
    std::stringstream sstr;
    copy(showList.begin(), showList.end(), std::ostream_iterator<int>(sstr, " "));
    std::string shownstr = sstr.str();
    std::ofstream file(filename);
    file << board << "\n" << shownstr;
    file.close();
}

//Loads file into a vector with two strings
std::vector<std::string> Load(const std::string& filename){
    std::ifstream file(filename);
    std::vector<std::string> temp;
    std::string line;
    while (getline(file, line))
    {
        temp.push_back(line);
    }
    file.close();

    return temp;
}

//Turns loaded strings back to vectors
std::vector<int> GetShown(std::vector<std::string> temp){
    std::stringstream ss(temp[1]);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> vstrings(begin, end);

    std::vector<int> output;

    for (auto&& s : vstrings) {
        std::stringstream parser(s);
        int x = 0;

        parser >> x;

        output.push_back(x);
    }
    return output;
}

std::vector<char> GetBoard(std::vector<std::string> temp){
    std::vector<char> board(temp[0].begin(), temp[0].end());

    return board;
}

//checks if a file exists. used for checking if save file exists
bool FileExists(const std::string& filename){
    std::ifstream file(filename);
    return file.good();
}

//prints current status of the gameboard
void Printer(std::vector<char> charList, std::vector<int> showList)
{
    std::vector<char> row = {'A', 'B', 'C', 'D'};
    int i;
    int x = 4;
    int showCheck = 0;
    std::cout << "   1  2  3  4";
    for (char character : charList)
    {
        if (x % 4 == 0)
        {
            std::cout << "\n" << row[i] << "  ";
            i++;
        }
        if(find(showList.begin(), showList.end(), x-4) != showList.end())
        {
            std::cout << character;
        }
        else
        {
            std::cout << "#";
        }
        std::cout << "  ";
        showCheck++;
        x++;
    }
}

//shuffles the board
std::vector<char> Shuffler(std::vector<char> items)
{
    srand(time(NULL));
    random_shuffle(items.begin(), items.end());
    return items;
}

//Gets selected coordinates and shows the character of the coordinate
int Selector(std::vector<int> shown)
{
    while(true)
    {
        std::string coord;
        bool check = true;
        char inputColumn;
        int useColumn = 0;
        int rowNum = 0;
        int showSpot = 0;

        while (check)
        {
            std::cout << "\n" << "Select full coordinate (A1-D4): ";
            std::cin >> coord;
            //makes rowNum the start of the chosen row
            switch (tolower(coord[0]))
            {
                case 'a':
                    rowNum = 0;
                    check = false;
                    break;
                case 'b':
                    rowNum = 4;
                    check = false;
                    break;
                case 'c':
                    rowNum = 8;
                    check = false;
                    break;
                case 'd':
                    rowNum = 12;
                    check = false;
                    break;
                default:
                    std::cout << "\nBad row";
                    break;
            }
        }
        //checks if last part of coordinate is a number
        if (isdigit(coord[1]))
        {
            useColumn = coord[1] - '0';
            if (useColumn > 4 || useColumn < 1)
            {
                std::cout << "\nBad column";
                continue;
            }
        }
        else
        {
            std::cout << "\nBad column";
            continue;
        }

        useColumn--;

        //adds both row start and column together to get the coordinate
        showSpot = rowNum + useColumn;
        //checks if the coordinate is already shown
        if(find(shown.begin(), shown.end(), showSpot) != shown.end())
        {
            std::cout << "\nSpot already open";
            continue;
        }
        else
        {
            return showSpot;
        }
    }
}

//checks if the two latest shown characters are the same
bool Checker(std::vector<char> charList, std::vector<int> showList)
{
    int x = showList[showList.size() - 1];
    int y = showList[showList.size() - 2];
    if (charList[x] == charList[y])
    {
        return true;
    }
    return false;
}

int main() {
    std::vector<char> tst = {'T', 'T', '@', '@', '=', '=', '?', '?', '8', '8', 'W', 'W', 'X', 'X', 'O', 'O'};
    std::vector<int> shown;
    std::string filename = "save.txt";
    char useSave;
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
                if (tolower(useSave) == 'y')
                {
                    //loading save
                    std::vector<std::string> loadvec = Load(filename);
                    tst = GetBoard(loadvec);
                    shown = GetShown(loadvec);
                    saveCheck = true;
                }
                else if (tolower(useSave) == 'n')
                {
                    //does not work with string
                    remove("save.txt");
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
                //if they are the same saves the game
                Save(filename, tst, shown);
            }
            //if the amount of shown spaces are equal to the boards spaces the game ends
            if (shown.size() == tst.size())
            {
                break;
            }
        }
        remove("save.txt");
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
