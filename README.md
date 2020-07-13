# TemperatureLogger

## Summary
A simple data logger that reads the user commands over UART, and sends the desired sensor's data over to the receiver module over the air that's hooked to a Raspberry Pi where graph of the data is generated in real-time.

## Supported terminal commands so far:


| Commands        | Function      
| ------------- |:-------------:| 
| temp     | outputs the current temperature value to the terminal and populates the graph on the receiving-end with the data
| led  | triggers the LED on the uC (trivial)     
