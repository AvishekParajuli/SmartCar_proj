# Interactive HOG Visualization
## Description
This files provides a UI for the users to interactively visualize the HOG descriptor and play with its parameters.
On summary it provides:

1. Interactive Visualization of HOG decriptor
2. UI for the user to select various color, and sliding window size
3. Color choices: RGB, HSV, Luv, Canny(binarized)

Note: Tested on Ubuntu 16.04 and opencv 3.0. 

## Usage
### 1: Use the default image located on ../test_images/test3.jpg
```
  $ cmake .
  $ make
  $./hogUI
```
 
 ### 2: User supplied image
 
```
  $ ./hogUI  myimage1.jpg
```
  
## UI instructions  

1. Open control Panel
<p align="center">
  <img src="https://github.com/AvishekParajuli/SmartCar_proj/blob/HOGUI/HOGUI/step1.png" width ="450" />
</p>
2. Left click to select the sliding window center 
<p align="center">
  <img src="https://github.com/AvishekParajuli/SmartCar_proj/blob/HOGUI/HOGUI/step2.png" width ="450" />
</p>
3. click "showHOG" to display the HOG, 
<p align="center">
  <img src="https://github.com/AvishekParajuli/SmartCar_proj/blob/HOGUI/HOGUI/step3.png" width ="450" />
</p>
4. change window size and repeat 1-3.

5. Press ESC to quit

![Alt text] (step1.png?raw=true " Click  to bring Control panel")

![step 1] (SmartCar_proj/blob/HOGUI/HOGUI/step1.png)

![step 2] (SmartCar_proj/tree/HOGUI/HOGUI/step1.png)

![step 3] (tree/HOGUI/HOGUI/step1.png)

![step 4] (/tree/HOGUI/HOGUI/step1.png)

![step 5] (SmartCar_proj/HOGUI/HOGUI/step1.png)

## Contributing

1. Fork it!
2. Create your feature branch:
3. Commit your changes: 
4. Push to the branch: 
5. Submit a pull request :D
   
