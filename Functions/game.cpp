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
            std::cout << "\n" << "Select full coordinate (A1-D4) or Q to quit: ";
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
                case 'q':
                    //Value would be something like q or quit if the variable wasn't an integer oopsie
                    return 99;
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
