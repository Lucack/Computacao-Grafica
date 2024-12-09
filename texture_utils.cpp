#include <cstdlib>
#include <ctime>

void createSandTexture(int width, int height, unsigned char* data) {
    srand(time(NULL));
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char c = rand() % 64 + 192; // Light color base
            data[(y * width + x) * 3 + 0] = c;
            data[(y * width + x) * 3 + 1] = c - 48; // Slightly less green
            data[(y * width + x) * 3 + 2] = c - 80; // Even less blue for sandy color
       }
}
}

