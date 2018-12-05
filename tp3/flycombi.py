import sys
import csv
from grafo import Grafo
import biblioteca as b

# Índice de cada peso de las aristas del grafo
TIEMPO = 0
PRECIO = 1
VUELOS = 2

# Lista de operaciones disponibles
OPERACIONES = [
            "camino_mas",
            "camino_escalas",
            "nueva_aerolinea",
            "vacaciones",
            "exportar_kml",
            "centralidad",
            "centralidad_aprox",
            "pagerank"
            ]

def listar_operaciones():
    """Muestra por pantalla todas las operaciones disponibles"""
    for op in OPERACIONES:
        print(op)

def obtener_aeropuertos(grafo, ciudades, ruta_aeropuertos):
    """Recibe un grafo, un diccionario de ciudades, una ruta de archivo
    csv de aeropuertos, y guarda los datos en el grafo y en el diccionario.
    PRE: El archivo debe tener el formato ciudad,codigo,latitud,longitud."""
    with open(ruta_aeropuertos, "r") as archivo:
        reader = csv.reader(archivo)
        for ciudad, codigo, latitud, longitud in reader:
            ubicacion = ",".join([longitud, latitud])
            grafo.agregar_vertice(codigo, ubicacion)
            if ciudad not in ciudades:
                ciudades[ciudad] = []
            ciudades[ciudad].append(codigo)

def obtener_vuelos(grafo, ruta_vuelos):
    """Recibe un grafo y una ruta a un archivo csv de vuelos,
    y guarda los datos en el grafo.
    PRE: El archivo debe tener el formato
    aeropuerto_i,aeropuerto_j,tiempo,precio,vuelos."""
    with open(ruta_vuelos, "r") as archivo:
        reader = csv.reader(archivo)
        for aeropuerto_i, aeropuerto_j, tiempo, precio, vuelos in reader:
            pesos = (int(tiempo), int(precio), int(vuelos))
            grafo.agregar_arista(aeropuerto_i, aeropuerto_j, pesos)

def get_camino_minimo(grafo, origen, destino, peso=None):
    """Llama a la funcion para obtener el camino minimo correspondiente segun
    si se le pasa un peso o no"""
    if peso == None: return b.escalas_minimas_bfs(grafo, origen, destino)
    return b.obtener_camino_minimo(grafo, peso, origen, destino)

def camino_minimo(grafo, ciudades, parametros):
    """Recibe un grafo, un diccionario ciudad: lista_aeropuertos, y una lista
    de parámetros. Imprime un camino mínimo desde el origen hasta el destino.
    Devuelve un booleano indicando si el comando se ejecutó correctamente."""
    if len(parametros) == 3:
        peso, ciudad_origen, ciudad_destino = parametros
        if peso != "barato" and peso != "rapido":
            return False
        if peso == "rapido": peso = TIEMPO
        if peso == "barato": peso = PRECIO
    elif len(parametros) == 2:
        ciudad_origen, ciudad_destino = parametros
        peso = None
    else:
        return False
    if ciudad_origen not in ciudades or ciudad_destino not in ciudades:
        return False
    camino, distancia = None, None
    for origen in ciudades[ciudad_origen]:
        for destino in ciudades[ciudad_destino]:
            c, d = get_camino_minimo(grafo, origen, destino, peso)
            if not distancia or d < distancia:
                camino, distancia = c, d
    print(" -> ".join(camino))
    return camino

def centralidad(grafo, parametros):
    """Recibe un grafo y una lista de parametros que contiene
    un entero n. Devuelve los n aeropuertos mas centrales"""
    if len(parametros) != 1 or not parametros[0].isdigit():
        return False
    n = int(parametros[0])
    centralidades = b.betweeness_centrality(grafo)
    frecuencia_vuelos = {}
    for v in centralidades:
        frecuencia_vuelos[v] = 0
        for w in grafo.obtener_adyacentes(v):
            frecuencia_vuelos[v] += grafo.obtener_peso_union(v,w)[VUELOS]
        centralidades[v] *= frecuencia_vuelos[v]
    n_mas_centrales = b.obtener_n_mayores(centralidades, n, True)
    print(", ".join(n_mas_centrales))
    return True

def centralidad_aproximada(grafo, parametros):
    """Recibe un grafo y una lista de parámetros, que debe contener
    un número entero n. Imprime los n aeropuertos más importantes
    aproximadamente. En caso de error, devuelve False."""
    if len(parametros) != 1 or not parametros[0].isdigit():
        return False
    n = int(parametros[0])
    centralidades = b.obtener_centralidad_aproximada(grafo) # multiplicar por frecuencias
    resultado = b.obtener_n_mayores(centralidades, n, True)
    print(", ".join(resultado))
    return True

def pagerank(grafo, parametros):
    """Recibe un grafo y una lista de parámetros, que debe contener
    un número entero n. Imprime los n aeropuertos más importantes
    según el algoritmo de Pagerank. En caso de error, devuelve False."""
    if len(parametros) != 1 or not parametros[0].isdigit():
        return False
    n = int(parametros[0])
    centralidades = b.obtener_pagerank(grafo)
    resultado = b.obtener_n_mayores(centralidades, n, True)
    print(", ".join(resultado))
    return True

def nueva_aerolinea(grafo, parametros):
    """Recibe un grafo y una lista de parámetros, que debe contener
    la ruta a un archivo. Exporta las rutas que minimizan el costo
    para la implementación de una nueva aerolínea."""
    if len(parametros) != 1:
        return False
    arbol = b.optimizar_rutas(grafo, PRECIO)
    rutas = []
    with open(parametros[0], "w") as archivo:
        b.exportar_aerolinea(arbol, archivo, rutas)
    return rutas

def vacaciones(grafo, ciudades, parametros):
    """Recibe un grafo y una lista de parámetros, que contiene una
    ciudad origen y un número entero n. Imprime un recorrido de n
    lugares que comienza y finaliza en origen."""
    if len(parametros) != 2 or not parametros[1].isdigit():
        return False
    origen, n = parametros[0], int(parametros[1])
    if origen not in ciudades:
        return False
    recorrido = None
    for aeropuerto in ciudades[origen]:
        recorrido = b.obtener_viaje(grafo, aeropuerto, n)
        if recorrido != None: break
    if not recorrido:
        print("No se encontro recorrido")
        return True
    print(" -> ".join(recorrido))
    return recorrido

def recorrer_mundo(grafo, ciudades, parametros):
    """Recibe un grafo, un diccionario de ciudades y una lista de parámetros.
    Imprime un recorrido por todas las ciudades del mundo de modo que se
    demore lo menos posible."""
    pass

def exportar_kml(grafo, parametros, recorrido):
    """Recibe un grafo, un recorrido y una lista de parámetros que contiene
    la ruta del archivo kml a exportar. Devuelve True en caso de ejecutarse
    correctamente. En caso de error o si el recorrido no es una lista,
    devuelve False."""
    if len(parametros) != 1:
        return False
    if recorrido is True or recorrido is False:
        return False
    with open(parametros[0], "w") as archivo:
        b.exportar_archivo_kml(grafo, recorrido, archivo)
    return True

def procesar_comando(grafo, ciudades, linea, ultimo):
    """Recibe un grafo, un diccionario de ciudades con todos los
    datos disponibles y una linea y procesa los comandos correspondientes.
    Devuelve True en caso de éxito, False en caso de error."""
    entrada = linea.rstrip("\n").split(" ")
    comando = entrada[0]
    if comando == "listar_operaciones":
        listar_operaciones()
        return True
    if len(entrada) < 2:
        return False
    parametros = " ".join(entrada[1:]).split(",")
    if comando == "camino_mas" or comando == "camino_escalas":
        return camino_minimo(grafo, ciudades, parametros)
    if comando == "centralidad":
        return centralidad(grafo, parametros)
    if comando == "centralidad_aprox":
        return centralidad_aproximada(grafo, parametros)
    if comando == "pagerank":
        return pagerank(grafo, parametros)
    if comando == "nueva_aerolinea":
        return nueva_aerolinea(grafo, parametros)
    if comando == "vacaciones":
        return vacaciones(grafo, ciudades, parametros)
    if comando == "exportar_kml":
        return exportar_kml(grafo, parametros, ultimo)
    return False

def main():
    """Funcion principal del programa, se encarga de cargar los datos
    en memoria y ejecutar los comandos recibidos"""
    ruta_aeropuertos = sys.argv[1]
    ruta_vuelos = sys.argv[2]
    grafo = Grafo()
    ciudades = {}
    obtener_aeropuertos(grafo, ciudades, ruta_aeropuertos)
    obtener_vuelos(grafo, ruta_vuelos)
    ultimo = False
    for linea in sys.stdin:
        ultimo = procesar_comando(grafo, ciudades, linea, ultimo)
        if ultimo:
            print("OK")
        else:
            print("ERROR")

main()