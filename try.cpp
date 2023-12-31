#include <iostream>
#include <vector>
#include <cmath>

// �㭪�� ��� Z-score ��ଠ����樨
std::vector<std::vector<double>> zScoreNormalization(const std::vector<std::vector<double>>& data) {
    // ��宦����� �।���� � �⠭���⭮�� �⪫������ ��� ������� �⮫��
    int rows = data.size();
    int cols = data[0].size();
    std::vector<double> mean(cols, 0.0);
    std::vector<double> stdDev(cols, 0.0);

    for (int j = 0; j < cols; ++j) {
        double sum = 0.0;
        for (int i = 0; i < rows; ++i) {
            sum += data[i][j];
        }
        mean[j] = sum / rows;

        double squaredSum = 0.0;
        for (int i = 0; i < rows; ++i) {
            squaredSum += pow(data[i][j] - mean[j], 2);
        }
        stdDev[j] = sqrt(squaredSum / rows);
    }

    // Z-score ��ଠ������
    std::vector<std::vector<double>> normalizedData(rows, std::vector<double>(cols, 0.0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            normalizedData[i][j] = (data[i][j] - mean[j]) / stdDev[j];
        }
    }

    return normalizedData;
}

int main() {
    // �ਬ�� ������ (��� ����� ������)
    std::vector<std::vector<double>> data = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
    };

    // ��ଠ������ ������ Z-score
    std::vector<std::vector<double>> normalizedDataZScore = zScoreNormalization(data);

    // �뢮� ��ଠ���������� ������ Z-score
    std::cout << "��ଠ��������� ����� Z-score:" << std::endl;
    for (const auto& row : normalizedDataZScore) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}



#include <iostream>
#include <vector>
#include <algorithm>

// �㭪�� ��� ��ଠ����樨 ����⨫��
std::vector<std::vector<double>> quantileNormalization(const std::vector<std::vector<double>>& data) {
    int rows = data.size();
    int cols = data[0].size();

    std::vector<std::vector<double>> normalizedData(rows, std::vector<double>(cols, 0.0));

    // �������� �६����� ����� ������ ��� ���஢��
    std::vector<std::vector<double>> tempData = data;

    for (int j = 0; j < cols; ++j) {
        std::sort(tempData.begin(), tempData.end(), [j](const std::vector<double>& a, const std::vector<double>& b) {
            return a[j] < b[j];
        });

        for (int i = 0; i < rows; ++i) {
            normalizedData[i][j] = (i + 1.0) / rows;
        }

        for (int i = 0; i < rows; ++i) {
            data[i][j] = normalizedData[i][j];
        }
    }

    return data;
}

int main() {
    // �ਬ�� ������ (��� ����� ������)
    std::vector<std::vector<double>> data = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
    };

    // ��ଠ������ ������ ����⨫��
    std::vector<std::vector<double>> normalizedDataQuantile = quantileNormalization(data);

    // �뢮� ��ଠ���������� ������ ����⨫��
    std::cout << "��ଠ��������� ����� ����⨫��:" << std::endl;
    for (const auto& row : normalizedDataQuantile) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

#include <iostream>
#include <vector>
#include <cmath>

// �㭪�� ��� ��ଠ����樨 �� ���ᨬ��쭮�� ��᮫�⭮�� ���祭��
std::vector<std::vector<double>> maxAbsNormalization(const std::vector<std::vector<double>>& data) {
    int rows = data.size();
    int cols = data[0].size();

    std::vector<double> maxAbsValues(cols, 0.0);

    // ��宦����� ���ᨬ��쭮�� ��᮫�⭮�� ���祭�� ��� ������� �⮫��
    for (int j = 0; j < cols; ++j) {
        double maxAbs = 0.0;
        for (int i = 0; i < rows; ++i) {
            double absVal = std::abs(data[i][j]);
            if (absVal > maxAbs) {
                maxAbs = absVal;
            }
        }
        maxAbsValues[j] = maxAbs;
    }

    // ��ଠ������ �� ���ᨬ��쭮�� ��᮫�⭮�� ���祭��
    std::vector<std::vector<double>> normalizedData(rows, std::vector<double>(cols, 0.0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            normalizedData[i][j] = data[i][j] / maxAbsValues[j];
        }
    }

    return normalizedData;
}

int main() {
    // �ਬ�� ������ (��� ����� ������)
    std::vector<std::vector<double>> data = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
    };

    // ��ଠ������ ������ �� ���ᨬ��쭮�� ��᮫�⭮�� ���祭��
    std::vector<std::vector<double>> normalizedDataMaxAbs = maxAbsNormalization(data);

    // �뢮� ��ଠ���������� ������ �� ���ᨬ��쭮�� ��᮫�⭮�� ���祭��
    std::cout << "��ଠ��������� ����� �� ���ᨬ��쭮�� ��᮫�⭮�� ���祭��:" << std::endl;
    for (const auto& row : normalizedDataMaxAbs) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}



