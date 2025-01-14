#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Estructuras
struct Producto {
    string categoria;
    string nombre;
    int cantidad;
    string lugarCompra; 
    string fecha;       
};

struct Categoria {
    string nombre;
};

struct Usuario {
    string nombre;
    string contrasena;
    string rol; // "admin" o "user"
};

// Variables globales
vector<Usuario> usuarios;
vector<Categoria> categorias;
vector<Producto> listaUsuario;


void mostrarListaGlobal() {
    cout << "\nLista global de pedidos (usuarios no admin):\n";
    for (const auto& usuario : usuarios) {
        if (usuario.rol != "admin") {
            string archivoLista = usuario.nombre + "_lista.txt";

            // Cargar la lista de compras del usuario actual
            vector<Producto> listaComprasUsuario;
            ifstream file(archivoLista);
            if (file.is_open()) {
                string linea;
                while (getline(file, linea)) {
                    stringstream ss(linea);
                    string categoria, nombre, lugarCompra, fecha;
                    int cantidad;
                    getline(ss, categoria, ',');
                    getline(ss, nombre, ',');
                    ss >> cantidad;
                    ss.ignore(); // Ignorar la coma después de la cantidad
                    getline(ss, lugarCompra, ',');
                    getline(ss, fecha);
                    listaComprasUsuario.push_back({ categoria, nombre, cantidad, lugarCompra, fecha });
                }
                file.close();
            }

            // Mostrar la lista del usuario actual
            cout << "\nPedidos de " << usuario.nombre << ":\n";
            if (listaComprasUsuario.empty()) {
                cout << "  (Sin compras registradas)\n";
            }
            else {
                for (const auto& producto : listaComprasUsuario) {
                    cout << "  - " << producto.categoria << ": " << producto.nombre
                        << " x" << producto.cantidad << "\n"
                        << "    Lugar de compra: " << producto.lugarCompra << "\n"
                        << "    Fecha de compra: " << producto.fecha << "\n";
                }
            }
        }
    }
}

// Funciones de carga y almacenamiento
void cargarUsuarios(const string& archivo) {
    ifstream file(archivo);
    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo de usuarios.\n";
        return;
    }
    string linea;
    while (getline(file, linea)) {
        size_t pos1 = linea.find(',');
        size_t pos2 = linea.rfind(',');

        string nombre = linea.substr(0, pos1);
        string contrasena = linea.substr(pos1 + 1, pos2 - pos1 - 1);
        string rol = linea.substr(pos2 + 1);

        usuarios.push_back({ nombre, contrasena, rol });
    }
    file.close();
}

void cargarCategorias(const string& archivo) {
    ifstream file(archivo);
    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo de categorias.\n";
        return;
    }
    string linea;
    categorias.clear();
    while (getline(file, linea)) {
        categorias.push_back({ linea });
    }
    file.close();
}

void cargarListaUsuario(const string& archivo) {
    ifstream file(archivo);
    if (!file.is_open()) {
        listaUsuario.clear(); // Si no hay archivo, la lista empieza vacía.
        return;
    }
    string linea;
    listaUsuario.clear();
    while (getline(file, linea)) {
        stringstream ss(linea);
        string categoria, nombre, lugarCompra, fecha;
        int cantidad;

        getline(ss, categoria, ',');
        getline(ss, nombre, ',');
        ss >> cantidad;
        ss.ignore(); // Ignorar la coma después del número
        getline(ss, lugarCompra, ',');
        getline(ss, fecha);

        listaUsuario.push_back({ categoria, nombre, cantidad, lugarCompra, fecha });
    }
    file.close();
}

void guardarListaUsuario(const string& archivo) {
    ofstream file(archivo, ios::trunc);
    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo para guardar la lista de compras.\n";
        return;
    }
    for (const auto& producto : listaUsuario) {
        file << producto.categoria << "," << producto.nombre << ","
            << producto.cantidad << "," << producto.lugarCompra << ","
            << producto.fecha << "\n";
    }
    file.close();
}

// Autenticación
bool autenticar(string& nombre, string& rol) {
    cout << "Usuario: ";
    cin >> nombre;
    string contrasena;
    cout << "Contrasena: ";
    cin >> contrasena;

    for (const auto& usuario : usuarios) {
        if (usuario.nombre == nombre && usuario.contrasena == contrasena) {
            rol = usuario.rol;
            return true;
        }
    }
    return false;
}

// Funciones del menú
void mostrarCategorias() {
    cout << "\nCategorias disponibles:\n";
    for (const auto& categoria : categorias) {
        cout << "- " << categoria.nombre << "\n";
    }
}

void agregarProducto(const string& archivoLista) {
    string categoria, nombre, lugarCompra, fecha;
    int cantidad;

    mostrarCategorias();
    cout << "Seleccione una categoria: ";
    cin.ignore();
    getline(cin, categoria);

    auto it = find_if(categorias.begin(), categorias.end(),
        [&](const Categoria& c) { return c.nombre == categoria; });
    if (it == categorias.end()) {
        cout << "Categoria no encontrada.\n";
        return;
    }

    cout << "Nombre del producto: ";
    getline(cin, nombre);

    do {
        cout << "Cantidad: ";
        cin >> cantidad;
        if (cantidad < 1) {
            cout << "La cantidad debe ser al menos 1. Intentelo nuevamente.\n";
        }
    } while (cantidad < 1);

    cin.ignore(); // Limpiar el buffer de entrada
    cout << "Lugar de compra: ";
    getline(cin, lugarCompra);

    cout << "Fecha de compra ( DD-MM-YY): ";
    getline(cin, fecha);

    listaUsuario.push_back({ categoria, nombre, cantidad, lugarCompra, fecha });
    guardarListaUsuario(archivoLista);
    cout << "Producto agregado con exito.\n";
}

void eliminarProducto(const string& archivoLista) {
    string nombre;
    cout << "Nombre del producto a eliminar: ";
    cin.ignore();
    getline(cin, nombre);

    listaUsuario.erase(remove_if(listaUsuario.begin(), listaUsuario.end(),
        [&](const Producto& p) { return p.nombre == nombre; }),
        listaUsuario.end());
    guardarListaUsuario(archivoLista);
    cout << "Producto eliminado.\n";
}

void modificarProducto(const string& archivoLista) {
    string nombre;
    cout << "Nombre del producto a modificar: ";
    cin.ignore();
    getline(cin, nombre);

    for (auto& producto : listaUsuario) {
        if (producto.nombre == nombre) {
            cout << "Nueva cantidad: ";
            cin >> producto.cantidad;
            guardarListaUsuario(archivoLista);
            cout << "Producto modificado con exito.\n";
            return;
        }
    }
    cout << "Producto no encontrado.\n";
}

void mostrarListaUsuario() {
    cout << "\nLista de compras:\n";
    for (const auto& producto : listaUsuario) {
        cout << "- " << producto.categoria << ": " << producto.nombre
            << " x" << producto.cantidad << "\n"
            << "  Lugar de compra: " << producto.lugarCompra << "\n"
            << "  Fecha de compra: " << producto.fecha << "\n";
    }
}

void hacerCompra() {
    cout << "\nProcesar pedidos de usuarios:\n";
    for (auto& usuario : usuarios) {
        if (usuario.rol != "admin") {
            string archivoLista = usuario.nombre + "_lista.txt";

            // Cargar la lista de compras del usuario actual
            vector<Producto> listaComprasUsuario;
            ifstream file(archivoLista);
            if (file.is_open()) {
                string linea;
                while (getline(file, linea)) {
                    stringstream ss(linea);
                    string categoria, nombre;
                    int cantidad;
                    getline(ss, categoria, ',');
                    getline(ss, nombre, ',');
                    ss >> cantidad;
                    listaComprasUsuario.push_back({ categoria, nombre, cantidad });
                }
                file.close();
            }

            // Procesar la lista del usuario
            cout << "\nPedidos de " << usuario.nombre << ":\n";
            vector<Producto> nuevaLista; // Lista con los productos pospuestos
            for (auto& producto : listaComprasUsuario) {
                cout << "  - " << producto.categoria << ": " << producto.nombre << " x" << producto.cantidad << "\n";
                cout << "Desea procesar este producto? (1: Si, 0: Posponer): ";
                int opcion;
                cin >> opcion;

                if (opcion == 0) {
                    nuevaLista.push_back(producto); // Si se pospone, mantener el producto en la lista
                }
            }

            // Guardar la nueva lista del usuario
            ofstream outFile(archivoLista, ios::trunc);
            if (outFile.is_open()) {
                for (const auto& producto : nuevaLista) {
                    outFile << producto.categoria << "," << producto.nombre << "," << producto.cantidad << "\n";
                }
                outFile.close();
            }

            cout << "Pedidos procesados para " << usuario.nombre << ".\n";
        }
    }
    cout << "\nTodos los pedidos han sido procesados.\n";
}


void menuUsuario(const string& archivoLista) {
    int opcion;
    do {
        cout << "\n1. Agregar producto\n2. Eliminar producto\n3. Modificar producto\n4. Mostrar lista\n5. Cerrar sesion\nOpcion: ";
        cin >> opcion;
        switch (opcion) {
        case 1: agregarProducto(archivoLista); break;
        case 2: eliminarProducto(archivoLista); break;
        case 3: modificarProducto(archivoLista); break;
        case 4: mostrarListaUsuario(); break;
        case 5: break;
        default: cout << "Opcion no valida.\n";
        }
    } while (opcion != 5);
}

void menuAdmin() {
    int opcion;
    do {
        cout << "\n1. Mostrar categorias\n2. Ver lista global de pedidos\n3. Hacer compra\n4. Cerrar sesion\nOpcion: ";
        cin >> opcion;
        switch (opcion) {
        case 1: mostrarCategorias(); break;
        case 2: mostrarListaGlobal(); break;
        case 3: hacerCompra(); break; // Nueva opción
        case 4: break;
        default: cout << "Opcion no valida.\n";
        }
    } while (opcion != 4);
}


// Programa principal
int main() {
    string archivoUsuarios = "usuarios.txt";
    string archivoCategorias = "categorias.txt";

    cargarUsuarios(archivoUsuarios);
    cargarCategorias(archivoCategorias);

    bool continuar = true;
    while (continuar) {
        string nombre, rol;
        if (!autenticar(nombre, rol)) {
            cout << "Autenticacion fallida.\n";
            continue;
        }

        string archivoLista = nombre + "_lista.txt";
        cargarListaUsuario(archivoLista);

        if (rol == "admin") {
            menuAdmin();  // Menú del admin
        }
        else {
            menuUsuario(archivoLista);
        }

        cout << "\nDesea continuar? (1: Si, 0: No): ";
        cin >> continuar;
    }

    return 0;
}