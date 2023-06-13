//Kelompok 4                             
//221402019 Sadtria Yudoyono Prasetio
//221402022 Janita Rakshanda Ingeyla
//221402038 Pelangi Sanrila Sinurat
//221402048 Khairunnisa 


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <filesystem>

using namespace std;

#ifdef _WIN32
const string EXECUTABLE_EXTENSION = ".exe";
#else
const string EXECUTABLE_EXTENSION = "";
#endif

namespace fs = std::filesystem;


enum TextColor
{
    BLACK = 0,
    RED = FOREGROUND_RED,
    GREEN = FOREGROUND_GREEN,
    YELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
    BLUE = FOREGROUND_BLUE,
    MAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
    CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
    WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
};


void setTextColor(TextColor color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}


void resetTextColor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, WHITE);
}


void createFile()
{
    string filename;
    cout << "Enter the filename: ";
    cin >> filename;

    ofstream file(filename);
    if (file)
    {
        setTextColor(GREEN);
        cout << "File created successfully." << endl;
        resetTextColor();
        file.close();
    }
    else
    {
        cout << "Error creating file." << endl;
    }
}


void inputFile()
{
    string filename;
    cout << "Enter the filename: ";
    cin >> filename;

    fstream file(filename, ios::in | ios::out | ios::app);
    if (file)
    {
        cout << "Enter the code (type 'exit' to stop): " << endl;
        string line;
        cin.ignore();
        while (getline(cin, line))
        {
            if (line == "exit")
            {
                break;
            }
            file << line << endl;
        }

        file.close();
    }
    else
    {
        cout << "Error opening file." << endl;
    }
}


void openFile(string& filename)
{
    ifstream file(filename);
    if (file)
    {
        setTextColor(GREEN);
        cout << "File opened successfully.\n" << endl;
        string line;
        resetTextColor();
        int lineCount = 1;

        while (getline(file, line))
        {
            cout << lineCount << "  ";

            
            stringstream ss(line);
            string token;
            vector<string> tokens;
            while (getline(ss, token, ' '))
            {
                tokens.push_back(token);
            }

            
            for (const string& token : tokens)
            {
                if (token == "int" || token == "float" || token == "string" || token == "#include" || token == "using" || token == "namespace" || token == "return")
                {
                    setTextColor(CYAN);
                }
                else if (token == "if" || token == "else" || token == "while" || token == "()")
                {
                    setTextColor(YELLOW);
                }
                else if (token == "{" || token == "}" || token == "(" || token == ")")
                {
                    setTextColor(YELLOW);
                }
                else if(token == "\" " || token == "<iostream>" || token == "<fstream>" || token == "<cstring>")
                {
                    setTextColor(RED);
                }
                else
                {
                    setTextColor(WHITE);
                }
                cout << token << " ";
            }

            resetTextColor();
            cout << endl;
            lineCount++;
        }

        file.close();
    }
    else
    {
        cout << "Error opening file." << endl;
    }
}


void addNewLine(const string& filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Failed to open file." << endl;
        return;
    }

    cout << "Enter the line number to add: ";
    int lineNumber;
    cin >> lineNumber;
    cin.ignore();

    cout << "Enter the text to add: ";
    string newText;
    getline(cin, newText);

    vector<string> lines;
    string line;
    while (getline(file, line))
    {
        lines.push_back(line);
    }

    if (lineNumber >= 1 && lineNumber <= lines.size() + 1)
    {
        lines.insert(lines.begin() + lineNumber - 1, newText);

        ofstream outfile(filename);
        for (const auto& line : lines)
        {
            outfile << line << endl;
        }

        setTextColor(GREEN);
        cout << "New line added successfully." << endl;
    }
    else
    {
        cout << "Invalid line number." << endl;
    }
}



void removeLine(const string& filename)
{
    ifstream file(filename);
    if (file)
    {
        vector<string> lines;
        string line;

        while (getline(file, line))
        {
            lines.push_back(line);
        }

        file.close();

        if (lines.empty())
        {
            cout << "File is empty." << endl;
            return;
        }

        int lineNumber;
        cout << "Enter the line number to remove: ";
        cin >> lineNumber;

        if (lineNumber < 1 || lineNumber > static_cast<int>(lines.size()))
        {
            cout << "Invalid line number." << endl;
            return;
        }

        lines.erase(lines.begin() + lineNumber - 1);

        ofstream outputFile(filename);
        if (outputFile)
        {
            for (const string& line : lines)
            {
                outputFile << line << endl;
            }

            setTextColor(GREEN);
            cout << "Line removed successfully." << endl;
            resetTextColor();

            outputFile.close();
        }
        else
        {
            cout << "Error opening file for writing." << endl;
        }
    }
    else
    {
        cout << "Error opening file." << endl;
    }
}



void editLine(fstream& file)
{
    cout << "Enter the line number to edit: ";
    int lineNumber;
    cin >> lineNumber;

    // Membaca seluruh konten file ke dalam string
    stringstream buffer;
    buffer << file.rdbuf();
    string fileContents = buffer.str();

    cout << "Enter the new text: ";
    cin.ignore();
    string newText;
    getline(cin, newText);

    string delimiter = "\n";
    size_t pos = 0;
    for (int i = 1; i < lineNumber; ++i)
    {
        pos = fileContents.find(delimiter, pos);
        if (pos == string::npos)
        {
            break;
        }
        // Memindahkan posisi setelah karakter '\n'
        pos += delimiter.length();
    }

    if (pos != string::npos)
    {
        // Mencari posisi akhir baris yang akan diedit
        size_t endPos = fileContents.find(delimiter, pos);
        if (endPos == string::npos)
        {
            endPos = fileContents.length();
        }
        // Mengganti teks pada posisi yang ditentukan
        fileContents.replace(pos, endPos - pos, newText);

        // Mengatur posisi penulisan pada awal file dan menulis ulang seluruh konten file
        file.seekp(0);
        file << fileContents;
        file.flush();
        setTextColor(GREEN);
        cout << "Line edited successfully." << endl;
        resetTextColor();
    }
    else
    {
        cout << "Invalid line number." << endl;
    }
}


void editPerWords(fstream& file)
{
    string searchText;
    cout << "Enter the text to search: ";
    cin.ignore();
    getline(cin, searchText);

    string newText;
    cout << "Enter the new text: ";
    getline(cin, newText);

    string line;
    string fileContents;
    while (getline(file, line))
    {
        // Mencari teks yang ingin diganti dan menggantinya dengan teks baru
        size_t foundIndex = line.find(searchText);
        if (foundIndex != string::npos)
        {
            line.replace(foundIndex, searchText.length(), newText);
        }
        fileContents += line + "\n";
    }

    // Mengatur posisi file ke awal
    file.clear();
    file.seekg(0);

    // Menulis ulang seluruh konten file
    file << fileContents;
    file.flush();

    setTextColor(GREEN);
    cout << "Text edited successfully." << endl;
    resetTextColor();
}


void editFile()
{
    string filename;
    cout << "Enter the filename: ";
    cin >> filename;

    openFile(filename);
    cin.ignore();

    
    fstream file(filename, ios::in | ios::out);
    if (file)
    {
        int choice;
        do
        {
            cout << endl;
            setTextColor(RED);
            cout << "------ Menu ------" << endl;
            setTextColor(YELLOW);
            cout << "1. Add new line" << endl;
            cout << "2. Remove line" << endl;
            cout << "3. Edit line" << endl;
            cout << "4. Edit word" << endl;
            cout << "5. Open Updated File" << endl;
            cout << "6. Back to main menu" << endl;
            setTextColor(RED);
            cout << "------------------" << endl;
            resetTextColor();
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice)
            {
                case 1:
                    addNewLine(filename);
                    break;
                case 2:
                    removeLine(filename);
                    break;
                case 3:
                    editLine(file);
                    break;
                case 4:
                    editPerWords(file);
                    break;
                case 5:
                    openFile(filename);
                    break;
                case 6:
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }



        }
        while (choice != 6);

        file.close();
    }
    else
    {
        cout << "Error opening file." << endl;
    }
}



void compileFile()
{
    string filename;
    cout << "Enter the filename: ";
    cin >> filename;

    // Memeriksa ekstensi file
    string extension = fs::path(filename).extension().string();
    transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    if (extension == ".cpp")
    {
        string command = "g++ " + filename + " -o output" + EXECUTABLE_EXTENSION;
        int compileResult = system(command.c_str());
        if (compileResult == 0)
        {
            setTextColor(GREEN);
            cout << "Compilation successful.\n" << endl;
            resetTextColor();

            string runCommand = "output" + EXECUTABLE_EXTENSION;
            int runResult = system(runCommand.c_str());
            if (runResult == 0)
            {
                setTextColor(GREEN);
                cout << "\nProgram executed successfully." << endl;
            }
            else
            {
                cout << "Error executing program." << endl;
            }
        }
        else
        {
            cout << "Error compiling file." << endl;
        }
    }
    else if (extension == ".c")
    {
        string command = "gcc " + filename + " -o output" + EXECUTABLE_EXTENSION;
        int compileResult = system(command.c_str());
        if (compileResult == 0)
        {
            setTextColor(GREEN);
            cout << "Compilation successful.\n" << endl;
            resetTextColor();

            string runCommand = "output" + EXECUTABLE_EXTENSION;
            int runResult = system(runCommand.c_str());
            if (runResult == 0)
            {
                setTextColor(GREEN);
                cout << "\nProgram executed successfully." << endl;
            }
            else
            {
                cout << "Error executing program." << endl;
            }
        }
        else
        {
            cout << "Error compiling file." << endl;
        }
    }
    else if (extension == ".py")
    {
        string command = "python " + filename;
        int runResult = system(command.c_str());
        if (runResult == 0)
        {
            setTextColor(GREEN);
            cout << "\nProgram executed successfully." << endl;
        }
        else
        {
            cout << "Error executing program." << endl;
        }
    }
    else
    {
        cout << "Unsupported file type." << endl;
    }
}



void countinFile()
{
    string filename;
    cout << "Enter the filename: ";
    cin >> filename;

    ifstream file(filename);
    if (file)
    {
        string line;
        int wordCount = 0;
        int sentenceCount = 0;
        int lineCount = 0;
        int letterCount = 0;

        while (getline(file, line))
        {
            stringstream ss(line);
            string word;

            bool lineHasText = false;  
            bool lineHasWord = false;  

            while (ss >> word)
            {
                wordCount++;
                letterCount += word.length();
                lineHasText = true;
                lineHasWord = true;
            }

            if (lineHasWord)
            {
                lineCount++;
                sentenceCount++;
                letterCount++; 
            }
            else if (line.empty() && !lineHasText)
            {
                lineCount++;  
            }
        }

        setTextColor(GREEN);
        cout << "Total words: " << wordCount << endl;
        cout << "Total sentences: " << sentenceCount << endl;
        cout << "Total lines: " << lineCount << endl;
        cout << "Total letters: " << letterCount << endl;
        resetTextColor();
        file.close();
    }
    else
    {
        cout << "Error opening file." << endl;
    }
}


void findText()
{
    string filename;
    cout << "Enter the filename: ";
    cin >> filename;

    string searchText;
    cout << "Enter the text to search: ";
    cin.ignore();
    getline(cin, searchText);

    ifstream file(filename);
    if (file)
    {
        string line;
        int lineNumber = 1;
        bool found = false;

        while (getline(file, line))
        {
            size_t foundIndex = line.find(searchText);
            if (foundIndex != string::npos)
            {
                setTextColor(GREEN);
                cout << "Found at line " << lineNumber << ": " << line << endl;
                found = true;
                resetTextColor();
            }
            lineNumber++;
        }

        if (!found)
        {
            cout << "Text not found." << endl;
        }

        file.close();
    }
    else
    {
        cout << "Error opening file." << endl;
    }
}



void deleteFile()
{
    string filename;
    cout << "Enter the filename: ";
    cin >> filename;

    if (fs::exists(filename))
    {
        fs::remove(filename);
        cout << "File deleted successfully." << endl;
    }
    else
    {
        cout << "File not found." << endl;
    }
}


int main()
{
    int choice;
    string filename;

    do
    {
        setTextColor(RED);
        cout << "==================== TEKS EDITOR ======================\n" <<endl;
        setTextColor(CYAN);
        cout << "Menu:" << endl;
        setTextColor(YELLOW);
        cout << "1. Create file" << endl;
        
        cout << "2. Input file" << endl;
        
        cout << "3. Open file" << endl;
        
        cout << "4. Edit file" << endl;
        
        cout << "5. Compile, run, and show output" << endl;
        
        cout << "6. Count the total words, sentences, lines, and letters" << endl;
        
        cout << "7. Find text or sentence" << endl;
        
        cout << "8. Delete file" << endl;
        
        cout << "9. Exit" << endl;
        resetTextColor();
        cout << "Enter your choice: ";
        cin >> choice;
        resetTextColor();
        setTextColor(RED);
        cout << "----------------------------------------------------------" << endl;
        resetTextColor();
        cout << endl;

        switch (choice)
        {
            case 1:
                createFile();
                break;
            case 2:
                inputFile();
                break;
            case 3:
            cout << "Enter the filename: "; 
                cin >> filename; 
                openFile(filename);
                break;
            case 4:
                editFile();
                break;
            case 5:
                compileFile();
                break;
            case 6:
                countinFile();
                break;
            case 7:
                findText();
                break;
            case 8:
                deleteFile();
                break;
            case 9:
                setTextColor(GREEN);
                cout << "Exiting program." << endl;
                resetTextColor();
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    
    }
    while (choice != 9);

    return 0;
}