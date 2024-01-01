#include "Matrix.h"
#include "json.h"
#include "Network.h"
#include <chrono>
#include "Utils.h"
#include <iomanip>
#include <cmath>

#define NETWORK_NUM_NEURONS_NAME    "Num_of_neurons"
#define NETWORK_ACTIVATION_FN_NAME  "Activation_function"
#define NETWORK_SYNAPSE_NAME        "Synapse"

//float call(FunctionType type, float arg) {
//    switch (type) {
//        case FunctionType::sigmoid:
//            return activation::sigmoid(arg);
//        case FunctionType::fast_sigmoid:
//            return activation::fast_sigmoid(arg);
//        case FunctionType::silu:
//            return activation::silu(arg);
//        case FunctionType::tanh:
//            return activation::tanh(arg);
//
//    }
//}

Network::Network() = default;

Network::Network(const char *filename) {
    nlohmann::json input;

    {
        std::ifstream file(filename, std::ios::in | std::ios::binary);

        if (!file.is_open())
            throw std::runtime_error("Unable to open file");

        file >> input;
    }

    int i = 0;
    for (const auto &layer_data: input) {
        int num_neurons = layer_data.at(NETWORK_NUM_NEURONS_NAME);

        const auto &fn_name = layer_data.at(NETWORK_ACTIVATION_FN_NAME).get_ref<const std::string &>();
        FunctionType ft = function_type_from_string(fn_name);

        float b = 0.0f;

        if (network.empty()) {
            add_layer(LayerType::input, num_neurons, ft, b);
        } else if (i == input.size() - 1) {
            add_layer(LayerType::output, num_neurons, ft, b);
            continue;
        } else {
            add_layer(LayerType::hidden, num_neurons, ft, b);
        }

        auto &synapse_data = layer_data.at(NETWORK_SYNAPSE_NAME);
        synapse.push_back(synapse_data.get<std::vector<std::vector<float>>>());
        i++;
    }
}

void Network::add_layer(LayerType type, int number_of_neurons, FunctionType af, float b) {
    network.emplace_back();

    switch (type) {
        case LayerType::input:
            for (int neuron = 0; neuron < number_of_neurons; neuron++) {
                network[0].emplace_back(1, 0, af);
            }
            break;
        case LayerType::hidden:
            for (int neuron = 0; neuron < number_of_neurons; neuron++) {
                network[layers].emplace_back(layers + 1, 0, af);
            }
            break;
        case LayerType::output:
            for (int neuron = 0; neuron < number_of_neurons; neuron++) {
                network[network.size() - 1].emplace_back(layers + 1, 0, af);
            }
            break;
    }

    layers += 1;
}

void Network::create_synapse() {
    static std::random_device sepsis;
    static std::default_random_engine gen2(sepsis());
    std::uniform_real_distribution<float> dis2(0.1f, 1.0f);

    for (size_t i = 0; i < network.size() - 1; ++i) {
        synapse.emplace_back();

        if (network[i + 1].size() != 1) {
            for (size_t j = 0; j < network[i + 1].size(); ++j) {
                std::vector<float> demi_synapse;

                for (size_t k = 0; k < network[i].size(); ++k) {
                    float c_weight = dis2(gen2);
                    demi_synapse.push_back(c_weight);
                }

                synapse[i].emplace_back(std::move(demi_synapse));
            }
        } else {
            for (size_t j = 0; j < network[i].size(); ++j) {
                std::vector<float> demi_synapse;

                for (size_t k = 0; k < network[i + 1].size(); ++k) {
                    float c_weight = dis2(gen2);
                    demi_synapse.push_back(c_weight);
                }

                synapse[i].emplace_back(std::move(demi_synapse));
            }
        }
    }
}


void Network::build() {
    std::cout << "><><><><><>><><><><><><><><><><><><><><" << std::endl;
    std::cout << "     NETWORK WAS BUILT SUCCESSFULLY" << std::endl;
    std::cout << "><><><><><>><><><><><><><><><><><><><><" << std::endl;
    create_synapse();
}

void Network::show_network() {
    show_weights();
    show_synapse();

}

void Network::show_synapse() {
    for (int i = 0; i < layers - 1; i++) {
        std::cout << "Synapse weights from Layer " << i + 1 << " to Layer " << i + 2 << ":\n";
        std::cout << "-----------------------" << std::endl;
        for (auto &j: synapse[i]) {
            for (float k: j) {
                std::cout << k << std::setprecision(5) << ' ';
            }
            std::cout << '\n';
        }
        std::cout << "-----------------------" << std::endl;
    }
}

void Network::show_weights() {
    std::cout << "Neuron Weights" << std::endl;
    std::cout << "-----------------------" << std::endl;
    size_t max_neurons = 0;
    for (const auto &i: network) {
        max_neurons = std::max(max_neurons, i.size());
    }
    for (size_t i = 0; i < max_neurons; ++i) {
        for (auto &j: network) {
            if (i < j.size()) {
                std::cout << std::fixed << j[i].weight << " ";
            } else {
                std::cout << "    ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "-----------------------" << std::endl;
}


void Network::train(const std::vector<std::vector<float>>& data, const std::vector<std::vector<float>> &answer, Normalisation normalisation,
                    int epochs, float test_data_per, float train_speed) {

    auto start_time = std::chrono::high_resolution_clock::now();
    process_data(data,answer, normalisation);
    std::cout << "-----------------------" << std::endl;
    std::cout << "    START TRAINING" << std::endl;
    std::cout << "-----------------------" << std::endl;

    for (int epoch = 0; epoch < epochs; ++epoch) {
        int data_ind = -1;
        for (auto &j: data) {
            data_ind += 1;
            clear_weights();
            std::vector<std::vector<float>> demi_res;


            //Forward
            for (int lay = 0; lay < layers - 1; lay++) {

                for (int k = 0; k < network[lay].size(); k++) {
                    if (lay == 0) {
                        network[lay][k].weight = j[k];
                    } else {
                        network[lay][k].weight = demi_res[k][0];
                    }
                }

                //show_weights();

                size_t k = network[lay + 1].size();
                std::vector<std::vector<float>> local_inp = {};
                for (auto &d: network[lay]) {
                    local_inp.push_back({d.weight});
                }
                if (k == 1) {
                    std::vector<std::vector<float>> semi = {{}};
                    for (auto &h: synapse[lay])
                        semi[0].push_back(h[0]);

                    demi_res = through_layer(Matrix(semi), local_inp, network[lay][0].fn_type).getData();
                } else {
                    demi_res = through_layer(Matrix(synapse[lay]), local_inp,
                                             network[lay][0].fn_type).getData();
                }


                for (int neu = 0; neu < network[lay + 1].size(); neu++) {
                    network[lay + 1][neu].weight = demi_res[neu][0];
                }


                //show_weights();

            }



            // Back
            std::vector<std::vector<float>> errors;
            std::vector<std::vector<std::vector<float>>> errors_by_lay = {}; //first errors vec is last in this vec

            if (answer[data_ind].size() >= 2) {
                for (int len = 0; len < network[network.size() - 1].size(); len++) {
                    errors.push_back({answer[data_ind][len] - network[network.size() - 1][len].weight});
                }
            } else {
                for (int len = 0; len < network[network.size() - 1].size() - 1; len++) {
                    errors.push_back({answer[data_ind][len] - network[network.size() - 1][len].weight});

                }
            }
            errors_by_lay.push_back(errors);


            for (int lay = layers - 1; lay > 0; lay--) {

                size_t k = network[lay - 1].size();

                if (k == 1) {
                    std::vector<std::vector<float>> semi = {{}};
                    for (auto &h: synapse[lay - 1])
                        semi[0].push_back(h[0]);

                    demi_res = multiply(Matrix(semi).transpose(), Matrix(errors)).getData();
                } else {

                    demi_res = multiply(Matrix(synapse[lay - 1]).transpose(), Matrix(errors)).getData();
                }
                errors = demi_res;
                errors_by_lay.push_back(demi_res);


            }
            std::vector<Matrix> d_synapse;

            std::reverse(neu_out.begin(), neu_out.end());
            std::vector<Matrix> se_de;
            for (int lay = layers - 1; lay > 0; lay--) {
                Matrix demi_mat;
                Matrix alpha;

                demi_mat = multiply(Matrix(synapse[lay - 1]), convert(neu_out[lay - 1]));
                se_de.push_back(demi_mat);

                alpha = multiply(collect_with_derivatives(lay, demi_mat, (errors_by_lay[lay]), train_speed),
                                 Matrix(convert(neu_out[lay - 1])).transpose());
                d_synapse.push_back(alpha);
                alpha.showMatrix("ALPHA");

            }
            // Updating of weights
            std::reverse(d_synapse.begin(), d_synapse.end());
            for (int si = 0; si < d_synapse.size(); si++){


                if (!std::isnan(d_synapse[si][0][0])){
                    Matrix(synapse[si]).showMatrix("SYNAPSE_BEFORE");
                    Matrix(d_synapse[si]).showMatrix("UPD");
                    (Matrix(synapse[si]) + Matrix(d_synapse[si])).showMatrix("WAIT_SYNAPSE");
                    synapse[si] = (Matrix(synapse[si]) + Matrix(d_synapse[si])).getData();
                } else{
                    continue;
                }


            }
            show_synapse();



            neu_out.clear();
        }
        std::cout << "-----------------------" << std::endl;
        std::cout << "    FINISH TRAINING" << std::endl;
        std::cout << "-----------------------" << std::endl;
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "Training Time: " << duration.count() << " ms" << std::endl;


    }

}

Matrix Network::through_layer(const Matrix &weights, const std::vector<std::vector<float>> &input, FunctionType af) {
    Matrix inp(input);

    Matrix res = weights * inp;

    std::vector<std::vector<float>> out = res.getData();
    std::vector<std::vector<float>> updatedData = res.getData();

    std::vector<float> edge = {};
    for (auto &i: updatedData) {
        edge.push_back(i[0]);
        i[0] = call(af, i[0]);
    }


    Matrix updatedRes(updatedData);

    neu_out.push_back(edge);
    return updatedRes;
}

void Network::save(const char *filename) const {
    nlohmann::json output;

    for (int i = 0; i <= layers - 1; ++i) {
        if (i != layers - 1) {
            nlohmann::json &layer_data = output.emplace_back();

            layer_data[NETWORK_NUM_NEURONS_NAME] = network[i].size();
            layer_data[NETWORK_ACTIVATION_FN_NAME] = function_type_to_string(network[i][0].fn_type);
            layer_data[NETWORK_SYNAPSE_NAME] = synapse[i];
        } else {
            nlohmann::json &layer_data = output.emplace_back();

            layer_data[NETWORK_NUM_NEURONS_NAME] = network[i].size();
            layer_data[NETWORK_ACTIVATION_FN_NAME] = function_type_to_string(network[i][0].fn_type);
        }

    }

    std::ofstream file(filename, std::ios::out | std::ios::binary);
    file << output.dump(/* indent= */ 4) << std::endl;
}

void Network::clear_weights() {
    for (auto &lay: network) {
        for (auto &neu: lay) {
            neu.weight = 0.0f;
        }
    }
}

Matrix Network::multiply(const Matrix &weights, const Matrix &input) {
    Matrix res = weights * input;
    return res;
}

Matrix Network::convert(const std::vector<float>& inp) {
    std::vector<std::vector<float>> out;
    for (float lene: inp) {
        out.push_back({lene});
    }
//    Matrix(out).showMatrix("LENA");
    return Matrix(out);
}

Matrix Network::collect_with_derivatives(int cur_lay, Matrix input, std::vector<std::vector<float>> errors, float speed) {
    using namespace Derivatives;
    std::vector<std::vector<float>> out;

    for (int layer = 0; layer < input.getData().size(); layer++) {
        float using_func;
        network[cur_lay][0].fn_type;
        switch (network[cur_lay][0].fn_type) {
            case FunctionType::sigmoid:
                using_func = Derivatives::sigmoid_derivative(errors[layer][0]);
            case FunctionType::fast_sigmoid:
                using_func = Derivatives::fast_sigmoid_derivative(errors[layer][0]);
            case FunctionType::silu:
                using_func = Derivatives::silu_derivative(errors[layer][0]);
            case FunctionType::tanh:
                using_func = Derivatives::tanh_derivative(errors[layer][0]);
        }

        out.push_back({-1 * speed * errors[layer][0] * using_func});

    }


    std::reverse(out.begin(), out.end());
    return Matrix(out);
}

void Network::process_data(std::vector<std::vector<float>> input_data, std::vector<std::vector<float>> output_data, Normalisation norm) {
    Data_Worker::check_data(input_data, output_data, network);
    Data_Worker::call()
}
