<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
</head>
<body>
    <h1>Gigantes de MDF</h1>
    <p>O projeto <strong>Gigantes de MDF</strong> consiste no desenvolvimento de um robô utilizando o microprocessador ATMEGA 328p, um circuito eletrônico projetado pelo grupo, e a aplicação dos conhecimentos de programação de hardware adquiridos em aula. O robô será equipado com um laser funcional, um sensor LDR para detectar os lasers emitidos por robôs adversários, capacidade de movimentação, e três LEDs que representarão suas "vidas" durante as interações. Além disso, o robô será controlado remotamente por meio de um módulo Bluetooth HC-05, permitindo a interação com um aplicativo desenvolvido pelo grupo.</p>
    <p>Esse projeto combina elementos de eletrônica, programação e mecânica, desafiando o grupo a integrar diferentes áreas do conhecimento.</p>
    <h2>OBJETIVO</h2>
    <p>O objetivo do robô Gigante de MDF é competir contra os robôs desenvolvidos por outros grupos, seguindo diretrizes específicas para a funcionalidade dos projetos. Cada robô deve possuir um laser que dispara automaticamente a cada 1 segundo, representando um ataque, e um sensor LDR responsável por detectar esses disparos.</p>
    <p>Quando o laser de um robô acerta o sensor LDR de outro, um dos três LEDs que representam as "vidas" do robô atingido será apagado, indicando a perda de uma vida. O robô será considerado derrotado ao perder todas as suas vidas. Para reiniciar a competição, cada robô deve conter um botão de reset que reativa os LEDs de vida.</p>
    <h2>Componentes Principais</h2>
    <ul>
        <li>1x ATmega328P – Microcontrolador principal.</li>
        <li>1x Cristal de 16 MHz – Para controle do clock do microcontrolador.</li>
        <li>2x Motores DC (3-6 V) – Para movimentação do robô.</li>
        <li>3x LEDs amarelos – Representando as vidas do robô.</li>
        <li>1x Sensor LDR (20 mm) – Para detectar os disparos de laser dos adversários.</li>
        <li>1x Laser (&lt;4.9 mW de saída) – Para realizar os disparos durante a competição.</li>
        <li>1x Módulo Bluetooth HC-05 – Para permitir o controle remoto do robô via aplicativo.</li>
        <li>1x Roda boba – Para suporte e estabilidade na movimentação.</li>
        <li>1x Placa de MDF – Estrutura base do robô.</li>
        <li>1x CI de ponte H L293d – CI para controle dos motores.</li>
        <li>Circuito eletrônico projetado pelo grupo – Para interligar e controlar os componentes.</li>
    </ul>
  <h2>Métodos Utilizados</h2>
    <p>O circuito eletrônico do robô foi projetado utilizando o software <strong>KiCad</strong> e prototipado no <strong>INOVFABLAB</strong>. O código do robô foi desenvolvido em linguagem C, escrito e simulado no software <strong>SimulIDE</strong> para testar e validar o comportamento do sistema.</p>
    <p>Além disso, o robô conta com um módulo Bluetooth, que possibilita o controle remoto através de um aplicativo personalizado, desenvolvido pelo grupo para facilitar a interação e operação do robô.</p>
    <h2>Circuito Eletrônico</h2>
    <img src="https://i.imgur.com/UgerWMk.jpeg" alt="esquemático do circuito"/>
    <img src="https://i.imgur.com/6nBmz6L.png" alt="PCI do projeto"/>
</body>
</html>
