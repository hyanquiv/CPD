import pandas as pd
import matplotlib.pyplot as plt

# Cargar los resultados desde el archivo CSV
data = pd.read_csv("matrix_multiplication_comparison.csv")

# Crear el gráfico para la multiplicación clásica
plt.figure(figsize=(12, 8))
plt.plot(data['Matrix Size'], data['Classic Time (ms)'], label="Multiplicación Clásica", marker='o')

# Crear gráficos para las multiplicaciones por bloques con diferentes tamaños de bloque
plt.plot(data['Matrix Size'], data['Block Size 16 (ms)'], label="Tamaño de Bloque 16", marker='o')
plt.plot(data['Matrix Size'], data['Block Size 32 (ms)'], label="Tamaño de Bloque 32", marker='o')
plt.plot(data['Matrix Size'], data['Block Size 64 (ms)'], label="Tamaño de Bloque 64", marker='o')
plt.plot(data['Matrix Size'], data['Block Size 128 (ms)'], label="Tamaño de Bloque 128", marker='o')

# Configurar las etiquetas y el título
plt.xlabel('Tamaño de Matriz')
plt.ylabel('Tiempo Promedio (ms)')
plt.title('Multiplicación de Matrices: Clásica vs Bloques (Múltiples Tamaños de Bloque)')
plt.legend()

# Mostrar el gráfico
plt.grid(True)
plt.show()
