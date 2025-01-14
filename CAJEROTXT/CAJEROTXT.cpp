#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

struct Cuenta {
    string nombre;
    int nip;
    double saldo;
    vector<string> movimientos;
};

struct Cajero {
    int billetes500 = 0;
    int billetes200 = 0;
    int billetes100 = 0;
    int billetes50 = 0;
    int billetes20 = 0;
};

vector<Cuenta> cargarCuentasDesdeArchivo() {
    vector<Cuenta> cuentas;
    ifstream archivo("cajero2.txt");

    if (!archivo) {
        cerr << "No se pudo abrir el archivo de cuentas (cajero2.txt).\n";
        return cuentas;
    }

    string linea;
    while (getline(archivo, linea)) {
        istringstream iss(linea);
        Cuenta cuenta;
        if (iss >> cuenta.nombre >> cuenta.nip >> cuenta.saldo) {
            cuentas.push_back(cuenta);
        }
    }

    archivo.close();
    return cuentas;
}

int obtenerEntero(const string& mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        cin >> valor;

        if (cin.fail()) { 
            cin.clear();  
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Entrada invalida. Por favor, ingrese un numero entero.\n";
        }
        else {
            return valor;
        }
    }
}

double obtenerDecimal(const string& mensaje) {
    double valor;
    while (true) {
        cout << mensaje;
        cin >> valor;

        if (cin.fail()) {
            cin.clear();  
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Entrada invalida. Por favor, ingrese un número valido.\n";
        }
        else {
            return valor;
        }
    }
}


void registrarMovimiento(const string& mensaje) {
    ofstream archivoMovimientos("movimientos.txt", ios::app);
    if (!archivoMovimientos) {
        cerr << "Error al abrir el archivo movimientos.txt.\n";
        return;
    }
    archivoMovimientos << mensaje << "\n";
    archivoMovimientos.close();
}

void inicializarCajero(Cajero& cajero) {
    cout << "Ingrese la cantidad de billetes:\n";
    cajero.billetes500 = obtenerEntero("Billetes de $500: ");
    cajero.billetes200 = obtenerEntero("Billetes de $200: ");
    cajero.billetes100 = obtenerEntero("Billetes de $100: ");
    cajero.billetes50 = obtenerEntero("Billetes de $50: ");
    cajero.billetes20 = obtenerEntero("Billetes de $20: ");
    cout << "Billetes ingresados con exito...\n";

    registrarMovimiento("Administrador: Inicializacion del cajero con billetes.");
}



void mostrarOpciones() {
    cout << "MENU:\n";
    cout << "1. Consultar saldo\n";
    cout << "2. Depositar\n";
    cout << "3. Retirar\n";
    cout << "4. Transferir\n";
    cout << "5. Pago de servicios\n";
    cout << "6. Cambiar NIP\n";
    cout << "7. Consultar últimos movimientos\n";
    cout << "0. Salir\n\n";
}

void consultarSaldo(const Cuenta& cuenta) {
    cout << "Saldo actual de " << cuenta.nombre << ": $" << fixed << setprecision(2) << cuenta.saldo << "\n\n";
    registrarMovimiento("Consulta de saldo por: " + cuenta.nombre);
}

void actualizarNipEnArchivo(const Cuenta& cuenta) {
    fstream archivo("cajero2.txt", ios::in | ios::out);
    if (!archivo) {
        cerr << "Error al abrir el archivo cajero2.txt para actualizar el NIP.\n";
        return;
    }

    string linea;
    streampos posicionInicial;
    while (archivo.tellg() >= 0) {
        posicionInicial = archivo.tellg(); 
        getline(archivo, linea);

        if (linea.find(cuenta.nombre) != string::npos) {
            
            ostringstream nuevaLinea;
            nuevaLinea << cuenta.nombre << " " << cuenta.nip << " " << fixed << setprecision(2) << cuenta.saldo;

            
            archivo.seekp(posicionInicial);
            archivo << nuevaLinea.str() << string(linea.size() - nuevaLinea.str().size(), ' ');
            break;
        }
    }

    archivo.close();
}


void cambiarNIP(Cuenta& cuenta, vector<Cuenta>& cuentas) {
    int nuevoNip = obtenerEntero("Ingrese el nuevo NIP: ");
    int confirmacionNip = obtenerEntero("Confirme el nuevo NIP: ");

    if (nuevoNip == confirmacionNip) {
        cuenta.nip = nuevoNip;
        cuenta.movimientos.push_back("Cambio de NIP.");
        registrarMovimiento("Cambio de NIP por: " + cuenta.nombre);

        
        actualizarNipEnArchivo(cuenta);

        cout << "NIP cambiado exitosamente.\n\n";
    }
    else {
        cout << "Los NIPs no coinciden. Intente nuevamente.\n\n";
    }
}


void consultarMovimientos(const Cuenta& cuenta) {
    cout << "ultimos movimientos de " << cuenta.nombre << ":\n";
    for (const auto& movimiento : cuenta.movimientos) {
        cout << movimiento << "\n";
    }
    registrarMovimiento("Consulta de movimientos por: " + cuenta.nombre);
    cout << "\n";
}


void guardarCuentasEnArchivo(const vector<Cuenta>& cuentas) {
    ofstream archivo("cajero2.txt");

    if (!archivo) {
        cerr << "Error al abrir el archivo cajero2.txt para guardar las cuentas.\n";
        return;
    }

    for (const auto& cuenta : cuentas) {
        archivo << cuenta.nombre << " " << cuenta.nip << " " << fixed << setprecision(2) << cuenta.saldo << "\n";
    }

    archivo.close();
    
}


void depositar(Cuenta& cuenta, vector<Cuenta>& cuentas) {
    double monto = obtenerDecimal("Ingrese el monto a depositar: ");
    if (monto > 0) {
        cuenta.saldo += monto;
        cuenta.movimientos.push_back("Deposito: $" + to_string(monto));
        registrarMovimiento("Deposito de $" + to_string(monto) + " por: " + cuenta.nombre);
        guardarCuentasEnArchivo(cuentas);
        cout << "Deposito exitoso.\n\n";
    }
    else {
        cout << "Monto invalido.\n\n";
    }
}

void retirar(Cuenta& cuenta, Cajero& cajero, vector<Cuenta>& cuentas) {
    double monto = obtenerDecimal("Ingrese el monto a retirar: ");
    if (monto > cuenta.saldo) {
        cout << "Saldo insuficiente.\n\n";
    }
    else if (monto <= 0) {
        cout << "Monto invalido.\n\n";
    }
    else {
        cuenta.saldo -= monto;
        cuenta.movimientos.push_back("Retiro: $" + to_string(monto));
        registrarMovimiento("Retiro de $" + to_string(monto) + " por: " + cuenta.nombre);
        guardarCuentasEnArchivo(cuentas); 
        cout << "Retiro exitoso.\n\n";
    }
}

void transferir(Cuenta& cuenta, vector<Cuenta>& cuentas) {
    string nombreDestino;
    double monto;
    cout << "Ingrese el nombre del usuario destino: ";
    cin >> nombreDestino;
    cout << "Ingrese el monto a transferir: ";
    cin >> monto;

    if (monto > 0 && monto <= cuenta.saldo) {
        bool encontrado = false;
        for (auto& cuentaDestino : cuentas) {
            if (cuentaDestino.nombre == nombreDestino) {
                cuenta.saldo -= monto;
                cuentaDestino.saldo += monto;
                cuenta.movimientos.push_back("Transferencia a " + cuentaDestino.nombre + ": $" + to_string(monto));
                cuentaDestino.movimientos.push_back("Transferencia recibida de " + cuenta.nombre + ": $" + to_string(monto));
                registrarMovimiento("Transferencia de $" + to_string(monto) + " de " + cuenta.nombre + " a " + cuentaDestino.nombre);
                guardarCuentasEnArchivo(cuentas);
                cout << "Transferencia exitosa a " << cuentaDestino.nombre << ".\n\n";
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            cout << "Usuario destino no encontrado.\n\n";
        }
    }
    else {
        cout << "Monto inválido o saldo insuficiente.\n\n";
    }
}

void pagarServicio(Cuenta& cuenta, vector<Cuenta>& cuentas) {
    int servicio = obtenerEntero("Seleccione el servicio a pagar:\n1. CFE\n2. Telcel\n3. Japama\nIngrese su opcion: ");
    double monto = obtenerDecimal("Ingrese el monto a pagar: ");
    if (monto > 0 && monto <= cuenta.saldo) {
        cuenta.saldo -= monto;
        string servicioStr = (servicio == 1) ? "CFE" : (servicio == 2) ? "Telcel" : "Japama";
        cuenta.movimientos.push_back("Pago de servicio " + servicioStr + ": $" + to_string(monto));
        registrarMovimiento("Pago de servicio " + servicioStr + " de $" + to_string(monto) + " por: " + cuenta.nombre);
        guardarCuentasEnArchivo(cuentas);
        cout << "Pago de servicio a " << servicioStr << " exitoso.\n\n";
    }
    else {
        cout << "Monto inválido o saldo insuficiente.\n\n";
    }
}


int main() {
    Cajero cajero;
    vector<Cuenta> cuentas = cargarCuentasDesdeArchivo();

    if (cuentas.empty()) {
        cerr << "No se cargaron cuentas. Verifique el archivo cajero2.txt.\n\n";
        return 1;
    }

    string nombreUsuario;
    int nipIngresado;
    bool salir = false;

    while (!salir) {
        cout << "Bienvenido al cajero automatico.\n\n";
        cout << "Ingrese su nombre: ";
        cin >> nombreUsuario;

        cout << "Ingrese su NIP: \n";
        nipIngresado = obtenerEntero("Ingrese su NIP: ");


        if (nombreUsuario == "Admin" && nipIngresado == 9999) {
            cout << "Bienvenido, administrador.\n\n";
            inicializarCajero(cajero);
            cout << "Volviendo al inicio de sesion...\n\n";
        }
        else {
            int cuentaSeleccionada = -1;
            for (int i = 0; i < cuentas.size(); ++i) {
                if (cuentas[i].nombre == nombreUsuario && cuentas[i].nip == nipIngresado) {
                    cuentaSeleccionada = i;
                    break;
                }
            }

            if (cuentaSeleccionada == -1) {
                cout << "Usuario o NIP incorrecto.\n\n";
                continue;
            }

            cout << "Bienvenido, " << cuentas[cuentaSeleccionada].nombre << ".\n";

            int opcion;
            do {
                mostrarOpciones();
                cout << "Seleccione una opcion: ";
                cin >> opcion;

                switch (opcion) {
                case 1:
                    consultarSaldo(cuentas[cuentaSeleccionada]);
                    break;
                case 2:
                    depositar(cuentas[cuentaSeleccionada],cuentas);
                    break;
                case 3:
                    retirar(cuentas[cuentaSeleccionada], cajero,cuentas);
                    break;
                case 4:
                    transferir(cuentas[cuentaSeleccionada], cuentas);
                    break;
                case 5:
                    pagarServicio(cuentas[cuentaSeleccionada],cuentas);
                    break;
                
                case 6: {
                    cout << "Cambio de NIP seleccionado.\n";
                    cambiarNIP(cuentas[cuentaSeleccionada],cuentas);
                    break;
                

                }
                case 7:
                    consultarMovimientos(cuentas[cuentaSeleccionada]);
                    break;
                case 0:
                    cout << "Cerrando sesion...\n\n";
                    break;
                default:
                    cout << "Opcion invalida.\n\n";
                    break;
                }
            } while (opcion != 0);
        }
    }

    return 0;
}
