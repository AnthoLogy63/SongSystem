#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int main() {
    ifstream file("ratings.csv");
    string linea;

    while (getline(file, linea)) {
        stringstream ss(linea);
        string celda;
        vector<string> fila;

        while (getline(ss, celda, ',')) {
            fila.push_back(celda);
        }

        for (const string& campo : fila) {
            cout << campo << " ";
        }
        cout << endl;
    }

    file.close();
    return 0;
}
