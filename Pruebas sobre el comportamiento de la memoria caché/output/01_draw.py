import pandas as pd
import matplotlib.pyplot as plt

# Cargar los resultados desde el archivo CSV
data = pd.read_csv("loop_times.csv")

# Crear el gráfico para el primer bucle
plt.figure(figsize=(12, 8))
plt.plot(data['Matrix Size'], data['First Loop Avg Time (ms)'], label="Tiempo Promedio Primer Bucle (primero filas)", marker='o')

# Crear gráfico para el segundo bucle
plt.plot(data['Matrix Size'], data['Second Loop Avg Time (ms)'], label="Tiempo Promedio Segundo Bucle (primero columnas)", marker='o')

# Configurar las etiquetas y el título
plt.xlabel('Tamaño de Matriz')
plt.ylabel('Tiempo Promedio (ms)')
plt.title('Comparación del Primer Bucle vs Segundo Bucle (Promedio de 50 intentos)')
plt.legend()

# Mostrar el gráfico
plt.grid(True)
plt.show()
