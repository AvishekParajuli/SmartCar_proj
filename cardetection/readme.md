# Car detection using HOG

## Description
Herein, we discuss the pipeline for detecting cars using HOG(Histogram of Oriented Gradients) in a video.

Supplemental tool: [Interactive Visualization of HOG descriptor](https://github.com/AvishekParajuli/SmartCar_proj/blob/master/HOGUI/readme.md)
<p align="center">
  <img src="https://github.com/AvishekParajuli/SmartCar_proj/blob/feature-cardetection/cardetection/cars1.png" width ="450" />
</p>

The Image dataset that we have used are of size 64x64. There is no size variation for these training and test image sets and HOG features can be calulated from these single image. ( for eg. window size 64X64). 
However, the realworld images are of different size. Hence we will need to implement a sliding window and go through the image, resize the extracted roi from sliding window region to the predefined 64x64 and use trained svm to predict whether the sliding window location is vahicle or not. Note that there would be many false detections and we would need to filter these false vehicle detections.
Here are the steps invlolved for the training and detection part:

Algorithm pipeline(Training svm ):
1. From the image dataset, randomly separate it into training and test set(80%, 20% split)
1. Compute the HOG features for the training image set containg vehicles and non-vehicles.
1. Train a svm using these HOG features. 
2. Test the trained SVM on the separate test imageset and evaluate.

Note: Tested on Ubuntu 16.04 and opencv 3.0. 

## Usage

 ### 1: run program with preset detector
 
```
  $ ./detectcar 
```


## Contributing

1. Fork it!
2. Create your feature branch:
3. Commit your changes: 
4. Push to the branch: 
5. Submit a pull request :D
   

