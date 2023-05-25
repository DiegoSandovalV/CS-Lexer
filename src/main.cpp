#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <filesystem>
#include <vector>
#include <thread>
#include "lexer.h"


//Reads the contents of a file and returns it as a string
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    
    if (file) {
        buffer << file.rdbuf();
        file.close();
    }
    
    return buffer.str();
}

//Generates an HTML file with the given content
void generateHTML(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);

    if (file) {
        file << "<!DOCTYPE html>\n";
        file << "<html>\n";
        file << "<head>\n";
        file << "    <title>File Contents</title>\n";
        file << "    <link rel=\"stylesheet\" type=\"text/css\" href=\"../styles/styles.css\">\n";
        file << "</head>\n";
        file << "<body>\n";
        file << "<pre>\n";
        file << content;
        file << "</pre>\n";
        file << "</body>\n";
        file << "</html>\n";
        
        file.close();
    }
    else {
        std::cout << "Error opening the file.\n";
    }
}

//Processes the files in the given vector and generates HTML files for each of them
void processFiles(const Lexer& lexer, const std::vector<std::string>& files, const std::string& outputDirectory) {
    for (const auto& file : files) {
        std::string inputFilename = file;
        std::string outputFilename = outputDirectory + "/" + std::filesystem::path(file).stem().string() + ".html";

        std::string fileContent = readFile(inputFilename);
        std::string processedContent = lexer.lex(fileContent);
        generateHTML(outputFilename, processedContent);
    }
}

int main() {
    std::string inputDirectory = "../csharp_examples";
    std::string outputDirectory = "../output";
    const int numThreads = 8;
    
    std::filesystem::create_directory(outputDirectory);

    Lexer lexer;

    // Get the list of files in the input directory
    std::vector<std::string> files;
    for (const auto& file : std::filesystem::directory_iterator(inputDirectory)) {
        if (file.path().extension() == ".cs") {
            files.push_back(file.path().string());
        }
    }

    std::cout << "Starting lexing..." << std::endl;

    // Measure single-threaded execution time
    auto startTimeSingle = std::chrono::high_resolution_clock::now();

    processFiles(lexer, files, outputDirectory);

    auto endTimeSingle = std::chrono::high_resolution_clock::now();
    auto durationSingle = std::chrono::duration_cast<std::chrono::milliseconds>(endTimeSingle - startTimeSingle).count();

    std::cout << "Single-threaded lexing completed. Time taken: " << durationSingle << " milliseconds.\n";

    // Divide the files evenly among threads
    int numFilesPerThread = files.size() / numThreads;
    int remainingFiles = files.size() % numThreads;

    // Create threads and assign files to each thread
    std::vector<std::thread> threads;
    auto fileIt = files.begin();

    auto startTimeMulti = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < numThreads; ++i) {
        int numFiles = numFilesPerThread;
        if (remainingFiles > 0) {
            numFiles++;
            remainingFiles--;
        }

        std::vector<std::string> threadFiles(fileIt, fileIt + numFiles);
        threads.emplace_back(std::thread(processFiles, std::ref(lexer), std::move(threadFiles), std::ref(outputDirectory)));

        std::advance(fileIt, numFiles);
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    auto endTimeMulti = std::chrono::high_resolution_clock::now();
    auto durationMulti = std::chrono::duration_cast<std::chrono::milliseconds>(endTimeMulti - startTimeMulti).count();

    std::cout << "Multithreaded lexing completed. Time taken: " << durationMulti << " milliseconds.\n";

    return 0;
}
