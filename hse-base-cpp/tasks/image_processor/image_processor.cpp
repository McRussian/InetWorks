// main.cpp
#include <iostream>
#include <string>
#include "src/image.h"
#include "src/loader.h"

using namespace std;

void ShowHelp() {
    cout << "=== BMP Image Processor ===" << endl;
    cout << "Usage: program input.bmp output.bmp" << endl;
    cout << "       program --help" << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2 || string(argv[1]) == "--help") {
        ShowHelp();
        return 0;
    }
    
    if (argc < 3) {
        cout << "Error: Need input and output files" << endl;
        return 1;
    }
    
    string inputFile = argv[1];
    string outputFile = argv[2];
    
    cout << "Loading: " << inputFile << endl;
    
    BMPLoader loader;
    Image img;
    
    if (!loader.Load(inputFile, img)) {
        cout << "Load failed: " << loader.GetLastError() << endl;
        return 1;
    }
    
    cout << "Size: " << img.GetWidth() << "x" << img.GetHeight() << endl;
    cout << "Saving: " << outputFile << endl;
    
    if (!loader.Save(outputFile, img)) {
        cout << "Save failed: " << loader.GetLastError() << endl;
        return 1;
    }
    
    cout << "Done!" << endl;
    return 0;
}