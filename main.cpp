#include "Core.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

// �㭪�� ��� �८�ࠧ������ ⥪�⮢�� ����ᮢ � �᫮�� ���祭��
int getClassNumber(const std::string &className) {
    static std::map<std::string, float> classMap = {
            {"Iris-setosa",     0.33},
            {"Iris-versicolor", 0.66},
            {"Iris-virginica",  0.99}
    };

    auto it = classMap.find(className);
    if (it != classMap.end()) {
        return it->second;
    } else {
        return -1; // �����頥� -1 � ��砥 ����।�������� �����
    }
}

int main() {
    std::filesystem::current_path("..");
    std::ifstream file(
            R"(C:\Users\batsi\OneDrive\Documents\CLionProjects\My_own_Neural_Network\Iris.csv)"); // ������� "example.csv" �� ��� ��襣� CSV 䠩��

    if (!file.is_open()) {
        std::cout << "�� 㤠���� ������ 䠩�." << std::endl;
        return 1;
    }

    std::vector<std::vector<float>> input; // ����� ��� �࠭���� �᫮��� ������
    std::vector<float> output; // ����� ��� �࠭���� �᫮��� ���祭�� ����ᮢ

    std::string line;
    bool headerSkipped = false;
    while (std::getline(file, line)) {
        if (!headerSkipped) {
            headerSkipped = true;
            continue; // �ய����� ����� ��ப� � �����������
        }

        std::vector<float> rowInput;
        std::stringstream ss(line);
        std::string cell;
        int classNumber = -1;

        int col = 0;
        while (std::getline(ss, cell, ',')) {
            if (col < 4) { // ���� ���� �⮫�� ᮤ�ঠ� �᫮�� �����
                rowInput.push_back(std::stod(cell));
            } else if (col == 4) { // ���� �⮫��� ᮤ�ন� ⥪�⮢� �����
                classNumber = getClassNumber(cell);
            }
            col++;
        }

        if (classNumber != -1) {
            input.push_back(rowInput);
            output.push_back(classNumber);
        }
    }

    file.close();

    // �뢮� ���祭�� ��� �஢�ન
    std::cout << "Input:" << std::endl;
    for (const auto &row: input) {
        for (const auto &value: row) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Output:" << std::endl;
    for (const auto &value: output) {
        std::cout << value << std::endl;
    }


    std::vector<std::vector<float>> out = {};
    for (int i = 0; i < output.size(); i++) {
        out.push_back({output[i]});
    }


    Network net;
    net.add_layer(LayerType::input, 4, FunctionType::sigmoid);
    net.add_layer(LayerType::hidden, 3, FunctionType::fast_sigmoid);
    net.add_layer(LayerType::hidden, 5, FunctionType::silu);
    net.add_layer(LayerType::hidden, 7, FunctionType::sigmoid);
    net.add_layer(LayerType::hidden, 3, FunctionType::tanh);
    net.add_layer(LayerType::output, 1, FunctionType::sigmoid);
    net.build();
    net.save("base.json");

    std::vector<std::vector<float>> input_data = {{0.6f, 0.2f}};
    std::vector<std::vector<float>> output_data = {{1.0f, 0.3f}};

    const int epochs = 1000;

    net.train(input, out, Normalisation::min_max_normalisation, epochs, 1, 0.1);
//    net.work(input_data);
    net.work({{7.6, 3.0, 6.6, 2.1}});

    return 0;
}
