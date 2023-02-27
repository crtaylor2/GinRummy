#ifndef GIN_RUMMY_CONSOLE_H
#define GIN_RUMMY_CONSOLE_H

#include "GinRummy.h"

class GinRummyConsole
{
    public:
        GinRummyConsole();

        void Play();

    private:
        static int LineLength;

        GinRummy ginRummy;

        void DrawGame(const std::string& StatusMessage);
        std::string UserInput();

        void PrintLine(const std::string& Left, const std::string& Right) const;
        void PrintLine(const std::string& Left, const std::string& Middle, const std::string& Right) const;
};

#endif //GIN_RUMMY_CONSOLE_H
