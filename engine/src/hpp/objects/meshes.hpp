#ifndef ENDGEN_MESHES
#define ENDGEN_MESHES

#include <iostream>
#include <fstream>

#include <string>
#include <vector>

#include <basic_objects.hpp>

namespace ObjFileUtils {
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
    private:
        void createTriangles() {
            for (size_t i = 0; i < faces.back().i_vert.size(); i += 3) {
                // faces.back().i_vert tells us the index we need
                // to fetch from this.vertices to form the triangle
                Vector3 v1 = vertices.at(faces.back().i_vert.at(i));
                Vector3 v2,v3;

                if (i >= 3) {
                    // reuse previous indices to form a triangle-fan face
                    v2 = vertices.at(faces.back().i_vert.at(i-1));
                    v3 = vertices.at(faces.back().i_vert.at(i-2));
                } else {
                    v2 = vertices.at(faces.back().i_vert.at(i+1));
                    v3 = vertices.at(faces.back().i_vert.at(i+2));
                }

                triangles.emplace_back(v1, v2, v3);
            }
        }
        void createFace(std::vector<std::string>& faceData) {
            // faceData => { "1/1/1", "2/2/1", "4/3/1", "3/4/1" }
            faceData.erase(faceData.begin());
            if (faceData.empty()) return;

            Face nFace;
            for (std::string faceEntry : faceData) {
                std::vector<std::string> faceMetaData = ObjFileUtils::split(faceEntry,'/');
                if (faceMetaData.empty()) continue;

                // insert i_vert (vertex index)
                
                // need to subtract 1 from the index so it fits within
                // array bounds
                nFace.i_vert.push_back(std::stoi(faceMetaData.at(0)) - 1);

                // insert t_vert (texture vert)
                nFace.t_vert.push_back(std::stoi(faceMetaData.at(1)) - 1);

                // insert n_vert (normal vert)
                nFace.t_vert.push_back(std::stoi(faceMetaData.at(2)) - 1);

            }

            if (!nFace.i_vert.empty()) {
                faces.push_back(nFace);
                createTriangles();
            }
        }
    public:
        Mesh(const char* objFilePath) {
            std::ifstream objFile(objFilePath);
            if (!objFile.is_open()) {
                std::cerr << "[-] Error opening: " << objFilePath << "\n";
                return;
            } else {
                std::cout << "[+] Parsing: " << objFilePath << "\n";
            }

            // parse .obj file contents
            std::string line;
            while (getline(objFile, line)) {
                std::vector<std::string> sections = ObjFileUtils::split(line);
                if (sections.empty()) continue;
                
                if (sections.at(0) == D_VERTEX) {
                    vertices.push_back({
                        std::stof(sections.at(1)),
                        std::stof(sections.at(2)),
                        std::stof(sections.at(3))
                    });
                } else if (sections.at(0) == D_FACE) {
                    createFace(sections);
                }
            }
            objFile.close();
            std::cout << " |__ object data created!\n";
            std::cout << " |__ number of faces: " << faces.size() << "\n";
            std::cout << " |__ number of triangles: " << triangles.size() << "\n";
        }

        std::vector<Vector3>    vertices;
        std::vector<Face>       faces;
        std::vector<Triangle>   triangles;
};

#endif