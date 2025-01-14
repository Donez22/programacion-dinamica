#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

// Estructuras y vectores declarados previamente
struct Producto {
    int id;
    std::string nombre;
    std::string categoria;
    double precio;
    int stock;
    int stockMinimo;
};

struct Cliente {
    int id;
    std::string nombre;
    double creditoMaximo;
    double creditoActual;
    std::vector<std::string> historial;
    std::vector<std::string> historialAbonos;
};

std::vector<Producto> productos;
std::vector<Cliente> clientes;


void cargarProductos();
void guardarProductos();
void cargarClientes();
void guardarClientes();
void gestionarProductos();
void gestionarClientes();
void realizarVenta();
void corteCaja();
void abonarCreditoCliente();
void imprimirMenu();
void resurtirStock();
void eliminarProducto();
void eliminarCliente();
void modificarCliente();


int main() {
    cargarProductos();
    cargarClientes();
    int opcion;
    do {
        imprimirMenu();
        std::cin >> opcion;
        switch (opcion) {
        case 1:
            gestionarProductos();
            break;
        case 2:
            gestionarClientes();
            break;
        case 3:
            realizarVenta();
            break;
        case 4:
            corteCaja();
            break;
        case 5:
            abonarCreditoCliente();
            break;
        case 6:
            guardarProductos();
            guardarClientes();
            std::cout << "Saliendo del programa...\n";
            break;
        default:
            std::cout << "Opcion invalida, intenta de nuevo.\n";
        }
    } while (opcion != 6);
    return 0;
}

void gestionarProductos() {
    int opcion;
    do {
        std::cout << "\n--- Gestionar Productos ---\n";
        std::cout << "1. Agregar producto\n";
        std::cout << "2. Listar productos\n";
        std::cout << "3. Resurtir stock de producto\n"; // Nueva opción
        std::cout << "4. Eliminar producto\n"; // Opción para eliminar producto
        std::cout << "5. Salir\n";
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;

        switch (opcion) {
        case 1: {
            Producto p;
            std::cout << "ID: ";
            std::cin >> p.id;
            std::cout << "Nombre: ";
            std::cin >> p.nombre;
            std::cout << "Categoria: ";
            std::cin >> p.categoria;
            std::cout << "Precio: ";
            std::cin >> p.precio;
            std::cout << "Stock: ";
            std::cin >> p.stock;
            std::cout << "Stock minimo: ";
            std::cin >> p.stockMinimo;
            productos.push_back(p);
            guardarProductos(); // Guardar cambios inmediatamente
            std::cout << "Producto agregado y guardado correctamente.\n";
            break;
        }
        case 2:
            for (const auto& p : productos) {
                std::cout << "ID: " << p.id << ", Nombre: " << p.nombre
                    << ", Categoria: " << p.categoria << ", Precio: " << p.precio
                    << ", Stock: " << p.stock << ", Stock Minimo: " << p.stockMinimo << "\n";
            }
            break;
        case 3:
            resurtirStock(); // Llama a la función para resurtir stock
            break;
        case 4:
            eliminarProducto(); // Llama a la función para eliminar producto
            break;
        case 5:
            std::cout << "Saliendo de la gestion de productos.\n";
            break;
        default:
            std::cout << "Opción invalida.\n";
        }
    } while (opcion != 5);
}

void imprimirMenu() {
    std::cout << "\n--- Tiendita ---\n";
    std::cout << "1. Gestionar productos\n";
    std::cout << "2. Gestionar clientes\n";
    std::cout << "3. Realizar venta\n";
    std::cout << "4. Corte de caja\n";
    std::cout << "5. Abonar a credito de cliente\n"; 
    std::cout << "6. Salir\n";
    std::cout << "Seleccione una opcion: ";
}



void abonarCreditoCliente() {
    std::cout << "\n--- Abonar a Credito de Cliente ---\n";

    // Mostrar clientes disponibles
    std::cout << "Clientes disponibles:\n";
    for (const auto& c : clientes) {
        std::cout << "ID: " << c.id << ", Nombre: " << c.nombre
            << ", Credito Maximo: $" << c.creditoMaximo
            << ", Credito Actual: $" << c.creditoActual << "\n";
    }

    // Seleccionar cliente
    int idCliente;
    std::cout << "Ingrese el ID del cliente: ";
    std::cin >> idCliente;

    auto itCliente = std::find_if(clientes.begin(), clientes.end(), [idCliente](const Cliente& c) {
        return c.id == idCliente;
        });

    if (itCliente == clientes.end()) {
        std::cout << "Cliente no encontrado.\n";
        return;
    }

    // Calcular deuda pendiente
    double deudaPendiente = itCliente->creditoMaximo - itCliente->creditoActual;
    std::cout << "Deuda pendiente para " << itCliente->nombre << ": $" << deudaPendiente << "\n";

    // Solicitar el monto a abonar
    double montoAbono;
    std::cout << "Ingrese el monto a abonar: ";
    std::cin >> montoAbono;

    // Validar que el abono no sea mayor que la deuda pendiente
    if (montoAbono > deudaPendiente) {
        std::cout << "El monto a abonar excede la deuda pendiente. Deuda actual: $" << deudaPendiente << "\n";
        return;
    }

    // Realizar el abono
    itCliente->creditoActual -= montoAbono; // Restar del crédito actual

    // Registrar el abono en el historial
    itCliente->historialAbonos.push_back("Abono de $" + std::to_string(montoAbono));

    std::cout << "Abono realizado exitosamente. Nuevo saldo de credito: $" << itCliente->creditoActual << "\n";
    std::cout << "Historial de abonos de " << itCliente->nombre << ":\n";
    for (const auto& abono : itCliente->historialAbonos) {
        std::cout << "- " << abono << "\n";
    }
}



void cargarProductos() {
    std::ifstream archivo("productos.txt");
    if (archivo.is_open()) {
        Producto p;
        while (archivo >> p.id >> p.nombre >> p.categoria >> p.precio >> p.stock >> p.stockMinimo) {
            productos.push_back(p);
        }
        archivo.close();
        std::cout << "Productos cargados correctamente.\n";
    }
    else {
        std::cout << "No se pudo abrir el archivo productos.txt.\n";
    }
}


void guardarProductos() {
    std::ofstream archivo("productos.txt");
    if (archivo.is_open()) {
        for (const auto& p : productos) {
            archivo << p.id << " " << p.nombre << " " << p.categoria << " "
                << p.precio << " " << p.stock << " " << p.stockMinimo << "\n";
        }
        archivo.close();
        std::cout << "Productos guardados correctamente.\n";
    }
    else {
        std::cout << "No se pudo abrir el archivo productos.txt para guardar.\n";
    }
}




void cargarClientes() {
    std::ifstream archivo("clientes.txt");
    if (archivo.is_open()) {
        Cliente c;
        std::string linea;
        while (archivo >> c.id >> c.nombre >> c.creditoMaximo >> c.creditoActual) {
            std::getline(archivo, linea); // Consumir el salto de línea
            c.historial.clear();
            c.historialAbonos.clear();

            // Leer historial de compras
            std::getline(archivo, linea);
            if (linea == "HISTORIAL_COMPRAS") {
                while (std::getline(archivo, linea) && linea != "FIN_HISTORIAL") {
                    c.historial.push_back(linea);
                }
            }

            // Leer historial de abonos
            std::getline(archivo, linea);
            if (linea == "HISTORIAL_ABONOS") {
                while (std::getline(archivo, linea) && linea != "FIN_ABONOS") {
                    c.historialAbonos.push_back(linea);
                }
            }

            clientes.push_back(c);
        }
        archivo.close();
        std::cout << "Clientes cargados correctamente.\n";
    }
    else {
        std::cout << "No se pudo abrir el archivo clientes.txt.\n";
    }
}





void guardarClientes() {
    std::ofstream archivo("clientes.txt");
    if (archivo.is_open()) {
        for (const auto& c : clientes) {
            archivo << c.id << " " << c.nombre << " " << c.creditoMaximo << " "
                << c.creditoActual << "\n";

            // Guardar historial de compras
            archivo << "HISTORIAL_COMPRAS\n";
            for (const auto& compra : c.historial) {
                archivo << compra << "\n";
            }
            archivo << "FIN_HISTORIAL\n";

            // Guardar historial de abonos
            archivo << "HISTORIAL_ABONOS\n";
            for (const auto& abono : c.historialAbonos) {
                archivo << abono << "\n";
            }
            archivo << "FIN_ABONOS\n";
        }
        archivo.close();
        std::cout << "Clientes guardados correctamente.\n";
    }
    else {
        std::cout << "No se pudo abrir el archivo clientes.txt para guardar.\n";
    }
}



void resurtirStock() {
    std::cout << "\n--- Resurtir Stock de Producto ---\n";

    // Mostrar productos disponibles
    std::cout << "Productos disponibles:\n";
    for (const auto& p : productos) {
        std::cout << "ID: " << p.id << ", Nombre: " << p.nombre
            << ", Stock actual: " << p.stock << ", Stock Minimo: " << p.stockMinimo << "\n";
    }

    // Seleccionar producto
    int idProducto;
    std::cout << "Ingrese el ID del producto a resurtir: ";
    std::cin >> idProducto;

    auto itProducto = std::find_if(productos.begin(), productos.end(), [idProducto](const Producto& p) {
        return p.id == idProducto;
        });

    if (itProducto == productos.end()) {
        std::cout << "Producto no encontrado.\n";
        return;
    }

    // Solicitar cantidad a resurtir
    int cantidad;
    std::cout << "Ingrese la cantidad a agregar al stock: ";
    std::cin >> cantidad;

    if (cantidad <= 0) {
        std::cout << "La cantidad debe ser mayor a 0.\n";
        return;
    }

    // Actualizar el stock del producto
    itProducto->stock += cantidad;
    std::cout << "Stock actualizado. Nuevo stock de " << itProducto->nombre << ": " << itProducto->stock << "\n";
}





void gestionarClientes() {
    int opcion;
    do {
        std::cout << "\n--- Gestionar Clientes ---\n";
        std::cout << "1. Agregar cliente\n";
        std::cout << "2. Listar clientes\n";
        std::cout << "3. Eliminar cliente\n";   // Nueva opción para eliminar cliente
        std::cout << "4. Modificar cliente\n";   // Nueva opción para modificar cliente
        std::cout << "5. Salir\n";
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;

        switch (opcion) {
        case 1: {
            Cliente c;
            std::cout << "ID: ";
            std::cin >> c.id;
            std::cout << "Nombre: ";
            std::cin >> c.nombre;
            std::cout << "Credito maximo: ";
            std::cin >> c.creditoMaximo;
            c.creditoActual = 0;
            clientes.push_back(c);
            guardarClientes(); // Guardar cambios inmediatamente
            std::cout << "Cliente agregado correctamente.\n";
            break;
        }
        case 2:
            for (const auto& c : clientes) {
                std::cout << "ID: " << c.id << ", Nombre: " << c.nombre
                    << ", Credito Maximo: " << c.creditoMaximo
                    << ", Credito Actual: " << c.creditoActual << "\n";
            }
            break;
        case 3:
            eliminarCliente(); // Llama a la función para eliminar cliente
            break;
        case 4:
            modificarCliente(); // Llama a la función para modificar cliente
            break;
        case 5:
            std::cout << "Saliendo de la gestion de clientes.\n";
            break;
        default:
            std::cout << "Opcion invalida.\n";
        }
    } while (opcion != 5);
}




void realizarVenta() {
    std::cout << "\n--- Realizar Venta ---\n";

    // Mostrar clientes disponibles
    std::cout << "Clientes disponibles:\n";
    for (const auto& c : clientes) {
        std::cout << "ID: " << c.id << ", Nombre: " << c.nombre
            << ", Credito Maximo: $" << c.creditoMaximo
            << ", Credito Actual: $" << c.creditoActual << "\n";
    }

    // Seleccionar cliente
    int idCliente;
    std::cout << "Ingrese el ID del cliente (0 si es venta al publico): ";
    std::cin >> idCliente;

    Cliente* clienteSeleccionado = nullptr;
    if (idCliente != 0) {
        auto itCliente = std::find_if(clientes.begin(), clientes.end(), [idCliente](const Cliente& c) {
            return c.id == idCliente;
            });

        if (itCliente == clientes.end()) {
            std::cout << "Cliente no encontrado.\n";
            return;
        }
        clienteSeleccionado = &(*itCliente);
    }

    double totalVenta = 0;
    bool agregarOtroProducto = true;
    std::string detalleVenta;
    std::vector<std::string> ticketDetalles;

    while (agregarOtroProducto) {
        // Mostrar productos
        std::cout << "\nProductos disponibles:\n";
        for (const auto& p : productos) {
            std::cout << "ID: " << p.id << ", Nombre: " << p.nombre
                << ", Precio: $" << p.precio << ", Stock: " << p.stock << "\n";
        }

        int idProducto, cantidad;
        std::cout << "Ingrese el ID del producto: ";
        std::cin >> idProducto;

        auto itProducto = std::find_if(productos.begin(), productos.end(), [idProducto](const Producto& p) {
            return p.id == idProducto;
            });

        if (itProducto == productos.end()) {
            std::cout << "Producto no encontrado.\n";
            return;
        }

        std::cout << "Ingrese la cantidad a vender: ";
        std::cin >> cantidad;

        if (cantidad > itProducto->stock) {
            std::cout << "Stock insuficiente.\n";
            return;
        }

        double totalProducto = cantidad * itProducto->precio;
        totalVenta += totalProducto;
        itProducto->stock -= cantidad;

        // Registrar el producto en el ticket
        ticketDetalles.push_back(itProducto->nombre + " x" + std::to_string(cantidad) +
            " - $" + std::to_string(totalProducto));
        detalleVenta += "Producto: " + itProducto->nombre + ", Cantidad: " + std::to_string(cantidad) +
            ", Total: $" + std::to_string(totalProducto) + "\n";

        char respuesta;
        std::cout << "Desea agregar otro producto? (s/n): ";
        std::cin >> respuesta;
        agregarOtroProducto = (respuesta == 's' || respuesta == 'S');
    }

    // Mostrar el ticket de compra
    std::cout << "\n--- Ticket de Compra ---\n";
    for (const auto& detalle : ticketDetalles) {
        std::cout << detalle << "\n";
    }
    std::cout << "------------------------\n";
    std::cout << "Total de la venta: $" << totalVenta << "\n";

    if (clienteSeleccionado) {
        if (totalVenta > clienteSeleccionado->creditoMaximo - clienteSeleccionado->creditoActual) {
            std::cout << "El cliente no tiene suficiente credito disponible.\n";
            return;
        }

        clienteSeleccionado->creditoActual += totalVenta;
        clienteSeleccionado->historial.push_back(detalleVenta);

        std::cout << "Credito disponible despues de la compra: $"
            << clienteSeleccionado->creditoMaximo - clienteSeleccionado->creditoActual << "\n";
        std::cout << "Venta registrada en el historial de " << clienteSeleccionado->nombre << ".\n";
    }
    else {
        std::cout << "Venta al publico registrada.\n";
    }
    std::cout << "------------------------\n";
}





void corteCaja() {
    double totalVentas = 0;
    // Solo sumar ventas a crédito realizadas en la sesión actual
    for (const auto& cliente : clientes) {
        // Acumulamos las ventas a crédito solo para los clientes que hayan hecho compras en esta sesión
        totalVentas += cliente.creditoActual; // Suponemos que 'creditoActual' solo refleja compras realizadas en la sesión
    }

    std::cout << "Total de ventas a credito en esta sesion: $" << totalVentas << "\n";

    // Calcular el valor del inventario
    double valorInventario = 0;
    for (const auto& producto : productos) {
        valorInventario += producto.stock * producto.precio;
    }

    std::cout << "Valor del inventario actual: $" << valorInventario << "\n";
}

void eliminarProducto() {
    std::cout << "\n--- Eliminar Producto ---\n";
    std::cout << "Productos disponibles:\n";
    for (const auto& p : productos) {
        std::cout << "ID: " << p.id << ", Nombre: " << p.nombre << "\n";
    }

    int idProducto;
    std::cout << "Ingrese el ID del producto a eliminar: ";
    std::cin >> idProducto;

    auto itProducto = std::find_if(productos.begin(), productos.end(), [idProducto](const Producto& p) {
        return p.id == idProducto;
        });

    if (itProducto != productos.end()) {
        productos.erase(itProducto);
        guardarProductos();
        std::cout << "Producto eliminado correctamente.\n";
    }
    else {
        std::cout << "Producto no encontrado.\n";
    }
}

void modificarProducto() {
    std::cout << "\n--- Modificar Producto ---\n";
    std::cout << "Productos disponibles:\n";
    for (const auto& p : productos) {
        std::cout << "ID: " << p.id << ", Nombre: " << p.nombre << "\n";
    }

    int idProducto;
    std::cout << "Ingrese el ID del producto a modificar: ";
    std::cin >> idProducto;

    auto itProducto = std::find_if(productos.begin(), productos.end(), [idProducto](const Producto& p) {
        return p.id == idProducto;
        });

    if (itProducto != productos.end()) {
        std::cout << "Nombre actual: " << itProducto->nombre << ", Ingrese nuevo nombre: ";
        std::cin >> itProducto->nombre;
        std::cout << "Categoria actual: " << itProducto->categoria << ", Ingrese nueva categoria: ";
        std::cin >> itProducto->categoria;
        std::cout << "Precio actual: " << itProducto->precio << ", Ingrese nuevo precio: ";
        std::cin >> itProducto->precio;
        std::cout << "Stock actual: " << itProducto->stock << ", Ingrese nuevo stock: ";
        std::cin >> itProducto->stock;
        std::cout << "Stock minimo actual: " << itProducto->stockMinimo << ", Ingrese nuevo stock minimo: ";
        std::cin >> itProducto->stockMinimo;

        guardarProductos();
        std::cout << "Producto modificado correctamente.\n";
    }
    else {
        std::cout << "Producto no encontrado.\n";
    }
}

void eliminarCliente() {
    std::cout << "\n--- Eliminar Cliente ---\n";
    std::cout << "Clientes disponibles:\n";
    for (const auto& c : clientes) {
        std::cout << "ID: " << c.id << ", Nombre: " << c.nombre << "\n";
    }

    int idCliente;
    std::cout << "Ingrese el ID del cliente a eliminar: ";
    std::cin >> idCliente;

    auto itCliente = std::find_if(clientes.begin(), clientes.end(), [idCliente](const Cliente& c) {
        return c.id == idCliente;
        });

    if (itCliente != clientes.end()) {
        clientes.erase(itCliente);
        guardarClientes();
        std::cout << "Cliente eliminado correctamente.\n";
    }
    else {
        std::cout << "Cliente no encontrado.\n";
    }
}

void modificarCliente() {
    std::cout << "\n--- Modificar Cliente ---\n";
    std::cout << "Clientes disponibles:\n";
    for (const auto& c : clientes) {
        std::cout << "ID: " << c.id << ", Nombre: " << c.nombre << "\n";
    }

    int idCliente;
    std::cout << "Ingrese el ID del cliente a modificar: ";
    std::cin >> idCliente;

    auto itCliente = std::find_if(clientes.begin(), clientes.end(), [idCliente](const Cliente& c) {
        return c.id == idCliente;
        });

    if (itCliente != clientes.end()) {
        std::cout << "Nombre actual: " << itCliente->nombre << ", Ingrese nuevo nombre: ";
        std::cin >> itCliente->nombre;
        std::cout << "Credito maximo actual: " << itCliente->creditoMaximo << ", Ingrese nuevo credito maximo: ";
        std::cin >> itCliente->creditoMaximo;

        guardarClientes();
        std::cout << "Cliente modificado correctamente.\n";
    }
    else {
        std::cout << "Cliente no encontrado.\n";
    }
}
