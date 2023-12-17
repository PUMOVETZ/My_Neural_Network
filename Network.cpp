#include <random>
#include <iostream>
#include <iomanip>
#include "Network.h"
#include "Neurons.h"
#include "Matrix.h"

std::random_device weight;
std::mt19937 gen1(weight());


std::random_device sepsis;
std::default_random_engine gen2(sepsis());


std::uniform_real_distribution<float> dis1(1.0f, 5.0f);
std::uniform_real_distribution<float> dis2(0.1f, 10.0f);


void Network::add_input_layer(int number_of_neurons, const std::function<float(float)> &activation_func, float b) {
    network.emplace_back();
    for (int neuron = 0; neuron < number_of_neurons; neuron++) {
        network[0].emplace_back(1, 0, activation_func);
    }
    layers += 1;


}


void Network::add_hidden_layer(int number_of_neurons, const std::function<float(float)> &activation_func, float b) {
    network.emplace_back();
    for (int neuron = 0; neuron < number_of_neurons; neuron++) {
        network[layers].emplace_back(layers + 1, 0, activation_func);
    }
    layers += 1;
}


void Network::add_output_layer(int number_of_neurons, const std::function<float(float)> &activation_func, float b) {
    network.emplace_back();

    for (int neuron = 0; neuron < number_of_neurons; neuron++) {
        network[network.size() - 1].emplace_back(layers + 1, 0, activation_func);
    }

    layers += 1;
}

void Network::create_synapse() {




    for (size_t i = 0; i < network.size() - 1; ++i) {
        synapse.emplace_back();


        if (network[i+1].size() != 1) {


            for (size_t j = 0; j < network[i + 1].size(); ++j) {
                std::vector<float> demi_synapse;





                for (size_t k = 0; k < network[i].size(); ++k) {
                    float c_weight = dis2(gen2);
                    demi_synapse.push_back(c_weight);
                }
                synapse[i].push_back(demi_synapse);
                demi_synapse.clear();
            }
        }
        else{
            for (size_t j = 0; j < network[i].size(); ++j) {
                std::vector<float> demi_synapse;

                for (size_t k = 0; k < network[i+1].size(); ++k) {
                    float c_weight = dis2(gen2);
                    demi_synapse.push_back(c_weight);
                }
                synapse[i].push_back(demi_synapse);
                demi_synapse.clear();
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

        for (int j = 0; j < synapse[i].size(); j++) {

            for (int k = 0; k < synapse[i][j].size(); k++) {
                std::cout << synapse[i][j][k] << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

}


void Network::show_weights() {
    std::cout << "Neuron Weights" << std::endl;
    size_t max_neurons = 0;
    for (const auto & i : network) {
        max_neurons = std::max(max_neurons, i.size());
    }


    for (size_t i = 0; i < max_neurons; ++i) {

        for (auto & j : network) {
            if (i < j.size()) {
                std::cout << std::fixed << std::setprecision(1) << j[i].weight << " ";
            } else {
                std::cout << "    ";
            }
        }
        std::cout << std::endl;
    }


}


void Network::train(std::vector<std::vector<float>> parameters, std::vector<std::vector<float>> answer, const int epochs, float test_data_per) {
    for (int epoch = 0; epoch < epochs; ++epoch) {
        for (const auto & parameter : parameters) {
            std::vector<float> defi = through(parameter);

        }


        //for (int i = 0; i < input_data.size(); ++i) {
        // ��אַ� �����࠭���� (����祭�� �।᪠�����)
        // �������� �㭪��, ����� ��।��� �室�� ����� �१ ��� � ����砥� �।᪠�����

        // ���⭮� �����࠭���� (���᫥��� �訡�� � ���������� ��ᮢ)
        // �������� ������ ���⭮�� �����࠭���� ��� ���᫥��� �訡�� � ���������� ��ᮢ
        // �ᯮ���� �㭪�� ����� (���ਬ��, MSE ��� ����� ॣ��ᨨ)

        // ���᫥��� �訡��
        // ���������� ��ᮢ (�ࠤ����� ���)
        // �������� ���������� ��ᮢ � ᮮ⢥��⢨� � �ࠤ����� ��᪮� � ᪮����� ���祭��
    }

    // �뢮� �訡�� �� ������ ���樨 (��樮���쭮)
    //std::cout << "Epoch: " << epoch + 1 << ", Error: " << total_error << std::endl;


}

std::vector<float> Network::through(std::vector<float> inp) {
    for (int i = 0; i < inp.size(); ++i) {
        network[0][i].weight = inp[i];
    }



    return {};
}


