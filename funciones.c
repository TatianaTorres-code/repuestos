#include "funciones.h"
#include <stdio.h>
#include <string.h>

// Utilidad para leer texto (evita problemas con saltos de línea)
void leerCadena(char *cadena, int tamano)
{
    fgets(cadena, tamano, stdin);
    int len = strlen(cadena);
    if (len > 0 && cadena[len - 1] == '\n')
    {
        cadena[len - 1] = '\0';
    }
}

// Muestra el menú de opciones
int menu()
{
    int opcion;
    printf("\n--- SISTEMA DE GESTION TIENDA DE REPUESTOS ---\n");
    printf("1. Inicializar Productos (Primer uso / Resetear)\n");
    printf("2. Registrar Nueva Venta\n");
    printf("3. Mostrar Productos Mas Vendidos\n");
    printf("4. Mostrar Stock de Productos\n");
    printf("5. Mostrar Clientes con Mas Compras\n");
    printf("6. Buscar Cliente por Nombre o Cedula\n");
    printf("7. Salir\n");
    printf(">>>>>: ");
    scanf("%d", &opcion);
    while (getchar() != '\n')
        ; // Limpiar buffer de entrada para evitar problemas con la siguiente lectura
    return opcion;
}

// Crea una lista inicial de productos y la guarda en un archivo
void inicializarProductos(struct Producto productos[], int numProductos)
{
    // Limpiar completamente el array antes de llenar los iniciales
    // Esto asegura que cualquier basura previa sea eliminada
    for (int i = 0; i < numProductos; i++)
    {
        strcpy(productos[i].nombre, ""); // Vaciar el nombre
        productos[i].precio = 0.0;
        productos[i].stock = 0;
        productos[i].cantidadVendida = 0;
    }

    // Nombres, precios, y stock inicial de algunos productos de ejemplo
    strcpy(productos[0].nombre, "Filtro de Aceite");
    productos[0].precio = 12.50;
    productos[0].stock = 50;
    productos[0].cantidadVendida = 0;

    strcpy(productos[1].nombre, "Pastillas de Freno");
    productos[1].precio = 35.00;
    productos[1].stock = 30;
    productos[1].cantidadVendida = 0;

    strcpy(productos[2].nombre, "Bateria 12V");
    productos[2].precio = 80.00;
    productos[2].stock = 15;
    productos[2].cantidadVendida = 0;

    strcpy(productos[3].nombre, "Bujias (set 4)");
    productos[3].precio = 25.00;
    productos[3].stock = 40;
    productos[3].cantidadVendida = 0;

    strcpy(productos[4].nombre, "Aceite Motor 1L");
    productos[4].precio = 10.00;
    productos[4].stock = 100;
    productos[4].cantidadVendida = 0;

    // Ya no se necesita el bucle `for (int i = 5; ...)` porque se hizo al principio
    guardarProductos(productos, numProductos);
}

// Guarda el array de productos en el archivo "productos.dat"
void guardarProductos(struct Producto productos[], int numProductos)
{
    FILE *f = fopen("productos.dat", "wb");
    if (f == NULL)
    {
        printf("Error al abrir el archivo de productos para escritura.\n");
        return;
    }
    fwrite(productos, sizeof(struct Producto), numProductos, f);
    fclose(f);
}

// Lee los datos de "productos.dat" y los carga en el array
int leerProductos(struct Producto productos[], int numProductos)
{
    FILE *f = fopen("productos.dat", "rb");
    if (f == NULL)
    {
        printf("Archivo de productos no encontrado. Por favor, inicialice los productos primero (Opcion 1).\n");
        // Asegurarse de que el array esté limpio si el archivo no existe
        for (int i = 0; i < numProductos; i++)
        {
            strcpy(productos[i].nombre, "");
            productos[i].precio = 0.0;
            productos[i].stock = 0;
            productos[i].cantidadVendida = 0;
        }
        return 0;
    }
    fread(productos, sizeof(struct Producto), numProductos, f);
    fclose(f);
    return 1;
}

// Función auxiliar para actualizar la cantidad vendida y el stock de un producto
void actualizarEstadisticasProducto(struct Producto productos[], int numProductos, const char *nombreProducto, int cantidad)
{
    for (int i = 0; i < numProductos; i++)
    {
        if (strcmp(productos[i].nombre, nombreProducto) == 0)
        {
            productos[i].cantidadVendida += cantidad;
            productos[i].stock -= cantidad; // Aquí usaba 'quantity' en lugar de 'cantidad'
            return;
        }
    }
    printf("Error: Producto '%s' no encontrado para actualizar estadisticas.\n", nombreProducto);
}

// Función auxiliar para actualizar las compras de un cliente, o añadirlo si es nuevo
void actualizarEstadisticasCliente(struct Cliente clientes[], int *numClientes, const char *nombreCliente, const char *cedulaCliente, int cantidadProductosComprados)
{
    for (int i = 0; i < *numClientes; i++)
    {
        if (strcmp(clientes[i].cedula, cedulaCliente) == 0) // Buscar por cédula
        {
            clientes[i].totalCompras += cantidadProductosComprados;
            return;
        }
    }
    if (*numClientes < MAX_CLIENTES)
    {
        strcpy(clientes[*numClientes].nombre, nombreCliente);
        strcpy(clientes[*numClientes].cedula, cedulaCliente);
        clientes[*numClientes].totalCompras = cantidadProductosComprados;
        (*numClientes)++;
    }
    else
    {
        printf("Advertencia: No se puede registrar mas clientes, limite alcanzado.\n");
    }
}

// Pide datos de la venta, actualiza estadísticas y registra clientes
void registrarVenta(struct Producto productos[], int numProductos, struct Cliente clientes[], int *numClientes)
{
    int idProducto, cantidadAVender;
    char nombreCliente[50];
    char cedulaCliente[15];
    int productosDisponiblesParaVenta = 0; // Contador de productos válidos para mostrar

    printf("\n--- Lista de Productos Disponibles ---\n");
    for (int i = 0; i < numProductos; i++)
    {
        // Solo mostrar productos con nombre no vacío y stock positivo
        if (strlen(productos[i].nombre) > 0 && productos[i].stock > 0)
        {
            printf("%d. %-20s (Stock: %d, Precio: %.2f)\n", i + 1, productos[i].nombre, productos[i].stock, productos[i].precio);
            productosDisponiblesParaVenta++;
        }
    }

    if (productosDisponiblesParaVenta == 0)
    {
        printf("No hay productos disponibles para la venta en este momento.\n");
        return;
    }

    printf("Seleccione el numero del Producto a vender: ");
    scanf("%d", &idProducto);
    while (getchar() != '\n')
        ;

    if (idProducto < 1 || idProducto > numProductos || strlen(productos[idProducto - 1].nombre) == 0 || productos[idProducto - 1].stock <= 0)
    {
        printf("Seleccion de producto invalida o producto sin stock.\n");
        return;
    }
    idProducto--;

    printf("Ingrese la cantidad a vender de %s (Stock actual: %d): ", productos[idProducto].nombre, productos[idProducto].stock);
    scanf("%d", &cantidadAVender);
    while (getchar() != '\n')
        ;

    if (cantidadAVender <= 0 || cantidadAVender > productos[idProducto].stock)
    {
        printf("Cantidad de venta invalida o stock insuficiente.\n");
        return;
    }

    printf("Ingrese el nombre del Cliente: ");
    leerCadena(nombreCliente, 50);
    printf("Ingrese la cedula del Cliente: ");
    leerCadena(cedulaCliente, 15);

    actualizarEstadisticasProducto(productos, numProductos, productos[idProducto].nombre, cantidadAVender);
    actualizarEstadisticasCliente(clientes, numClientes, nombreCliente, cedulaCliente, cantidadAVender);

    guardarProductos(productos, numProductos);
    guardarClientes(clientes, *numClientes);
    printf("Venta registrada con exito!\n");
}

// Muestra la tabla de productos más vendidos ordenada
void mostrarProductosMasVendidos(struct Producto productos[], int numProductos)
{
    struct Producto productosOrdenados[MAX_PRODUCTOS];
    int countProductosValidos = 0;

    // Solo copiar los productos que tienen un nombre válido
    for (int i = 0; i < numProductos; i++)
    {
        if (strlen(productos[i].nombre) > 0)
        {
            productosOrdenados[countProductosValidos] = productos[i];
            countProductosValidos++;
        }
    }

    // Si no hay productos válidos, no tiene sentido ordenar ni mostrar
    if (countProductosValidos == 0)
    {
        printf("\nNo hay productos registrados para mostrar estadisticas de ventas.\n");
        return;
    }

    struct Producto temp;
    for (int i = 0; i < countProductosValidos - 1; i++)
    {
        for (int j = 0; j < countProductosValidos - i - 1; j++)
        {
            if (productosOrdenados[j].cantidadVendida < productosOrdenados[j + 1].cantidadVendida)
            {
                temp = productosOrdenados[j];
                productosOrdenados[j] = productosOrdenados[j + 1];
                productosOrdenados[j + 1] = temp;
            }
        }
    }

    printf("\n------------------- PRODUCTOS MAS VENDIDOS -------------------\n");
    printf("POS\tPRODUCTO\t\t\tVENDIDOS\tSTOCK ACTUAL\n");
    printf("--------------------------------------------------------------\n");
    int mostradosConVentas = 0;
    for (int i = 0; i < countProductosValidos; i++)
    {
        if (productosOrdenados[i].cantidadVendida > 0)
        { // Solo mostrar productos que se han vendido
            printf("%d\t%-20s\t%d\t\t%d\n",
                   i + 1,
                   productosOrdenados[i].nombre,
                   productosOrdenados[i].cantidadVendida,
                   productosOrdenados[i].stock);
            mostradosConVentas++;
        }
    }
    if (mostradosConVentas == 0)
    {
        printf("No hay productos vendidos para mostrar estadisticas.\n");
    }
}

// Muestra la tabla de stock de productos
void mostrarStockProductos(struct Producto productos[], int numProductos)
{
    printf("\n------------------- STOCK DE PRODUCTOS -------------------\n");
    printf("PRODUCTO\t\t\tPRECIO\t\tSTOCK\n");
    printf("----------------------------------------------------------\n");
    int productosMostrados = 0;
    for (int i = 0; i < numProductos; i++)
    {
        // **Clave de la solución:** Solo mostrar si el nombre NO está vacío.
        // Esto filtra los espacios no inicializados del array.
        if (strlen(productos[i].nombre) > 0)
        {
            printf("%-20s\t%.2f\t\t%d\n",
                   productos[i].nombre,
                   productos[i].precio,
                   productos[i].stock);
            productosMostrados++;
        }
    }
    if (productosMostrados == 0)
    {
        printf("No hay productos registrados en el inventario.\n");
    }
}

// Guarda la lista de clientes en "clientes.dat"
void guardarClientes(struct Cliente clientes[], int numClientes)
{
    FILE *f = fopen("clientes.dat", "wb");
    if (f == NULL)
    {
        printf("Error al abrir el archivo de clientes para escritura.\n");
        return;
    }
    fwrite(clientes, sizeof(struct Cliente), numClientes, f);
    fclose(f);
}

// Lee los clientes desde el archivo
int leerClientes(struct Cliente clientes[], int *numClientes)
{
    FILE *f = fopen("clientes.dat", "rb");
    if (f == NULL)
    {
        *numClientes = 0;
        // Asegurarse de que el array de clientes esté limpio si el archivo no existe
        for (int i = 0; i < MAX_CLIENTES; i++)
        { // Usa MAX_CLIENTES para limpiar todo el array
            strcpy(clientes[i].nombre, "");
            strcpy(clientes[i].cedula, "");
            clientes[i].totalCompras = 0;
        }
        return 1;
    }
    fseek(f, 0, SEEK_END);
    long tamanoArchivo = ftell(f);
    rewind(f);
    *numClientes = tamanoArchivo / sizeof(struct Cliente);

    fread(clientes, sizeof(struct Cliente), *numClientes, f);
    fclose(f);
    return 1;
}

// Muestra la tabla de clientes con más compras ordenada
void mostrarTopClientes(struct Cliente clientes[], int numClientes)
{
    // Crear una copia del array para ordenar sin modificar el original
    struct Cliente clientesOrdenados[MAX_CLIENTES];
    int countClientesValidos = 0;

    // Solo copiar los clientes que tienen un nombre válido
    for (int i = 0; i < numClientes; i++)
    {
        if (strlen(clientes[i].nombre) > 0)
        {
            clientesOrdenados[countClientesValidos] = clientes[i];
            countClientesValidos++;
        }
    }

    if (countClientesValidos == 0)
    {
        printf("\nNo hay clientes registrados con compras para mostrar.\n");
        return;
    }

    struct Cliente temp;
    for (int i = 0; i < countClientesValidos - 1; i++)
    {
        for (int j = 0; j < countClientesValidos - i - 1; j++)
        {
            if (clientesOrdenados[j].totalCompras < clientesOrdenados[j + 1].totalCompras)
            {
                temp = clientesOrdenados[j];
                clientesOrdenados[j] = clientesOrdenados[j + 1];
                clientesOrdenados[j + 1] = temp;
            }
        }
    }

    printf("\n--------------- CLIENTES CON MAS COMPRAS ---------------\n");
    printf("POS\tCLIENTE\t\t\tCEDULA\t\t\tPRODUCTOS COMPRADOS\n");
    printf("----------------------------------------------------------------\n");
    int mostrados = 0;
    for (int i = 0; i < countClientesValidos; i++)
    {
        if (clientesOrdenados[i].totalCompras > 0)
        {
            printf("%d\t%-20s\t%-15s\t%d\n",
                   i + 1,
                   clientesOrdenados[i].nombre,
                   clientesOrdenados[i].cedula,
                   clientesOrdenados[i].totalCompras);
            mostrados++;
        }
    }
    if (mostrados == 0)
    {
        printf("No hay clientes con compras registradas para mostrar.\n");
    }
}

// Nueva función para buscar un cliente por nombre o cédula
void buscarCliente(struct Cliente clientes[], int numClientes)
{
    char terminoBusqueda[50];
    int encontrados = 0;

    printf("\n--- BUSCAR CLIENTE POR NOMBRE O CEDULA ---\n");
    printf("Ingrese el nombre o la cedula del cliente a buscar: ");
    leerCadena(terminoBusqueda, 50);

    printf("\nRESULTADOS DE LA BUSQUEDA:\n");
    printf("CLIENTE\t\t\tCEDULA\t\t\tPRODUCTOS COMPRADOS\n");
    printf("--------------------------------------------------------\n");

    for (int i = 0; i < numClientes; i++)
    {
        // Asegurarse de que el cliente tiene un nombre válido antes de buscar
        if (strlen(clientes[i].nombre) > 0)
        {
            // Realizar la búsqueda sin distinguir mayúsculas/minúsculas para el nombre
            // y una búsqueda exacta para la cédula
            // Para strstr insensible a mayúsculas/minúsculas, necesitaríamos strcasecmp o convertir a minúsculas temporalmente,
            // pero para mantener la simplicidad y no introducir cosas nuevas, usaremos strstr directo.
            if ((strstr(clientes[i].nombre, terminoBusqueda) != NULL) ||
                (strcmp(clientes[i].cedula, terminoBusqueda) == 0))
            {
                printf("%-20s\t%-15s\t%d\n",
                       clientes[i].nombre,
                       clientes[i].cedula,
                       clientes[i].totalCompras);
                encontrados++;
            }
        }
    }

    if (encontrados == 0)
    {
        printf("No se encontraron clientes con ese nombre o cedula.\n");
    }
}