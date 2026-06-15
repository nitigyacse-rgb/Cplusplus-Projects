#include <iostream>
#include <string>
#include <stack>
#include <vector>

class SyntaxChecker
{
private:
    std::vector<std::string> sourceCode;

    // Helper function to check if braces, parentheses, and brackets match properly
    bool checkBalancedBrackets(const std::string &code)
    {
        std::stack<char> s;
        for (char ch : code)
        {
            if (ch == '(' || ch == '{' || ch == '[')
            {
                s.push(ch);
            }
            else if (ch == ')' || ch == '}' || ch == ']')
            {
                if (s.empty())
                    return false;

                char open = s.top();
                if ((ch == ')' && open == '(') ||
                    (ch == '}' && open == '{') ||
                    (ch == ']' && open == '['))
                {
                    s.pop();
                }
                else
                {
                    return false; // Mismatched bracket type
                }
            }
        }
        return s.empty();
    }

    // Helper function to verify simple semicolon rules for statements
    bool checkSemicolons()
    {
        for (size_t i = 0; i < sourceCode.size(); ++i)
        {
            std::string line = sourceCode[i];

            // Trim trailing spaces for accurate checking
            while (!line.empty() && (line.back() == ' ' || line.back() == '\t' || line.back() == '\r'))
            {
                line.pop_back();
            }

            if (line.empty())
                continue;

            // If a line contains common statement words like printf, cout, return, assignment etc.
            if (line.find("printf") != std::string::npos ||
                line.find("return") != std::string::npos ||
                line.find("cout") != std::string::npos ||
                (line.find('=') != std::string::npos && line.find("==") == std::string::npos))
            {

                // It shouldn't end with opening brace or be a function head, must have semicolon
                if (line.back() != ';' && line.back() != '{')
                {
                    std::cout << "[Error] Missing semicolon ';' at line " << (i + 1) << " -> \"" << line << "\"\n";
                    return false;
                }
            }
        }
        return true;
    }

public:
    // Function 1: Take input syntax
    void takeInputSyntax()
    {
        sourceCode.clear();
        std::cout << "\nEnter your C/C++ code snippet line by line.\n";
        std::cout << "Type 'END' on a new line when you are finished:\n";
        std::cout << "--------------------------------------------------\n";

        std::string line;
        while (true)
        {
            std::getline(std::cin, line);
            if (line == "END")
            {
                break;
            }
            sourceCode.push_back(line);
        }
    }

    // Function 2: Check if the syntax is correct or not
    void checkSyntax()
    {
        if (sourceCode.empty())
        {
            std::cout << "\n[Result] No code provided to check.\n";
            return;
        }

        // Combine all lines into a single string for parsing blocks/brackets
        std::string fullCode = "";
        for (const auto &line : sourceCode)
        {
            fullCode += line + "\n";
        }

        std::cout << "\n--- Syntax Analysis ---\n";

        // 1. Check bracket consistency
        if (!checkBalancedBrackets(fullCode))
        {
            std::cout << "[Error] Mismatched or unclosed brackets/braces '{}', '()', '[]'.\n";
            std::cout << "❌ Result: Incorrect Syntax\n";
            return;
        }

        // 2. Check basic statement terminations
        if (!checkSemicolons())
        {
            std::cout << "❌ Result: Incorrect Syntax\n";
            return;
        }

        // If all conditions clear
        std::cout << "✔ Result: Correct Syntax\n";
    }
};

int main()
{
    SyntaxChecker checker;
    int choice;

    do
    {
        std::cout << "\n====== SYNTAX CHECKER MENU ======\n";
        std::cout << "1. Take Input Syntax\n";
        std::cout << "2. Check Syntax\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore(); // Clear the buffer newline after reading integer

        switch (choice)
        {
        case 1:
            checker.takeInputSyntax();
            break;
        case 2:
            checker.checkSyntax();
            break;
        case 3:
            std::cout << "Exiting Syntax Checker. Goodbye!\n";
            break;
        default:
            std::cout << "Invalid selection! Please pick 1, 2, or 3.\n";
        }
    } while (choice != 3);

    return 0;
}