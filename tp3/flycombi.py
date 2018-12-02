import sys
import csv
from grafo import Grafo
import biblioteca as b

# Índice de cada peso de las aristas del grafo
TIEMPO = 0
PRECIO = 1
VUELOS = 2

def listar_operaciones():
	"""Muestra por pantalla todas las operaciones disponibles"""
	operaciones = ["camino_mas", "nueva_aerolinea", "vacaciones"]
	for op in operaciones:
		print(op)

def obtener_aeropuertos(grafo, ciudades, ruta_aeropuertos):
	"""Recibe un grafo, un diccionario de ciudades, una ruta de archivo
	csv de aeropuertos, y guarda los datos en el grafo y en el diccionario.
	PRE: El archivo debe tener el formato ciudad,codigo,latitud,longitud."""
	with open(ruta_aeropuertos, "r") as archivo:
		reader = csv.reader(archivo)
		for ciudad, codigo, latitud, longitud in reader:
			ubicacion = (latitud, longitud)
			grafo.agregar_vertice(codigo, (ciudad, ubicacion))
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

def camino_minimo(grafo, ciudades, parametros):
	"""Recibe un grafo, un diccionario ciudad: lista_aeropuertos, y una lista
	de parámetros con un peso, una ciudad origen y destino. Imprime un camino
	mínimo desde el origen hasta el destino en términos del peso pasado.
	Devuelve un booleano indicando si el comando se ejecutó correctamente."""
	if len(parametros) != 3:
		return False
	peso, ciudad_origen, ciudad_destino = parametros
	if peso != "barato" and peso != "rapido":
		return False
	if peso == "rapido": peso = TIEMPO
	if peso == "barato": peso = PRECIO
	if ciudad_origen not in ciudades or ciudad_destino not in ciudades:
		return False
	camino, distancia = None, None
	for origen in ciudades[ciudad_origen]:
		for destino in ciudades[ciudad_destino]:
			c, d = b.obtener_camino_minimo(grafo, peso, origen, destino)
			if not distancia or d < distancia:
				camino, distancia = c, d
	resultado = ""
	for aeropuerto in camino:
		resultado += "{} -> ".format(aeropuerto)
	print(resultado.rstrip(" -> "))
	return True

def nueva_aerolinea(grafo, parametros):
	pass

def vacaciones(grafo, parametros):
	pass

def procesar_comando(grafo, ciudades, linea):
	"""Recibe un grafo, un diccionario de ciudades con todos los
	datos disponibles y una linea y procesa los comandos correspondientes.
	Devuelve True en caso de éxito, False en caso de error."""
	comando = linea.rstrip("\n").split(" ")
	if comando[0] == "listar_operaciones":
		listar_operaciones()
		return True
	if len(comando) < 2:
		return False
	parametros = " ".join(comando[1:]).split(",")
	if comando[0] == "camino_mas":
		return camino_minimo(grafo, ciudades, parametros)
	if comando[0] == "nueva_aerolinea":
		return nueva_aerolinea(grafo, parametros)
	if comando[0] == "vacaciones":
		return vacaciones(grafo, parametros)
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
	for linea in sys.stdin:
		if procesar_comando(grafo, ciudades, linea):
			print("OK")
		else:
			print("ERROR")

main()