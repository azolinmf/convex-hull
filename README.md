# Convex-hull

## Debug (vscode)
O vscode vai compilar o programa e colocar os arquivos de resultado na pasta `debug`. A execução utilizará o arquivo: `debug/input.txt`
1. Abrir o arquivo `main.cpp`.
2. Abrir a aba de debug.
3. Executar: `g++ Debug active file`.

## Simular 
O script gera um arquivo de entrada com base na entrada informada e executa o programa seguindo os requisitos do trabalho.
```shell
./simulate.sh
```

## Execução manual
```shell
make
./hull input.txt
make clean
```
