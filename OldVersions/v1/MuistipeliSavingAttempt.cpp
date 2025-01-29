#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iterator>

using namespace std;

//Copied off the internet could not get this to work
class Serializable {
private:
    string board;
    string shown;

public:
    Serializable(){};
    // Constructor to initialize the data members
    Serializable(const string& board, string shown)
        : board(board)
        , shown(shown)
    {
    }

    // Getter methods for the class
    vector<char> getBoard() const
    {
        vector<char> returnvec(board.begin(), board.end());
        return returnvec;
    }
    vector<int> getShown() const
    {
        stringstream ss(shown);
        istream_iterator<string> begin(ss);
        istream_iterator<string> end;
        vector<string> vstrings(begin, end);

        vector<string> input = {"50", "60", "2"};
        vector<int> output;

        for (auto&& s : vstrings) {
            stringstream parser(s);
            int x = 0;

            parser >> x;

            output.push_back(x);
        }
        return output;
    }

    //  Function for Serialization
    void save(const string& filename)
    {
        ofstream file(filename);
        file << board << "\n" << shown;
        file.close();
    }

    //  Function for Deserialization
    static Serializable load(const string& filename)
    {
        ifstream file(filename);
        string x, y;
        string line;
        while (getline(file, line))
        {
            if (x == "")
            {
                x = line;
            }
            else
            {
                y = line;
            }

        }
        file.close();
        Serializable obj(x, y);
        return obj;
    }
};

bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

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

vector<char> Shuffler(vector<char> items)
{
    srand(time(NULL));
    random_shuffle(items.begin(), items.end());
    return items;
}

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

        showSpot = rowNum + useColumn;
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
    vector<int> test = {0,1,2,3};
    char useSave;
    while (true)
    {
        char again;
        //tst = Shuffler(tst);
        shown = {};
        if (fileExists("data.bin"))
        {
            cout << "Save detected. Do you want to continue previous session? (Y/N) ";
            cin >> useSave;
            if (tolower(useSave) == 'y')
            {
                Serializable restored() = Serializable::load("data.bin");
                Printer(restored.getBoard(), test);
                tst = restored.getBoard();
                shown = restored.getShown();
            }
            else if (tolower(useSave) == 'n')
            {
                remove("data.bin");
            }
        }
        int y = 0;
        while (true)
        {
            int show;
            if (y == 0)
            {
                Printer(tst, shown);
                y++;
            }
            int selected = 0;
            while (selected < 2)
            {
                show = Selector(shown);
                shown.push_back(show);
                cout << "\n";
                Printer(tst, shown);
                cout << "\n";
                selected++;
            }
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
                string str(tst.begin(), tst.end());
                stringstream sstr;
                copy(shown.begin(), shown.end(), ostream_iterator<int>(sstr, " "));
                string shownstr = sstr.str();
                Serializable original(str, shownstr);
                original.save("data.bin");
            }
            if (shown.size() == tst.size())
            {
                break;
            }
        }
        cout << "You win! Do you want to play another round? (Y/N): ";
        remove("data.bin");
        cin >> again;
        if (tolower(again) == 'y')
        {
            continue;
        }
        else if (tolower(again) == 'n')
        {
            break;
        }
    }

    return 0;
}
