# Simulador de Eventos Discretos para Análise de Redes VoIP

## Descrição do Projeto

Este repositório contém os resultados e análises de um projeto de simulação que visa estudar o desempenho de redes VoIP (Voz sobre IP) sob diferentes condições de carga. Utilizando um simulador discreto de eventos, o projeto avalia o impacto de várias taxas de ocupação em um link dedicado de comunicação VoIP.

## Objetivos

O principal objetivo do estudo é entender como diferentes níveis de ocupação do link (60%, 80%, 90% e 99%) afetam o desempenho de uma rede VoIP, especialmente em termos de tempo de transmissão de pacotes e número médio de pacotes no sistema. 

## Relatório

O relatório completo do simulador e a análise detalhada dos eventos estão disponíveis no seguinte [link](relatorio/RelatórioFinal_Análise_de_Desempenho.pdf).

## Tecnologias 
<div style="display: flex; gap: 5px;">
    <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=00599C&labelColor=070707" alt="C++">
    <img src="https://img.shields.io/badge/Python-3670A0?style=for-the-badge&logo=python&logoColor=3670A0&labelColor=070707" alt="Python">
</div>

## Execução do simulador

- Apenas compilar:
     ```shell
     make compile
     ```
    
- Compilar e executar:
     ```shell
     make
     ```

- Remover arquivos gerados:
     ```shell
     make clean
     ```

## Autores
<div style="display: flex; gap: 8px;">
    <a href="https://github.com/gabriel-francelino" target="_blank"><img src="https://img.shields.io/static/v1?label=Github&message=Gabriel Francelino&color=f8efd4&style=for-the-badge&logo=GitHub"></a>
    <a href="https://github.com/gabriel-piva" target="_blank"><img src="https://img.shields.io/static/v1?label=Github&message=Gabriel Piva&color=f8efd4&style=for-the-badge&logo=GitHub"></a>
</div>
