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

using namespace std;

//Save/Load stuff probably subpar horrendously bad code
//Turns vectors into strings to save into a file
void Save(const string& filename, vector<char> charList, vector<int> showList){
    string board(charList.begin(), charList.end());
    stringstream sstr;
    copy(showList.begin(), showList.end(), ostream_iterator<int>(sstr, " "));
    string shownstr = sstr.str();
    ofstream file(filename);
    file << board << "\n" << shownstr;
    file.close();
}

//Loads file into a vector with two strings
vector<string> Load(const string& filename){
    ifstream file(filename);
    vector<string> temp;
    string line;
    while (getline(file, line))
    {
        temp.push_back(line);
    }
    file.close();

    return temp;
}

//Turns loaded strings back to vectors
vector<int> GetShown(vector<string> temp){
    stringstream ss(temp[1]);
    istream_iterator<string> begin(ss);
    istream_iterator<string> end;
    vector<string> vstrings(begin, end);

    vector<int> output;

    for (auto&& s : vstrings) {
        stringstream parser(s);
        int x = 0;

        parser >> x;

        output.push_back(x);
    }
    return output;
}

vector<char> GetBoard(vector<string> temp){
    vector<char> board(temp[0].begin(), temp[0].end());

    return board;
}

//checks if a file exists. used for checking if save file exists
bool FileExists(const string& filename){
    ifstream file(filename);
    return file.good();
}

//prints current status of the gameboard
void Printer(vector<char> charList, vector<int> showList)
{
    vector<char> row = {'A', 'B', 'C', 'D'};
    int i;
    int x = 4;
    int showCheck = 0;
    cout << "   1  2  3  4";
    for (char character : charList)
    {
        if (x % 4 == 0)
        {
            cout << "\n" << row[i] << "  ";
            i++;
        }
        if(find(showList.begin(), showList.end(), x-4) != showList.end())
        {
            cout << character;
        }
        else
        {
            cout << "#";
        }
        cout << "  ";
        showCheck++;
        x++;
    }
}

//shuffles the board
vector<char> Shuffler(vector<char> items)
{
    srand(time(NULL));
    random_shuffle(items.begin(), items.end());
    return items;
}

//Gets selected coordinates and shows the character of the coordinate
int Selector(vector<int> shown)
{
    while(true)
    {
        string coord;
        bool check = true;
        char inputColumn;
        int useColumn = 0;
        int rowNum = 0;
        int showSpot = 0;

        while (check)
        {
            cout << "\n" << "Select full coordinate (A1-D4): ";
            cin >> coord;
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
                    cout << "\nBad row";
                    break;
            }
        }
        //checks if last part of coordinate is a number
        if (isdigit(coord[1]))
        {
            useColumn = coord[1] - '0';
            if (useColumn > 4 || useColumn < 1)
            {
                cout << "\nBad column";
                continue;
            }
        }
        else
        {
            cout << "\nBad column";
            continue;
        }

        useColumn--;

        //adds both row start and column together to get the coordinate
        showSpot = rowNum + useColumn;
        //checks if the coordinate is already shown
        if(find(shown.begin(), shown.end(), showSpot) != shown.end())
        {
            cout << "\nSpot already open";
            continue;
        }
        else
        {
            return showSpot;
        }
    }
}

//checks if the two latest shown characters are the same
bool Checker(vector<char> charList, vector<int> showList)
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
    vector<char> tst = {'T', 'T', '@', '@', '=', '=', '?', '?', '8', '8', 'W', 'W', 'X', 'X', 'O', 'O'};
    vector<int> shown;
    string filename = "save.txt";
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
                cout << "Save detected. Do you want to continue previous session? (Y/N) ";
                cin >> useSave;
                if (tolower(useSave) == 'y')
                {
                    //loading save
                    vector<string> loadvec = Load(filename);
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
                    cout << "Invalid input!\n";
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
                cout << "\n";
                Printer(tst, shown);
                cout << "\n";
                selected++;
            }
            //checks the coordinates and if they are not the same removes the two newest coordinates
            if (Checker(tst, shown) == false)
            {
                shown.pop_back();
                shown.pop_back();
                cout << "\n";
                Printer(tst, shown);
                cout << "\n";
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
            cout << "You win! Do you want to play another round? (Y/N): ";
            cin >> again;
            if (tolower(again) == 'y' || tolower(again) == 'n')
            {
                continueCheck = true;
            }
            else
            {
                cout << "Invalid input!\n";
            }
        }

    }

    return 0;
}
