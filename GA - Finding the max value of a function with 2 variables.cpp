#include <string>
#include <bits/stdc++.h>
#include <time.h>
using namespace std;

const int banyakGenerasi = 50;
const int banyakPopulasi = 60;
const int elitismNumber = 10;
const int mutasiRate = 5;
const int banyakGene = 8;

struct kromosom {
    string DNA;
};

struct titik {
    double x, y;
};

struct generasi {
    kromosom populasi[banyakPopulasi];
};

kromosom getRandomKromosom() {
    kromosom sebuahKromosom;
    sebuahKromosom.DNA = "";
    for (int i = 0; i < banyakGene*2; i++) {
        sebuahKromosom.DNA += to_string(rand()%2);
    }
    return sebuahKromosom;
}

double fitnessFuction(titik sebuahTitik) {
    return cos(pow(sebuahTitik.x, 2))*sin(pow(sebuahTitik.y, 2)) + sebuahTitik.x + sebuahTitik.y;
}

titik decodeKromosom(kromosom sebuahKromosom) {
    titik sebuahTitik;
    string xPertama = sebuahKromosom.DNA.substr(0, banyakGene);
    string xKedua = sebuahKromosom.DNA.substr(banyakGene, banyakGene*2);
    double constKanan = 0, constBawah = 0, x, y;
    for (int i = 0; i < banyakGene; i++) {
        constKanan += double(int(xPertama[i])%48)*pow(2, -i);
        constBawah += pow(2, -i);
    }
    x = -1 + ((2-(-1))*constKanan)/constBawah;
    constKanan = 0; constBawah = 0;
    for (int i = 0; i < banyakGene; i++) {
        constKanan += double(int(xKedua[i])%48)*pow(2, -i);
        constBawah += pow(2, -i);
    }
    y = -1 + ((1-(-1))*constKanan)/constBawah;
    sebuahTitik.x = x; sebuahTitik.y = y;
    return sebuahTitik;
}

kromosom seleksiParentviaRouletteWheel(generasi sebuahGenerasi) {
    int listSkor[banyakPopulasi]; double fitnessScore;
    for (int i = 0; i < banyakPopulasi; i++) {
        fitnessScore = 10*fitnessFuction(decodeKromosom(sebuahGenerasi.populasi[i]));
        listSkor[i] = int(fitnessScore);
        if (fitnessScore - int(fitnessScore) >= 0.5) {
            listSkor[i]++;
        }
    }
    vector <int> selectingProbability;
    for (int i = 0; i < banyakPopulasi; i++) {
        selectingProbability.push_back(i);
        for (int j = 0; j < listSkor[i]; j++) {
            selectingProbability.push_back(i);
        }
    }
    return sebuahGenerasi.populasi[selectingProbability[rand()%selectingProbability.size()]];
}

kromosom mating(generasi sebuahGenerasi) {
    kromosom parentP, parentK, childNew;
    parentP = seleksiParentviaRouletteWheel(sebuahGenerasi);
    parentK = seleksiParentviaRouletteWheel(sebuahGenerasi);
    int titikPotong = 1+rand()%((banyakGene*2)-1);
    childNew.DNA = parentP.DNA.substr(0, titikPotong) + parentK.DNA.substr(titikPotong, banyakGene*2);
    return childNew;
}

kromosom mutasi(kromosom sebuahKromosom) {
    for (int i = 0; i < banyakGene*2; i++) {
        if (rand()%100 + 1 <= mutasiRate) {
            if (sebuahKromosom.DNA[i] == '0') {
                sebuahKromosom.DNA[i] = '1';
            } else {
                sebuahKromosom.DNA[i] = '0';
            }
        }
    }
    return sebuahKromosom;
}

void showScoreGEN(generasi sebuahGenerasi, kromosom &bestKromosom) {
    for (int i = 0; i < banyakPopulasi; i++) {
        cout << sebuahGenerasi.populasi[i].DNA << " dengan skor: ";
        cout << fitnessFuction(decodeKromosom(sebuahGenerasi.populasi[i])) << endl;
        if (fitnessFuction(decodeKromosom(sebuahGenerasi.populasi[i])) > fitnessFuction(decodeKromosom(bestKromosom))) {
            bestKromosom = sebuahGenerasi.populasi[i];
        }
    }
    cout << "- end of generation -" << endl;
}

kromosom genElite[elitismNumber];

bool isSelected(vector<int> indexKromosom, int index) {
    for (int i = 0; i < indexKromosom.size(); i++) {
        if (indexKromosom[i] == index) {
            return true;
        }
    }
    return false;
}

void selectingElite(generasi suatuGenerasi) {
    vector <int> indexesK; int idxBesar;
    for (int j = 0; j < elitismNumber; j++) {
        idxBesar = 0;
        for (int i = 0; i < banyakPopulasi; i++) {
            if (!isSelected(indexesK, i)) {
                if (fitnessFuction(decodeKromosom(suatuGenerasi.populasi[idxBesar])) < fitnessFuction(decodeKromosom(suatuGenerasi.populasi[i]))) {
                    idxBesar = i;
                }
            }
        }
        indexesK.push_back(idxBesar);
        genElite[j] = suatuGenerasi.populasi[idxBesar];
    }
}

int main() {
    srand(time(NULL));
    vector <generasi> listGenerasi;
    generasi generasiPertama; kromosom bestKromosom;
    for (int i = 0; i < banyakPopulasi; i++) {
        generasiPertama.populasi[i] = getRandomKromosom();
    }
    bestKromosom = generasiPertama.populasi[0];
    showScoreGEN(generasiPertama, bestKromosom);
    listGenerasi.push_back(generasiPertama);
    seleksiParentviaRouletteWheel(listGenerasi[0]);
    while (listGenerasi.size() < banyakGenerasi) {
        cout << "Generasi ke-" << listGenerasi.size()+1 << endl;
        generasi generasiNew;
        selectingElite(listGenerasi[listGenerasi.size()-1]);
        for (int i = 0; i < elitismNumber; i++) {
            generasiNew.populasi[i] = genElite[i];
        }
        for (int i = elitismNumber; i < banyakPopulasi; i++) {
            generasiNew.populasi[i] = mating(listGenerasi[listGenerasi.size()-1]);
            generasiNew.populasi[i] = mutasi(generasiNew.populasi[i]);
        }
        showScoreGEN(generasiNew, bestKromosom);
        cout << "Kromosom terbaik: " << bestKromosom.DNA << " dengan skor: " << fitnessFuction(decodeKromosom(bestKromosom)) << endl;
        cout << "dengan titik x = " << decodeKromosom(bestKromosom).x << " titik y = " << decodeKromosom(bestKromosom).y << endl;
        listGenerasi.push_back(generasiNew);
    }
}