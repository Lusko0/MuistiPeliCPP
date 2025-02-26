//#include "load.h"

//Turns vectors into strings to save into a file
void Save(const std::string& filename, std::vector<char> charList, std::vector<int> showList){
    std::string board(charList.begin(), charList.end());
    std::stringstream sstr;
    std::copy(showList.begin(), showList.end(), std::ostream_iterator<int>(sstr, " "));
    std::string shownstr = sstr.str();
    std::ofstream file(filename);
    file << board << "\n" << shownstr;
    file.close();
}

//makes a vector of the files lines
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

//I don't get how this works but it does
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
