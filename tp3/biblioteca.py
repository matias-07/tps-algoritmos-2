from grafo import Grafo
from heap import Heap
from cola import Cola

INFINITO = float("inf")

def reconstruir_camino(destino, padres):
	"""Devuelve una lista ordenada con el camino desde origen
	hasta destino."""
	camino = [destino]
	v = destino
	while padres[v] != None:
		camino.append(padres[v])
		v = padres[v]
	camino.reverse()
	return camino

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
			return (reconstruir_camino(destino, padres), distancias[v])
		for w in grafo.obtener_adyacentes(v):
			if distancias[v] + grafo.obtener_peso_union(v, w)[peso] < distancias[w]:
				distancias[w] = distancias[v] + grafo.obtener_peso_union(v, w)[peso]
				padres[w] = v
				q.encolar(w, distancias[w])
	return distancias, padres

def escalas_minimas_bfs(grafo, origen, destino=None):
	"""Recibe un grafo, la clave del vertice origen y la del destino,
	y devuelve un camino con la minima cantidad de escalas aplicando
	el algoritmo bfs. Si no se recibe un destino, devuelve los caminos
	con menor cantidad de escalas desde origen hasta todos los demás
	vértices del grafo."""
	visitados = []
	padres, orden = {}, {}
	q = Cola()
	visitados.append(origen)
	padres[origen] = None
	orden[origen] = 0
	q.encolar(origen)
	while not q.esta_vacia():
		v = q.desencolar()
		for w in grafo.obtener_adyacentes(v):
			if w not in visitados:
				visitados.append(w)
				padres[w] = v
				orden[w] = orden[v] + 1
				q.encolar(w)
			if w == destino:
				return reconstruir_camino(destino, padres), orden[w]
	return padres, orden
