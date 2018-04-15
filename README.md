# Gesture-Detection-Project

### Problem Definition
The goal is to design and implement an algoirthm to recognize 3 different hand gestures in real time: an okay sign, a V-sign, 
and a wave. Along the way, different thresholds for recognition, functions for determining distance between matching images, 
and scales for templates will be tested to find optimal numbers or functions to use in the final algorithm. 

### Method and Implementation 
Template matching is used to match templates of the different gestures to each frame of a streaming video feed. To create the templates,
images of each gesture were found online and resized in the algorithm to adjust for different distances in the webcamera and then skin colordetection is applied to create a black and white template to use in the matching algorithm. In the implementation of the matching algorithm, a video feed is read from the webcamera. For each frame of the video feed, streaming at 30 frames per second, the difference from the previous frame is calculated. This difference image is then searched using the OpenCV matchTemplate function, returning the location of the match and the value of the function used to calculate the match. This is then displayed on the user interface.

The important functions of the algorithm are the skin color detection funtion, used to create the templates, the frame differencing function, used to help the algorithm visualize movement, and the template matching function, used to actually match a template to a frame. The skin color detection function simply looks at the different intensities of red, blue, and green in the colored templates and marks where skin color is detected. The frame differencing function looks at two frames and marks where the two are different, and the template matching function goes through all possible areas of the image and attempts to maximize a value that determines a match. 
