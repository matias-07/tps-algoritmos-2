from grafo import Grafo
from heap import Heap

INFINITO = float("inf")

def reconstruir_camino(origen, destino, padres, distancia):
	"""Devuelve una lista ordenada con el camino desde origen
	hasta destino."""
	camino = [destino]
	v = destino
	while padres[v] != None:
		camino.append(padres[v])
		v = padres[v]
	camino.reverse()
	return camino, distancia

def obtener_camino_minimo(grafo, peso, origen, destino=None):
	"""Recibe un grafo, la clave del vértice origen y la del destino,
	el tipo de peso a tener en cuenta, y devuelve un camino mínimo
	aplicando el algoritmo de Dijkstra. Si no se recibe un destino,
	devuelve los caminos mínimos desde origen hasta todos los demás
	vértices del grafo."""
	distancias, padres = {}, {}
	for v in grafo:
		distancias[v] = INFINITO
	distancias[origen] = 0
	padres[origen] = None
	q = Heap()
	q.encolar(origen, distancias[origen])
	while not q.esta_vacio():
		v = q.desencolar()
		if v == destino:
			return reconstruir_camino(origen, destino, padres, distancias[v])
		for w in grafo.obtener_adyacentes(v):
			if distancias[v] + grafo.obtener_peso_union(v, w)[peso] < distancias[w]:
				distancias[w] = distancias[v] + grafo.obtener_peso_union(v, w)[peso]
				padres[w] = v
				q.encolar(w, distancias[w])
	return distancias, padres