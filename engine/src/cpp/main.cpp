#include <engine.hpp>
#include <cstring>

int main(int argc, char* argv[])
{
    bool enableInspector = (argc == 2 && strcmp(argv[1], "inspect") == 0);
    EndgenEngine engine(enableInspector);
    return 0;
}