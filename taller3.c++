#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <iostream>
#include <fstream>
#include <string>
#include <optional>
class HashTable {
    private: 
        struct nodo{
            std::string palabra;
            nodo *siguiente;
            nodo(std::string palabra){
                this->palabra = palabra;
                siguiente = nullptr;
            }
        };
public:
int colisiones;
int ingresados;
int num_bits;
int tamaño;
    nodo **tabla;
    HashTable(){
        tamaño = 104334;
        ingresados =0;
        colisiones =0;
        num_bits = 8;
        tabla = new nodo*[tamaño];
        for(int i = 0; i < tamaño; i++){
            tabla[i] = nullptr;
        }
    }

    // Insert or update a key-value pair.
    void setKey(const std::string& key, int value);

    // Get value by key. Returns nullopt if key is not found.
    std::optional<int> getKey(const std::string& key) const;
    unsigned long int hashSummation(std::string characters, int);
    unsigned long int hashProducts(std::string characters, int);
    unsigned long int hashXORs(std::string characters);
    void resize();
    void insertar(std::string);
    // Remove all key-value pairs from the hash table.
    void clear();
    void mostrar()
    {
        nodo *j;
        for(int i = 0; i < tamaño;i++){
            j = tabla[i];
            while(j != nullptr){
                std::cout <<i << " : " << j->palabra <<" -> " << j -> palabra << "   ";
                j = j->siguiente; 

            }
        }
        
    }
};

    unsigned long int HashTable::hashSummation(std::string characters, int num_bits){
        int i = 0;
        unsigned long int sum =0;
        while(characters[i]){
             sum +=characters[i];
            i++;
        }
        return (sum & ((1 << num_bits)-1));
    }


     unsigned long int HashTable::hashProducts(std::string characters,int num_bits){
        int i =0;
        unsigned long int product = 1;
        while(characters[i]){
             product *= characters[i];
            i++;
            product = product & ((1 << num_bits)-1);
        }
        return product;
    }
    unsigned long int HashTable::hashXORs(std::string characters){
        unsigned long int xorr= 0;
        int i =0;
        while(characters[i]){
            xorr ^= (characters[i]) ;

            i++;
        }
        return (xorr & ((1 << 14)-1));
    }

    void HashTable::insertar(std::string linea) {
        nodo *nuevo = new nodo(linea);
        int i = hashSummation(linea, num_bits);
        nodo *iterador = tabla[i];
            if(tabla[i]){
            colisiones++;
            while(iterador->siguiente)iterador = iterador->siguiente;
            iterador ->siguiente = nuevo;
            
            }
            else{  
                tabla[i] = nuevo;
            }
          
    }



int main(){
    HashTable l;
    std::ifstream file("/usr/share/dict/words");
    if(!file)
        std::cout<< "error al abrir el archivo";
    std::string lineas;
    while(std::getline(file, lineas)){
        l.insertar(lineas);
   }
      std::cout << l.colisiones<< "\n";
            std::cout << l.tamaño;

}
 
#endif // HASHTABLE_H
