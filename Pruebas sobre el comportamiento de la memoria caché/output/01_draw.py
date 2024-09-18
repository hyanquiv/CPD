import pandas as pd
import matplotlib.pyplot as plt

# Cargar los datos del archivo CSV
data = pd.read_csv('loop_times.csv')

# Crear la gráfica
plt.figure(figsize=(10, 6))
plt.plot(data['Matrix Size'], data['First Loop (ms)'], label='First Loop (First rows)', marker='o')
plt.plot(data['Matrix Size'], data['Second Loop (ms)'], label='Second Loop (First columns)', marker='o')

# Etiquetas y título
plt.xlabel('Matrix Size')
plt.ylabel('Execution Time (ms)')
plt.title('Comparison of Execution Times for Two Nested Loops')
plt.legend()

# Mostrar la gráfica
plt.grid(True)
plt.show()
