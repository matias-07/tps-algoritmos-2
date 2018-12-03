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

def obtener_viaje(grafo, origen, n):
    """Recibe un grafo, un origen y un número entero n.
    Devuelve un viaje de n lugares que comienza y finaliza
    en origen."""
    recorrido = [origen]
    visitados = set()
    if not _obtener_viaje(grafo, origen, n, recorrido, visitados):
        return None
    return recorrido

def _obtener_viaje(grafo, origen, n, recorrido, visitados):
    """Función auxiliar para obtener un viaje de n lugares."""
    ultimo = recorrido[-1]
    if len(recorrido) == n:
        if origen not in grafo.obtener_adyacentes(ultimo):
            return False
        recorrido.append(origen)
        return True
    for v in grafo.obtener_adyacentes(ultimo):
        if v in visitados:
            continue
        recorrido.append(v)
        visitados.add(v)
        if _obtener_viaje(grafo, origen, n, recorrido, visitados):
            return True
        recorrido.pop()
        visitados.remove(v)
    return False

def exportar_archivo_kml(grafo, recorrido, archivo):
    """Recibe una lista con un recorrido y un archivo, y exporta
    a dicho archivo el recorrido en formato kml."""
    archivo.write('<?xml version="1.0" encoding="UTF-8"?>\n')
    archivo.write('<kml xmlns="http://www.opengis.net/kml/2.2">\n')
    archivo.write('    <Document>\n')
    for i in range(len(recorrido)):
        coordenadas = grafo.obtener_dato(recorrido[i])
        archivo.write('        <Placemark>\n')
        archivo.write('            <name>{}</name>\n'.format(recorrido[i]))
        archivo.write('            <Point>\n')
        archivo.write('                <coordinates>{}</coordinates>\n'.format(coordenadas))
        archivo.write('            </Point>\n')
        archivo.write('        </Placemark>\n')
    for i in range(len(recorrido) - 1):
        origen = grafo.obtener_dato(recorrido[i])
        destino = grafo.obtener_dato(recorrido[i+1])
        archivo.write('        <Placemark>\n')
        archivo.write('            <LineString>\n')
        archivo.write('                <coordinates>{} {}</coordinates>\n'.format(origen, destino))
        archivo.write('            </LineString>\n')
        archivo.write('        </Placemark>\n')
    archivo.write('    </Document>\n')
    archivo.write('</kml>\n')