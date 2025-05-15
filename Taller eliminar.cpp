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

void mostrar(Pelicula *nodo) {
    if (nodo != NULL) {
        mostrar(nodo->izq);
        cout << "\nNombre: " << nodo->nombre;
        cout << "\nAño: " << nodo->anio;
        cout << "\nGénero: " << nodo->genero;
        cout << "\nRecaudación: " << nodo->recaudacion << " millones\n";
        mostrar(nodo->der);
    }
}

void recorridoInorden(Pelicula *nodo) {
    if (nodo != NULL) {
        recorridoInorden(nodo->izq);
        cout << nodo->nombre << " (" << nodo->recaudacion << "M) -> ";
        recorridoInorden(nodo->der);
    }
}

void recorridoPreorden(Pelicula *nodo) {
    if (nodo != NULL) {
        cout << nodo->nombre << " (" << nodo->recaudacion << "M) -> ";
        recorridoPreorden(nodo->izq);
        recorridoPreorden(nodo->der);
    }
}

void recorridoPosorden(Pelicula *nodo) {
    if (nodo != NULL) {
        recorridoPosorden(nodo->izq);
        recorridoPosorden(nodo->der);
        cout << nodo->nombre << " (" << nodo->recaudacion << "M) -> ";
    }
}

void buscarPorNombre(Pelicula *nodo, const char *nombreBuscado) {
    if (nodo != NULL) {
        buscarPorNombre(nodo->izq, nombreBuscado);
        if (strcmp(nodo->nombre, nombreBuscado) == 0) {
            cout << "\nPelícula encontrada:\n";
            cout << "Nombre: " << nodo->nombre << endl;
            cout << "Año: " << nodo->anio << endl;
            cout << "Género: " << nodo->genero << endl;
            cout << "Recaudación: " << nodo->recaudacion << " millones\n";
        }
        buscarPorNombre(nodo->der, nombreBuscado);
    }
}

void buscarPorGenero(Pelicula *nodo, const char *generoBuscado) {
    if (nodo != NULL) {
        buscarPorGenero(nodo->izq, generoBuscado);
        if (strcmp(nodo->genero, generoBuscado) == 0) {
            cout << "\nNombre: " << nodo->nombre;
            cout << "\nAño: " << nodo->anio;
            cout << "\nRecaudación: " << nodo->recaudacion << " millones\n";
        }
        buscarPorGenero(nodo->der, generoBuscado);
    }
}

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

void mostrarFracasos() {
    Pelicula *fracasos[3] = {NULL, NULL, NULL};
    encontrarFracasos(raiz, fracasos);
    cout << "\n--- 3 Fracasos Taquilleros ---\n";
    for (int i = 0; i < 3 && fracasos[i] != NULL; i++) {
        cout << "\nNombre: " << fracasos[i]->nombre;
        cout << "\nRecaudación: " << fracasos[i]->recaudacion << " millones\n";
    }
}

// Buscar nodo y su padre
Pelicula* buscarConPadre(Pelicula *actual, const char *nombre, Pelicula **padre) {
    *padre = NULL;
    while (actual != NULL) {
        if (strcmp(actual->nombre, nombre) == 0) {
            return actual;
        }
        *padre = actual;
        if (strcmp(nombre, actual->nombre) < 0) {
            actual = actual->izq;
        } else {
            actual = actual->der;
        }
    }
    return NULL;
}

// Eliminar nodo
void eliminar() {
    if (raiz == NULL) {
        cout << "El árbol está vacío.\n";
        return;
    }

    char nombreEliminar[100];
    cin.ignore();
    cout << "Ingrese el nombre de la película a eliminar: ";
    cin.getline(nombreEliminar, 100);

    Pelicula *padre = NULL;
    Pelicula *target = buscarConPadre(raiz, nombreEliminar, &padre);

    if (target == NULL) {
        cout << "Película no encontrada.\n";
        return;
    }

    // Caso con dos hijos
    if (target->izq != NULL && target->der != NULL) {
        Pelicula *sustituto = target->der;
        Pelicula *padreSustituto = target;

        while (sustituto->izq != NULL) {
            padreSustituto = sustituto;
            sustituto = sustituto->izq;
        }

        strcpy(target->nombre, sustituto->nombre);
        target->anio = sustituto->anio;
        strcpy(target->genero, sustituto->genero);
        target->recaudacion = sustituto->recaudacion;

        // Eliminar el nodo sustituto (que tiene como máximo un hijo)
        if (padreSustituto->izq == sustituto) {
            padreSustituto->izq = (sustituto->der != NULL) ? sustituto->der : NULL;
        } else {
            padreSustituto->der = (sustituto->der != NULL) ? sustituto->der : NULL;
        }

        free(sustituto);
    } else {
        Pelicula *hijo = (target->izq != NULL) ? target->izq : target->der;

        if (padre == NULL) {
            raiz = hijo;
        } else if (padre->izq == target) {
            padre->izq = hijo;
        } else {
            padre->der = hijo;
        }

        free(target);
    }

    cout << "Película eliminada correctamente.\n";
}

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
        cout << "\n7. Eliminar película";
        cout << "\n8. Salir";
        cout << "\nOpción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: agregar(); break;
            case 2: mostrar(raiz); break;
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
                mostrarFracasos(); break;
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
                eliminar(); break;
            case 8:
                cout << "Saliendo...\n"; break;
            default:
                cout << "Opción inválida.\n";
        }
    } while (opcion != 8);

    return 0;
}
