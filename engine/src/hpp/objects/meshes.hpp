#ifndef ENDGEN_MESHES
#define ENDGEN_MESHES

#include <iostream>
#include <fstream>

#include <string>
#include <vector>

#include <basic_objects.hpp>

namespace ObjFileUtils {
    inline std::string getLine(std::ifstream& f) {
        std::string line;

        char c;
        while (c != EOF) {
            f.get(c);

            if (c == '\n') {
                return line;
            } else {
                line += c;
            }
        }
        f.close();

        return line;
    }

    inline std::vector<std::string> split(const std::string& s, char c = ' ') {
        std::vector<std::string> splitStr;
        std::string partialStr;

        for (size_t i = 0; i < s.length(); ++i) {
            if (s[i] == c) {
                splitStr.push_back(partialStr);
                partialStr.clear();
            } else {
                partialStr += s[i];
            }
        }

        // ensure we capture the final trailing string
        splitStr.push_back(partialStr);

        return splitStr;
    }
};

#define D_VERTEX "v"
#define D_VERTEX_NORMAL "vn"
#define D_VERTEX_TEXTURE "vt"
#define D_FACE "f"

struct Face {
    std::vector<int> i_vert; // vertex
    std::vector<int> t_vert; // texture vertex
    std::vector<int> n_vert; // normal vector
};

class Mesh {
    public:
        Mesh(const char* objFilePath) {
            std::ifstream objFile(objFilePath);
            if (!objFile.is_open()) {
                std::cerr << "[-] Error opening: " << objFilePath << "\n";
                return;
            }

            // parse .obj file contents
            while (objFile.is_open()) {
                std::string line = ObjFileUtils::getLine(objFile);
                std::vector<std::string> sections = ObjFileUtils::split(line);

                if (sections.at(0) == D_VERTEX) {
                    vertices.push_back({
                        std::stof(sections.at(1)),
                        std::stof(sections.at(2)),
                        std::stof(sections.at(3))
                    });
                } else if (sections.at(0) == D_FACE) {
                    // sections => { "1/1/1", "2/2/1", "4/3/1", "3/4/1" }
                    for (size_t i = 1; sections.size(); ++i) {
                        std::vector<std::string> faceMetaData = ObjFileUtils::split(sections.at(i),'/');
                        Face nFace;

                        // insert i_vert
                        nFace.i_vert.push_back(std::stoi(faceMetaData.at(0)));

                        // insert t_vert
                        nFace.t_vert.push_back(std::stoi(faceMetaData.at(1)));

                        // insert n_vert
                        nFace.t_vert.push_back(std::stoi(faceMetaData.at(2)));

                        faces.push_back(nFace);
                    }
                }
            }
        }

        /**
         * Generates the triangles in the form of a triangle-fan
         */
        void drawConvex() {
        }

        std::vector<Vector3>    vertices;
        std::vector<Face>       faces;
        std::vector<Triangle>   triangles;
};

#endif