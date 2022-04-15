#include "text.hpp"
#include "textures.hpp"
#include "object_models.hpp"
#include "SDL_handler.hpp"




void Font::loadFont(std::string fontPathName){
    font = new Texture();
    font->load(fontPathName.c_str());
    sheetSize = 128;
    glyphesPerRow = 18;
    glypheSizeX = 7;
    glypheSizeY = 10;
    ASCII_offset = 32-2;

    float sizeX = 1.0/((float)sheetSize/(float)glypheSizeX);
	float sizeY = 1.0/((float)sheetSize/(float)glypheSizeY);;

    mesh = new TextMesh(sizeX, sizeY);

}

Font::Font(){
    this->loadFont("graphics/fontAtlas.png");
    textHasChanged = false;
    aspectRatio = SDL_handler::getAspectRatio();

}
Font::~Font(){
    font->~Texture();
    mesh->~Mesh();
}

void Font::clear(){
    textHasChanged = true;
    mesh->clearMesh();



}

void Font::update(){
    if (textHasChanged){
        mesh->update();
    }
    
}

void Font::addLine(std::string text, float x, float y, float size){
    textHasChanged = true;
    float index_x, index_y;
    float size_x = size * aspectRatio;
    float size_y = size;
    float x_offset = size_x * 0.8;
    //float x_space_offset = size_x * 0.5;

    std::string::const_iterator c;
    char ascii_char;
    glm::vec2 pos = glm::vec2(x, y);

    for (c = text.begin(); c != text.end(); c++){
        ascii_char = *c - ASCII_offset;
        if(ascii_char < 0){
            ascii_char = 0;
        }

        index_x = ascii_char % glyphesPerRow;
        index_y = ascii_char / glyphesPerRow;

        mesh->addCharacter(pos, 0, index_x, index_y, size_x, size_y);
        pos.x += x_offset;
    } 
}

void Font::addMultipleLines(std::string text, float x, float y, float size, float line_offset){
    std::string delimiter = "\n";
    std::string line;
    size_t last = 0; 
    size_t next = 0; 
    int i = 0;
    while ((next = text.find(delimiter, last)) != std::string::npos){
        line = text.substr(last, next-last);
        addLine(line, x, y-(line_offset + size)*i, size);
        last = next + 1;
        i++;
    }
}

void Font::render(){
    mesh->render(font->get_textureId());
}

