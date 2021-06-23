# final
The complete demo video is posted in Youtube. Here is the link: https://youtu.be/uV47R6RpB8w

From here, I would show how I implement my final project.

I divided the project into four subtasks. When I start to demo, I would use keyboard to key in the starting command, which is esc+"1".
Then, the screen would pop out a messenge to choose AprilTag's ID, I would demonstrate this part in the following. And these messenge 
would be transmitted to board via XBee, to trigger the whole project process. After each subtask is finished, LED1 would flash for one 
second. Then, it will enter the next subtask automatically.
![image](https://user-images.githubusercontent.com/55796795/123086675-4c75da00-d456-11eb-89e5-814f014f39df.png)
It is the beginning page. 

(1) Line following:
The BBcar would follow a white line to obstacles. After BBcar is driven about 20cm from the obstacle, it would stop and enter the 
next part.

(2) Circling:
The target of this part is make BBcar circle an obstacle. It is designated that once the car is about 20 cm straightly from an object, 
it would turn right for 90 degree to avoid crashing; otherwise, it would run in certain curve. In this part, I add the implementation 
of  AprilTag's ID detection. I placed two AprilTags at some positions where BBcar can scan them. The car would record the scanned ID 
and transmit to PC. When the received ID is correct, BBcar would stop in front of the AprilTag.
![image](https://user-images.githubusercontent.com/55796795/123085344-b4c3bc00-d454-11eb-9f5f-daa3ee535bc5.png)
The picture is PC's screen when BBcar detect the AprilTag with ID 1. Since I key "0" in the beginning, BBcar would stop when detecting 
ID 0. Therefore, BBcar would not stop here. The picture shown below is the screen when BBcar detect ID 0.
![image](https://user-images.githubusercontent.com/55796795/123085880-56e3a400-d455-11eb-8f41-7ee970aa8b3a.png)


(3) Position Calibration:
This part is same as that in homework 4. BBcar would calibrate its position automatically till it faces directly from the AprilTag.

(4) Line following with TF human identification:
The subtask is similar to the first one, which makes the car follow a white line. The different part is that I use the human 
identification of TF to identify a human in front of BBcar. In the video, I place a man's photo to let BBcar detect. Once BBcar detect 
the man, it would stop running and end the project demo.
![image](https://user-images.githubusercontent.com/55796795/123086176-b04bd300-d455-11eb-8b5a-8e34b308d7ba.png)

