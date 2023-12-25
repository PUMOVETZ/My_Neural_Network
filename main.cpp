#include "Network.h"
#include "Activations.h"
#include "DataFunc.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

int main() {
    std::filesystem::current_path("..");

    Network net;
    net.add_layer(LayerType::input, 2, FunctionType::silu, 0.0f);
    net.add_layer(LayerType::hidden, 3, FunctionType::sigmoid, 0.0f);
    net.add_layer(LayerType::output, 2, FunctionType::tanh, 0.0f);

    net.build();
    net.show_network();
    net.save("base.json");
    //std::vector<std::vector<std::vector<float>>> input_data = {{{1.0f}, {2.0f}}, {{3.0f}, {4.0f}}};
    //std::vector<std::vector<float>> output_data = {{1.3f, 9.4f}, {7.2f, 8.4f}};

    const int epochs = 1;

    std::vector<std::vector<std::vector<float>>> input_data;
    std::vector<std::vector<float>> output_data;

    DataLoader::readDataset("fruits_dataset.txt", input_data, output_data);
    DataLoader::normalize_dataset(input_data, output_data);


    net.train(input_data, output_data, epochs, 1, 0.02);



    //std::cout << "--------------------------------" << std::endl;


//    Network loadedNetwork("base.json");
//
//
//    loadedNetwork.show_network();
//    std::vector<std::vector<std::vector<float>>> input_data = {{{1.0f}, {2.0f}}, {{3.0f}, {4.0f}}};
//    std::vector<std::vector<float>> output_data = {{3.0f, 4.09f}, {7.0f, 8.07f}};
//
//    const int epochs = 1;
//
//
//    loadedNetwork.train(input_data, output_data, epochs, 1, 0.02);

    return 0;
}
