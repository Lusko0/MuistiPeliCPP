enum MemCardState
{
    Open,
    Closed,
    Shown
};

class MemCard
{
    public:
        char CardChar;
        enum MemCardState CardState;
};

//creates cards without loading save file
std::vector<MemCard> CreateCards()
{
    std::vector<MemCard> cards;
    std::vector<char> characters = {'T', 'T', '@', '@', '=', '=', '?', '?', '8', '8', 'W', 'W', 'X', 'X', 'O', 'O'};
    for (char character : characters)
    {
        MemCard card;
        card.CardChar = character;
        card.CardState = Closed;
        cards.push_back(card);
    }

    return cards;
}

//after load
std::vector<MemCard> CreateCards(std::vector<char> characters, std::vector<int> cardS)
{
    std::vector<MemCard> cards;
    for (int i = 0; i < characters.size(); i++)
    {
        MemCard card;
        card.CardChar = characters[i];
        switch(cardS[i])
        {
            case 1:
                card.CardState = Closed;
                break;
            case 2:
                card.CardState = Shown;
                break;
        }
        cards.push_back(card);
    }

    return cards;
}

//prints current status of the gameboard
void Printer(std::vector<MemCard> cardList)
{
    std::vector<char> row = {'A', 'B', 'C', 'D'};
    int i;
    int x = 4;
    int showCheck = 0;
    std::cout << "   1  2  3  4";
    for (MemCard card : cardList)
    {
        if (x % 4 == 0)
        {
            std::cout << "\n" << row[i] << "  ";
            i++;
        }
        if(card.CardState == Open || card.CardState == Shown)
        {
            std::cout << card.CardChar;
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

//shuffles and closes all cards on the board
std::vector<MemCard> Shuffler(std::vector<MemCard> items)
{
    srand(time(NULL));
    random_shuffle(items.begin(), items.end());
    return items;
}

//makes all the cards closed
std::vector<MemCard> Closer(std::vector<MemCard> cards)
{
    for (int i = 0; i < cards.size(); i++)
    {
        cards[i].CardState = Closed;
    }
    return cards;
}

//Gets selected coordinates and shows the character of the coordinate
int Selector(std::vector<MemCard> cards)
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
        if(cards[showSpot].CardState == Open || cards[showSpot].CardState == Shown)
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

//checks if the two opened characters are the same
std::vector<MemCard> Checker(std::vector<MemCard> cardList)
{
    int x = 0;
    int y = 0;
    bool stopX = false;
    for(MemCard card : cardList)
    {
        if(card.CardState == Open)
        {
            if(stopX == true)
            {
                break;
            }
            stopX = true;
        }
        else if(stopX == false)
        {
            x++;
        }
        y++;
    }
    //if the cards are the same changes them to shown if they aren't closes them
    if (cardList[x].CardChar == cardList[y].CardChar)
    {
        cardList[x].CardState = Shown;
        cardList[y].CardState = Shown;
        return cardList;
    }
    else
    {
        cardList[x].CardState = Closed;
        cardList[y].CardState = Closed;
        return cardList;
    }
}

//if one of the cards are not shown you haven't won
bool WinCheck(std::vector<MemCard> cardList)
{
    for(MemCard card : cardList)
    {
        if(card.CardState != Shown)
        {
            return false;
        }
    }
    return true;
}

//creates a vector of characters with the cards chars
std::vector<char> PrepSave(std::vector<MemCard> cardList)
{
    std::vector<char> charList;
    for (MemCard card : cardList)
    {
        charList.push_back(card.CardChar);
    }
    return charList;
}

//creates an integer vector for each card with 2 being shown and 1 being closed. could also be changed to boolean i guess
std::vector<int> PrepSave(std::vector<MemCard> cardList, int x)
{
    std::vector<int> intList;
    for (MemCard card : cardList)
    {
        switch(card.CardState)
        {
            case Shown:
                intList.push_back(2);
                break;
            default:
                intList.push_back(1);
                break;
        }
    }
    return intList;
}
