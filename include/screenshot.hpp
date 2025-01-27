#pragma once
#include <string>
#include <glew.h>
#include <sys/stat.h>
#include <thread>
#include <mutex>
#include <condition_variable>

struct ScreenshotData{
    std::string filename;
    int width;
    int height;
    int channels;
    std::shared_ptr<unsigned char*> pixels;
};


std::string numerate_name(std::string name, std::string ending=".png", int max_tests=500);

void savePNG(std::string filename, int windowWidth, int windowHeight, int num_chanels, unsigned char* pixels);


class ScreenshotHandeler{
public:
    ScreenshotHandeler();
    ~ScreenshotHandeler();

    void takeScreenshot();

    private:
    void saveScreenshotToFile(std::string filename, int windowWidth, int windowHeight);
    void runScreenshotThread();


    std::thread thread;
    std::vector<std::shared_ptr<ScreenshotData>> queue;

    bool threadShouldRun;
    bool dataIsReady;
    std::mutex m;
    std::condition_variable cv;
};
