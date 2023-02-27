#include "GinRummyConsole.h"

int GinRummyConsole::LineLength = 80;

GinRummyConsole::GinRummyConsole()
{
}

//////////////////////////////////////////////////////////////////////
/// Repeatedly makes calls to draw the game and to request user input
/// until the user elects to quit the game
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
void GinRummyConsole::Play()
{
    std::string StatusMessage = "Welcome! Good Luck.";
    while(true)
    {
        DrawGame(StatusMessage);
        StatusMessage = UserInput();
    }
}

//////////////////////////////////////////////////////////////////////
/// Prints the current state of the game
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
void GinRummyConsole::DrawGame(const std::string& StatusMessage)
{
    static const std::string border(LineLength, 'x');

    std::cout << border << std::endl; //1
    std::cout << std::endl; //2

    std::string LineLeft; //start to build line 3
    LineLeft = "YOU (" + std::to_string(ginRummy.getPlayerScore()) + " points)";
    if(ginRummy.getPlayerTurn())
        LineLeft += " - your turn";
    std::string LineRight;
    if(!ginRummy.getPlayerTurn())
        LineRight += "your turn - ";
    LineRight += "(" + std::to_string(ginRummy.getComputerScore()) + " points) COMPUTER";
    PrintLine(LineLeft, LineRight); //3

    std::cout << std::endl; //4

    for(int idx = 0; idx < ginRummy.getPlayerHand().size(); ++idx) //5-14, maybe 15
    {
        std::string Left = std::to_string(idx+1) + ". " + ginRummy.getPlayerHand().at(idx).CardToString();
        std::string Right;
        std::string Middle;
        if(idx < ginRummy.getComputerHand().size())
        {
            if(ginRummy.getShowingComputerHand())
                Right = std::to_string(idx+1) + ". " + ginRummy.getComputerHand().at(idx).CardToString();
            else
                Right = "Card " + std::to_string(idx+1);
        }
        if(idx == 1)
            Middle = "DISCARD PILE";
        else if(idx == 2)
            Middle = ginRummy.getDiscardDeckSize() != 0 ? ginRummy.getDiscardBack().CardToString() : "";
        else if(idx == 5)
            Middle = "(F) - Take Face Down (" + std::to_string(ginRummy.getFaceDownDeckSize()) + ")";
        else if(idx == 6)
            Middle = "(D) - Take Discard (" + std::to_string(ginRummy.getDiscardDeckSize()) + ")";
        PrintLine(Left, Middle, Right);
    }

    if(ginRummy.getPlayerHand().size() == 10) //15
        std::cout << std::endl;

    std::cout << std::endl; //16

    PrintLine("(R) - Sort for Runs", "(S) - Show Hand"); //17
    PrintLine("(P) - Sort for Sets", "(H) - Hide Hand"); //18
    std::cout << std::endl; //19
    PrintLine("(Dn) - Discard card #n", "(K) - Knock"); //20
    PrintLine("(A) - Pass", "(G) - Gin"); //21
    PrintLine("(C) - Computer Play", "(Q) - Quit"); //22
    std::cout << border << std::endl; //23
    std::cout << "**" << StatusMessage << "** Enter Command: "; //24
}

//////////////////////////////////////////////////////////////////////
/// Accepts input from the user and responds accordingly
///
/// Returns: std::string (status message from turns)
//////////////////////////////////////////////////////////////////////
std::string GinRummyConsole::UserInput()
{
    std::string Input;
    std::cin >> Input;

    std::transform(Input.begin(), Input.end(), Input.begin(), ::toupper);

    std::string StatusMessage;

    if(Input == "R")
    {
        StatusMessage = ginRummy.SortByRuns();
    }
    else if(Input == "P")
    {
        StatusMessage = ginRummy.SortBySets();
    }
    else if(Input == "S")
    {
        StatusMessage = ginRummy.ShowComputerHand();
    }
    else if(Input == "H")
    {
        StatusMessage = ginRummy.HideComputerHand();
    }
    else if(Input == "A")
    {
        StatusMessage = ginRummy.Pass();
    }
    else if(Input == "Q")
    {
        exit(0);
    }
    else if(Input == "D")
    {
        StatusMessage = ginRummy.ChooseDiscard();
    }
    else if(Input == "F")
    {
        StatusMessage = ginRummy.ChooseFaceDown();
    }
    else if(!Input.empty() && Input.at(0) == 'D')
    {
        int idx = std::stoi(Input.substr(1)) - 1;
        StatusMessage = ginRummy.Discard(idx);
    }
    else if(Input == "C")
    {
        StatusMessage = ginRummy.ComputersTurn();
    }
    else if(Input == "K")
    {
        StatusMessage = ginRummy.Knock();
    }
    else if(Input == "G")
    {
        StatusMessage = ginRummy.Gin();
    }
    else
    {
        StatusMessage = "ERROR: Unexpected Input of " + Input;
    }

    return StatusMessage;
}

//////////////////////////////////////////////////////////////////////
/// Prints a console line of text with two strings. The total length
/// of the line will be LineLength long with the first parameter being
/// left aligned and the second parameter being right aligned
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
void GinRummyConsole::PrintLine(const std::string& Left, const std::string& Right) const
{
    int SpaceCount = LineLength - (Left.length() + Right.length());
    std::cout << Left << std::string(SpaceCount, ' ') << Right << std::endl;
}

//////////////////////////////////////////////////////////////////////
/// Prints a console line of text with three strings. The total length
/// of the line will be LineLength long with the first parameter being
/// left aligned, second parameter being center aligned, and the third
/// parameter being right aligned
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
void GinRummyConsole::PrintLine(const std::string& Left, const std::string& Middle, const std::string& Right) const
{
    static const int HalfLineLength = LineLength / 2;
    int LeftSpaceCount = HalfLineLength - (Left.length() + (Middle.length() / 2));
    int RightSpaceCount = HalfLineLength - (Right.length() + (Middle.length() / 2));

    // Integer math will result in non-exact lengths so we add/subtract as needed to get to the
    // required line length
    int TotalLineLength = Left.length() + Middle.length() + Right.length() + LeftSpaceCount + RightSpaceCount;
    if(TotalLineLength > LineLength)
        --LeftSpaceCount;
    else if(TotalLineLength < LineLength)
        ++LeftSpaceCount;

    // If LineLength isn't big enough to print the strings, the space counts can become negative and cause
    // exceptions to be thrown in the std::string constructors below so we always print at least one space
    if(LeftSpaceCount <= 0)
        LeftSpaceCount = 1;

    if(RightSpaceCount <= 0)
        RightSpaceCount = 1;

    std::cout << Left << std::string(LeftSpaceCount, ' ') << Middle << std::string(RightSpaceCount, ' ') << Right << std::endl;
}
