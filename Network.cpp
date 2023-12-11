#include <random>
#include <iostream>
#include <iomanip>
#include "Network.h"
#include "Neurons.h"

std::random_device weight;
std::mt19937 gen1(weight());


std::random_device sepsis;
std::default_random_engine gen2(sepsis());


std::uniform_real_distribution<float> dis1(1.0f, 5.0f);
std::uniform_real_distribution<float> dis2(0.1f, 10.0f);


void Network::add_input_layer(int number_of_neurons, const std::function<float(float)> &activation_func, float b) {
    network.emplace_back();
    for (int neuron = 0; neuron < number_of_neurons; neuron++) {
        network[0].emplace_back(1, dis1(gen1), activation_func);
    }
    layers += 1;


}


void Network::add_hidden_layer(int number_of_neurons, const std::function<float(float)> &activation_func, float b) {
    network.emplace_back();
    for (int neuron = 0; neuron < number_of_neurons; neuron++) {
        network[layers].emplace_back(layers + 1, dis1(gen1), activation_func);
    }
    layers += 1;
}


void Network::add_output_layer(int number_of_neurons, const std::function<float(float)> &activation_func, float b) {
    network.emplace_back();

    for (int neuron = 0; neuron < number_of_neurons; neuron++) {
        network[network.size() - 1].emplace_back(layers + 1, dis1(gen1), activation_func);
    }

    layers += 1;
}

void Network::create_synapse() {


    for (size_t i = 0; i < network.size() - 1; ++i) {
        synapse.emplace_back();
        for (size_t j = 0; j < network[i].size(); ++j) {
            for (size_t k = 0; k < network[i + 1].size(); ++k) {
                float c_weight = dis2(gen2);
                synapse[i].push_back(c_weight);
            }
        }
    }
}


void Network::build() {
    create_synapse();
}

void Network::show_network() {
    show_weights();
    show_synapse();
}

void Network::show_synapse() {
    for (int i = 0; i < layers - 1; i++) {
        std::cout << "Synapse weights from Layer " << i + 1 << " to Layer " << i + 2 << ":\n";

        for (size_t j = 0; j < network[i].size(); j++) {
            for (size_t k = 0; k < network[i + 1].size(); k++) {

                float cur_weight = synapse[i][(j * network[i + 1].size()) + k];

                std::cout << std::fixed << std::setprecision(1) << std::setw(5) << cur_weight;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void Network::show_weights() {
    // ��।������ ���ᨬ��쭮�� ������⢠ ���஭�� � ᫮�
    size_t max_neurons = 0;
    for (size_t i = 0; i < network.size(); ++i) {
        max_neurons = std::max(max_neurons, network[i].size());
    }

    // �뢮� ��ᮢ ��� ���஭�� ����।�� � ������ ᫮�
    for (size_t i = 0; i < max_neurons; ++i) {
        // �뢮� ��ᮢ ��� ���஭�� �� ࠧ��� ᫮��
        for (size_t j = 0; j < network.size(); ++j) {
            if (i < network[j].size()) {
                std::cout << std::fixed << std::setprecision(1) << network[j][i].getWeight() << " ";
            } else {
                std::cout << "    "; // ��� ��砥�, ����� ᫮� ᮤ�ন� ����� ���஭��
            }
        }
        std::cout << std::endl;
    }
}

