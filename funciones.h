#ifndef FUNCIONES_H
#define FUNCIONES_H

#define MAX_PRODUCTOS 20
#define MAX_CLIENTES 100

// Estructura para almacenar la información de cada producto
struct Producto
{
    char nombre[50];
    float precio;
    int stock;
    int cantidadVendida;
};

// Estructura para almacenar la información de cada cliente
struct Cliente
{
    char nombre[50];
    char cedula[15]; // Añadimos el campo para la cédula
    int totalCompras;
};

// Prototipos de funciones
void leerCadena(char *cadena, int tamano);
int menu();

// --- Funciones de Gestión de Productos ---
void inicializarProductos(struct Producto productos[], int numProductos);
void guardarProductos(struct Producto productos[], int numProductos);
int leerProductos(struct Producto productos[], int numProductos);
void mostrarProductosMasVendidos(struct Producto productos[], int numProductos);
void mostrarStockProductos(struct Producto productos[], int numProductos);

// --- Funciones de Gestión de Clientes ---
void guardarClientes(struct Cliente clientes[], int numClientes);
int leerClientes(struct Cliente clientes[], int *numClientes);
void mostrarTopClientes(struct Cliente clientes[], int numClientes);
// Nuevo prototipo para buscar clientes
void buscarCliente(struct Cliente clientes[], int numClientes);

// --- Funciones de Transacciones ---
void registrarVenta(struct Producto productos[], int numProductos, struct Cliente clientes[], int *numClientes);

// --- Funciones Auxiliares Internas ---
void actualizarEstadisticasProducto(struct Producto productos[], int numProductos, const char *nombreProducto, int cantidad);
void actualizarEstadisticasCliente(struct Cliente clientes[], int *numClientes, const char *nombreCliente, const char *cedulaCliente, int cantidadProductosComprados);

#endif // FUNCIONES_H