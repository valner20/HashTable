#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <cmath>
#include <iostream>
#include <fstream>

template <typename T>
class HashTable {
private: 
//Se crea la estructura del nodo
        struct nodo{
            T palabra;
            nodo *siguiente;
            nodo(T palabra){
                this->palabra = palabra;
                this->siguiente = nullptr;
            }
        };

public:
//se crea el constructor con las variales necesarias
    int colisiones;
    int ingresados;
    unsigned long int tamaño;
    nodo **tabla;
    ulong num_bits;
    HashTable(int num){
        num_bits = num;
        ingresados = 0;
        colisiones = 0;
        tamaño = pow(2,num_bits);
        //se crea la tabla con el tamaño requerido(pow 2,n_bits) y se inicializa con nullptr
        tabla = new nodo*[tamaño];
        for(int i = 0; i < tamaño; i++){
            tabla[i] = nullptr;
        }
    }
    
    unsigned long int hashXors(T);
    void leer();
    bool insertar(unsigned long int, T);
    void resize();
    void operador(); 
    void imprimirlista(int index);
};
template <typename T>
unsigned long int HashTable<T>::hashXors(T palabra){
    unsigned long int resultado =0;
    unsigned long int fragmento = 0;
    unsigned long int bits_actuales=0;

    for(ulong i = 0; i < palabra.size(); i++){
        fragmento = (fragmento << 8) | static_cast<int>(palabra[i]); //se desplaza 8 bits a la izquierda y se suma el valor de la letra con un or
        bits_actuales +=8;//cuenta los bits actuales de los fragmentos del dato
        while(bits_actuales >= num_bits){
            unsigned long int fragmento_xor = fragmento >> (bits_actuales- num_bits); /* Extrae los 'num_bits' menos significativos de 'fragmento' desplazándolo a la derecha, 
 alineando así el fragmento que se utilizará en la operación XOR.
 como ejemplo: 0b110101100 >> 5  ->  0b000000110 (los 4 bits menos significativos extraídos son '0110' que quedaran de complemento para el siguiente fragmento )
 */
            resultado ^=fragmento_xor;// se realiza el xor con el resultado de la fragmentacion
            fragmento &= (1 << (bits_actuales-num_bits))-1 ; // Mantiene solo los 'bits_actuales - num_bits' bits menos significativos de 'fragmento',  
// Ejemplo:  
// 1 << 5  ->  0b00100000  (genera una máscara con un bit '1' en la posición 5)  
// (1 << 5) - 1  ->  0b00011111  (convierte la máscara en 5 bits de '1')  
// Aplicando la máscara con AND:  
// 0b110101100 & 0b00011111  ->  0b000001100 (mantiene solo los 5 bits menos significativos '01100') que quedaran 
            bits_actuales -= num_bits; // se resta el número de bits utilizados en la operación XOR

        }
    }
    resultado ^= fragmento; //opera con xor los ultimos bits que quedan
    return resultado & ((1 << num_bits)-1); //asegura que se conserve el resultado siempre en el rango de bits definido
}
template <typename T>
bool HashTable<T>::insertar(unsigned long int clave, T palabra){
    //verifica las colisiones y toma en cuenta el caso indeseado donde las colisiones sean constantes a los ingresados
    if((double)colisiones/tamaño  > 0.7)resize();
    nodo *nuevo = new nodo(palabra);
    if(tabla[clave]){
        nodo *iterador = tabla[clave];
        //busca la palabra, evita repetidos, la inserta y suma colisiones
        while(iterador ->siguiente) {if(iterador->palabra == palabra || iterador->siguiente->palabra == palabra){std:: cout<<"palabra repetida"<<std::endl;return false;}
        iterador =  iterador-> siguiente;}

        iterador->siguiente = nuevo;
        ingresados++;
        return true;
    }
    //si esta vacio solo inserta
    tabla[clave] = nuevo;
    ingresados++;
    return false;
}

template <typename T>
void HashTable<T>::leer(){
     std::ifstream file("/usr/share/dict/words");
    if(!file)
        std::cout<< "error al abrir el archivo";
    std::string lineas;
    while(getline(file, lineas)){
        if(insertar(hashXors(lineas), lineas))colisiones++;        
}
}
template <typename T>
void HashTable<T>::resize() {
    ingresados=0;
    std::cout << "Radio de colisiones : " <<  (double)colisiones/ tamaño << std::endl<< "\n";
    int bits_antiguos = num_bits;
    num_bits++;
    colisiones=0;
    unsigned long int nuevo_tamaño = tamaño * 2;
    nodo **nueva_tabla = new nodo*[nuevo_tamaño];
    for (int i = 0; i < nuevo_tamaño; i++) {
        nueva_tabla[i] = nullptr;
    }
    
    // Guarda la tabla antigua
    nodo **tabla_antigua = tabla;
    int tamaño_antiguo = tamaño;
    
    // Actualiza la tabla y el tamaño de la instancia para que insertar use la nueva tabla
    tabla = nueva_tabla;
    tamaño = nuevo_tamaño;
    
    std::cout << "Rehasheando de : " << tamaño_antiguo << " a: " << nuevo_tamaño << " y de : " << bits_antiguos << " bits a : "<< num_bits << " bits" << std::endl<< "\n";
    // Rehash: se insertan los nodos de la tabla antigua en la nueva tabla
    for (int i = 0; i < tamaño_antiguo; i++) {
        nodo *iterador = tabla_antigua[i];
        while (iterador) {
            unsigned long int nueva_clave = hashXors(iterador->palabra);
            // Se usa insertar para colocar el nodo en la nueva tabla
            if (insertar(nueva_clave, iterador->palabra))
                colisiones++;
            iterador = iterador->siguiente;
        }
    }
    
    delete tabla_antigua;
}
template <typename T>
void HashTable<T>::imprimirlista(int index){
    if(tabla[index]){
        nodo *iterador = tabla[index];
        while(iterador){
            std::cout<<iterador->palabra<<"   \n";
            iterador=iterador->siguiente;
            }
    }
}
template <typename T>
void HashTable<T>::operador() {
    int exit =1;
    T dato;
    int opciones;
    while(exit){
        
        std::cout<<"1. Insertar un elemento\n";
        std::cout<<"2. Buscar un elemento\n";
        std::cout<<"3. Mostrar datos de la tabla\n";
        std::cout<<"4. Salir\n";
        std::cout<<"Opcion: \n";
        std::cin >>opciones;
        switch (opciones)
        {
        case 1:
            
            std::cout<<"Ingrese el dato a insertar: \n";
            std::cin>>dato;
            if(insertar(hashXors(dato), dato))colisiones++;
            std::cout<< dato<<" se ha insertado en la tabla en la posicion : "<<hashXors(dato)<<"\n";
            break;
        case 2:
    std::cout << "======================================Ingrese lo que desea buscar============================================"<< "\n";
    std ::cin >> dato;
    {
    int index = hashXors(dato);
    bool imprimir = false;
    bool encontrado= false;

    nodo *iterador = tabla[index];
    while (iterador) {
        if (iterador->palabra == dato) {
            std::cout << "======================================Encontrado============================================" << "\n";            
            std::cout << iterador->palabra << " en indice : "<< index << std::endl;
            encontrado = true;
            std::cout << "============================¿Quieres imprimir la lista en ese indice? Ingresar 1 para Si y 0 Para no==============================================" << "\n";
            std::cin >> imprimir;
            if(imprimir)imprimirlista(index);
            break;
            }
            iterador = iterador->siguiente;
            }
            if(!encontrado)std::cout << "======================================No se encontro el dato============================================" << "\n";}
        break;
        case 3:
            std::cout << "Colisiones finales:  "<< colisiones<<"\n";
            std::cout << "Radio de colisiones finales: " <<  (double)colisiones/tamaño << std::endl;
            std::cout << "Cantidad de ingresados: "<< ingresados<<"\n";
            std::cout << "Tamaño final de la tabla: "<< tamaño<<"\n\n";
            break;

        case 4: 
            exit = 0;
            break;
        
        default:
        std:: cout << "======================================Opcion invalida============================================" << std::endl;

            break;
        }
    }
}


int main(){
    HashTable<std::string> tabla= HashTable<std::string>(14);
    tabla.leer();
    std::cout << "Colisiones finales:  "<< tabla.colisiones<<"\n";
    std::cout << "Radio de colisiones finales: " <<  (double)tabla.colisiones/ tabla.tamaño << std::endl;
    std::cout << "Cantidad de ingresados: "<< tabla.ingresados<<"\n";
    std::cout << "Tamaño final de la tabla: "<< tabla.tamaño<<"\n\n";
    tabla.operador();


}
#endif // HASHTABLE_H 
