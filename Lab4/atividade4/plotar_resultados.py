import matplotlib.pyplot as plt

with open('resultados.txt', 'r') as file:
    content = file.read().strip()

blocks = content.split('\n\n')

'''num_plots = len(blocks)
fig, axes = plt.subplots(1, num_plots, figsize=(12, 6), sharey=True)

if num_plots == 1:
    axes = [axes]'''

plt.figure(figsize=(10, 6))
for i, block in enumerate(blocks):
    x = []
    y = []
    lines = block.split('\n')
    key = int(lines[0]) 
    
    for line in lines[1:]:
        values = line.split(' ')
        x.append(float(values[0]))
        y.append(float(values[1]))
    
    plt.bar(x, y, width=1, edgecolor='black', alpha=0.7)
    plt.title(f'N = {key}')
    plt.xlabel('Número de threads')
    plt.ylabel('Tempo de Execução Médio')

    plt.show()
