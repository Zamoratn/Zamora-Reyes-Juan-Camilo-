//Juan Camilo Zamora Reyes y Mauricio Orozco Montilla 
#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

struct Pelicula {
    char nombre[100];
    int anio;
    char genero[50];
    float recaudacion;
    Pelicula *izq;
    Pelicula *der;
};

Pelicula *raiz = NULL;
Pelicula *nueva = NULL;

// Insertar en el árbol binario (mayores a la derecha, menores o iguales a la izquierda)
void insertar(Pelicula *actual) {
    if (nueva->recaudacion <= actual->recaudacion) {
        if (actual->izq == NULL) {
            actual->izq = nueva;
        } else {
            insertar(actual->izq);
        }
    } else {
        if (actual->der == NULL) {
            actual->der = nueva;
        } else {
            insertar(actual->der);
        }
    }
}

// Agregar una nueva película
void agregar() {
    nueva = (Pelicula*)malloc(sizeof(Pelicula));
    nueva->izq = nueva->der = NULL;

    cin.ignore();
    cout << "Nombre: ";
    cin.getline(nueva->nombre, 100);
    cout << "Año: ";
    cin >> nueva->anio;
    cin.ignore();
    cout << "Género: ";
    cin.getline(nueva->genero, 50);
    cout << "Recaudación (millones): ";
    cin >> nueva->recaudacion;

    if (raiz == NULL) {
        raiz = nueva;
    } else {
        insertar(raiz);
    }

    cout << "Película registrada.\n";
}

// Mostrar las películas (inorden)
void mostrar(Pelicula *nodo) {
    if (nodo != NULL) {
        mostrar(nodo->izq);
        cout << "\nNombre: " << nodo->nombre;
        cout << "\nAño: " << nodo->anio;
        cout << "\nGénero: " << nodo->genero;
        cout << "\nRecaudación: $" << nodo->recaudacion << " millones\n";
        mostrar(nodo->der);
    }
}

// Recorridos
void recorridoInorden(Pelicula *nodo) {
    if (nodo != NULL) {
        recorridoInorden(nodo->izq);
        cout << nodo->nombre << " ($" << nodo->recaudacion << "M) -> ";
        recorridoInorden(nodo->der);
    }
}

void recorridoPreorden(Pelicula *nodo) {
    if (nodo != NULL) {
        cout << nodo->nombre << " ($" << nodo->recaudacion << "M) -> ";
        recorridoPreorden(nodo->izq);
        recorridoPreorden(nodo->der);
    }
}

void recorridoPosorden(Pelicula *nodo) {
    if (nodo != NULL) {
        recorridoPosorden(nodo->izq);
        recorridoPosorden(nodo->der);
        cout << nodo->nombre << " ($" << nodo->recaudacion << "M) -> ";
    }
}

// Buscar una película por nombre
void buscarPorNombre(Pelicula *nodo, const char *nombreBuscado) {
    if (nodo != NULL) {
        buscarPorNombre(nodo->izq, nombreBuscado);
        if (strcmp(nodo->nombre, nombreBuscado) == 0) {
            cout << "\nPelícula encontrada:\n";
            cout << "Nombre: " << nodo->nombre << endl;
            cout << "Año: " << nodo->anio << endl;
            cout << "Género: " << nodo->genero << endl;
            cout << "Recaudación: $" << nodo->recaudacion << " millones\n";
        }
        buscarPorNombre(nodo->der, nombreBuscado);
    }
}

// Buscar películas por género
void buscarPorGenero(Pelicula *nodo, const char *generoBuscado) {
    if (nodo != NULL) {
        buscarPorGenero(nodo->izq, generoBuscado);
        if (strcmp(nodo->genero, generoBuscado) == 0) {
            cout << "\nNombre: " << nodo->nombre;
            cout << "\nAño: " << nodo->anio;
            cout << "\nRecaudación: $" << nodo->recaudacion << " millones\n";
        }
        buscarPorGenero(nodo->der, generoBuscado);
    }
}

// Obtener los 3 fracasos taquilleros (menor recaudación)
void encontrarFracasos(Pelicula *nodo, Pelicula *fracasos[3]) {
    if (nodo == NULL) return;
    encontrarFracasos(nodo->izq, fracasos);

    for (int i = 2; i >= 0; i--) {
        if (fracasos[i] == NULL || nodo->recaudacion < fracasos[i]->recaudacion) {
            for (int j = 0; j < i; j++) fracasos[j] = fracasos[j+1];
            fracasos[i] = nodo;
            break;
        }
    }

    encontrarFracasos(nodo->der, fracasos);
}

// Mostrar los 3 fracasos taquilleros
void mostrarFracasos() {
    Pelicula *fracasos[3] = {NULL, NULL, NULL};
    encontrarFracasos(raiz, fracasos);
    cout << "\n--- 3 Fracasos Taquilleros ---\n";
    for (int i = 0; i < 3 && fracasos[i] != NULL; i++) {
        cout << "\nNombre: " << fracasos[i]->nombre;
        cout << "\nRecaudación: $" << fracasos[i]->recaudacion << " millones\n";
    }
}

// Menú principal
int main() {
    int opcion;
    char nombreBuscado[100];
    char generoBuscado[50];

    do {
        cout << "\n1. Agregar película";
        cout << "\n2. Mostrar películas";
        cout << "\n3. Buscar por nombre";
        cout << "\n4. Buscar por género";
        cout << "\n5. Mostrar 3 fracasos taquilleros";
        cout << "\n6. Recorridos (Inorden, Preorden, Posorden)";
        cout << "\n7. Salir";
        cout << "\nOpción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                agregar(); break;
            case 2:
                mostrar(raiz); break;
            case 3:
                cin.ignore();
                cout << "Nombre a buscar: ";
                cin.getline(nombreBuscado, 100);
                buscarPorNombre(raiz, nombreBuscado);
                break;
            case 4:
                cin.ignore();
                cout << "Género a buscar: ";
                cin.getline(generoBuscado, 50);
                buscarPorGenero(raiz, generoBuscado);
                break;
            case 5:
                mostrarFracasos();
                break;
            case 6:
                cout << "\n--- Recorrido Inorden ---\n";
                recorridoInorden(raiz);
                cout << "NULL\n";

                cout << "\n--- Recorrido Preorden ---\n";
                recorridoPreorden(raiz);
                cout << "NULL\n";

                cout << "\n--- Recorrido Posorden ---\n";
                recorridoPosorden(raiz);
                cout << "NULL\n";
                break;
            case 7:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opción inválida.\n";
        }
    } while (opcion != 7);

    return 0;
}
