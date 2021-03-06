# lifi-messenger
message transfer through visible light using UART protocol, BH1750,2 microcontrollers
This project is to demonstrate data transmisssion through light
this project is able to transmit message through light using UART protocol
Refer to this video  https://www.youtube.com/watch?v=IyGwvGzrqp8  for UART protocol details
Getting to the project
for this we need two microcontrollers for ex esp32,esp8266, arduino etc
(if esp : esp libraries needed to be installed from board manager)
for receiver

install BH1750 library in arduino ide
upload receiver.ino to the board
connect microcrotroller with BH1750(light intensity sensor)
    VCC -> 3V3 or 5V
    GND -> GND
    SCL -> SCL (A5 on Arduino Uno, Leonardo, etc or 21 on Mega and Due, on esp8266 free selectable)(22 in esp32)
    SDA -> SDA (A4 on Arduino Uno, Leonardo, etc or 20 on Mega and Due, on esp8266 free selectable)(21 in esp32)
    ADD -> (not connected) or GND
for transmitter
upload transmitter.ino to the board
connect led to board to pin 13(by default )and ground


arrange led of transmitter and bh1750 of receiver  facing  eachother 

open serial monitor on both sides
send a message from the transmitter serial monitor
wait for the message to be transmitted 
check themessage display on the receiver
change some parameters in code if the received message is incorrect
