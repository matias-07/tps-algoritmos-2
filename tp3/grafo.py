from random import choice

class Vertice:
    """Implementación de la clase vértice."""

    def __init__(self, clave, valor):
        """Constructor de la clase Vertice."""
        self.clave = clave
        self.valor = valor
        self.adyacentes = {}

    def agregar_adyacente(self, clave_vertice, peso):
        """Recibe la clave de un vértice y lo agrega a
        los adyacentes."""
        self.adyacentes[clave_vertice] = peso

    def obtener_adyacentes(self):
        """Devuelve un diccionario con los vértices adyacentes
        de la forma clave_vertice: peso."""
        return self.adyacentes

    def obtener_peso(self, v):
        """Recibe la clave de un vértice y devuelve el
        peso de la unión con dicho vértice."""
        return self.adyacentes[v]

    def obtener_clave(self):
        """Devuelve la clave asociada al vértice."""
        return self.clave

    def obtener_valor(self):
        """Devuelve el valor asociado al vértice."""
        return self.valor

class Grafo:
    """Implementación de la clase grafo no dirigido."""

    def __init__(self):
        """Constructor de la clase Grafo."""
        self.vertices = {}
        self.numero_vertices = 0
        self.numero_aristas = 0

    def agregar_vertice(self, clave, valor):
        """Recibe una clave y un valor, y los agrega al grafo,
        guardando en el diccionario de vértices un par
        clave: Vertice(clave, valor)."""
        self.vertices[clave] = Vertice(clave, valor)
        self.numero_vertices += 1

    def agregar_arista(self, clave1, clave2, peso):
        """Recibe dos claves pertenecientes a vértices del
        Grafo y agrega una arista entre ellos, con el peso
        pasado."""
        self.vertices[clave1].agregar_adyacente(clave2, peso)
        self.vertices[clave2].agregar_adyacente(clave1, peso)
        self.numero_aristas += 1

    def estan_conectados(self, clave1, clave2):
        """Recibe dos claves de vértices en el grafo y
        devuelve True si están conectados, o False en el
        caso contrario."""
        return clave1 in self.vertices[clave2].obtener_adyacentes()

    def obtener_vertice(self):
        """Devuelve un vértice aleatorio del grafo."""
        return choice(list(self.vertices.keys()))

    def obtener_dato(self, clave):
        """Recibe la clave asociada a un vértice del grafo y
        devuelve su dato."""
        return self.vertices[clave].obtener_valor()

    def obtener_peso_union(self, clave1, clave2):
        """Recibe dos claves de vértices en el grafo y devuelve
        el peso de la arista que los conecta."""
        return self.vertices[clave1].obtener_peso(clave2)

    def obtener_adyacentes(self, clave):
        """Recibe la clave de un vértice en el grafo y devuelve un
        diccionario con los vértices adyacentes a él, de la forma
        clave_vertice: peso."""
        return self.vertices[clave].obtener_adyacentes()

    def __len__(self):
        return self.numero_vertices

    def __iter__(self):
        """Iterador del grafo."""
        for v in self.vertices:
            yield v