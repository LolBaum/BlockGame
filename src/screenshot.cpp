#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
//#include "core_includes.hpp"
#include <iostream>
#include <string>
#include <glew.h>
#include "util_funcs.hpp"
#include "screenshot.hpp"
#include "SDL_handler.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>


#include <sys/stat.h>

/// based on https://stackoverflow.com/a/17223443/14779894
/// and https://stackoverflow.com/a/58523115/14779894
std::string return_current_time_and_date()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y_%m_%d_%H%M%S");
    return ss.str();
}


std::string numerate_name(std::string name, std::string ending, int max_tests){
	int i = 0;
	std::string new_name = name;
	new_name.append(ending);
	while(i<max_tests && exists(new_name)){
		//std::cout << new_name << " exists arleady" << std::endl;
		new_name = name;
		new_name.append(std::to_string(i));
		new_name.append(ending);
		i++;
	}
    if (i==max_tests){
        std::cout << "numerate_name exceeded maximum of " << max_tests << "Iterations (" << name << "*" << ending << ")" << std::endl;
    }
	return new_name;
}

void savePNG(std::string filename, int windowWidth, int windowHeight, int num_chanels, unsigned char* pixels){
    int no_error = stbi_write_png(filename.c_str(), windowWidth, windowHeight, num_chanels, pixels, windowWidth * num_chanels);
    if (no_error){
        std::cout << "Saved image as: " << filename << std::endl;
    }
    else{
        std::cerr << "Couldn't save Image." << std::endl;
    }
}


void ScreenshotHandeler::runScreenshotThread() {
    while(threadShouldRun){
        std::unique_lock lk(m);
        cv.wait(lk, [this]{ return dataIsReady; });
        for (auto s : queue) {
            savePNG(s->filename, s->width, s->height, s->channels, *s->pixels.get());
        }
        queue.clear();
        lk.unlock();
        cv.notify_one();
    }
    std::cout << "screenshout thread ending" << std::endl;
}


ScreenshotHandeler::ScreenshotHandeler() {
    queue = std::vector<std::shared_ptr<ScreenshotData>>();
    threadShouldRun = true;
    thread = std::thread(&ScreenshotHandeler::runScreenshotThread, this);
}

ScreenshotHandeler::~ScreenshotHandeler() {
    threadShouldRun = false;
    cv.notify_one();
    thread.join();
}


void ScreenshotHandeler::saveScreenshotToFile(std::string filename, int windowWidth, int windowHeight) {
    const int numberOfPixels = windowWidth * windowHeight * 3;
    std::shared_ptr<unsigned char*> pixels = std::make_shared<unsigned char*>(new unsigned char[numberOfPixels]);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, *pixels.get());
    stbi_flip_vertically_on_write(1);

    // TODO QUEUE
    //  https://en.cppreference.com/w/cpp/thread/condition_variable
    dataIsReady = false;
    {
        std::lock_guard lk(m);
        auto data = std::make_shared<ScreenshotData>(ScreenshotData({filename, windowWidth, windowHeight, 3, pixels}));
        queue.push_back(data);
        dataIsReady = true;
    }
    cv.notify_one();
}

void ScreenshotHandeler::takeScreenshot() {
    saveScreenshotToFile("screenshots/screenshot_" + return_current_time_and_date() + ".png", SDL_handler::getWidth(), SDL_handler::getHeight());
}
