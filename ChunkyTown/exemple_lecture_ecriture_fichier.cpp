#include <iostream>
#include <fstream>
#include <cstdlib>

void FichierTexte()
{
    std::ofstream sortie("fichier.txt"); // std::fstream::app
    if(!sortie.is_open())
    {
        std::cerr << "Erreur d'ouverture de fichier" << std::endl;
        return;
    }

    sortie << "Premiere ligne de plusieurs mots" << std::endl;
    sortie << "Age: " << 6 << std::endl;
    sortie.close();


    std::ifstream entree("fichier.txt");
    if(!entree.is_open())
    {
        std::cerr << "Erreur d'ouverture de fichier" << std::endl;
        return;
    }

    std::string mot;
    std::string line;
    int age;
    entree >> mot;
    std::cout << mot << std::endl;

    std::getline(entree, line);
    std::cout << line << std::endl;

    entree >> mot;
    entree >> age;
    std::cout << age << std::endl;


    // TODO montrer eof..
}


void FichierBinaire()
{
    srand(time(0));
    char data[1024];
    for(int i = 0; i < sizeof(data); ++i)
        data[i] = rand() % 256;

    std::ofstream sortie("fichier.bin", std::fstream::binary);
    sortie.write(data, sizeof(data));
    sortie.close();


    // Relire le fichier et comparer...
    std::ifstream entree("fichier.bin", std::fstream::binary);

    // Obtenir la taille du fichier
    entree.seekg(0, std::ios_base::end);
    int size = entree.tellg();
    entree.seekg(0, std::ios_base::beg);

    char* data2 = new char[size];
    entree.read(data2, size);
    entree.close();

    // Comparaison
    bool pareil = true;
    for(int i = 0; i < size; ++i)
    {
        if(data[i] != data2[i])
        {
            pareil = false;
            break;
        }
    }

    std::cout << "Les donnees sont " << (pareil ? "pareilles" : "differentes") << std::endl;


    delete [] data2;
}

int main()
{
    FichierTexte();
    FichierBinaire();

}
