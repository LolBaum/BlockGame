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
    emptySpace = 1;
    glypheRatio = (float)glypheSizeX / (float)glypheSizeY;
    ASCII_offset = 32-2;
    renderShadow = true;
    shadowBrightness = 0.2;

    float sizeX = 1.0/((float)sheetSize/(float)glypheSizeX);
	float sizeY = 1.0/((float)sheetSize/(float)glypheSizeY);;

    mesh = new TextMesh(sizeX, sizeY);

}

Font::Font(){
    this->loadFont("graphics/FontAtlas.png");
    textHasChanged = false;

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
void Font::_addLine(std::string text, float x, float y, float size, float brightness){
    textHasChanged = true;
    float index_x, index_y;
    float size_x = size * glypheRatio;
    float size_y = size;
    float x_offset = size_x - (size_x / glypheSizeX);
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

        mesh->addCharacter(pos, 0, index_x, index_y, size_x, size_y, brightness);
        pos.x += x_offset;
    }
}
void Font::addLine(std::string text, float x, float y, float size, float brightness){
    float shadow_offset = size / glypheSizeY;
    if (renderShadow){
        _addLine(text, x+shadow_offset, y - shadow_offset, size, shadowBrightness);
    }
    _addLine(text, x, y, size, brightness);
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

