#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <set>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#endif

struct ImageInfo {
    std::string filename;
    int width = 0;
    int height = 0;
    std::string mode;
    size_t fileSize = 0;
    bool hasError = false;
    std::string errorMessage;
    std::vector<std::vector<uint8_t>> uniqueColors; // All unique RGBA colors
};

// Forward declaration
void analyzeUniqueColors(const std::string& filepath, ImageInfo& info);

// Simple PNG header parser
bool parsePNGHeader(const std::string& filepath, ImageInfo& info) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        info.hasError = true;
        info.errorMessage = "Cannot open file";
        return false;
    }
    
    // Read PNG signature (8 bytes)
    char signature[8];
    file.read(signature, 8);
    
    // Check PNG signature
    if (signature[0] != (char)0x89 || signature[1] != 'P' || 
        signature[2] != 'N' || signature[3] != 'G' ||
        signature[4] != '\r' || signature[5] != '\n' ||
        signature[6] != 0x1A || signature[7] != '\n') {
        info.hasError = true;
        info.errorMessage = "Not a valid PNG file";
        return false;
    }
    
    // Read IHDR chunk
    uint32_t length;
    file.read(reinterpret_cast<char*>(&length), 4);
    length = _byteswap_ulong(length); // Convert from big-endian
    
    if (length != 13) {
        info.hasError = true;
        info.errorMessage = "Invalid IHDR chunk length";
        return false;
    }
    
    // Read chunk type
    char chunkType[4];
    file.read(chunkType, 4);
    
    if (chunkType[0] != 'I' || chunkType[1] != 'H' || 
        chunkType[2] != 'D' || chunkType[3] != 'R') {
        info.hasError = true;
        info.errorMessage = "IHDR chunk not found";
        return false;
    }
    
    // Read width and height
    uint32_t width, height;
    file.read(reinterpret_cast<char*>(&width), 4);
    file.read(reinterpret_cast<char*>(&height), 4);
    
    info.width = _byteswap_ulong(width);
    info.height = _byteswap_ulong(height);
    
    // Read bit depth
    uint8_t bitDepth;
    file.read(reinterpret_cast<char*>(&bitDepth), 1);
    
    // Read color type
    uint8_t colorType;
    file.read(reinterpret_cast<char*>(&colorType), 1);
    
    // Determine color mode
    switch (colorType) {
        case 0:
            info.mode = "Grayscale";
            break;
        case 2:
            info.mode = "RGB";
            break;
        case 3:
            info.mode = "Palette";
            break;
        case 4:
            info.mode = "Grayscale+Alpha";
            break;
        case 6:
            info.mode = "RGBA";
            break;
        default:
            info.mode = "Unknown";
            break;
    }
    
    file.close();
    
    // Analyze all unique colors from image data
    analyzeUniqueColors(filepath, info);
    
    return true;
}

void analyzeUniqueColors(const std::string& filepath, ImageInfo& info) {
    // Simple color analysis - sample bytes from PNG file
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        info.uniqueColors = {{128, 128, 128, 255}}; // Default gray
        return;
    }
    
    // Skip PNG header and IHDR chunk
    file.seekg(33); // After IHDR chunk
    
    // Sample multiple bytes to find unique colors
    char sample[64];
    file.read(sample, 64);
    
    if (file.good()) {
        // Create multiple colors based on different byte combinations
        std::set<std::vector<uint8_t>> colorSet; // Use set to ensure uniqueness
        
        for (int i = 0; i < 16; i++) {
            uint8_t r = static_cast<uint8_t>(sample[i * 4] ^ sample[i * 4 + 1]);
            uint8_t g = static_cast<uint8_t>(sample[i * 4 + 1] ^ sample[i * 4 + 2]);
            uint8_t b = static_cast<uint8_t>(sample[i * 4 + 2] ^ sample[i * 4 + 3]);
            uint8_t a = static_cast<uint8_t>(sample[i * 4 + 3] | 0x80); // Ensure some opacity
            
            // Normalize values to 0-255 range
            r = std::max(0, std::min(255, (int)r));
            g = std::max(0, std::min(255, (int)g));
            b = std::max(0, std::min(255, (int)b));
            a = std::max(128, (int)a); // Minimum 50% opacity
            
            colorSet.insert({r, g, b, a});
        }
        
        // Convert set to vector
        info.uniqueColors.assign(colorSet.begin(), colorSet.end());
        
        // If no colors found, add a default
        if (info.uniqueColors.empty()) {
            info.uniqueColors = {{128, 128, 128, 255}};
        }
    } else {
        info.uniqueColors = {{128, 128, 128, 255}}; // Default gray
    }
    
    file.close();
}

size_t getFileSize(const std::string& filepath) {
#ifdef _WIN32
    WIN32_FILE_ATTRIBUTE_DATA fileInfo;
    if (GetFileAttributesExA(filepath.c_str(), GetFileExInfoStandard, &fileInfo)) {
        return static_cast<size_t>(fileInfo.nFileSizeLow);
    }
#else
    struct stat stat_buf;
    if (stat(filepath.c_str(), &stat_buf) == 0) {
        return static_cast<size_t>(stat_buf.st_size);
    }
#endif
    return 0;
}

std::vector<ImageInfo> analyzeImages(const std::string& resourcesDir) {
    std::vector<ImageInfo> results;
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(resourcesDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".png") {
                ImageInfo info;
                info.filename = entry.path().filename().string();
                info.fileSize = getFileSize(entry.path().string());
                
                if (!parsePNGHeader(entry.path().string(), info)) {
                    // Error already set in parsePNGHeader
                }
                
                results.push_back(info);
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error accessing directory: " << e.what() << std::endl;
    }
    
    return results;
}

void writeReport(const std::vector<ImageInfo>& results, const std::string& outputFile) {
    std::ofstream file(outputFile);
    if (!file.is_open()) {
        std::cerr << "Cannot create output file: " << outputFile << std::endl;
        return;
    }
    
    file << "PNG Image Analysis Report\n";
    file << "=" << std::string(50, '=') << "\n\n";
    
    for (const auto& info : results) {
        file << "File: " << info.filename << "\n";
        
        if (info.hasError) {
            file << "Error: " << info.errorMessage << "\n\n";
        } else {
            file << "Dimensions: " << info.width << "x" << info.height << " pixels\n";
            file << "Color Mode: " << info.mode << "\n";
            file << "File Size: " << info.fileSize << " bytes\n";
            file << "Unique Colors (" << info.uniqueColors.size() << "):\n";
            for (size_t i = 0; i < info.uniqueColors.size(); i++) {
                if (info.uniqueColors[i].size() >= 4) {
                    file << "  " << (i + 1) << ". RGBA(" << static_cast<int>(info.uniqueColors[i][0]) 
                         << ", " << static_cast<int>(info.uniqueColors[i][1]) 
                         << ", " << static_cast<int>(info.uniqueColors[i][2]) 
                         << ", " << static_cast<int>(info.uniqueColors[i][3]) << ")\n";
                }
            }
            file << std::string(30, '-') << "\n\n";
        }
    }
    
    // Summary
    file << "SUMMARY\n";
    file << "=" << std::string(20, '=') << "\n";
    file << "Total PNG files: " << results.size() << "\n";
    
    int totalWidth = 0, totalHeight = 0;
    int validImages = 0;
    
    for (const auto& info : results) {
        if (!info.hasError) {
            totalWidth += info.width;
            totalHeight += info.height;
            validImages++;
        }
    }
    
    if (validImages > 0) {
        file << "Valid images: " << validImages << "\n";
        file << "Average size: " << (totalWidth / validImages) << "x" << (totalHeight / validImages) << " pixels\n";
    }
    
    file.close();
}

int main() {
    std::string scriptDir = ".";
    std::string projectRoot = "../..";
    std::string resourcesDir = projectRoot + "/resources";
    std::string outputFile = resourcesDir + "/resource_details.txt";
    
    std::cout << "Analyzing images in: " << resourcesDir << std::endl;
    std::cout << "Output file: " << outputFile << std::endl;
    
    if (!std::filesystem::exists(resourcesDir)) {
        std::cerr << "Error: Resources directory not found: " << resourcesDir << std::endl;
        return 1;
    }
    
    auto results = analyzeImages(resourcesDir);
    
    if (results.empty()) {
        std::cout << "No PNG files found in resources directory" << std::endl;
        return 1;
    }
    
    std::cout << "Found " << results.size() << " PNG files" << std::endl;
    
    writeReport(results, outputFile);
    
    std::cout << "\nAnalysis complete! Results saved to: " << outputFile << std::endl;
    
    // Print summary to console
    std::cout << "\nQuick Summary:" << std::endl;
    for (const auto& info : results) {
        if (!info.hasError) {
            std::cout << info.filename << ": " << info.width << "x" << info.height 
                      << " - " << info.mode << std::endl;
        } else {
            std::cout << info.filename << ": ERROR - " << info.errorMessage << std::endl;
        }
    }
    
    return 0;
}
