# TemperatureLogger

## Summary
A simple data logger that reads the user commands over UART, and if temp is received, the apps sends the temperature sensor's data over to the nRF2401 receiver module over the air that's hooked to a Raspberry Pi where graph is populated is in real-time.

## Supported terminal commands so far:


| Commands        | Function      
| ------------- |:-------------:| 
| temp     | outputs the current temperature value to the terminal and populates the graph on the receiving-end with the data
| led  | triggers the LED on the uC (trivial)     

## g++ command to run the receiver program:
```
g++ -lrf24 receiver.cpp -o run
./run```
