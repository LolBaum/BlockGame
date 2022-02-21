#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
#include <string>
#include <glew.h>

#include <sys/stat.h>
inline bool exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

std::string numerate_name(std::string name, std::string ending=".png", int max_tests=100){
	int i = 0;
	std::string new_name = name;
	new_name.append(ending);
	while(i<max_tests && exists(new_name)){
		//std::cout << new_name << " exists arleady" << std::endl;
		new_name = name;
		new_name.append(to_string(i));
		new_name.append(ending);
		i++;
	}
	return new_name;
}



void savePNG(std::string filename, int windowWidth, int windowHeight, int num_chanels, unsigned char* pixels){
    int no_error = stbi_write_png(filename.c_str(), windowWidth, windowHeight, num_chanels, pixels, windowWidth * num_chanels);
    if (no_error){
        std::cout << "Saved image as: " << filename << std::endl;
    }
    else{
        std::cout << "Couldn't save Image." << std::endl;
    }
}


void saveScreenshotToFile(std::string filename, int windowWidth, int windowHeight) {    
    const int numberOfPixels = windowWidth * windowHeight * 3;
    unsigned char *pixels =  new unsigned char[numberOfPixels];

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, windowWidth, windowHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

	int num_chanels = 3;
	unsigned char *correctpixels = new unsigned char[numberOfPixels];
	for(int i=0; i<numberOfPixels-2; i=i+3){
		correctpixels[i] = pixels[i+2];
		correctpixels[i+1] = pixels[i+1];
		correctpixels[i+2] = pixels[i];
	} 

	stbi_flip_vertically_on_write(1);
    savePNG(filename.c_str(), windowWidth, windowHeight, num_chanels, correctpixels);

	delete[] pixels;
	delete[] correctpixels;
}

