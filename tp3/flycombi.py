import sys
import csv
import grafo as g


def listar_operaciones():
	"""Muestra por pantalla todas las operaciones disponibles"""
	operaciones = ["comando1", "comando2", "etc"]
	for op in operaciones:
		print(op)

def main():
	"""Funcion principal del programa, se encarga de cargar los datos
	en memoria y ejecutar los comandos recibidos"""

	ruta_aeropuertos = sys.argv[1]
	ruta_vuelos = sys.argv[2]
	grafo = g.Grafo()

	with open(ruta_aeropuertos) as file_aeropuertos, open(ruta_vuelos) as file_vuelos:
		aeropuertos = csv.reader(file_aeropuertos)
		vuelos = csv.reader(file_vuelos)
		for airport in aeropuertos:
			grafo.agregar_vertice(airport[1], list(airport))
		for flight in vuelos:
			grafo.agregar_arista(flight[0], flight[1], (flight[2], flight[3], flight[4]))

	for linea in sys.stdin:
		comando = linea.rstrip("\n").split(" ")[0]
		if comando == "listar_operaciones":
			listar_operaciones()
		elif comando == "otro_comando":
			#Asi con todos los comandos que implementemos
			continue
		else:
			print("Comando invalido")

main()