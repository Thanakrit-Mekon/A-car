# A car - ABCS group
<img width="544" alt="Screen Shot 2022-04-15 at 20 34 00" src="https://user-images.githubusercontent.com/65077907/163577048-6506dd12-51be-4d36-86c8-fc2e2f85d3ae.png">

A car is an embedded project. It's a game that requires four players. Each player has a remote control that has forward and reverse buttons with an LED indicator. Each remote control is capable of controlling a wheel. The objective is to reach the finish line without colliding with the obstacle. The car's body is attached to tracking IR sensors and an LDR sensor to determine whether the game status is losing or winning. The car body uses ESP32-CAM for broadcasting to the webserver, showing game status, receiving data from remote control and UART communication with the Nucleo 432kc board.

## Hardware list


a) Remote Controller (x4)
- 4 x ESP32 (Get input data from button and transmit data through ethernet to main car)
- 8 x Button (2 items/controller) (User input)
- 8 x LEDs (4 x Green, 4 x Red)
- 4 x Breadboard400
- 4 x AA Battery socket


b) Car
- 1 x ESP32Cam (Receive data from controller, transmitt data to Nucleo board, WebServer)
- 1 x Nucleo l432kc (Control motor, Tracking sensors, LDR sensor)
- 4 x Motor
- 4 x Wheel
- 2 x l298n motor driver
- 4 x IR sensor (checking whether a car is on a track)
- 1 x Breadboard830
- 1 x LDR (check if the car reaches the finish line)
- 4 x 18650 battery


## Car schematic
<img width="718" alt="Screen Shot 2022-04-15 at 20 40 49" src="https://user-images.githubusercontent.com/65077907/163577767-e355cfab-6329-4e5d-ac96-c54cc7f16473.png">


## Remote schematic
<img width="718" alt="Screen Shot 2022-04-15 at 20 41 18" src="https://user-images.githubusercontent.com/65077907/163577818-27a6fd9f-3644-48cc-9771-739d68f0a749.png">

## Sample video
<a href="https://www.youtube.com/watch?v=36a-GIQDX9A&t">Youtube link</a> (Thai language)

## Sample images
|![Default](https://user-images.githubusercontent.com/65077907/163578954-8ed4dfc7-995c-4720-924d-c509b0455ad9.jpg)|
|:--:| 
| *Webserver UI* |

|![LINE_ALBUM_Project pic_220410_2](https://user-images.githubusercontent.com/65077907/163579524-e201ca7b-bcfe-481c-90bc-caa6e92a651b.jpg)|
|:--:| 
| *Remote control* |

|<img width="544" alt="Screen Shot 2022-04-15 at 20 54 17" src="https://user-images.githubusercontent.com/65077907/163579345-a224f899-42f4-4749-a782-2ec72856823e.png">|
|:--:| 
| *Main car* |


