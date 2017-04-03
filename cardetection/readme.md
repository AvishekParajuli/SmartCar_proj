# Car detection using HOG

## Description
Herein, we discuss the pipeline for detecting cars using HOG(Histogram of Oriented Gradients) in a video.

Supplemental tool: [Interactive Visualization of HOG descriptor](https://github.com/AvishekParajuli/SmartCar_proj/blob/master/HOGUI/readme.md)
<p align="center">
  <img src="https://github.com/AvishekParajuli/SmartCar_proj/blob/feature-cardetection/cardetection/cars1.png" width ="450" />
</p>

These example images come from a combination of the [GTI vehicle image database](http://www.gti.ssr.upm.es/data/Vehicle_database.html), the [KITTI vision benchmark suite](http://www.cvlibs.net/datasets/kitti/), and video examples are from the udacity project video itself.The Image dataset that we have used are of size 64x64. 
There is no size variation for these training and test image sets and HOG features can be calulated from these single image. ( for eg. window size 64X64). 
However, the realworld images are of different size. Hence, we will need to implement a sliding window and go through the image, resize the extracted roi from sliding window region to the predefined 64x64 and use trained svm to predict whether the sliding window location is vehicle or not. Note, that there would be many false detections and we would need to filter these false vehicle detections.
Here are the steps invlolved for the training and detection part:

Algorithm pipeline(Training svm ):
1. From the image dataset, randomly separate it into training and test set(80%, 20% split)
1. Choose the hog windowSize(64x64) and Compute the HOG features for the training image set containg vehicles and non-vehicles.
1. Train a svm using these HOG features. 
2. Test the trained SVM on the separate test imageset and evaluate.
3. For the real world test image, run a sliding window and use the svm from above to predict vehicle/non-vehicle.

Algorithm pipeline(Detectting car in video ):
1. Extract individual frame of the video.
2. Convert each frame into grayscale image.
3. Discard the upper half containing the sky part.
4. Choose a sliding window size, and resize to the trained size; Use SVM to predict
5. Filter the detect points for false positive(use template matching only for the subsection of detected)


## Usage
### 1: train
 
```
  $ ./train
```
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
   

