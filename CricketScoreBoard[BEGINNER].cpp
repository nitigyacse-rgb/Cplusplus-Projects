#include <iostream>
#include <string>
#include <iomanip>

class CricketScoreBoard
{
private:
    // Match State Variables
    int totalRuns;
    int wicketsLost;
    double oversBowled;
    int remainingBalls;
    int targetRuns;

    // Batsmen Data
    int strikerRuns;
    int nonStrikerRuns;

    // Last Wicket Info
    int lastWicketScore;
    std::string howOut;

    // Bowler Info
    int currentBowlerNo;
    int bowlerScore;

public:
    // Constructor to initialize default match values
    CricketScoreBoard()
    {
        totalRuns = 396;
        wicketsLost = 7;
        oversBowled = 132.2;
        remainingBalls = 60;
        targetRuns = 450; // Dynamic target to calculate runs to win

        strikerRuns = 8;
        nonStrikerRuns = 13;

        lastWicketScore = 419;
        howOut = "LBW";

        currentBowlerNo = 12;
        bowlerScore = 5;
    }

    // Function 1: Print Match Statistics (Formatted Scoreboard Layout)
    void printMatchStatistics()
    {
        std::cout << "\n============================================\n";
        std::cout << "            CRICKET SCORE BOARD             \n";
        std::cout << "============================================\n";
        std::cout << " BATS (Striker): " << strikerRuns << "          TOTAL      BATS (Non-Striker): " << nonStrikerRuns << "\n";
        std::cout << "                     " << totalRuns << "\n";
        std::cout << "                   FOR " << wicketsLost << " WKTS\n\n";

        std::cout << " RUNS TO WIN    LAST WKT.      OVERS      REMAIN\n";
        std::cout << "    " << std::setw(3) << (targetRuns - totalRuns)
                  << "           " << std::setw(3) << lastWicketScore
                  << "         " << std::setw(5) << oversBowled
                  << "        " << std::setw(2) << remainingBalls << "\n\n";

        std::cout << " HOW OUT        BWLR. NO.      SCORE\n";
        std::cout << "  " << std::setw(4) << howOut
                  << "            " << std::setw(2) << currentBowlerNo
                  << "            " << std::setw(2) << bowlerScore << "\n";
        std::cout << "============================================\n";
    }

    // Function 2: Print runs scored
    void printRunsScored()
    {
        std::cout << "\n--- CURRENT RUNS SUMMARY ---\n";
        std::cout << "Total Score: " << totalRuns << "/" << wicketsLost << "\n";
        std::cout << "Striker Batsman: " << strikerRuns << " runs\n";
        std::cout << "Non-Striker Batsman: " << nonStrikerRuns << " runs\n";
        std::cout << "----------------------------\n";
    }

    // Function 3: Update score
    void updateScore()
    {
        int runs;
        char choice;
        std::cout << "\n--- UPDATE SCORE ---\n";
        std::cout << "Enter runs scored on this ball (0-6): ";
        std::cin >> runs;

        totalRuns += runs;
        strikerRuns += runs; // Assuming striker takes the runs for simplicity

        std::cout << "Was it a wicket? (y/n): ";
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y')
        {
            wicketsLost++;
            lastWicketScore = totalRuns;
            std::cout << "Enter mode of dismissal (e.g., LBW, Bowled, Caught): ";
            std::cin >> howOut;
            strikerRuns = 0; // Reset striker runs for incoming batsman
        }

        // Update ball progression
        remainingBalls--;
        // Standard cricket over progression math
        int currentBalls = static_cast<int>((oversBowled - static_cast<int>(oversBowled)) * 10);
        currentBalls++;
        if (currentBalls >= 6)
        {
            oversBowled = static_cast<int>(oversBowled) + 1;
        }
        else
        {
            oversBowled = static_cast<int>(oversBowled) + (currentBalls / 10.0);
        }

        std::cout << "Score updated successfully!\n";
    }

    // Function 4: Show results
    void showResults()
    {
        std::cout << "\n--- MATCH RESULT STATUS ---\n";
        if (totalRuns >= targetRuns)
        {
            std::cout << "Batting team won the match!\n";
        }
        else if (remainingBalls <= 0 || wicketsLost >= 10)
        {
            std::cout << "Bowling team won by " << (targetRuns - totalRuns) << " runs.\n";
        }
        else
        {
            std::cout << "Match is currently IN PROGRESS.\n";
            std::cout << "Required " << (targetRuns - totalRuns) << " runs from " << remainingBalls << " balls.\n";
        }
        std::cout << "---------------------------\n";
    }
};

int main()
{
    CricketScoreBoard game;
    int choice;

    do
    {
        std::cout << "\n====== CRICKET SYSTEM MENU ======\n";
        std::cout << "1. Print Match Statistics\n";
        std::cout << "2. Print Runs Scored\n";
        std::cout << "3. Update Score\n";
        std::cout << "4. Show Results\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice (1-5): ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            game.printMatchStatistics();
            break;
        case 2:
            game.printRunsScored();
            break;
        case 3:
            game.updateScore();
            break;
        case 4:
            game.showResults();
            break;
        case 5:
            std::cout << "Exiting Scoreboard Application. Goodbye!\n";
            break;
        default:
            std::cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}