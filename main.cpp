#include "Core.h"


int main() {
    std::filesystem::current_path("..");

    Network net;
    net.add_layer(LayerType::input, 2, FunctionType::fast_sigmoid, 0.0f);
    net.add_layer(LayerType::hidden, 3, FunctionType::sigmoid, 0.0f);
    net.add_layer(LayerType::hidden, 5, FunctionType::silu, 0.0f);
    net.add_layer(LayerType::hidden, 7, FunctionType::sigmoid, 0.0f);
    net.add_layer(LayerType::hidden, 3, FunctionType::tanh, 0.0f);
    net.add_layer(LayerType::output, 2, FunctionType::sigmoid, 0.0f);
    net.build();
    net.show_network();
    net.save("base3.json");

    std::vector<std::vector<float>> input_data = {{0.6f, 0.2f}};
    std::vector<std::vector<float>> output_data = {{1.0f, 0.3f}};

    const int epochs = 10000;

    net.train(input_data, output_data, Normalisation::min_max_normalisation, epochs, 1, 0.1);
    net.work(input_data);

    return 0;
}
