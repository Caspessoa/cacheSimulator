# Simulador de Cache
###### [Cassiano Teixeira](https://github.com/Caspessoa) & [Enderson Kruger](https://github.com/endersonkruger)

#### Compilação: `g++ cache_simulator.cpp cache.cpp -o cache_simulator`

#### Execução: cache_simulator `<nsets> <bsize> <assoc> <substituição> <flag_saida> <arquivo_de_entrada>`

###### Onde:
- `cache_simulator` - nome do arquivo de execução principal do simulador
- `nsets` - número de conjuntos na cache
- `bsize` - tamanho do bloco em bytes
- `assoc` - grau de associatividade
- `substituição` - política de substituição, que pode ser Random (R), FIFO (F) ou L (LRU)
`flag_saida` - flag que ativa o modo padrão de saída de dados, que pode ser 0 (Padrão), 1 (Detalhado) ou 2 (Detalhado+)
- `arquivo_de_entrada` - arquivo com os endereços para acesso à cache.
---
Trabalho como requisito parcial de avaliação semestral \
Disciplina: **Arquitetura e Organização de Computadores 2** - UFPEL
