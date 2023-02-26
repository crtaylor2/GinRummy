#include "GinRummy.h"
#include "GinRummyRestServer.h"

int main(int argc, char *argv[])
{
    if(argc == 2 && strcmp(argv[1], "--server") == 0)
    {
        std::string addr = "http://localhost:34568/ginrummy";
        std::unique_ptr<GinRummyRestServer> restGinRummy = std::unique_ptr<GinRummyRestServer>(new GinRummyRestServer(addr));
        restGinRummy->open().wait();
        ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;
        std::cout << "Press ENTER to exit." << std::endl;
        std::string line;
        std::getline(std::cin, line);
        restGinRummy->close().wait();
    }
    else
    {
        GinRummy gr;
        gr.Play();
    }

    return 0;
}
