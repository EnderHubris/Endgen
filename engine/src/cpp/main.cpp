#include <engine.hpp>

int main(int argc, char** argv) {
    if (argc == 2 && std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h") {
        std::cout << "Endgen [Poorly-Made Render Endgen]\n\n";
        std::cout << "  Flags:\n";
        std::cout << "--debug                   Show Frame Render Times\n";
        std::cout << "--exotic-triangles        Show Lots of Triangles\n";
        return 0;
    }

    EndgenEngine engine(argc, argv);
    return 0;
}