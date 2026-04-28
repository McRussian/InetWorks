#include <iostream>

using namespace std;

template <typename T, int n, int m>
class TMatrix {
public:
    TMatrix();
    T& at(int i, int j);
    T mean();
    void print();
private:
    T data[n][m];
};

template <typename T, int n, int m>
TMatrix<T, n, m>::TMatrix() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            data[i][j] = T();
}

template <typename T, int n, int m>
T& TMatrix<T, n, m>::at(int i, int j) {
    return data[i][j];
}

template <typename T, int n, int m>
T TMatrix<T, n, m>::mean() {
    T sum = T();
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            sum += data[i][j];
    return sum / (n * m);
}

template <typename T, int n, int m>
void TMatrix<T, n, m>::print() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            cout << data[i][j] << "\t";
        cout << endl;
    }
}

int main() {
    // int matrix 3x3
    TMatrix<int, 3, 3> a;
    int val = 1;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            a.at(i, j) = val++;

    cout << "TMatrix<int, 3, 3>:" << endl;
    a.print();
    cout << "Mean: " << a.mean() << endl;

    cout << endl;

    // double matrix 2x4
    TMatrix<double, 2, 4> b;
    double d = 0.5;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 4; j++, d += 1.5)
            b.at(i, j) = d;

    cout << "TMatrix<double, 2, 4>:" << endl;
    b.print();
    cout << "Mean: " << b.mean() << endl;

    return 0;
}
