import pandas as pd
import matplotlib.pyplot as plt

# Cargar los resultados desde el archivo CSV
data = pd.read_csv("matrix_multiplication_comparison.csv")

# Crear el gráfico
plt.figure(figsize=(10, 6))
plt.plot(data['Matrix Size'], data['Classic Time (ms)'], label="Classic Multiplication", marker='o')
plt.plot(data['Matrix Size'], data['Block Time (ms)'], label="Block Multiplication", marker='o')

# Configurar las etiquetas y el título
plt.xlabel('Matrix Size')
plt.ylabel('Time (ms)')
plt.title('Matrix Multiplication: Classic vs Block')
plt.legend()

# Mostrar el gráfico
plt.grid(True)
plt.show()
