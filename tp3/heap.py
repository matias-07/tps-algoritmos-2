import heapq

class Nodo:
	"""Implementación de la clase nodo a ser utilizado
	por un Heap de mínimos."""

	def __init__(self, dato, valor):
		"""Constructor de la clase Nodo. Recibe un dato y
		un valor."""
		self.dato = dato
		self.valor = valor

	def __lt__(self, otro):
		"""Función de comparación con otro elemento de
		clase Nodo."""
		return self.valor < otro.valor

	def __gt__(self, otro):
		"""Función de comparación con otro elemento de
		clase Nodo."""
		return self.valor > otro.valor

	def __eq__(self, otro):
		"""Devuelve un booleano que indica si el nodo tiene
		igual valor a otro nodo pasado por parámetro."""
		return self.valor == otro.valor

	def obtener_dato(self):
		"""Devuelve el dato asociado al nodo."""
		return self.dato

class Heap:
	"""Implementación de heap de mínimos."""

	def __init__(self):
		"""Constructor de la clase Heap."""
		self.datos = []

	def encolar(self, dato, valor):
		"""Recibe un dato a encolar, y un valor por el cual se
		lo comparará."""
		nuevo = Nodo(dato, valor)
		heapq.heappush(self.datos, nuevo)

	def desencolar(self):
		"""Quita el elemento con el valor mínimo del Heap
		y lo devuelve."""
		nodo = heapq.heappop(self.datos)
		return nodo.obtener_dato()
	def ver_prioridad_max(self):
		"""Si el heap no está vacío, devuelve el elemento con
		mayor prioridad"""
		if self.esta_vacio():
			raise ValueError("El heap está vacío")
		nodo = heapq.heappop(self.datos)
		dato = nodo.obtener_dato()
		heapq.heappush(self.datos, nodo)
		return dato

	def esta_vacio(self):
		"""Devuelve un booleano indicando si el Heap
		está vacío o no."""
		return len(self.datos) == 0
