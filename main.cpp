#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>

class Methods {
public:
    std::vector<int> convert_to_binary(int n) {
        std::vector<int> binary(8, 0);
        for (int i = 7; i >= 0; --i) {
            binary[i] = n % 2;
            n /= 2;
        }
        return binary;
    }

    int convert_to_decimal(const std::vector<int>& binary) {
        int decimal = 0;
        for (int i = 0; i < 8; ++i) {
            decimal += binary[i] * std::pow(2, 7 - i);
        }
        return decimal;
    }
};

cv::Mat rgb_to_gray(const cv::Mat& image) {
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    return gray;
}

cv::Mat cellular_automata_encrypt(const cv::Mat& image, const std::vector<int>& rule_list, int cycles) {
    int height = image.rows;
    int width = image.cols;
    cv::Mat encrypted_image = image.clone();
    Methods methods;

    for (int cycle = 0; cycle < cycles; ++cycle) {
        cv::Mat new_image = cv::Mat::zeros(height, width, CV_8UC1);
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                int pixel = encrypted_image.at<uchar>(i, j);
                std::vector<int> binary_pixel = methods.convert_to_binary(pixel);
                std::vector<int> new_binary_pixel;

                for (int k = 0; k < 8; ++k) {
                    int rule = rule_list[k];
                    int a, b, c;

                    if (k == 0) {
                        a = 0; b = binary_pixel[k]; c = binary_pixel[k + 1];
                    } else if (k == 7) {
                        a = binary_pixel[k - 1]; b = binary_pixel[k]; c = 0;
                    } else {
                        a = binary_pixel[k - 1]; b = binary_pixel[k]; c = binary_pixel[k + 1];
                    }

                    int exp;
                    switch (rule) {
                        case 51: exp = !b; break;
                        case 85: exp = !c; break;
                        case 86: exp = (a && !c) || (b && !c) || (c && !a && !b); break;
                        case 102: exp = (b && !c) || (c && !b); break;
                        case 105: exp = (a && b && !c) || (a && c && !b) || (b && c && !a) || (!a && !b && !c); break;
                        case 150: exp = (a && b && c) || (a && !b && !c) || (b && !a && !c) || (c && !a && !b); break;
                        case 153: exp = (b && c) || (!b && !c); break;
                        case 165: exp = (a && c) || (!a || !c); break;
                        case 240: exp = a; break;
                        default: exp = 0;
                    }

                    new_binary_pixel.push_back(exp);
                }

                int new_pixel = methods.convert_to_decimal(new_binary_pixel);
                new_image.at<uchar>(i, j) = new_pixel;
            }
        }
        encrypted_image = new_image;
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            encrypted_image.at<uchar>(i, j) = (encrypted_image.at<uchar>(i, j) * 187) % 256;
        }
    }

    return encrypted_image;
}

cv::Mat cellular_automata_decrypt(const cv::Mat& image, const std::vector<int>& rule_list, int cycles) {
    int height = image.rows;
    int width = image.cols;
    cv::Mat decrypted_image = image.clone();

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            decrypted_image.at<uchar>(i, j) = (decrypted_image.at<uchar>(i, j) * 115) % 256;
        }
    }

    Methods methods;
    for (int cycle = 0; cycle < cycles; ++cycle) {
        cv::Mat new_image = cv::Mat::zeros(height, width, CV_8UC1);
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                int pixel = decrypted_image.at<uchar>(i, j);
                std::vector<int> binary_pixel = methods.convert_to_binary(pixel);
                std::vector<int> new_binary_pixel;

                for (int k = 0; k < 8; ++k) {
                    int rule = rule_list[k];
                    int a, b, c;

                    if (k == 0) {
                        a = 0; b = binary_pixel[k]; c = binary_pixel[k + 1];
                    } else if (k == 7) {
                        a = binary_pixel[k - 1]; b = binary_pixel[k]; c = 0;
                    } else {
                        a = binary_pixel[k - 1]; b = binary_pixel[k]; c = binary_pixel[k + 1];
                    }

                    int exp;
                    switch (rule) {
                        case 51: exp = !b; break;
                        case 85: exp = !c; break;
                        case 86: exp = (a && !c) || (b && !c) || (c && !a && !b); break;
                        case 102: exp = (b && !c) || (c && !b); break;
                        case 105: exp = (a && b && !c) || (a && c && !b) || (b && c && !a) || (!a && !b && !c); break;
                        case 150: exp = (a && b && c) || (a && !b && !c) || (b && !a && !c) || (c && !a && !b); break;
                        case 153: exp = (b && c) || (!b && !c); break;
                        case 165: exp = (a && c) || (!a || !c); break;
                        case 240: exp = a; break;
                        default: exp = 0;
                    }

                    new_binary_pixel.push_back(exp);
                }

                int new_pixel = methods.convert_to_decimal(new_binary_pixel);
                new_image.at<uchar>(i, j) = new_pixel;
            }
        }
        decrypted_image = new_image;
    }

    return decrypted_image;
}

int main() {
    cv::Mat input_image = cv::imread("Lenna_(test_image).png");
    if (input_image.empty()) {
        std::cerr << "Error: Unable to load image!" << std::endl;
        return -1;
    }

    cv::Mat gray_image = rgb_to_gray(input_image);
    cv::imwrite("gray_image.png", gray_image);

    std::vector<int> rule_list = {102, 102, 102, 102, 51, 86, 240, 153};
    cv::Mat encrypted_image = cellular_automata_encrypt(gray_image, rule_list, 4);
    cv::imwrite("encrypted_image.png", encrypted_image);

    cv::Mat decrypted_image = cellular_automata_decrypt(encrypted_image, rule_list, 4);
    cv::imwrite("decrypted_image.png", decrypted_image);

    std::cout << "Images saved: gray_image.png, encrypted_image.png, decrypted_image.png" << std::endl;

    return 0;
}
