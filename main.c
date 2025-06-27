#include "funciones.h"
#include <stdio.h>
#include <string.h>

#define NUM_PRODUCTOS_INICIALES 5

int main(int argc, char *argv[])
{
    struct Producto productos[MAX_PRODUCTOS];
    struct Cliente clientes[MAX_CLIENTES];

    int numClientes = 0;
    int opc;

    leerProductos(productos, MAX_PRODUCTOS);
    leerClientes(clientes, &numClientes);

    do
    {
        opc = menu();
        switch (opc)
        {
        case 1:
            inicializarProductos(productos, MAX_PRODUCTOS);
            printf("Productos inicializados con exito y stock/ventas reseteados a cero.\n");
            numClientes = 0;
            guardarClientes(clientes, numClientes);
            printf("Los datos de clientes tambien han sido reiniciados.\n");
            break;
        case 2:
            if (leerProductos(productos, MAX_PRODUCTOS))
            {
                registrarVenta(productos, MAX_PRODUCTOS, clientes, &numClientes);
            }
            break;
        case 3:
            if (leerProductos(productos, MAX_PRODUCTOS))
            {
                mostrarProductosMasVendidos(productos, MAX_PRODUCTOS);
            }
            break;
        case 4:
            if (leerProductos(productos, MAX_PRODUCTOS))
            {
                mostrarStockProductos(productos, MAX_PRODUCTOS);
            }
            break;
        case 5:
            mostrarTopClientes(clientes, numClientes);
            break;
        case 6: // Nueva opción para buscar cliente
            buscarCliente(clientes, numClientes);
            break;
        case 7:
            printf("Saliendo del programa. ¡Hasta luego!\n");
            break;
        default:
            printf("Opcion invalida. Intente de nuevo.\n");
        }
    } while (opc != 7); // El menú ahora tiene 7 opciones, la última es salir

    return 0;
}