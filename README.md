# final
The complete demo video is posted in Youtube. Here is the link: https://youtu.be/uV47R6RpB8w

From here, I would show how I implement my final project.

I divided the project into four subtasks. When I start to demo, I would use keyboard to key in the starting command, which is esc+"1".
Then, the screen would pop out a messenge to choose AprilTag's ID, I would demonstrate this part in the following. And these messenge 
would be transmitted to board via XBee, to trigger the whole project process. After each subtask is finished, LED1 would flash for one 
second. Then, it will enter the next subtask automatically.

(1) Line following:
The BBcar would follow a white line to obstacles. After BBcar is driven to about 20cm from the obstacle, it would stop and enter the 
next part.
(2) Circling:
The target of this part is make BBcar circle an obstacle. It is designated that once the car is about 20 cm straightly from an object, 
it would turn right for 90 degree to avoid crashing; otherwise, it would run in certain curve. In this part, I add the implementation 
of  AprilTag's ID detection. I placed two AprilTags at some positions where BBcar can scan them. The car would record the scanned ID 
and transmit to PC. When the received ID is correct, BBcar would stop in front of the AprilTag.
