#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

struct Texture
{
    unsigned int id;
    std::string type;
    // store the path of the texture to compare with other textures 
    // to see if one texture has already been loaded and so to avoid loading it again
    std::string path;
};

#endif  // TEXTURE_H
